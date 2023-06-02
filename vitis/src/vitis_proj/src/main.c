/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"
#ifndef __PPC__
#include "xil_printf.h"
#endif
#include "xstatus.h"
#include "xil_types.h"
#if defined (ARMR5) || (__aarch64__) || (__arm__)
#include "xscugic.h"
#else
#include "xintc.h"
#endif
#include "xil_exception.h"
#include "xil_cache.h"
#include "xvidc.h"
#include "tpg/tpg.h"
#include "clk_wiz/clk_wiz.h"
#include "xvtc.h"
#include "vtc/video_resolution.h"
#include "vtc/vtiming_gen.h"
#include "xaxivdma.h"
#include "xgpio.h"
#include "sleep.h"
#include "bitmanip.h"
#include "trace_zzg_debug.h"

#define VDMA_ID          		XPAR_AXIVDMA_0_DEVICE_ID
#define VDMA_BASE_ADDR 			0x08000000
//#define VDMA1_ID          		XPAR_AXIVDMA_1_DEVICE_ID
//#define VDMA1_BASE_ADDR			0x10000000

//#define FRAME_BUFFER_BASE_ADDR  0x10000000 // for zynq
////#define FRAME_BUFFER_BASE_ADDR  0x81000000 // for microblaze

#if defined (__MICROBLAZE__)
	#define DDR_BASEADDR XPAR_MICROBLAZE_DCACHE_BASEADDR
#else
	#define DDR_BASEADDR XPAR_DDR_MEM_BASEADDR
#endif

#define FRAME_BUFFER_BASE_ADDR  	(DDR_BASEADDR + (0x10000000))
//#define FRAME_BUFFER_BASE_ADDR  	(DDR_BASEADDR + (512 * 1024 * 1024))
//#define FRAME_BUFFER_BASE_ADDR	0x10000000
//#define FRAME_BUFFER_BASE_ADDR	0x81000000
//#define FRAME_BUFFER_SIZE		0x400000	//0x400000 for max 1080p YCbCr422 8bpc
//#define FRAME_BUFFER_SIZE		0x600000	//0x600000 for max 1080p RGB888 8bpc
#define FRAME_BUFFER_SIZE       0x2000000    //0x2000000 for max 4KW RGB888 8bpc
#define FRAME_BUFFER_1			FRAME_BUFFER_BASE_ADDR
#define FRAME_BUFFER_2			FRAME_BUFFER_BASE_ADDR + FRAME_BUFFER_SIZE
#define FRAME_BUFFER_3			FRAME_BUFFER_BASE_ADDR + (FRAME_BUFFER_SIZE*2)

#define FRAME_BUFFER_BASE_ADDR1	FRAME_BUFFER_BASE_ADDR + (FRAME_BUFFER_SIZE*3)
#define FRAME_BUFFER_4			FRAME_BUFFER_BASE_ADDR1
#define FRAME_BUFFER_5			FRAME_BUFFER_BASE_ADDR1 + FRAME_BUFFER_SIZE
#define FRAME_BUFFER_6			FRAME_BUFFER_BASE_ADDR1 + (FRAME_BUFFER_SIZE*2)

#define  XGPIO_ID  XPAR_AXI_GPIO_0_DEVICE_ID
XGpio XGpioInst;
XClk_Wiz ClkWizInst0;
XVtc        VtcInst0;       /**< Instance of the VTC core. */
XVtc_Config *VtcConfig0;
XV_tpg tpg_inst0;
XV_tpg_Config *tpg_config0;
u32 bckgndId0=XTPG_BKGND_COLOR_BARS;
XVidC_ColorFormat colorFmtIn0 = XVIDC_CSF_RGB;
XVidC_ColorFormat colorFmtOut0 = XVIDC_CSF_RGB;

//XGPIO初始化
int xgpio_init(void)
{
	int Status ;

//	u32 ret;

	Status = XGpio_Initialize(&XGpioInst, XGPIO_ID) ;
	if (Status != XST_SUCCESS)
	{
		return XST_FAILURE ;
	}

	/* set as output */
	//设置 gpio端口 为输出
//	XGpio_DiscreteWrite(&XGpioInst, 1, 0xfff);
	XGpio_SetDataDirection(&XGpioInst, 1, 0xf0);
	XGpio_DiscreteWrite(&XGpioInst, 1, 0xf0);
//	XGpio_SetDataDirection(&XGpioInst, 2, 0x0);
//	XGpio_DiscreteWrite(&XGpioInst, 2, 1);

	return XST_SUCCESS ;
}

