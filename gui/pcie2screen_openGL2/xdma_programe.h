#ifndef XDMA_PROGRAME_H
#define XDMA_PROGRAME_H

#include <pthread.h>
#include <sys/time.h>
#include <QtWidgets>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

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
#define VDMA_PARK_PTR   0x28

class xdma_programe
{
public:
    xdma_programe();
    ~xdma_programe();
    bool getDevice(void);
    int read_pack(unsigned int ddr_addr,char *pData, int len);

    int open_event(char *filename);
    int open_control(char *filename);
    void * mmap_control(int fd,long mapsize);
    int wait_for_s2mm_intr();
    int read_event(int fd);
    uint32_t read_control(void *base_addr,int offset);
    void write_control(void *base_addr,int offset,uint32_t val);
    uint32_t set_vdma_park_ptr();
    int c2h_event_fd;
    int control_fd;
    void *control_base;
    uint32_t ddr_frm_addr;

private:
    int dev_fd; // c2h_dma_fd
};

#endif // XDMA_PROGRAME_H
