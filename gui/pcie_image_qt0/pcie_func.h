
#ifndef PCIE_FUNC_H
#define PCIE_FUNC_H
#ifdef __cplusplus
 extern "C" {
 #endif
void put_pic_to_sys_memory(char *path);
int pcie_init();
void pcie_deinit();
unsigned int h2c_transfer(unsigned int size);
unsigned int c2h_transfer(unsigned int size);
int wait_for_s2mm_intr();
int wait_for_mm2s_intr();
int disp_init(int *width,int *height,char *pic_name);
void disp_deinit();
void disp_start();
void disp_reset();
int pcie_read_frame(unsigned char *buffer);
void set_saturation(unsigned char satu);
#ifdef __cplusplus
}
#endif
#endif