//void clear_vdma(void)
//{
//	u32 bytePerPixels = 3;
//	u32 line = 0;
//	u32 column = 0;
//
//	line = 1920;
//	column = 1080;
//
//    Xil_Out32(XPAR_AXIVDMA_1_BASEADDR + 0x00, 0x8A);//stop mm2s
//	Xil_Out32(XPAR_AXIVDMA_1_BASEADDR + 0x30, 0x8A);//stop s2mm
//
//	Xil_DCacheDisable();
//    memset(FRAME_BUFFER_1,0xff,line*column*bytePerPixels);//background
//    memset(FRAME_BUFFER_2,0xff,line*column*bytePerPixels);//background
//    memset(FRAME_BUFFER_3,0xff,line*column*bytePerPixels);//background
//	Xil_DCacheEnable();
//
//	xil_printf("clear vdma Done\n\r");
//	vdma_config();
//}
//
//void clear_display(void)
//{
//    //print("Initialize frame buffer data...");
//	//Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x00, 0x8A);
//    //Disable the data cache to initialize the frame buffer with a blue color
////    Xil_DCacheDisable();
//    UINTPTR Addr1=FRAME_BUFFER_1,Addr2=FRAME_BUFFER_2,Addr3=FRAME_BUFFER_3;
////    UINTPTR Addr4=FRAME_BUFFER_4,Addr5=FRAME_BUFFER_5,Addr6=FRAME_BUFFER_6;
//    int line, column;
//	for(line=0; line < 2160; line++)
//	{
//		for(column=0; column < 3840*3/4; column++)
//		{
//			Xil_Out32(Addr1, 0xFFFFFFFF);
//			Xil_Out32(Addr2, 0xFFFFFFFF);
//			Xil_Out32(Addr3, 0xFFFFFFFF);
//
//			Addr1+=4;
//			Addr2+=4;
//			Addr3+=4;
//		}
//	}
////	memset((void*)Addr4, 0, 6220800);
////	memset((void*)Addr5, 0, 6220800);
////	memset((void*)Addr6, 0, 6220800);
////	print_no_link_notify();
//	//vdma_config_blank();
//    //Re-enable the data cache
////	Xil_DCacheEnable();
//	Xil_DCacheFlush();
////	print("Done\n\r");
//}

void clear_display(void)
{
	u32 bytePerPixels = 3;
	u32 line = 0;
	u32 column = 0;

	line = 1920;
	column = 1080;

	Xil_DCacheDisable();
    memset((void *)FRAME_BUFFER_1,0xff,line*column*bytePerPixels);//background
//    memset(FRAME_BUFFER_2,0xff,line*column*bytePerPixels);//background
//    memset(FRAME_BUFFER_3,0xff,line*column*bytePerPixels);//background
	Xil_DCacheEnable();
}

//void vdma_config_32(void)
//{
//    /* Start of VDMA Configuration */
//    u32 bytePerPixels = 3;
//
//    int offset0 = 0; // (y*w+x)*Bpp
//    int offset1 = 0; // (y*w+x)*Bpp
//
//    u32 stride0 = 1920;
//    u32 width0 = 1920;
//    u32 height0 = 1080;
//    u32 stride1 = 1920;  // crop keeps write Stride
//    u32 width1 = 1920;
//    u32 height1 = 1080;
//
//    /* Configure the Write interface (S2MM)*/
//    // S2MM Control Register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x30, 0x8B);
//    //S2MM Start Address 1
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xAC, FRAME_BUFFER_1 + offset0);
//    //S2MM Start Address 2
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xB0, FRAME_BUFFER_2 + offset0);
//    //S2MM Start Address 3
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xB4, FRAME_BUFFER_3 + offset0);
//    //S2MM Frame delay / Stride register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xA8, stride0*bytePerPixels);
//    // S2MM HSIZE register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xA4, width0*bytePerPixels);
//    // S2MM VSIZE register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xA0, height0);
//
//    /* Configure the Read interface (MM2S)*/
//    // MM2S Control Register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x00, 0x8B);
//    // MM2S Start Address 1
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x5C, FRAME_BUFFER_1 + offset1);
//    // MM2S Start Address 2
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x60, FRAME_BUFFER_2 + offset1);
//    // MM2S Start Address 3
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x64, FRAME_BUFFER_3 + offset1);
//    // MM2S Frame delay / Stride register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x58, stride1*bytePerPixels);
//    // MM2S HSIZE register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x54, width1*bytePerPixels);
//    // MM2S VSIZE register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x50, height1);
//
//    //xil_printf("VDMA started!\r\n");
//    /* End of VDMA Configuration */
//}

