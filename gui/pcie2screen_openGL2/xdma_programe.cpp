#include "xdma_programe.h"
#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


//#define DEVICE_NAME_DEFAULT "/dev/xdma/card0/c2h0"
#define DEVICE_NAME_DEFAULT "/dev/xdma0_c2h_0"
//#define IMG_RAM_POS     (512*1024*1024)
//#define IMG_RAM_POS (0x10000000)

xdma_programe::xdma_programe()
{
    dev_fd = -1;
}

xdma_programe::~xdma_programe()
{
    if(dev_fd != -1)
    {
        close(dev_fd);
    }
}

int xdma_programe::read_pack(unsigned int ddr_addr, char *pData, int len)
{
    if(dev_fd != -1)
    {
        lseek(dev_fd, ddr_addr, SEEK_SET);
        read(dev_fd, pData, len);
        return len;
    }
    else
    {
        return 0;
    }
}

bool xdma_programe::getDevice()
{

    if(dev_fd == -1)
    {
        dev_fd = open(DEVICE_NAME_DEFAULT, O_RDWR | O_NONBLOCK);
    }
    if(dev_fd == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int xdma_programe::open_event(char *filename)
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

int xdma_programe::open_control(char *filename)
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

void * xdma_programe::mmap_control(int fd,long mapsize)
{
    void *vir_addr;
    vir_addr = mmap(0, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    return vir_addr;
}

int xdma_programe::read_event(int fd)
{
    int val;
    read(fd,&val,4);
    return val;
}

uint32_t xdma_programe::read_control(void *base_addr,int offset)
{
    uint32_t read_result = *((uint32_t *)(base_addr+offset));
    read_result = ltohl(read_result);
    return read_result;
}

void xdma_programe::write_control(void *base_addr,int offset,uint32_t val)
{
    uint32_t writeval = htoll(val);
    *((uint32_t *)(base_addr+offset)) = writeval;
}

int xdma_programe::wait_for_s2mm_intr()
{
    this->read_event(this->c2h_event_fd);
    this->write_control(this->control_base,S2MM_VDMA_BASE+VDMA_S2MM_SR,0xffffffff);
    return 1;
}

uint32_t xdma_programe::set_vdma_park_ptr()
{
    uint32_t writeval = this->read_control(this->control_base, S2MM_VDMA_BASE+VDMA_PARK_PTR);
    uint32_t current_ptr = (writeval & 0x00001f00) >> 8;

    if(current_ptr == 0)
    {
        this->ddr_frm_addr = 0x10000000;
        this->write_control(this->control_base, S2MM_VDMA_BASE+VDMA_PARK_PTR, 0x00000100);
    }
    else if(current_ptr == 1)
    {
        this->ddr_frm_addr = 0x12000000;
        this->write_control(this->control_base, S2MM_VDMA_BASE+VDMA_PARK_PTR, 0x00000200);
    }
    else if(current_ptr == 2)
    {
        this->ddr_frm_addr = 0x14000000;
        this->write_control(this->control_base, S2MM_VDMA_BASE+VDMA_PARK_PTR, 0x00000000);
    }
    else
    {
        this->ddr_frm_addr = 0x10000000;
        this->write_control(this->control_base, S2MM_VDMA_BASE+VDMA_PARK_PTR, 0x00000100);
    }

    return current_ptr;
}

