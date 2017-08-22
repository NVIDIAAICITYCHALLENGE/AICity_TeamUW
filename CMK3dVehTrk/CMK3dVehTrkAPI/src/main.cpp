// main.cpp
//

#include "cvut.h"
#include "LogTool.h"
#include "IndVehicleTracking.h"
#include <direct.h>


vector<CvScalar> gColorBox;
void initgColorBox()
{
	gColorBox.clear();
	gColorBox.push_back(cvScalar(255,0,0,0));
	gColorBox.push_back(cvScalar(0,255,0,0));
	gColorBox.push_back(cvScalar(0,0,255,0));		
	gColorBox.push_back(cvScalar(255,255,0,0));
	gColorBox.push_back(cvScalar(255,0,255,0));
	gColorBox.push_back(cvScalar(0,255,255,0));
	gColorBox.push_back(cvScalar(0,0,0,0));
	gColorBox.push_back(cvScalar(255,255,255,0));

	gColorBox.push_back(cvScalar(32,64,255,0));
	gColorBox.push_back(cvScalar(0,255,128,0));
	gColorBox.push_back(cvScalar(128,0,255,0));		
	gColorBox.push_back(cvScalar(255,255,128,0));
	gColorBox.push_back(cvScalar(255,128,255,0));
	gColorBox.push_back(cvScalar(64,150,255,0));
	gColorBox.push_back(cvScalar(128,0,32,0));
	gColorBox.push_back(cvScalar(255,128,0,0));

	gColorBox.push_back(cvScalar(100,160,40,0));
	gColorBox.push_back(cvScalar(40,160,100,0));
	gColorBox.push_back(cvScalar(160,40,100,0));		
	gColorBox.push_back(cvScalar(160,100,40,0));
	gColorBox.push_back(cvScalar(32,75,220,0));
	gColorBox.push_back(cvScalar(220,32,75,0));
	gColorBox.push_back(cvScalar(75,220,32,0));
	gColorBox.push_back(cvScalar(111,111,111,0));

	gColorBox.push_back(cvScalar(255,0,0,0));
	gColorBox.push_back(cvScalar(0,255,0,0));
	gColorBox.push_back(cvScalar(0,0,255,0));		
	gColorBox.push_back(cvScalar(255,255,0,0));
	gColorBox.push_back(cvScalar(255,0,255,0));
	gColorBox.push_back(cvScalar(0,255,255,0));
	gColorBox.push_back(cvScalar(0,0,0,0));
	gColorBox.push_back(cvScalar(255,255,255,0));
}

BYTE* getImageData(IplImage* img)
{
	BYTE* pbRGBdata = NULL;
	pbRGBdata = new BYTE[img->width*img->height*3];
	if (!pbRGBdata) return NULL;

	for (int j = 0; j < img->height; j++) {
		for (int i = 0; i < img->width; i++) {
			CvScalar pxl = cvGet2D(img, j, i);;
			pbRGBdata[(j*img->width+i)*3]   = BYTE(pxl.val[2]);
			pbRGBdata[(j*img->width+i)*3+1] = BYTE(pxl.val[1]);
			pbRGBdata[(j*img->width+i)*3+2] = BYTE(pxl.val[0]);
		}
	}

	return pbRGBdata;
}

void drawShapeModel(IplImage** pImg, int id, CShapeModel model)
{
	CvScalar color = gColorBox[id%32];
	for (int p = 0; p < model.getPointNum(); p++) {
		cvCircle(*pImg, cvPointFrom32f(model.getPoint(p)), 2, color, 1);
	}

	for (int l = 0; l < model.getLineNum(); l++) {
		CvPoint start = cvPointFrom32f(model.getLine(l).start),
		        end = cvPointFrom32f(model.getLine(l).end);
		cvLine(*pImg, start, end, color, 2);
	}
}

void saveFrame(IplImage* frame, int frame_cnt)
{
	char name[20] = {0};
	sprintf(name, "%08d.bmp", frame_cnt);
	cvSaveImage(name, frame);
}