void vdma0_config_32(void)
{
    /* Start of VDMA Configuration */
    u32 bytePerPixels = 3;

    int offset0 = 0; // (y*w+x)*Bpp
//    int offset1 = 0; // (y*w+x)*Bpp

    u32 stride0 = 1920;
    u32 width0 = 1920;
    u32 height0 = 1080;
//    u32 stride1 = 1920;  // crop keeps write Stride
//    u32 width1 = 1920;
//    u32 height1 = 1080;

    /* Configure the Write interface (S2MM)*/
    // S2MM Control Register
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x30, 0x8B);
    //S2MM Start Address 1
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xAC, FRAME_BUFFER_1 + offset0);
    //S2MM Start Address 2
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xB0, FRAME_BUFFER_2 + offset0);
    //S2MM Start Address 3
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xB4, FRAME_BUFFER_3 + offset0);
    //S2MM Frame delay / Stride register
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xA8, stride0*bytePerPixels);
    // S2MM HSIZE register
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xA4, width0*bytePerPixels);
    // S2MM VSIZE register
    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0xA0, height0);

//    /* Configure the Read interface (MM2S)*/
//    // MM2S Control Register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x00, 0x8B);
//    // MM2S Start Address 1
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x5C, FRAME_BUFFER_1 + offset1);
//    // MM2S Start Address 2
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x60, FRAME_BUFFER_2 + offset1);
//    // MM2S Start Address 3
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x64, FRAME_BUFFER_3 + offset1);
//    // MM2S Frame delay / Stride register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x58, stride1*bytePerPixels);
//    // MM2S HSIZE register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x54, width1*bytePerPixels);
//    // MM2S VSIZE register
//    Xil_Out32(XPAR_AXI_VDMA_0_BASEADDR + 0x50, height1);

    //xil_printf("VDMA started!\r\n");
    /* End of VDMA Configuration */
}

void vdma_config(void)
{
	vdma0_config_32();
}

void clkwiz_vtc_cfg(void)
{
    u32 Status;

    // dynamic config clkwiz
    Status = XClk_Wiz_dynamic_reconfig(&ClkWizInst0, XPAR_CLK_WIZ_0_DEVICE_ID);
    if (Status != XST_SUCCESS)
    {
      xil_printf("XClk_Wiz0 dynamic reconfig failed.\r\n");
//      return XST_FAILURE;
    }
    xil_printf("XClk_Wiz0 dynamic reconfig ok\n\r");

    // vtc configuration
    VtcConfig0 = XVtc_LookupConfig(XPAR_VTC_0_DEVICE_ID);
    Status = XVtc_CfgInitialize(&VtcInst0, VtcConfig0, VtcConfig0->BaseAddress);
    if(Status != XST_SUCCESS)
    {
        xil_printf("VTC0 Initialization failed %d\r\n", Status);
//      return(XST_FAILURE);
    }

    vtiming_gen_run(&VtcInst0, VIDEO_RESOLUTION_1080P, 2);
}

void tpg_config()
{
    u32 Status;

    // tpg0
    xil_printf("TPG0 Initializing\n\r");

    Status = XV_tpg_Initialize(&tpg_inst0, XPAR_V_TPG_0_DEVICE_ID);
    if(Status!= XST_SUCCESS)
    {
        xil_printf("TPG0 configuration failed\r\n");
//      return(XST_FAILURE);
    }

    tpg_cfg(&tpg_inst0, 1080, 1920, colorFmtIn0, bckgndId0);

    //Configure the moving box of the TPG0
    tpg_box(&tpg_inst0, 50, 1);

    //Start the TPG0
    XV_tpg_EnableAutoRestart(&tpg_inst0);
    XV_tpg_Start(&tpg_inst0);
    xil_printf("TPG0 started!\r\n");
}

int main()
{
    init_platform();
    xgpio_init();
    print("\n\r");
    print("pcie c2h test\n\r");
    clkwiz_vtc_cfg();
    tpg_config();
//    clear_display();
    vdma_config();

	while(1)
	{
		XGpio_DiscreteWrite(&XGpioInst, 1, 0xf0);
		usleep(1000000);
		XGpio_DiscreteWrite(&XGpioInst, 1, 0xf1);
		usleep(1000000);
		XGpio_DiscreteWrite(&XGpioInst, 1, 0xf2);
		usleep(1000000);
		XGpio_DiscreteWrite(&XGpioInst, 1, 0xf4);
		usleep(1000000);
		XGpio_DiscreteWrite(&XGpioInst, 1, 0xf8);
		usleep(1000000);
	}

    cleanup_platform();
    return 0;
}
