#include <opencv2/opencv.hpp>
#include "framework.h"
#include "LVopenCVcamDLL.h"

using namespace cv;
using namespace std;

static VideoCapture cap;
static Mat frame;

LVOPENCVCAMDLL_API int fnOpenCam(int CamIdx)
{
    VideoCapture video(CamIdx);
    cap = video;

    return cap.isOpened()? 0:-1;
}

LVOPENCVCAMDLL_API int fnGetSize(int *Width, int *Height)
{
   *Width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
   *Height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);

    return 0;
}

LVOPENCVCAMDLL_API int fnGetFrame(uint8_t* dst, int LVWidth, int LVHeight)
{
    if(!cap.read(frame) || !dst) return -1;

    // Copy, line by line, the frame into the LabVIEW Arrray (must be allocated outside)
    for (int y = 0; y < LVHeight; ++y) {
        memcpy(dst + y * LVWidth * 3, (uint8_t*)frame.data + y * LVWidth * 3, LVWidth * sizeof(uint8_t) * 3);
    }

    return 0;
}

LVOPENCVCAMDLL_API void fnCloseCam()
{
    cap.release();
}
