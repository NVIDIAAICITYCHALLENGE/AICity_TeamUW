/*******************************************************************************
 *  Program: openCV UTilities (for 2.4.3)                                      *
 *  Programmer: Young Lee                                                      *
 *  File Name: cvut.h                                                          *
 *  History:                                                                   *
 *      2012/10/26 v0.5.7:  upgrade to 2.4.3.                                  *
 *      2012/07/04 v0.5.6:  add cvContrast().                                  *
 *      2012/05/16 v0.5.5:  add cvSlope() and cvIntersection().                *
 *      2012/05/16 v0.5.4:  modify cvGetBlobImage(), fix bug in cvDot().       *
 *      2012/05/03 v0.5.3:  add cvSetROIImage().                               *
 *      2012/04/27 v0.5.2:  add point/line utility functions.                  *
 *      2012/04/22 v0.5.1:  add function cvGetCannyEdge().                     *
 *      2012/04/15 v0.5:    alpha version, included many functions for image.  *
 *      2011/xx/xx v0.0:    initial version.                                   *
 *******************************************************************************/
#pragma once


#ifndef __CVUT_H__
#define __CVUT_H__


//---- Include ------------------------------------------------------------------------------------------------------//
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>


//---- Library ------------------------------------------------------------------------------------------------------//
#ifdef _DEBUG
	#pragma comment(lib, "opencv_calib3d243d.lib")
	#pragma comment(lib, "opencv_contrib243d.lib")
	#pragma comment(lib, "opencv_core243d.lib")
	#pragma comment(lib, "opencv_features2d243d.lib")
	#pragma comment(lib, "opencv_flann243d.lib")
	#pragma comment(lib, "opencv_gpu243d.lib")
	#pragma comment(lib, "opencv_highgui243d.lib")
	#pragma comment(lib, "opencv_imgproc243d.lib")
	#pragma comment(lib, "opencv_legacy243d.lib")
	#pragma comment(lib, "opencv_ml243d.lib")
	#pragma comment(lib, "opencv_objdetect243d.lib")
	#pragma comment(lib, "opencv_ts243d.lib")
	#pragma comment(lib, "opencv_video243d.lib")
#else
	#pragma comment(lib, "opencv_calib3d243.lib")
	#pragma comment(lib, "opencv_contrib243.lib")
	#pragma comment(lib, "opencv_core243.lib")
	#pragma comment(lib, "opencv_features2d243.lib")
	#pragma comment(lib, "opencv_flann243.lib")
	#pragma comment(lib, "opencv_gpu243.lib")
	#pragma comment(lib, "opencv_highgui243.lib")
	#pragma comment(lib, "opencv_imgproc243.lib")
	#pragma comment(lib, "opencv_legacy243.lib")
	#pragma comment(lib, "opencv_ml243.lib")
	#pragma comment(lib, "opencv_objdetect243.lib")
	#pragma comment(lib, "opencv_ts243.lib")
	#pragma comment(lib, "opencv_video243.lib")
#endif


using namespace std;