int main(int argc, char* argv[])
{
	CvCapture* pCapture = NULL;
	IplImage * pFrame = NULL,
		     * pBg = NULL,
			 * pRoi = NULL;
	BYTE* pbRGB_Frame = NULL,
		* pbRGB_Bg = NULL, 
		* pbRGB_Roi = NULL;
	int iFrameCnt = 0, vdo_w, vdo_h, timestamp = 0;
	CIndVehicleTracking ind_vehicle_tracker(1);
	int cam_id = 0;

	initgColorBox();
	ltEnableLog("log.txt");


	// read input files
	char file_name[MAX_PATH] = {0};
	sprintf(file_name, "..\\data\\cam%02d\\video.avi", cam_id);
	pCapture = cvCaptureFromAVI(file_name); // video
	//if (!pCapture) { printf("can not open video file.\n"); goto error; }
	vdo_w = (int)cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH);
	vdo_h = (int)cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT);

	file_name[0] = '\0';
	sprintf(file_name, "..\\data\\cam%02d\\background.png", cam_id);
	pBg = cvLoadImage(file_name); // background
	//if (!pBg) { printf("can not open background image.\n"); goto error; }
	pbRGB_Bg = getImageData(pBg); 
	//if (!pbRGB_Bg) { printf("insufficient memory.\n"); goto error; }
	cv::Mat mpBg(pBg, true);
	cvReleaseImage(&pBg);

	file_name[0] = '\0';
	sprintf(file_name, "..\\data\\cam%02d\\roi.jpeg", cam_id);
	pRoi = cvLoadImage(file_name); // ROI
	//if (!pRoi) { printf("can not open ROI image.\n"); goto error; }
	pbRGB_Roi = getImageData(pRoi);
	//if (!pbRGB_Roi) { printf("insufficient memory.\n"); goto error; }
	//cvut::cvReShowImage("show re",pRoi, 1);
	//cv::waitKey(0);

	cv::Mat mpRoi(pRoi, true);
	cv::Mat gpRoi;
	cvtColor(mpRoi,gpRoi,CV_RGB2GRAY);

	cvReleaseImage(&pRoi);

	// initialize specific camera by ID
	char cam_config_file[MAX_PATH] = {0};
	sprintf(cam_config_file, "..\\data\\cam%02d\\cam_config.ini", cam_id);
	ind_vehicle_tracker.IndVehicleTracking_Init(cam_id, vdo_w, vdo_h, cam_config_file);

	// set background image, this can also set during processing
	ind_vehicle_tracker.IndVehicleTracking_SetBgImg(cam_id, pbRGB_Bg);

	// set ROI image, this can also set during processing
	ind_vehicle_tracker.IndVehicleTracking_SetRoiImg(cam_id, pbRGB_Roi);

	while (true) {
		printf("frame: %d\n", timestamp);
		pFrame = cvut::cvGetCurrentFrame(pCapture);
		if (!pFrame) break;

		pbRGB_Frame = getImageData(pFrame);
		if (!pbRGB_Frame) break;
	
		// set background image
		ind_vehicle_tracker.IndVehicleTracking_SetBgImg(cam_id, pbRGB_Bg);

		// set ROI image
		ind_vehicle_tracker.IndVehicleTracking_SetRoiImg(cam_id, pbRGB_Roi);

		// do tracking
		ind_vehicle_tracker.IndVehicleTracking_Process(cam_id, pbRGB_Frame, timestamp);

		// get object(vehicle) and draw
		int num_vehicle = ind_vehicle_tracker.IndVehicleTracking_GetObjectCnt(cam_id);
		for (int i = 0; i < num_vehicle; i++) {
			CVehicleClass vehicle = ind_vehicle_tracker.IndVehicleTracking_GetObject(cam_id, i);
			cvut::cvDrawBox2D(pFrame, vehicle.getObjEllipse(), CV_RGB(0, 255, 0));

			if (vehicle.isModelVaild()) {
				CShapeModel model = vehicle.getShapeModel();
				int nID = vehicle.getObjID();
				drawShapeModel(&pFrame, nID, model);
				
				char cIdD[20] = {0}; sprintf(cIdD, "%d", nID);
				CvPoint center = vehicle.getObjPosition();
				cvut::cvDrawText(pFrame, cIdD, cvPoint2D32f(center.x, center.y), gColorBox[nID%32], 2, 0.8f);
			}
		}

		cvut::cvReShowImage("Results", pFrame, 1, cvSize(640,480));
		//saveFrame(pFrame, timestamp);

		delete [] pbRGB_Frame; pbRGB_Frame = NULL;
		cvReleaseImage(&pFrame);
		timestamp++;
	}

	ind_vehicle_tracker.IndVehicleTracking_Uninit(cam_id);
	ltDisableLog();

error:
	if (pBg) cvReleaseImage(&pBg);
	if (pRoi) cvReleaseImage(&pRoi);
	if (pCapture) cvReleaseCapture(&pCapture);
	if (pbRGB_Bg) delete pbRGB_Bg;
	if (pbRGB_Roi) delete pbRGB_Roi;

	return 0;
}

