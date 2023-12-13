#ifdef __cplusplus
 extern "C" {
 #endif
#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>  
#include <semaphore.h>
#include <stdarg.h>
#include <syslog.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "rwif.h"
/* ltoh: little to host */
/* htol: little to host */
#if __BYTE_ORDER == __LITTLE_ENDIAN
#  define ltohl(x)       (x)
#  define ltohs(x)       (x)
#  define htoll(x)       (x)
#  define htols(x)       (x)
#elif __BYTE_ORDER == __BIG_ENDIAN
#  define ltohl(x)     __bswap_32(x)
#  define ltohs(x)     __bswap_16(x)
#  define htoll(x)     __bswap_32(x)
#  define htols(x)     __bswap_16(x)
#endif
#define MAP_SIZE (1024*1024UL)
#define MAP_MASK (MAP_SIZE - 1)

#define S2MM_VDMA_BASE 0x10000
#define MM2S_VDMA_BASE 0x00000

#define VDMA_S2MM_SR 0x34
#define VDMA_S2MM_CR 0x30
#define VDMA_S2MM_START_ADDR 0xAC
#define VDMA_S2MM_HSIZE 0xA8
#define VDMA_S2MM_STRID 0xA4
#define VDMA_S2MM_VSIZE 0xA0

#define VDMA_MM2S_SR 0x04
#define VDMA_MM2S_CR 0x00
#define VDMA_MM2S_START_ADDR 0x5C
#define VDMA_MM2S_HSIZE 0x58
#define VDMA_MM2S_STRID 0x54
#define VDMA_MM2S_VSIZE 0x50

#define VDMA_DATA_BYTES 4 

#define FPGA_DDR_START_ADDR (0)

#define VIDEO_FRAME_STORE_NUM 16

#define CMV_CONTROL_REG 0x20000 //pcie axi lite
#define VTC_CTRL_REG 0x00
#define VTC_SET_VALID_REG 0x18
#define VTC_H_V_TOTAL_REG 0x04
#define VTC_H_V_SIZE_REG 0x08
#define VTC_H_V_START_REG 0x0c
#define VTC_V_SYNC_REG 0x10
#define VTC_H_SYNC_REG 0x14
#define VIDEO_SATU_REG 0x20
void *control_base;
int control_fd;
int c2h_dma_fd;
int h2c_dma_fd;
int c2h_event_fd;
int h2c_event_fd;
static int video_height;
static int video_width;

static unsigned int h2c_fpga_ddr_addr[VIDEO_FRAME_STORE_NUM];
static unsigned int c2h_fpga_ddr_addr[VIDEO_FRAME_STORE_NUM];

static unsigned char *h2c_align_mem;
static unsigned char *c2h_align_mem;
static unsigned char *h2c_align_mem_tmp;
static unsigned char *c2h_align_mem_tmp;
//static struct timespec ts_start, ts_end;
static unsigned int frame_cnt = 0;

static void timespec_sub(struct timespec *t1, const struct timespec *t2)
{
  assert(t1->tv_nsec >= 0);
  assert(t1->tv_nsec < 1000000000);
  assert(t2->tv_nsec >= 0);
  assert(t2->tv_nsec < 1000000000);
  t1->tv_sec -= t2->tv_sec;
  t1->tv_nsec -= t2->tv_nsec;
  if (t1->tv_nsec >= 1000000000)
  {
    t1->tv_sec++;
    t1->tv_nsec -= 1000000000;
  }
  else if (t1->tv_nsec < 0)
  {
    t1->tv_sec--;
    t1->tv_nsec += 1000000000;
  }
}
static int open_event(char *filename)
{
	int fd;
	fd = open(filename, O_RDWR | O_SYNC);
	if(fd == -1)
	{
		printf("open event error\n");
		return -1;
	}
	return fd;
}
static int read_event(int fd)
{
	int val;
	read(fd,&val,4);
	return val;
}
static int open_control(char *filename)
{
	int fd;
	fd = open(filename, O_RDWR | O_SYNC);
	if(fd == -1)
	{
		printf("open control error\n");
		return -1;
	}
	return fd;
}
static void *mmap_control(int fd,long mapsize)
{
	void *vir_addr;
	vir_addr = mmap(0, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	return vir_addr;
}
static void write_control(void *base_addr,int offset,uint32_t val)
{
	uint32_t writeval = htoll(val);
	*((uint32_t *)(base_addr+offset)) = writeval;
}
static uint32_t read_control(void *base_addr,int offset)
{
	uint32_t read_result = *((uint32_t *)(base_addr+offset));
	read_result = ltohl(read_result);
	return read_result;
}
static void config_mm2s_vdma(void *base_addr,int num_frame)
{
	int i;
	write_control(base_addr,MM2S_VDMA_BASE+VDMA_MM2S_CR,0x4);
	usleep(100);
	write_control(base_addr,MM2S_VDMA_BASE+VDMA_MM2S_CR,0x01011003);//0x01011003
	write_control(base_addr,MM2S_VDMA_BASE+VDMA_MM2S_HSIZE,video_width*3);
	write_control(base_addr,MM2S_VDMA_BASE+VDMA_MM2S_STRID,video_width*3);
	//write_control(base_addr,MM2S_VDMA_BASE+VDMA_MM2S_VSIZE,video_height);
	for(i=0;i<num_frame;i++)
		write_control(base_addr,MM2S_VDMA_BASE+VDMA_MM2S_START_ADDR+i*0x4,h2c_fpga_ddr_addr[i]);
	write_control(base_addr,MM2S_VDMA_BASE+VDMA_MM2S_SR,0xffffffff);
	write_control(base_addr,MM2S_VDMA_BASE+VDMA_MM2S_VSIZE,video_height);

}
static void config_s2mm_vdma(void *base_addr,int num_frame)
{
	int i;
	write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_CR,0x4);
	usleep(100);
	write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_CR,0x01011003);
	write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_HSIZE,video_width*3);
	write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_STRID,video_width*3);
	//write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_VSIZE,video_height);
	for(i=0;i<num_frame;i++)
		write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_START_ADDR+i*0x4,c2h_fpga_ddr_addr[i]);
	write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_SR,0xffffffff);
	write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_VSIZE,video_height);
}


