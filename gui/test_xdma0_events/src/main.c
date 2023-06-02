/*
 ============================================================================
 Name        : hello.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>
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
#define MAP_SIZE (1024*1024*1024UL)
#define S2MM_VDMA_BASE 0x04A30000
#define VIDEO_FRAME_STORE_NUM 1
#define VDMA_S2MM_CR 0x30
#define VDMA_S2MM_SR 0x34
#define VDMA_S2MM_ADDR1 0xAC
#define VDMA_S2MM_ADDR2 0xB0
#define VDMA_S2MM_ADDR3 0xB4
#define VDMA_S2MM_HSIZE 0xA8
#define VDMA_S2MM_STRID 0xA4
#define VDMA_S2MM_VSIZE 0xA0

#define VIDEO_STREAM_DDR_PHY_START_ADDR 0x90000000

static int video_height;
static int video_width;

static int read_event(int fd)
{
    int val;
    read(fd,&val,4);
    return val;
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

static void *mmap_control(int fd,long mapsize)
{
    void *vir_addr;
    vir_addr = mmap(0, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    return vir_addr;
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

static void config_s2mm_vdma(void *base_addr,int num_frame)
{
    int i;
    write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_CR,0x4);
    usleep(1000);
    write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_CR,0x01011089);
    write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_HSIZE,video_width*3);
    write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_STRID,video_width*3);
    //write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_VSIZE,video_height);
    for(i=0;i<num_frame;i++)
        write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_ADDR1+i*0x4,VIDEO_STREAM_DDR_PHY_START_ADDR+i*0x20000000);
    write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_SR,0xffffffff);
    write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_VSIZE,video_height);
    //write_control(base_addr,MASTER_VDMA_BASE+VDMA_S2MM_CR,0x0101100b);
}

static void stop_vdma(void *base_addr)
{
    uint32_t ret;

    ret = read_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_CR);
    write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_CR,ret&~(0x00000001));
    write_control(base_addr,S2MM_VDMA_BASE+VDMA_S2MM_CR,0x4);
//    write_control(base_addr,MM2S_VDMA_BASE+VDMA_MM2S_CR,0x4);
    usleep(100000);
}

int main(int argc, char* argv[])
{
//    printf ("Hello World!\n");
//    puts ("!!!Hello World!!!"); /* prints !!!Hello World!!! */
    int ret;
    int event_fd;
    event_fd = open_event("/dev/xdma0_events_0");
    while(1)
    {
        ret = read_event(event_fd);
        printf("ret = %d\n", ret);
    }

#if 0
    int control_fd;
    void *control_base;
    video_height = 1080;
    video_width = 1920;

    control_fd = open_control("/dev/xdma0_user");
    control_base = mmap_control(control_fd,MAP_SIZE);
    ret = read_control(control_base,S2MM_VDMA_BASE+VDMA_S2MM_CR);
    printf("VDMA_S2MM_CR = 0x%08x\n", ret);
    ret = read_control(control_base,S2MM_VDMA_BASE+VDMA_S2MM_SR);
    printf("VDMA_S2MM_SR = 0x%08x\n", ret);
    ret = read_control(control_base,S2MM_VDMA_BASE+VDMA_S2MM_ADDR1);
    printf("VDMA_S2MM_ADDR1 = 0x%08x\n", ret);
    ret = read_control(control_base,S2MM_VDMA_BASE+VDMA_S2MM_ADDR2);
    printf("VDMA_S2MM_ADDR2 = 0x%08x\n", ret);
    ret = read_control(control_base,S2MM_VDMA_BASE+VDMA_S2MM_ADDR3);
    printf("VDMA_S2MM_ADDR3 = 0x%08x\n", ret);
    ret = read_control(control_base,S2MM_VDMA_BASE+VDMA_S2MM_HSIZE);
    printf("VDMA_S2MM_HSIZE = 0x%08x\n", ret);
    ret = read_control(control_base,S2MM_VDMA_BASE+VDMA_S2MM_STRID);
    printf("VDMA_S2MM_STRID = 0x%08x\n", ret);
    ret = read_control(control_base,S2MM_VDMA_BASE+VDMA_S2MM_VSIZE);
    printf("VDMA_S2MM_VSIZE = 0x%08x\n", ret);

    //write_control(control_base, S2MM_VDMA_BASE+VDMA_S2MM_CR, 0x01011088);
    stop_vdma(control_base);
    config_s2mm_vdma(control_base,VIDEO_FRAME_STORE_NUM);
#endif

    return EXIT_SUCCESS;
}
