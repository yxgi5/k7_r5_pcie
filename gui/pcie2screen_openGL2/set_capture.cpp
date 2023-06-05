#include <unistd.h>
#include "xdma_programe.h"
#include "set_capture.h"

xdma_getImg::xdma_getImg(xdma_programe *pXdma, char *pRGB, pthread_mutex_t *pOptNotice)
{
    stateWorking = false;
    this->pRGB = pRGB;
    this->pXdma = pXdma;
    this->pOptNotice = pOptNotice;
    start();
}

void xdma_getImg::run()
{
    int i;

    while(1)
    {
        if(stateWorking)
        {
            pthread_mutex_lock(pOptNotice);
            pXdma->wait_for_s2mm_intr();
            pXdma->set_vdma_park_ptr();
            i = pXdma->read_pack(pXdma->ddr_frm_addr, pRGB, 1920*1080*3);
            if(i != 1920*1080*3)
            {
                qDebug("read one pack error %d", i);
            }
            emit flushImg();
        }
        else
        {
            usleep(100000);
        }

    }
}

void xdma_getImg::setstart(bool isRuning)
{
    stateWorking = isRuning;
}