static int get_align_mem()
{
	posix_memalign((void *)&h2c_align_mem,4096,video_height*video_width*3+4096);
	if(NULL == h2c_align_mem)
	{
		return -1;
	}
	return 0;
}
static void reset_vdma(void *base_addr)
{
	write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_CR,0x4);
	write_control(base_addr,MM2S_VDMA_BASE+VDMA_MM2S_CR,0x4);
}

static void put_video_to_fpga_ddr(unsigned int fpga_ddr_addr,unsigned char *buffer)
{
	memcpy(h2c_align_mem,buffer,video_height*video_width*3);
	lseek(h2c_dma_fd,fpga_ddr_addr,SEEK_SET);
	write(h2c_dma_fd,h2c_align_mem,video_height*video_width*3);
}
static void get_video_from_fpga_ddr(unsigned int fpga_ddr_addr,unsigned char *buffer)
{
	lseek(c2h_dma_fd,fpga_ddr_addr,SEEK_SET);
	read(c2h_dma_fd,buffer,video_height*video_width*3);
	//memcpy(buffer,c2h_align_mem,video_height*video_width*3);
}
static struct timespec ts_1, ts_2;
int pcie_read_frame(unsigned char *buffer)
{
	int fps;
	clock_gettime(CLOCK_MONOTONIC, &ts_2);
    timespec_sub(&ts_2, &ts_1);
	fps = 1000000000/ts_2.tv_nsec;
	clock_gettime(CLOCK_MONOTONIC, &ts_1);
	get_video_from_fpga_ddr(c2h_fpga_ddr_addr[frame_cnt],buffer);
	frame_cnt++;
	frame_cnt = frame_cnt%VIDEO_FRAME_STORE_NUM;
	return fps;
}
static void vtc_stream_run(void *base_addr,int isstream,int isrun)
{
	int val=0;
	val = (isstream<<1) | isrun;
	write_control(base_addr,CMV_CONTROL_REG+VTC_CTRL_REG,val);
	write_control(base_addr,CMV_CONTROL_REG+VTC_SET_VALID_REG,1);
	write_control(base_addr,CMV_CONTROL_REG+VTC_SET_VALID_REG,0);
}
static void vtc_config(void *base_addr,int target_fps,double pixclk_mhz)
{
	unsigned short v_total;
	unsigned short h_total;
	unsigned short v_size;
	unsigned short h_size;
	unsigned short v_start;
	unsigned short h_start;
	unsigned short v_sync_start;
	unsigned short v_sync_stop;
	unsigned short h_sync_start;
	unsigned short h_sync_stop;
	
	unsigned int hblank;
	unsigned int v;
	double one_frame_nsec = 1000.0/target_fps * 1000000;
	double period = 1000.0/pixclk_mhz;
	double n_clk = one_frame_nsec/period;
	if(video_height == 1080 && video_width == 1920)
	{
		v_total      = 1125;
		h_total      = 2200;
		v_size       = 1080;
		h_size       = 1920;
		v_start      = 10;
		h_start      = 60;
		v_sync_start = 0;
		v_sync_stop  = 4;
		h_sync_start = 0;
		h_sync_stop  = 40;
	}
	else
	{
		h_size = video_width;
		v_size = video_height;
		hblank = video_width/4;
		v_total = n_clk / (hblank+video_width);
		h_total = hblank+video_width;
		v_start = hblank/2;
		v_start = (v_total-v_size)/2;
		v_sync_start = 0;
		v_sync_stop  = (v_total-v_size)/4;
		h_sync_start = 0;
		h_sync_stop  = hblank/4;
	}
	write_control(control_base,0x20004,(v_total<<16) | h_total);
	write_control(control_base,0x20008,(v_size<<16) | h_size);
	write_control(control_base,0x2000c,(v_start<<16) | h_start);
	write_control(control_base,0x20010,(v_sync_start<<16) | v_sync_stop);
	write_control(control_base,0x20014,(h_sync_start<<16) | h_sync_stop);
	write_control(control_base,0x20018,1);
	write_control(control_base,0x20018,0);
}
static void gen_fpga_vdma_addr()
{
	int i;
	unsigned long size;
	unsigned long size_align;
	size = video_height * video_width * 3;
	size_align = (size/VDMA_DATA_BYTES + ((size%VDMA_DATA_BYTES) == 0 ? 0 : 1))*VDMA_DATA_BYTES;
	for(i=0;i<VIDEO_FRAME_STORE_NUM;i++)
	{
		h2c_fpga_ddr_addr[i] = FPGA_DDR_START_ADDR+size_align*i;
		c2h_fpga_ddr_addr[i] = FPGA_DDR_START_ADDR+size_align*(VIDEO_FRAME_STORE_NUM+i);
	}
}
static void clear_mm2s_vdma_buffer()
{
	int i;
	for(i=0;i<VIDEO_FRAME_STORE_NUM;i++)
	{
		memset(h2c_align_mem,0,video_height*video_width*3);
		lseek(h2c_dma_fd,h2c_fpga_ddr_addr[i],SEEK_SET);
		write(h2c_dma_fd,h2c_align_mem,video_height*video_width*3);
	}
}
static void fill_mm2s_vdma_buffer(unsigned char *buffer)
{
	int i;
	int h,w;
	unsigned char val;
	for(h=0;h<video_height;h++)
		for(w=0;w<video_width;w++)
		{
			val = buffer[h*video_width*3+w*3];
			buffer[h*video_width*3+w*3] = buffer[h*video_width*3+w*3+2];
			buffer[h*video_width*3+w*3+2] = val;
		}
	for(i=0;i<VIDEO_FRAME_STORE_NUM;i++)
	{
		memcpy(h2c_align_mem,buffer,video_height*video_width*3);
		lseek(h2c_dma_fd,h2c_fpga_ddr_addr[i],SEEK_SET);
		write(h2c_dma_fd,h2c_align_mem,video_height*video_width*3);
	}
}
int wait_for_s2mm_intr()
{
	read_event(c2h_event_fd);
	write_control(control_base,S2MM_VDMA_BASE+VDMA_S2MM_SR,0xffffffff);
	return 1;
}
int wait_for_mm2s_intr()
{
	read_event(h2c_event_fd);
	write_control(control_base,MM2S_VDMA_BASE+VDMA_MM2S_SR,0xffffffff);
	return 1;
}
void set_saturation(unsigned char satu)
{
	write_control(control_base,0x20020,satu);
}

