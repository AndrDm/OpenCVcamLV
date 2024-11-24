#include <opencv2/opencv.hpp>
#include <cintools/extcode.h>
#include "framework.h"
#include "LVopenCVcamDLL.h"

using namespace cv;
using namespace std;

static VideoCapture cap;
static Mat frame;
static int width, height;


LVOPENCVCAMDLL_API void fnOpenCam(int CamIdx, LVError* Error)
{
	if (Error->status) return;
	VideoCapture video(CamIdx);

	if (video.isOpened()) {
		cap = video;
		width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
		height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
	}
	else SetError(Error, true, OCV_CAM_OPEN_ERROR, "Unable to open camera");	   
}


LVOPENCVCAMDLL_API int fnGetSize(int *Width, int *Height)
{
	*Width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
	*Height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);

	return 0;
}


LVOPENCVCAMDLL_API void fnGetFrame(ImageArrHdl image, LVError* Error)
{
	if (Error->status) return;
	if (!cap.read(frame)) { SetError(Error, true, OCV_FRAME_ERROR, "Unable to read frame"); return;};
	if (!image) { SetError(Error, true, OCV_NULL_POINTER, "Invalid image (null pointer)"); return; };
	MgErr err = NumericArrayResize(uL, 2, (UHandle*)&(image), width * height);
	if (err) { SetError(Error, true, err, "NumericArrayResize() Error"); return; };
	(*(image))->dimSizes[0] = height; // rows;
	(*(image))->dimSizes[1] = width; // cols;
	
	uint32_t* pixel_out = (*(image))->pixel;
	uint8_t* pixel_in = frame.data;
	if (!pixel_in || !pixel_out) { SetError(Error, true, OCV_NULL_POINTER, "Null pointer"); return; };

	// Copy, pixel by pixel, the frame into the LabVIEW Arrray
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {				
			*pixel_out++ = (*pixel_in << 16) | (*(pixel_in + 1) << 8) | (*(pixel_in + 2));
			pixel_in += 3;
		}
	}
}


LVOPENCVCAMDLL_API void fnCloseCam()
{
	cap.release();
}

LVOPENCVCAMDLL_API MgErr fnCloseCamCallBack(InstanceDataPtr *instanceState)
{
	OutputDebugStringA("OCV: abort - closing camera");
	fnCloseCam();
	OutputDebugStringA("OCV: camera closed");
	return noErr;
}


LVOPENCVCAMDLL_API void fnSettings()
{
	//doesn't work on my laptop :(
	cap.set(CAP_PROP_SETTINGS, 1);
}