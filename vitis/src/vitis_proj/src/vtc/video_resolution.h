#ifndef __VIDEO_RESOLUTION_H__
#define __VIDEO_RESOLUTION_H__

//#include "xbasic_types.h"
#include "xil_types.h"

// Video Pattern Generator - Video Resolution values
//#define VIDEO_RESOLUTION_VGA       0
//#define VIDEO_RESOLUTION_480P      1
//#define VIDEO_RESOLUTION_576P      2
//#define VIDEO_RESOLUTION_SVGA      3
//#define VIDEO_RESOLUTION_XGA       4
//#define VIDEO_RESOLUTION_720P      5
//#define VIDEO_RESOLUTION_SXGA      6
//#define VIDEO_RESOLUTION_1080P     7
//#define VIDEO_RESOLUTION_UXGA      8
//#define NUM_VIDEO_RESOLUTIONS      9

typedef enum video_resolution_no
{
	VIDEO_RESOLUTION_VGA = 0,
	VIDEO_RESOLUTION_480P,
	VIDEO_RESOLUTION_576P,
	VIDEO_RESOLUTION_SVGA,
	VIDEO_RESOLUTION_XGA,
	VIDEO_RESOLUTION_720P,
	VIDEO_RESOLUTION_SXGA,
	VIDEO_RESOLUTION_1080P,
	VIDEO_RESOLUTION_FHDPLUS,
	VIDEO_RESOLUTION_UXGA,
	VIDEO_RESOLUTION_1920X960,
	VIDEO_RESOLUTION_4K,//4K30
	VIDEO_RESOLUTION_4KFULL,//4K30W
	VIDEO_RESOLUTION_CUSTOM1,
	VIDEO_RESOLUTION_4KCUST,
	VIDEO_RESOLUTION_BAIDUSF,

	VIDEO_RESOLUTION_NO_BUTT,
} resolution_no;

struct struct_vres_timing_t
{
	char *pName;
	u32 VActiveVideo;
	u32 VFrontPorch;
	u32 VSyncWidth;
	u32 VBackPorch;
	u32 VSyncPolarity;
	u32 HActiveVideo;
	u32 HFrontPorch;
	u32 HSyncWidth;
	u32 HBackPorch;
	u32 HSyncPolarity;
};
typedef struct struct_vres_timing_t vres_timing_t;

char *  vres_get_name(u32 resolutionId);
u32 vres_get_width(u32 resolutionId);
u32 vres_get_height(u32 resolutionId);
u32 vres_get_timing(u32 resolutionId, vres_timing_t *pTiming );

s32 vres_detect( u32 width, u32 height );


#endif // __VIDEO_RESOLUTION_H__