int disp_init(int *width,int *height,char *pic_name)
{
	ImageData image;
	read_image_file(pic_name,&image);
	
	video_height = image.height;
	video_width  = image.width;
	*width = video_width;
	*height = video_height;
	vtc_config(control_base,30,74.25);
	if(get_align_mem()<0)
		return -1;
	gen_fpga_vdma_addr();
	clear_mm2s_vdma_buffer();
	fill_mm2s_vdma_buffer(image.pixelArray);
	clear_image_data(&image);
	frame_cnt = 0;
	return 0;
}
void disp_deinit()
{
	vtc_stream_run(control_base,0,0);
	usleep(50000);
	reset_vdma(control_base);
	free(h2c_align_mem);
	frame_cnt = 0;
}
void disp_reset()
{
	vtc_stream_run(control_base,0,0);
	usleep(50000);
	reset_vdma(control_base);
	frame_cnt = 0;
}
void disp_start()
{
	frame_cnt = 0;
	config_mm2s_vdma(control_base,VIDEO_FRAME_STORE_NUM);//start
	config_s2mm_vdma(control_base,VIDEO_FRAME_STORE_NUM);//start
	usleep(1000);
	vtc_stream_run(control_base,1,1);
}
int pcie_init()
{
    c2h_dma_fd = open("/dev/xdma0_c2h_0",O_RDWR | O_NONBLOCK);
    if(c2h_dma_fd < 0)
        return -1;
    h2c_dma_fd = open("/dev/xdma0_h2c_0",O_RDWR );
    if(h2c_dma_fd < 0)
        return -2;
    c2h_event_fd = open_event("/dev/xdma0_events_1");
    if(c2h_event_fd < 0)
        return -3;
    h2c_event_fd = open_event("/dev/xdma0_events_0");
    if(h2c_event_fd < 0)
        return -4;
    control_fd = open_control("/dev/xdma0_user");
    if(control_fd < 0)
        return -5;
    control_base = mmap_control(control_fd,MAP_SIZE);
    posix_memalign((void *)&h2c_align_mem_tmp,4096,0x800000);
    posix_memalign((void *)&c2h_align_mem_tmp,4096,0x800000);
    if(NULL == h2c_align_mem_tmp || NULL == c2h_align_mem_tmp)
        return -6;
    return 1;
}
void pcie_deinit()
{
	close(c2h_dma_fd);
	close(h2c_dma_fd);
	close(c2h_event_fd);
	close(h2c_event_fd);
	close(control_fd);
	free(h2c_align_mem_tmp);
	free(c2h_align_mem_tmp);
}
unsigned int h2c_transfer(unsigned int size)
{
    double bd=0;
    long s=0;
    long ns=0;
	static struct timespec ts_start, ts_end;
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    lseek(h2c_dma_fd,FPGA_DDR_START_ADDR,SEEK_SET);
    write(h2c_dma_fd,h2c_align_mem_tmp,size);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    timespec_sub(&ts_end, &ts_start);
    s = ts_end.tv_sec;
    ns = ts_end.tv_nsec;
    bd = ((double)size)/(double)(s+(double)ns/1000000000.0)/1024.0/1024.0;
    
    return (unsigned int)bd;
}
unsigned int c2h_transfer(unsigned int size)
{
    double bd=0;
    long s=0;
    long ns=0;
	static struct timespec ts_start, ts_end;
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    lseek(c2h_dma_fd,FPGA_DDR_START_ADDR,SEEK_SET);
    read(c2h_dma_fd,c2h_align_mem_tmp,size);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    timespec_sub(&ts_end, &ts_start);
    s = ts_end.tv_sec;
    ns = ts_end.tv_nsec;
    bd = ((double)size)/(double)(s+(double)ns/1000000000.0)/1024.0/1024.0;
    
    return (unsigned int)bd;
}


#ifdef __cplusplus
}
#endif
