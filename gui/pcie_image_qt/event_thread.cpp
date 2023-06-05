#include "pcie_func.h"
#include "pcie_image_qt.h"

event_thread::event_thread()
{

}
event_thread::~event_thread()
{
    // close event
    //close_event(pcie_event_fd);
}
/*
void event_thread::pcie_event_open(char *event_dev)
{
    // open event
    //pcie_event_fd = open_event(event_dev);
}

void event_thread::pcie_event_close()
{
    // close event
    //close_event(pcie_event_fd);
}
*/

void event_thread::run()
{
    while(1)
    {
		wait_for_s2mm_intr();
		emit sig_inform_disp();
        /*
            read_event(pcie_event_fd);
            clear_vdma_irq(pcie_ctrl_addr,dev_index);
            fpga_dma_wr_ptr = (fpga_dma_wr_ptr+1)%16;
            frame_cnt--;
            if(frame_cnt == 0)
            {
                emit sig_inform_disp();
                frame_cnt = cmv_fps/disp_fps;
            }
            */
    }
}
