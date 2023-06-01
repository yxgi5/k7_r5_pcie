#include <unistd.h>
#include "xdma_programe.h"
#include <assert.h>
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

#define DEVICE_NAME_READ "/dev/xdma0_c2h_0"
#define DEVICE_NAME_WRITE "/dev/xdma0_h2c_0"
#define DEVICE_NAME_CONTROL "/dev/xdma0_control"
#define DEVICE_NAME_USER "/dev/xdma0_user"
xdma_programe::xdma_programe()
    :QThread()
{
    memset(devReadHandle, 0, sizeof(devReadHandle));
    memset(devWriteHandle, 0, sizeof(devWriteHandle));
    pReadyData = NULL;
}

xdma_programe::~xdma_programe()
{
    for(int i=0;i<3;i++)
    {
        if(devReadHandle[i] != -1)
        {
            close(devReadHandle[i]);
        }
        if(devWriteHandle[i] != -1)
        {
            close(devWriteHandle[i]);
        }
    }
}

int xdma_programe::read_pack(int hand, char *pData, int len, unsigned int offset)
{
	if(hand != -1)
    {
        lseek(hand, offset, SEEK_SET);
        read(hand, pData, len);
        return len;
    }
    else
    {
        return 0;
    }
}


int xdma_programe::write_pack(int hand, char *pData, int len, unsigned int offset)
{
    char buff[1920*200*4];
    lseek(hand, offset, SEEK_SET);
    for(int i=0,n;i<len;i+=n)
    {
        n = ((len-i)>=(int)sizeof(buff)) ? (int)sizeof(buff) : (len-i);
        memcpy(buff, pData+i, n);
        write(hand, buff, n);
    }
    return len;
}

void xdma_programe::run()
{
    while(1)
    {
        if(pReadyData)
        {
            if(mode < 3)
            {
                read_pack(devReadHandle[mode], pReadyData, pReadyLen, pReadyOffset);
                emit opt_end(0);
            }
            else
            {
                write_pack(devWriteHandle[mode-3], pReadyData, pReadyLen, pReadyOffset);
                emit opt_end(1);
            }
            pReadyData = NULL;
        }
        usleep(10000);
    }
}

void xdma_programe::opt_pack(int mode, char *pData, int len, unsigned int offset)
{
    this->mode = mode;
    this->pReadyLen = len;
    this->pReadyOffset = offset;
    this->pReadyData = pData;
}


bool xdma_programe::getDevice()
{
    bool b = false;

    for(int i=0;i<3;i++)
    {
        if(devReadHandle[i] != -1)
        {
            close(devReadHandle[i]);
        }
        if(devWriteHandle[i] != -1)
        {
            close(devWriteHandle[i]);
        }
    }
    memset(devReadHandle, 0, sizeof(devReadHandle));
    memset(devWriteHandle, 0, sizeof(devWriteHandle));
    //获取设备路径
	
    if(devReadHandle[0] == 0)
	{
		devReadHandle[0] = open(DEVICE_NAME_CONTROL, O_RDWR | O_SYNC);
        printf("devReadHandle[0] = %d\n",devReadHandle[0]);
		devWriteHandle[0] = devReadHandle[0];
		if(devReadHandle[0] == -1)
		{
			b = false;
		}
        else
        {
            b = true;
        }
	}
    if(devReadHandle[1] == 0)
	{
		devReadHandle[1] = open(DEVICE_NAME_USER, O_RDWR | O_SYNC);
        printf("devReadHandle[1] = %d\n",devReadHandle[1]);
		devWriteHandle[1] = devReadHandle[1];
		if(devReadHandle[1] == -1)
		{
			b = false;
		}
        else
        {
            b = true;
        }
	}
    if(devReadHandle[2] == 0)
	{
		devReadHandle[2] = open(DEVICE_NAME_READ, O_RDWR | O_NONBLOCK);
        printf("devReadHandle[2] = %d\n",devReadHandle[2]);
		if(devReadHandle[2] == -1)
		{
			b = false;
        }
        else
        {
            b = true;
        }
	}
    if(devWriteHandle[2] == 0)
	{
		devWriteHandle[2] = open(DEVICE_NAME_WRITE, O_RDWR | O_NONBLOCK);
        printf("devWriteHandle[2] = %d\n",devWriteHandle[2]);
		if(devWriteHandle[2] == -1)
		{
			b = false;
		}
        else
        {
            b = true;
        }
	}	

    return b;
}