namespace cvut {


//---- Macro & Defined type/struct ----------------------------------------------------------------------------------//
#define OPF_WIN_SIZE                     5

#define CV_DEG2RAD(deg)                  ((deg)*CV_PI/180)
#define CV_RAD2DEG(rad)                  ((rad)/CV_PI*180)
#define CV_L2NORM2D(x, y)                (sqrt(float(x)*float(x)+float(y)*float(y)))
#define CV_L2NORM3D(x, y, z)             (sqrt(float(x)*float(x)+float(y)*float(y)+float(z)*float(z)))

// result code
#define CVUT_SUCCESS                     0
#define CVUT_NULL_PTR                    -1
#define CVUT_INSUFFICIENT_MEMORY         -2
#define CVUT_INCOMPATIBLE_CHANNEL        -3
#define CVUT_INCOMPATIBLE_ROI            -4

// defined struct
struct CvLine2D32f {
	CvPoint2D32f start;
	CvPoint2D32f end;
};


//---- Global functions/variables -----------------------------------------------------------------------------------//
static CvFont gTextFont; // text font



//==== Point/Line Utility ======================================================================================//

/**
 *  Constructor of CvLine2D32f structure.
 *
 *  @param  start [in] start point of the line.
 *  @param  end [in] end point of the line.
 *  @return line in terms of CvLine2D32f structure.
 */
static CvLine2D32f cvLine2D32f(CvPoint2D32f start, CvPoint2D32f end)
{
	CvLine2D32f line;

	line.start = start;
	line.end = end;

	return line;
}


/**
 *  Dot product of vector OA and vector OB (2D).
 *
 *  @param  o [in] origin point O.
 *  @param  a [in] arrow point of vector A.
 *  @param  b [in] arrow point of vector B.
 *  @return dot product of vector OA and vector OB.
 */
static float cvDot(CvPoint2D32f o, CvPoint2D32f a, CvPoint2D32f b)
{
	return (a.x-o.x) * (b.x-o.x) + (a.y-o.y) * (b.y-o.y);
}


/**
 *  Dot product of vector OA and vector OB (3D).
 *
 *  @param  o [in] origin point O.
 *  @param  a [in] arrow point of vector A.
 *  @param  b [in] arrow point of vector B.
 *  @return dot product of vector OA and vector OB.
 */
static float cvDot(CvPoint3D32f o, CvPoint3D32f a, CvPoint3D32f b)
{
	return (a.x-o.x) * (b.x-o.x) + (a.y-o.y) * (b.y-o.y) + (a.z-o.z) * (b.z-o.z);
}


/**
 *  Dot product of vector v1 and v2 (2D).
 *
 *  @param  v1 [in] vector 1.
 *  @param  v2 [in] vector 2.
 *  @return dot product of two vectors.
 *
 */
static float cvDot(CvPoint2D32f v1, CvPoint2D32f v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}


/**
 *  Dot product of vector v1 and v2 (3D).
 *
 *  @param  v1 [in] vector 1.
 *  @param  v2 [in] vector 2.
 *  @return dot product of two vectors.
 *
 */
static float cvDot(CvPoint3D32f v1, CvPoint3D32f v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


/**
 *  Cross product of vector OA and vector OB (2D).
 *
 *  @param  o [in] origin point O.
 *  @param  a [in] arrow point of vector A.
 *  @param  b [in] arrow point of vector B.
 *  @return cross product of vector OA and vector OB.
 *
 */
static float cvCross(CvPoint2D32f o, CvPoint2D32f a, CvPoint2D32f b)
{
	return (a.x-o.x) * (b.y-o.y) - (a.y-o.y) * (b.x-o.x);
}


/**
 *  Cross product of vector v1 and v2 (2D).
 *
 *  @param  v1 [in] vector 1.
 *  @param  v2 [in] vector 2.
 *  @return cross product of two vectors.
 *
 */
static float cvCross(CvPoint2D32f v1, CvPoint2D32f v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}


/**
 *  Cross product of vector v1 and v2 (3D).
 *
 *  @param  v1 [in] vector 1.
 *  @param  v2 [in] vector 2.
 *  @return cross product of two vectors.
 *
 */
static CvPoint3D32f cvCross(CvPoint3D32f v1, CvPoint3D32f v2)
{
	CvPoint3D32f res = cvPoint3D32f(0, 0, 0);

	res.x = v1.y*v2.z - v1.z*v2.y;
	res.y = v1.z*v2.x - v1.x*v2.z;
	res.z = v1.x*v2.y - v1.y*v2.x;

	return res;
}


/**
 *  Distance between point 1 and 2 (2D).
 *
 *  @param  p1 [in] point 1.
 *  @param  p2 [in] point 2.
 *  @return distance between 2 points.
 *
 */
static float cvDistance(CvPoint2D32f p1, CvPoint2D32f p2)
{
	return sqrt(pow(p2.x - p1.x, 2.f) + pow(p2.y - p1.y, 2.f));
}


/**
 *  Distance between point 1 and 2 (3D).
 *
 *  @param  p1 [in] point 1.
 *  @param  p2 [in] point 2.
 *  @return distance between 2 points.
 *
 */
static float cvDistance(CvPoint3D32f p1, CvPoint3D32f p2)
{
	return sqrt(pow(p2.x - p1.x, 2.f) + pow(p2.y - p1.y, 2.f) + pow(p2.z - p1.z, 2.f));
}


/**
 *  Distance between point and line (2D).
 *
 *  @param  pt [in] point.
 *  @param  line [in] line.
 *  @return distance between point and line.
 *
 */
static float cvDistance(CvPoint2D32f pt, CvLine2D32f line)
{
	if (cvDot(line.start, line.end, pt) > 0 && cvDot(line.end, line.start, pt) > 0)
		// point is beside the line: area divided by base can get height
		return fabs(cvCross(line.start, line.end, pt)) / cvDistance(line.start, line.end);
	else
		// point is beside the start/end point of the line:
		return MIN(cvDistance(pt, line.start), cvDistance(pt, line.end));
}


/**
 *  Distance between line 1 and 2 (2D).
 *
 *  @param  line1 [in] line 1 .
 *  @param  line2 [in] line 2.
 *  @return distance between line 1 and 2.
 */
static float cvDistance(CvLine2D32f line1, CvLine2D32f line2)
{
	CvPoint2D32f v1 = cvPoint2D32f(line1.end.x-line1.start.x, line1.end.y-line1.start.y),
		         v2 = cvPoint2D32f(line2.end.x-line2.start.x, line2.end.y-line2.start.y);

	if (cvCross(v1, v2) == 0) // lines are parallel
		return cvDistance(line1.start, line2);
	else // lines intersect.
		return 0;
}


/**
 *  Get slope of a line
 *
 *  @param  line [in] line.
 *  @return slope of the line (opposite to normal slope due to x/y directions in image).
 */
static float cvSlope(CvLine2D32f line)
{
	return (line.end.y - line.start.y) / (line.end.x - line.start.x);
}


/**
 *  Get slope of a line
 *
 *  @param  start [in] start point of the line.
 *  @param  end [in] end point of the line.
 *  @return slope of the line (opposite to normal slope due to x/y directions in image).
 */
static float cvSlope(CvPoint2D32f start, CvPoint2D32f end)
{
	return (end.y - start.y) / (end.x - start.x);
}


/**
 *  Get intersection of two lines
 *
 *  @param  line1 [in] line in type of CvLine2D32f.
 *  @param  line2 [in] another line in type of CvLine2D32f.
 *  @return intersection of two lines, (-1, -1) implies two lines are parallel/overlap.
 */
static CvPoint2D32f cvIntersect(CvLine2D32f line1, CvLine2D32f line2)
{
	CvPoint2D32f a = cvPoint2D32f(line1.end.x - line1.start.x, line1.end.y - line1.start.y),
		         b = cvPoint2D32f(line2.end.x - line2.start.x, line2.end.y - line2.start.y),
		         s = cvPoint2D32f(line2.start.x - line1.start.x, line2.start.y - line1.start.y);
	float c1 = cvCross(line1.start, line1.end, line2.start),
		  c2 = cvCross(line1.start, line1.end, line2.end),
		  c3 = cvCross(line2.start, line2.end, line1.start),
		  c4 = cvCross(line2.start, line2.end, line1.end);

	// two lines are parallel: no intersection
	// two lines are overlap: infinite intersections
	if (cvCross(a, b) == 0 ||
		c1 * c2 >= 0 || c3 * c4 >= 0)
		return cvPoint2D32f(-1, -1);

	// find intersection
	float temp = cvCross(s, b) / cvCross(a, b);
	return cvPoint2D32f(line1.start.x + a.x * temp, line1.start.y + a.y * temp);
}


static CvPoint2D32f cvNormalize2D32f(CvPoint2D32f v)
{
	float norm = CV_L2NORM2D(v.x, v.y);
	return cvPoint2D32f(v.x/norm, v.y/norm);
}

static CvPoint3D32f cvNormalize3D32f(CvPoint3D32f v)
{
	float norm = CV_L2NORM3D(v.x, v.y, v.z);
	return cvPoint3D32f(v.x/norm, v.y/norm, v.z/norm);
}



//==== Image Utility ===========================================================================================//

/**
 *  Show the histogram.
 *
 *  @param  pHist [in] pointer of histogram data.
 *  @param  bins [in] number of bins in histogram.
 *  @param  name [in] name of display window.
 *  @param  size [in] size of display window.
 *  @param  color [in] color of bins in histogram.
 *  @return result code.
 *
 */
static int cvShowHistogram(float* pHist, int bins,
	                       const char* name = "Histogram", CvSize size = cvSize(640, 480),
	                       CvScalar color = CV_RGB(255, 255, 255))
{
	int res = CVUT_SUCCESS;
	IplImage* img_hist = NULL;
	int bin_width = 1;
	float scale = 1;


	// create histogram image
	img_hist = cvCreateImage(size, IPL_DEPTH_8U, 3);
	if (!img_hist) { res = CVUT_INSUFFICIENT_MEMORY; exit(1); } cvZero(img_hist);
	bin_width = (int)(img_hist->width / (float)bins);

	// find scale to fit the histogram image
	float max = 0;
	for (int b = 0; b < bins; b++)
		if (pHist[b] > max)
			max = pHist[b];
	scale = (max) ? (float)(img_hist->height/max/1.1) : 0;

	// draw the histogram
	for (int b = 0; b < bins; b++) {
		int bin_value = (int)(pHist[b]*scale);
		CvPoint pt1 = cvPoint(b*bin_width, img_hist->height),
		        pt2 = cvPoint((b+1)*bin_width, img_hist->height-bin_value);
		cvRectangle(img_hist, pt1, pt2, color, CV_FILLED);
	}

	cvShowImage(name, img_hist);
	cvWaitKey(0);

	return res;
}


/**
 *  Calculate Histogram of Oriented Gradient (HOG).
 *
 *  @param  src [in] source image.
 *  @param  pHOG [out] pointer of histogram data, need to be allocated first.
 *  @param  bins [in] number of bins in histogram.
 *  @param  szWin [in] window size in the image.
 *  @param  szWinStride [in] stride of window, the #window in the image =
 *          ((src.width-szWin.x)/szWinStride.x+1) * ((src.height-szWin.y)/szWinStride.y+1).
 *  @param  szBlk [in] block size in a window.
 *  @param  szBlkStride [in] stride of block. the #block in a window =
 *          ((szWin.x-szBlk.x)/szBlkStride.x+1) * ((szWin.y-szBlk.y)/szBlkStride.y+1).
 *  @param  szCell [in] calculated cell size in a block.
 *  @return result code.
 *
 */
static int cvGetHOG(IplImage* src, float* pHOG, int bins = 9,
	                CvSize szWin = cvSize(64,128), CvSize szWinStride = cvSize(32,32), CvSize szBlk = cvSize(16,16),
	                CvSize szBlkStride = cvSize(8,8), CvSize szCell = cvSize(8,8))
{
	int res = CVUT_SUCCESS;
	cv::HOGDescriptor hog(szWin, szBlk, szBlkStride, szCell, bins);
	vector<float> vDescriptors;

	if (!pHOG) { res = CVUT_NULL_PTR; goto error; }

	hog.compute(src, vDescriptors, cvSize(32, 32), cvSize(0,0));

	for (unsigned int b = 0; b < vDescriptors.size(); b++)
		pHOG[b%bins] += vDescriptors.at(b);

error:
	return res;
}


/**
 *  Equalize color histogram.
 *
 *  @param  src [in] source image.
 *  @param  dst [out] destination image.
 *  @return result code.
 *
 */
static int cvEqualizeColorHist(IplImage* src, IplImage* dst)
{
	IplImage *Red = NULL, *Green = NULL, *Blue = NULL;
	int res = CVUT_SUCCESS;

	if (src->nChannels == 3) {
		Red   = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		if (!Red)   { res = CVUT_INSUFFICIENT_MEMORY; goto error; }
		Green = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		if (!Green) { res = CVUT_INSUFFICIENT_MEMORY; goto error; }
		Blue  = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		if (!Blue)  { res = CVUT_INSUFFICIENT_MEMORY; goto error; }

		cvSplit(src, Blue, Green, Red, 0);

		cvEqualizeHist(Blue,  Blue);
		cvEqualizeHist(Green, Green);
		cvEqualizeHist(Red,   Red);

		cvMerge(Blue, Green, Red, 0, dst);
	}
	else
		cvEqualizeHist(src, dst);

	// release memory
error:
	if (Red)   cvReleaseImage(&Red);
	if (Green) cvReleaseImage(&Green);
	if (Blue)  cvReleaseImage(&Blue);

	return res;
}


/**
 *  Draw text onto source image.
 *
 *  @param  src [in] source image.
 *  @param  text [in] text to be shown.
 *  @param  pt [in] location of the text.
 *  @param  color [in] text's color.
 *  @param  thickness [in] text's thickness.
 *  @param  hscale [in] horizontal scale.
 *  @param  vscale [in] vertical scale.
 *  @param  angle [in] tilt angle related to vertical line.
 *  @param  font_face [in] type of font.
 */
static void cvDrawText(IplImage* src, char* text, CvPoint2D32f pt,
	                   CvScalar color = CV_RGB(255, 255, 255), int thickness = 1,
					   float hscale = 1, float vscale = 1, float angle = 0,
					   int font_face = CV_FONT_HERSHEY_SIMPLEX)
{
	// initialization
	if (!gTextFont.nameFont || gTextFont.font_face != font_face)
		cvInitFont(&gTextFont, font_face, hscale, vscale, angle, thickness);

	cvPutText(src, text, cvPointFrom32f(pt), &gTextFont, color);
}


/**
 *  Draw a arrow from point a to point b.
 *
 *  @param  src [in] source image.
 *  @param  a [in] start point of arrow.
 *  @param  b [in] end point of arrow.
 *  @param  scale [in] scale of arrow length.
 *  @param  color [in] arrow's color.
 *  @param  thickness [in] arrow's thickness.
 */
static void cvDrawArrow(IplImage* src, CvPoint a, CvPoint b, double scale,
                        CvScalar color, int thickness = 1)
{
	double angle = atan2((double)a.y - b.y, (double)a.x - b.x);
	double hypotenuse = sqrt((double)((a.y-b.y)*(a.y-b.y) + (a.x-b.x)*(a.x-b.x)));

	b.x = (int)(a.x - scale * hypotenuse * cos(angle));
	b.y = (int)(a.y - scale * hypotenuse * sin(angle));

	cvLine(src, a, b, color, thickness, CV_AA, 0);

	a.x = (int) (b.x + 3 * scale * cos(angle + CV_PI / 4));
	a.y = (int) (b.y + 3 * scale * sin(angle + CV_PI / 4));
	cvLine(src, a, b, color, thickness, CV_AA, 0);
	a.x = (int) (b.x + 3 * scale * cos(angle - CV_PI / 4));
	a.y = (int) (b.y + 3 * scale * sin(angle - CV_PI / 4));
	cvLine(src, a, b, color, thickness, CV_AA, 0);
}


/**
 *  Draw a box in form of CvBox2D.
 *
 *  @param  src [in] image to be drawn.
 *  @param  box [in] drawn box.
 *  @param  color [in] color of box's line.
 *  @param  thickness [in] thickness of box's line.
 */
static void cvDrawBox2D(IplImage* src, CvBox2D box, CvScalar color, int thickness = 1)
{
	CvPoint2D32f boxPoints[4];
	cvBoxPoints(box, boxPoints);

	cvLine(src, cvPoint((int)boxPoints[0].x, (int)boxPoints[0].y),
		        cvPoint((int)boxPoints[1].x, (int)boxPoints[1].y),
		        color, thickness);
	cvLine(src, cvPoint((int)boxPoints[1].x, (int)boxPoints[1].y),
		        cvPoint((int)boxPoints[2].x, (int)boxPoints[2].y),
				color, thickness);
	cvLine(src, cvPoint((int)boxPoints[2].x, (int)boxPoints[2].y),
		        cvPoint((int)boxPoints[3].x, (int)boxPoints[3].y),
				color, thickness);
	cvLine(src, cvPoint((int)boxPoints[3].x, (int)boxPoints[3].y),
		        cvPoint((int)boxPoints[0].x, (int)boxPoints[0].y),
				color, thickness);
}


/**
 *  Rotate an image with an angle.
 *
 *  @param  src [in] source image.
 *  @param  angle [in] rotated angle in terms of radian.
 *  @return rotated image, need to be released.
 */
static IplImage* cvRotateImage(IplImage* src, float angle)
{
	float angle_radians = 0, right_angle;
	IplImage* img_rot = NULL;

	// calculate radians.
	if (abs(angle) > 45) {
		right_angle = (angle > 0) ? 90.f : -90.f;
		angle_radians = float(CV_DEG2RAD(right_angle-angle));
	}
	else
		angle_radians = float(CV_DEG2RAD(-angle));

	// create a mapping matrix.
	float m[6];
	CvMat Mrotate = cvMat(2, 3, CV_32F, m);
	int w = src->width;
	int h = src->height;

	m[0] = (float)(cos(angle_radians));
	m[1] = (float)(sin(angle_radians));
	m[3] = -m[1];
	m[4] = m[0];
	m[2] = src->width * 0.5f;
	m[5] = src->height * 0.5f;

	// make a spare image for the result
	img_rot = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
	if (!img_rot) goto error;

	// transform the image
	cvGetQuadrangleSubPix(src, img_rot, &Mrotate);

error:
	return img_rot;
}


/**
 *  Copy source image to the ROI of destination image.
 *
 *  @param  src [in] source image.
 *  @param  dst [in] destination image.
 *  @param  roi [in] ROI of destination image.
 *  @return result code.
 */
static int cvSetROIImage(IplImage* src, IplImage* dst, CvRect roi)
{
	int res = CVUT_SUCCESS;
	CvRect roi_old = cvGetImageROI(dst);

	if (roi.x < 0 || roi.x+roi.width > dst->width ||
		roi.y < 0 || roi.y+roi.height > dst->height)
		return CVUT_INCOMPATIBLE_ROI;

	cvSetImageROI(dst, roi);

	if (src->width == roi.width && src->height == roi.height)
		cvCopy(src, dst);
	else {
		IplImage* pTemp = NULL;
		pTemp = cvCreateImage(cvSize(roi.width, roi.height), src->depth, src->nChannels);
		if (!pTemp) { res = CVUT_INSUFFICIENT_MEMORY; goto error; }

		cvResize(src, pTemp);
		cvCopy(pTemp, dst);

		cvReleaseImage(&pTemp);
	}

	cvSetImageROI(dst, roi_old);

error:
	return res;
}


/**
 *  Extend source image with a ratio and put it in the middle.
 *
 *  @param  src [in] source image.
 *  @param  dst [in] destination image.
 *  @param  roi [out] shift of the source image.
 *  @return extended image, need to be released.
 */
static IplImage* cvExtendImage(IplImage* src, float ratio, CvPoint2D32f& shift)
{
	IplImage* dst = NULL;
	CvSize szExt = cvSize(int(src->width*ratio), int(src->height*ratio));
	CvRect roi = cvRect(int((ratio-1)*src->width/2), int((ratio-1)*src->height/2), src->width, src->height);

	if (ratio < 1)
		return dst;

	shift = cvPoint2D32f(roi.x, roi.y);
	dst = cvCreateImage(szExt, src->depth, src->nChannels);
	if (!dst) { return dst; } cvZero(dst);

	cvSetROIImage(src, dst, roi);

	return dst;
}


/**
 *  Obtain a desired blob in an image (by CvRect).
 *
 *  @param  src [in] source image.
 *  @param  r [in] rectangle of the desired blob.
 *  @return blob image, need to be released.
 */
static IplImage* cvGetBlobImage(IplImage* src, CvRect r)
{
	IplImage* img_roi = NULL;
	CvRect roi_old = cvGetImageROI(src);

	cvSetImageROI(src, r);

	img_roi = cvCreateImage(cvSize(r.width, r.height), src->depth, src->nChannels);
	if (!img_roi) goto error;

	// catch blob from the source image
	cvCopyImage(src, img_roi);

	cvSetImageROI(src, roi_old);

error:
	return img_roi;
}


/**
 *  Obtain a desired blob in an image (by CvBox2D).
 *
 *  @param  src [in] source image.
 *  @param  box [in] box of the desired blob.
 *  @return blob image, need to be released.
 */
static IplImage* cvGetBlobImage(IplImage* src, CvBox2D box)
{
	CvPoint2D32f ptBox[4];
	IplImage* img_roi = NULL, * img_tmp = NULL, * img_rot = NULL, * dst = NULL;
	int dis_01, dis_12, dst_w, dst_h;
	CvRect roi;

	cvBoxPoints(box, ptBox);
	dis_01 = cvRound(cvDistance(ptBox[0], ptBox[1]));
	dis_12 = cvRound(cvDistance(ptBox[1], ptBox[2]));
	if (abs(box.angle) < 45) {
		dst_w = int(box.size.width);
		dst_h = int(box.size.height);
	}
	else {
		dst_w = int(box.size.height);
		dst_h = int(box.size.width);
		box.angle = (box.angle > 0) ? box.angle-90 : box.angle+90;
	}

	// get ROI image from src image
	roi.x      = cvRound(MAX(MIN(MIN(MIN(ptBox[0].x, ptBox[1].x), ptBox[2].x),ptBox[3].x), 0));
	roi.y      = cvRound(MAX(MIN(MIN(MIN(ptBox[0].y, ptBox[1].y), ptBox[2].y),ptBox[3].y), 0));
	roi.width  = cvRound(MIN(MAX(MAX(MAX(ptBox[0].x, ptBox[1].x), ptBox[2].x),ptBox[3].x), src->width) - roi.x);
	roi.height = cvRound(MIN(MAX(MAX(MAX(ptBox[0].y, ptBox[1].y), ptBox[2].y),ptBox[3].y), src->height) - roi.y);
	img_roi = cvGetBlobImage(src, roi);

	// copy ROI image to a buffer for rotation
	img_tmp = cvCreateImage(cvSize(MAX(dst_w, roi.width), MAX(dst_h, roi.height)),
		                    img_roi->depth, img_roi->nChannels);
	if (!img_tmp) goto error; cvZero(img_tmp);
	roi.x = (img_tmp->width-roi.width)/2;
	roi.y = (img_tmp->height-roi.height)/2;
	cvSetROIImage(img_roi, img_tmp, roi);

	// rotate ROI image
	img_rot = cvRotateImage(img_tmp, box.angle);
	if (!img_rot) goto error;

	// get rotated ROI from buffer
	roi.x = (img_rot->width-dst_w)/2;
	roi.y = (img_rot->height-dst_h)/2;
	roi.width = dst_w;
	roi.height = dst_h;
	dst = cvGetBlobImage(img_rot, roi);

error:
	if (img_roi) cvReleaseImage(&img_roi);
	if (img_tmp) cvReleaseImage(&img_tmp);
	if (img_rot) cvReleaseImage(&img_rot);

	return dst;
}


/**
 *  Paste a desired blob onto an image (by CvBox2D).
 *
 *  @param  src [in] source image.
 *  @param  dst [out] destination image.
 *  @param  box [in] rectangle of the desired blob.
 */
static void cvSetBlobImage(IplImage* src, IplImage* dst, CvBox2D box)
{
	float angle = box.angle,
		  angle_rad = float(CV_DEG2RAD(box.angle));
	IplImage* img_rot = NULL;


	for (int j = 0; j < src->height; j++) {
		for (int i = 0; i < src->width; i++) {
			CvPoint pt_rot;
			pt_rot.x = int(cos(angle_rad)*(i-src->width/2) - sin(angle_rad)*(j-src->width/2) + box.center.x + 0.5);
			pt_rot.y = int(sin(angle_rad)*(i-src->width/2) + cos(angle_rad)*(j-src->height/2) + box.center.y + 0.5);

			if (pt_rot.x < 0 || pt_rot.x >= dst->width ||
				pt_rot.y < 0 || pt_rot.y >= dst->height)
				continue;

			cvSet2D(dst, pt_rot.y, pt_rot.x, cvGet2D(src, j, i));
		}
	}
}


/**
 *  Show resized image.
 *
 *  @param  name [in] name of window.
 *  @param  src [in] source image.
 *  @param  wait [in] waiting time for cvWaitKey().
 *  @param  size [in] size to be shown.
 */
static void cvReShowImage(char* name, IplImage* src, int wait = 0, CvSize size = cvSize(0, 0))
{
	IplImage* pDisplay = NULL;

	if (size.width == 0 && size.height == 0)
		size = cvGetSize(src);

	pDisplay = cvCreateImage(size, src->depth, src->nChannels);
	if (!pDisplay) return;

	cvResize(src, pDisplay);
	cvShowImage(name, pDisplay);
	cvWaitKey(wait);

	cvReleaseImage(&pDisplay);
}


/**
 *  Get current frame from opened video.
 *
 *  @param  pCapture [in] capture pointer of opened video.
 *  @return current frame.
 */
static IplImage* cvGetCurrentFrame(CvCapture* pCapture)
{
	IplImage* pFrame = NULL;
	IplImage* pFrame_query = cvQueryFrame(pCapture);
	if (!pFrame_query) goto error;

	if (!pFrame) {
		CvSize szframe = cvSize(pFrame_query->width, pFrame_query->height);
		pFrame = cvCreateImage(szframe, pFrame_query->depth, pFrame_query->nChannels);
		if (!pFrame) goto error;
	}

	if (pFrame_query->origin == IPL_ORIGIN_TL)
		cvCopy(pFrame_query, pFrame, 0);
	else
		cvFlip(pFrame_query, pFrame, 0);

error:
	return pFrame;
}


/**
 *  Sobel edge detector.
 *
 *  @param  src [in] source image.
 *  @param  xorder [in] .
 *  @param  yorder [in] .
 *  @param  aperture_size [in] Sobel: 1,3,5,7 or Scharr: -1.
 *  @return edged image, need to be released.
 */
static IplImage* cvSobelEdge(IplImage *src, int xorder, int yorder, int aperture_size CV_DEFAULT(3))
{
	IplImage* pImg_gray = NULL,
		    * dst = NULL;

	dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	if (!dst) goto error;

	if (src->nChannels == 1) {
		cvSobel(src, dst, xorder, yorder, aperture_size);
	}
	else {
		pImg_gray = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		if (!pImg_gray) goto error;
		cvCvtColor(src, pImg_gray, CV_BGR2GRAY);

		cvSobel(pImg_gray, dst, xorder, yorder, aperture_size);

		cvReleaseImage(&pImg_gray);
	}

error:
	return dst;
}


/**
 *  Canny edge detector.
 *
 *  @param  src [in] source image.
 *  @param  thresh [in] threshold for edge detection.
 *  @return edged image, need to be released.
 */
static IplImage* cvCannyEdge(IplImage* src, float thresh = 200)
{
	IplImage* pImg_gray = NULL,
		    * dst = NULL;

	dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	if (!dst) goto error;

	if (src->nChannels == 1) {
		cvCanny(src, dst, 0.4*thresh, thresh);
	}
	else {
		pImg_gray = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		if (!pImg_gray) goto error;
		cvCvtColor(src, pImg_gray, CV_BGR2GRAY);

		cvCanny(pImg_gray, dst, 0.4*thresh, thresh);

		cvReleaseImage(&pImg_gray);
	}

error:
	return dst;
}


/**
 *  Adjust contrast of an image.
 *
 *  @param  src [in] source image.
 *  @param  dst [out] destination image.
 *  @param  contrast [in] contrast being adjusted, range is [-255, 255].
 *  @return result code.
 */
static int cvContrast(const IplImage* src, IplImage* dst, int contrast)
{
	int res = CVUT_SUCCESS;

	if (!src || !dst)
		return CVUT_NULL_PTR;


	// calculate mean value
	CvScalar mean = {0,0,0,0};
	for (int j = 0; j < src->height; j++) {
		for (int i = 0; i < src->width; i++) {
			for (int c = 0; c < src->nChannels; c++) {
				CvScalar ori = cvGet2D(src, j, i);
				mean.val[c] += ori.val[c];
			}
		}
	}
	for (int c = 0; c < src->nChannels; c++)
		mean.val[c] /= src->width * src->height;


	// calculate contrast value
	if (contrast <= -255) {
		for (int j = 0; j < src->height; j++)
			for (int i = 0; i < src->width; i++)
				cvSet2D(dst, j, i, mean);
	}
	else if(contrast > -255 &&  contrast < 0) {
		CvScalar value_ori, value_new;
		for (int j = 0; j < src->height; j++) {
			for (int i = 0; i < src->width; i++) {
				value_ori = cvGet2D(src, j, i);
				for (int c = 0; c < src->nChannels; c++)
					value_new.val[c] = value_ori.val[c] + (value_ori.val[c] - mean.val[c]) * contrast / 255;
				cvSet2D(dst, j, i, value_new);
			}
		}
	}
	else if (contrast == 0) {
		cvCopyImage(src, dst);
	}
	else if(contrast > 0 && contrast < 255) {
		CvScalar value_new, value_ori;
		for (int j = 0; j < src->height; j++) {
			for (int i = 0; i < src->width; i++) {
				value_ori = cvGet2D(src, j, i);
				for (int c = 0; c <src->nChannels; c++)
					value_new.val[c] = value_ori.val[c] + (value_ori.val[c] - mean.val[c]) * (255.f / (255 - contrast) - 1);
				cvSet2D(dst, j, i, value_new);
			}
		}
	}
	else {
		CvScalar value_new, value_ori;
		for (int j = 0; j < src->height; j++) {
			for (int i = 0; i < src->width; i++) {
				value_ori = cvGet2D(src, j, i);
				for (int c = 0; c < src->nChannels; c++)
					value_new.val[c] = (value_ori.val[c] > mean.val[c]) ? 255 : 0;
				cvSet2D(dst, j, i, value_new);
			}
		}
	}

	return res;
}


/**
 *  Get information of optical flow/motion vector.
 *
 *  @param  frame_pre [in] previous frame.
 *  @param  frame [in] current frame.
 *  @param  frame_pre_feat [in] features of previous frame,
 *  @param  frame_feat [in] features of current frame,
 *  @param  opf_feat [out]
 *  @param  opf_feat_err [out]
 *  @param  num_of_feat [in] maximum number of features.
 *  @param  opf_win_sz [in] size of window used for searching similar block.
 *  @return result code.
 */
static int cvGetOpticalFlow(IplImage* frame_pre, IplImage* frame,
                            CvPoint2D32f* frame_pre_feat, CvPoint2D32f* frame_feat,
                            char* opf_feat, float* opf_feat_err,
                            int num_of_feat = 400, int opf_win_sz = OPF_WIN_SIZE)
{
	IplImage *frame_gray     = NULL,
	         *frame_pre_gray = NULL,
	         *eig_image      = NULL,
	         *temp_image     = NULL,
	         *pyramid1       = NULL,
	         *pyramid2       = NULL;
	CvSize szframe = cvGetSize(frame);
	CvSize opf_window = cvSize(opf_win_sz, opf_win_sz);
	CvTermCriteria opf_termi_criteria;
	int res = CVUT_SUCCESS;

	if (!frame_pre || !frame ||
		!frame_pre_feat || !frame_feat ||
		!opf_feat || !opf_feat_err)
	{
		res = CVUT_NULL_PTR;
		goto error;
	}

	// get gray level of frame and previous frame
	frame_gray = cvCreateImage(szframe, IPL_DEPTH_8U, 1);
	if (!frame_gray) { res = CVUT_INSUFFICIENT_MEMORY; goto error; }

	if (frame->nChannels != 1)
		cvCvtColor(frame, frame_gray, CV_RGB2GRAY);
	else
		cvCopyImage(frame, frame_gray);

	frame_pre_gray = cvCreateImage(szframe, IPL_DEPTH_8U, 1);
	if (!frame_pre_gray) {
		res = CVUT_INSUFFICIENT_MEMORY;
		goto error;
	}
	if (frame_pre->nChannels != 1)
		cvCvtColor(frame_pre, frame_pre_gray, CV_RGB2GRAY);
	else
		cvCopyImage(frame_pre, frame_pre_gray);

	// temp buffer/image for optical flow
	eig_image = cvCreateImage(szframe, IPL_DEPTH_32F, 1);
	if (!eig_image) {
		res = CVUT_INSUFFICIENT_MEMORY;
		goto error;
	}
	temp_image = cvCreateImage(szframe, IPL_DEPTH_32F, 1);
	if (!temp_image) {
		res = CVUT_INSUFFICIENT_MEMORY;
		goto error;
	}

	cvGoodFeaturesToTrack(frame_gray, eig_image, temp_image,
	                      frame_feat, &num_of_feat, .01, .01, NULL);

	opf_termi_criteria = cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 50, .03);

	// pyramids for optical flow.
	pyramid1 = cvCreateImage(szframe, IPL_DEPTH_8U, 1);
	if (!pyramid1) {
		res = CVUT_INSUFFICIENT_MEMORY;
		goto error;
	}
	pyramid2 = cvCreateImage(szframe, IPL_DEPTH_8U, 1);
	if (!pyramid2) {
		res = CVUT_INSUFFICIENT_MEMORY;
		goto error;
	}

	// calculate optical flow
	cvCalcOpticalFlowPyrLK(frame_pre_gray, frame_gray,
	                       pyramid1, pyramid2,
	                       frame_feat, frame_pre_feat, num_of_feat,
	                       opf_window, 5, opf_feat,
	                       opf_feat_err, opf_termi_criteria, 0);

	// release memory
error:
	if (frame_gray)     cvReleaseImage(&frame_gray);
	if (frame_pre_gray) cvReleaseImage(&frame_pre_gray);
	if (eig_image)      cvReleaseImage(&eig_image);
	if (temp_image)     cvReleaseImage(&temp_image);
	if (pyramid1)       cvReleaseImage(&pyramid1);
	if (pyramid2)       cvReleaseImage(&pyramid2);

	return res;
}


//==== Mathematics =============================================================================================//


/**
 *  Convolute an image with a filter.
 *
 *  @param  src [in] source image, must be single channel.
 *  @param  dst [out] destination image.
 *  @param  filter[] [in] filter to be convoluted.
 *  @param  flt_c [in] column of filter.
 *  @param  flt_r [in] row of filter.
 *  @return result code.
 *
 */
static int cvConvolution2D(IplImage* src, IplImage* dst,
                           float filter[], int flt_c, int flt_r)
{
	IplImage *src32 = NULL,
	         *dst32 = NULL;
	CvMat kernel;
	int res = CVUT_SUCCESS;

	// allocate memory.
	src32 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
	if (!src32) {
		res = CVUT_INSUFFICIENT_MEMORY;
		goto error;
	}

	dst32 = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
	if (!dst32) {
		res = CVUT_INSUFFICIENT_MEMORY;
		goto error;
	}

	cvConvertScale(src, src32, 1.0);

	// 2D convolution with kernel filter.
	kernel = cvMat(flt_c, flt_r, CV_32FC1, filter);
	cvFilter2D(src32, dst32, &kernel);

	cvConvertScaleAbs(dst32, dst, 1.0);

	// release memory
error:
	if (src32) cvReleaseImage(&src32);
	if (dst32) cvReleaseImage(&dst32);

	return res;
}


}


#endif  /*__CVUT_H__*/
