//-----------------------------------------------------------------------------------
//
//  Copyright (c) 2017 Zheng Tang <zhtang@uw.edu>.  All rights reserved.
//
//  Description:
//      Implementation of object detection with multi-scale testing
//
//-----------------------------------------------------------------------------------

//#include <direct.h>	// in Windows
#include <sys/stat.h>	// in Linux
#include "Cfg.h"
#include "RoiSel.h"
#include "ObjDet.h"

int main(int argc, char *argv[])
{
	int nFrmCnt = 0, nProcFrmNum = 0;
	//double fCurrTmSec;	// in Windows
	std::time_t tCurrTstpSec;	// in Linux
	long nCurrTstpMsec;	// in Linux
	//SYSTEMTIME tCurrSysTm;	// in Windows
	//struct _timeb stbCurrTm;	// in Windows
	struct timespec stsTstp;	// in Linux
	cv::VideoCapture oVdoCap;
	cv::VideoWriter oVdoWrt;
	cv::Mat oImgFrm, oImgRoi;
	cv::Size oFrmSz;
	std::vector<CDetNd> voDetNd;
	CObjDet oObjDet;
	CCfg oCfg;
	oCfg.loadCfgFile();

	cv::namedWindow("current frame", CV_WINDOW_NORMAL);

	// read video source
	// from video file
	if (0 == oCfg.getInVdoTyp())
		oVdoCap = cv::VideoCapture(oCfg.getInVdoPth());
	// from USB camera
	else if (1 == oCfg.getInVdoTyp())
		oVdoCap = cv::VideoCapture(0);
	// from IP camera
	else if (2 == oCfg.getInVdoTyp())
		oVdoCap.open(oCfg.getInVdoPth());
	// from updating image
	else if (3 == oCfg.getInVdoTyp())
		oImgFrm = cv::imread(oCfg.getInFrmPth(), CV_LOAD_IMAGE_COLOR);

	// handle error in reading video source
	if ((3 > oCfg.getInVdoTyp()) && (!oVdoCap.isOpened()))
	{
		std::cout << "Error: The video is not captured properly" << std::endl;
		return 0;
	}

	// test USB/IP camera
	if ((1 == oCfg.getInVdoTyp()) || (2 == oCfg.getInVdoTyp()))
	{
		std::cout << "Test USB/IP camera" << std::endl;
		cv::namedWindow("camera test", CV_WINDOW_NORMAL);

		// press "Esc" when the camera is ready
		while (1)
		{
			oVdoCap >> oImgFrm;
			cv::imshow("camera test", oImgFrm);
			int nKeyboardIn = cv::waitKey(0);	// read keyboard event
			if (nKeyboardIn == 27)
				break;
		}

		cv::destroyWindow("camera test");
	}

	// set frame size
	if (3 > oCfg.getInVdoTyp())
	{
		oFrmSz.width = (int)oVdoCap.get(CV_CAP_PROP_FRAME_WIDTH);
		oFrmSz.height = (int)oVdoCap.get(CV_CAP_PROP_FRAME_HEIGHT);
	}
	else
	{
		oFrmSz.width = oImgFrm.cols;
		oFrmSz.height = oImgFrm.rows;
	}

	// resize frame if necessary
	if (0 >= oCfg.getRszFrmHei())
		oCfg.setFrmSz(oFrmSz);
	else
	{
		oFrmSz = cv::Size((((float)oFrmSz.width / (float)oFrmSz.height) * oCfg.getRszFrmHei()), oCfg.getRszFrmHei());
		oCfg.setFrmSz(oFrmSz);
	}

	// set video frame rate
	if (0 == oCfg.getInVdoTyp())
		oCfg.setFrmRt(oVdoCap.get(CV_CAP_PROP_FPS));
	else
		oCfg.setFrmRt(oCfg.getOutFrmRt());

	// select ROI
	if (oCfg.getSelRoiFlg())
	{
		while (true)
		{
			int nKeyboardIn = cv::waitKey(0);	// read keyboard event

			if (nKeyboardIn == 'p')	// proceed to next frame
			{
				if (3 > oCfg.getInVdoTyp())
					oVdoCap >> oImgFrm;
				else
					oImgFrm = cv::imread(oCfg.getInFrmPth(), CV_LOAD_IMAGE_COLOR);

				if (oImgFrm.empty())
					std::cout << "Error: Reach the end of the video." << std::endl;

				cv::imshow("current frame", oImgFrm);
			}

			if (nKeyboardIn == 27)	// press "Esc" to terminate
			{
				cv::destroyWindow("current frame");
				break;
			}

			if (nKeyboardIn == 's')			// select ROI
			{
				if (3 > oCfg.getInVdoTyp())
					oVdoCap >> oImgFrm;
				else
					oImgFrm = cv::imread(oCfg.getInFrmPth(), CV_LOAD_IMAGE_COLOR);

				if (oImgFrm.empty())
					std::cout << "Error: Reach the end of the video." << std::endl;

				cv::resize(oImgFrm, oImgFrm, oFrmSz);

				oRoiSel.selRoi(oImgFrm, oCfg);
				cv::destroyWindow("current frame");
				break;
			}

			nFrmCnt++;
		}

		oImgRoi = cv::imread(oCfg.getInRoiPth(), CV_LOAD_IMAGE_GRAYSCALE);

		// reset video capture
		if (0 == oCfg.getInVdoTyp())
			oVdoCap = cv::VideoCapture(oCfg.getInVdoPth());
	}
	else
	{
		oImgRoi = cv::imread(oCfg.getInRoiPth(), CV_LOAD_IMAGE_GRAYSCALE);

		if (oImgRoi.empty())	// if ROI image does not exist, create a non-constraint ROI image
		{
			oImgRoi = cv::Mat(oFrmSz, CV_8UC1, cv::Scalar_<uchar>(255));
			cv::imwrite(oCfg.getInRoiPth(), oImgRoi);
		}
	}

	// set the ROI area
	oCfg.setRoiArea(cv::countNonZero(oImgRoi));

	// create video writer for output video
	if (oCfg.getOutVdoFlg())
	{
		if (0 == oCfg.getInVdoTyp())
			oVdoWrt = cv::VideoWriter(oCfg.getOutVdoPth(), CV_FOURCC('M', 'P', '4', '2'), (double)oVdoCap.get(CV_CAP_PROP_FPS), oFrmSz);
		else
			oVdoWrt = cv::VideoWriter(oCfg.getOutVdoPth(), CV_FOURCC('M', 'P', '4', '2'), oCfg.getFrmRt(), oFrmSz);
	}

	// create directory for output images
	if (oCfg.getOutImgFlg())
	{
		//_mkdir(oCfg.getOutImgFlrPth());	// in Windows
		mkdir(oCfg.getOutImgFlrPth(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);	// in Linux
	}

	// reset video capture
	if (0 == oCfg.getInVdoTyp())
		oVdoCap = cv::VideoCapture(oCfg.getInVdoPth());

	// regular object detection
	cv::namedWindow("current frame", CV_WINDOW_NORMAL);
	nFrmCnt = 0;
	nProcFrmNum = 0;
	std::printf("\nStart object detection...\n");
	while (true)
	{
		// read video frame
		if (3 > oCfg.getInVdoTyp())
			oVdoCap >> oImgFrm;
		else
			oImgFrm = cv::imread(oCfg.getInFrmPth(), CV_LOAD_IMAGE_COLOR);

		// resize video frame if necessary
		if (0 < oCfg.getRszFrmHei())
			cv::resize(oImgFrm, oImgFrm, oFrmSz);

		// run till the end of the video
		if ((oImgFrm.empty()) || ((0 < oCfg.getProcFrmNum()) && (oCfg.getProcFrmNum() < nProcFrmNum)))
			break;

		// manually end the process by pressing "Esc"
		int nKeyboardIn = cv::waitKey(1);	// read keyboard event
		if (nKeyboardIn == 27)
			break;

		// showing frame count and time stamp
		if (0 == oCfg.getInVdoTyp())
			std::printf("frame %06d\n", nFrmCnt);
		else
		{
			//GetSystemTime(&tCurrSysTm);	// in Windows
			//std::printf("frame %05d: %04d-%02d-%02d %02d:%02d:%02d.%03d\n", nFrmCnt, tCurrSysTm.wYear, tCurrSysTm.wMonth, tCurrSysTm.wDay,
			////	tCurrSysTm.wHour, tCurrSysTm.wMinute, tCurrSysTm.wSecond, tCurrSysTm.wMilliseconds);	// in Windows

			//_ftime(&stbCurrTm); // C4996	// in Windows
			//// Note: _ftime is deprecated; consider using _ftime_s instead
			//std::printf("frame %05d: %I64d%03d\n", nFrmCnt, stbCurrTm.time, stbCurrTm.millitm);	// in Windows

			tCurrTstpSec = (intmax_t)stsTstp.tv_sec;	// in Linux
			nCurrTstpMsec = round(stsTstp.tv_nsec / 1.0e6);	// in Linux
			clock_gettime(CLOCK_REALTIME, &stsTstp);	// in Linux
			std::printf("frame %05d: %lld.%03ld\n", nFrmCnt, (long long)tCurrTstpSec, nCurrTstpMsec);	// in Linux

			// output timestamp text file
			FILE* pfOutTstp = std::fopen(oCfg.getOutTstpPth(), "a");

			//std::fprintf(pfOutTstp, "\n%d,%04d%02d%02d,%02d%02d%02d%03d",
			//	m_nFrmCnt, tCurrSysTm.wYear, tCurrSysTm.wMonth, tCurrSysTm.wDay,
			//	tCurrSysTm.wHour, tCurrSysTm.wMinute, tCurrSysTm.wSecond, tCurrSysTm.wMilliseconds);	// in Windows

			//std::fprintf(pfOutTstp, "\n%d,%I64d%03d",
			//	nFrmCnt, stbCurrTm.time, stbCurrTm.millitm);	// in Windows

			std::fprintf(pfOutTstp, "\n%d,%lld.%03ld",
				nFrmCnt, (long long)tCurrTstpSec, nCurrTstpMsec);	// in Linux

			std::fclose(pfOutTstp);
		}

		// initialize at the beginning
		if (oCfg.getProcStFrmCnt() == nFrmCnt)
		{
            // set starting time stamp
            if (0 < oCfg.getInVdoTyp())
            {
                //GetSystemTime(&tCurrSysTm);	// in Windows
                //oCfg.setStSysTm(tCurrSysTm);	// in Windows

                //_ftime(&stbCurrTm);	// C4996	// in Windows
                //// Note: _ftime is deprecated; consider using _ftime_s instead
                //oCfg.setStTstp(stbCurrTm);	// in Windows

                clock_gettime(CLOCK_REALTIME, &stsTstp);	// in Linux
                oCfg.setStTstp(stsTstp);	// in Linux
            }

			// initialize object detector
			oObjDet.initialize(oCfg, oImgRoi);
		}

		// for debug
		//if (-1 < nFrmCnt)
		//	cv::waitKey(0);

        if (oCfg.getProcStFrmCnt() <= nFrmCnt)
        {
            // run object detection
            oObjDet.process(oImgFrm, voDetNd, nFrmCnt);

            // output object detection results
            oObjDet.output(oImgFrm, voDetNd);

            cv::imshow("current frame", oImgFrm);
            //cv::waitKey(0);

            // write (plotted) frame to output video
            if (oCfg.getOutVdoFlg())
                oVdoWrt.write(oImgFrm);

            // save output (plotted) frame image
            if (oCfg.getOutImgFlg())
            {
                char acOutImgNm[128] = { 0 };
                std::sprintf(acOutImgNm, "%06d.jpg", nFrmCnt);
                char acOutImgPth[128] = { 0 };
                std::sprintf(acOutImgPth, oCfg.getOutImgFlrPth());
                std::strcat(acOutImgPth, acOutImgNm);
                cv::imwrite(acOutImgPth, oImgFrm);
            }

            nProcFrmNum++;
        }

		nFrmCnt++;
	}

	cv::destroyWindow("current frame");

	return 0;
}
