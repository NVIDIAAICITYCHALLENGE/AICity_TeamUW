/*******************************************************************************
 *  Program: Common UTilities                                                  *
 *  Programmer: Young Lee                                                      *
 *  File Name: commonut.h                                                      *
 *  History:                                                                   *
 *      2013/06/08 v0.0:    initial version.                                   *
 *******************************************************************************/
#pragma once


//---- Include ------------------------------------------------------------------------------------------------------//
#include "cvut.h"


//---- Global functions/variables -----------------------------------------------------------------------------------//

/**                                                                  
 *  print the matrix on the screen.
 *
 *  @param  name [in] matrix's name.
 *  @param  matrix [in] matrix's data.
 */
static void cmPrintMatrix(char* name, CvMat* matrix)
{
	printf("%s\n", name);
	for(int i = 0; i < matrix->rows; i++) {
		for(int j = 0 ;j < matrix->cols; j++) {
			printf("%.3f\t",cvGet2D(matrix, i, j).val[0]);
		}
		printf("\n");
	}
	printf("\n");
}


/**                                                                  
 *  Check if a point is in a polygon.
 *
 *  @param  pt [in] checked point.
 *  @param  polygon [in] checked polygon, a series of points (order should be clockwise/counterclockwise).
 *  @return true means the point is in the polygon, else not.
 */
static bool cmPointinPloygon(CvPoint2D32f pt, vector<CvPoint2D32f>& polygon)
{
	for (int i = 0; i < polygon.size(); i++)
		if (pt.x == polygon[i].x && pt.y == polygon[i].y)
			return false;

	bool bInPloy = false;
	for (int i = 0, j = int(polygon.size())-1; i < int(polygon.size()); j = i++) {
		if (((polygon[i].y > pt.y) != (polygon[j].y > pt.y)) &&
			(pt.x < (polygon[j].x-polygon[i].x) * (pt.y-polygon[i].y) / (polygon[j].y-polygon[i].y) + polygon[i].x))
			bInPloy = !bInPloy;
	}
	return bInPloy;
}


/** 
 *  check if point is in box or not.
 *
 *  @param  pt [in] checked point.
 *  @param  box [in] checked box, in terms of CvBox2D.
 *  @return true means the point is in the polygon, else not.
 */
static bool cmPointinBox2D(CvPoint2D32f pt, CvBox2D box)
{
	bool bInBox = false;
	CvPoint2D32f ptBoxf[4]; 

	cvBoxPoints(box, ptBoxf);
	for (int i = 0, j = 3; i < 4; j = i++) {
		if (((ptBoxf[i].y > pt.y) != (ptBoxf[j].y > pt.y)) &&
			(pt.x < (ptBoxf[j].x-ptBoxf[i].x) * (pt.y-ptBoxf[i].y) / (ptBoxf[j].y-ptBoxf[i].y) + ptBoxf[i].x))
			bInBox = !bInBox;
	}
	return bInBox;
}

/**                                                                  
 *  get ROI binary mask.
 *
 *  @param  pMask [in] input mask.
 *  @param  roi [in] ROI region in terms of CvBox2D.
 */
typedef enum { MT_RECTANGLE, MT_ELLIPSE, MT_RECT_OBJECT, MT_ELLI_OBJECT } MaskType;
static IplImage* cmGetROIMask(IplImage* pMask, CvBox2D roi, MaskType type = MT_RECTANGLE)
{
	IplImage* pROImask = NULL;
	CvMemStorage* storage = NULL;
	CvSeq* seq = NULL;
	CvPoint2D32f ptBoxf[4]; 
	CvPoint ptBox[4];

	pROImask = cvCreateImage(cvGetSize(pMask), pMask->depth, pMask->nChannels);
	if (!pROImask) goto error; cvZero(pROImask);

	switch (type) {
	default:
	case MT_RECTANGLE: 
		storage = cvCreateMemStorage(0);
		if (!storage) goto error;
		seq = cvCreateSeq(CV_SEQ_POLYGON, sizeof(CvSeq), sizeof(CvPoint), storage);
		if (!seq) goto error;

		cvBoxPoints(roi, ptBoxf);
		ptBox[0] = cvPointFrom32f(ptBoxf[0]);
		ptBox[1] = cvPointFrom32f(ptBoxf[1]);
		ptBox[2] = cvPointFrom32f(ptBoxf[2]);
		ptBox[3] = cvPointFrom32f(ptBoxf[3]);
		cvSeqPushMulti(seq, ptBox, 4); 
		cvDrawContours(pROImask, seq, cvScalar(255, 255, 255), cvScalar(255, 255, 255), 0, CV_FILLED);
		break;
	case MT_RECT_OBJECT:
		storage = cvCreateMemStorage(0);
		if (!storage) goto error;
		seq = cvCreateSeq(CV_SEQ_POLYGON, sizeof(CvSeq), sizeof(CvPoint), storage);
		if (!seq) goto error;

		cvBoxPoints(roi, ptBoxf);
		ptBox[0] = cvPointFrom32f(ptBoxf[0]);
		ptBox[1] = cvPointFrom32f(ptBoxf[1]);
		ptBox[2] = cvPointFrom32f(ptBoxf[2]);
		ptBox[3] = cvPointFrom32f(ptBoxf[3]);
		cvSeqPushMulti(seq, ptBox, 4); 
		cvDrawContours(pROImask, seq, cvScalar(255, 255, 255), cvScalar(255, 255, 255), 0, CV_FILLED);
		cvAnd(pMask, pROImask, pROImask); 
		break;
	case MT_ELLIPSE:
		cvEllipseBox(pROImask, roi, CV_RGB(255, 255, 255), -1); 
		break;
	case MT_ELLI_OBJECT:
		cvEllipseBox(pROImask, roi, CV_RGB(255, 255, 255), -1);
		cvAnd(pMask, pROImask, pROImask); 
		break;
	}

error:
	if (seq) cvClearSeq(seq);
	if (storage) cvReleaseMemStorage(&storage);

	return pROImask;
}

static IplImage* getBinaryMask(IplImage* pMask, vector<CvPoint2D32f> polygon)
{
	IplImage* pROImask = NULL;
	CvMemStorage* storage = NULL;
	CvSeq* seq = NULL;
	CvPoint* ptBox = NULL;
	int num_t = int(polygon.size());

	if (!num_t) goto error;
	ptBox = new CvPoint[num_t];
	if (!ptBox) goto error;

	pROImask = cvCreateImage(cvGetSize(pMask), pMask->depth, pMask->nChannels);
	if (!pROImask) goto error; cvZero(pROImask);
	storage = cvCreateMemStorage(0);
	if (!storage) goto error;
	seq = cvCreateSeq(CV_SEQ_POLYGON, sizeof(CvSeq), sizeof(CvPoint), storage);
	if (!seq) goto error;

	for (int i = 0; i < num_t; i++)
		ptBox[i] = cvPointFrom32f(polygon[i]);

	cvSeqPushMulti(seq, ptBox, num_t); 
	cvDrawContours(pROImask, seq, cvScalar(255, 255, 255), cvScalar(255, 255, 255), 0, CV_FILLED);

error:
	if (ptBox) delete [] ptBox;
	if (seq) cvClearSeq(seq);
	if (storage) cvReleaseMemStorage(&storage);

	return pROImask;
}

static IplImage* getROIMaskByBox2D(IplImage* pMask, CvBox2D roi)
{
	//*
	IplImage* pROImask = NULL;
	CvMemStorage* storage = NULL;
	CvSeq* seq = NULL;
	CvPoint2D32f ptBoxf[4]; 
	CvPoint ptBox[4];

	pROImask = cvCreateImage(cvGetSize(pMask), pMask->depth, pMask->nChannels);
	if (!pROImask) goto error; cvZero(pROImask);
	storage = cvCreateMemStorage(0);
	if (!storage) goto error;
	seq = cvCreateSeq(CV_SEQ_POLYGON, sizeof(CvSeq), sizeof(CvPoint), storage);
	if (!seq) goto error;

	cvBoxPoints(roi, ptBoxf);
	ptBox[0] = cvPointFrom32f(ptBoxf[0]);
	ptBox[1] = cvPointFrom32f(ptBoxf[1]);
	ptBox[2] = cvPointFrom32f(ptBoxf[2]);
	ptBox[3] = cvPointFrom32f(ptBoxf[3]);
	cvSeqPushMulti(seq, ptBox, 4); 
	cvDrawContours(pROImask, seq, cvScalar(255, 255, 255), cvScalar(255, 255, 255), 0, CV_FILLED);

error:
	if (seq) cvClearSeq(seq);
	if (storage) cvReleaseMemStorage(&storage);

	return pROImask;//*/

	/*
	IplImage* pROImask = NULL;

	pROImask = cvCreateImage(cvGetSize(pMask), pMask->depth, pMask->nChannels);
	if (!pROImask) goto error; cvZero(pROImask);

	cvEllipseBox(pROImask, roi, CV_RGB(255, 255, 255), -MAX(roi.size.width/2, roi.size.height/2));

error:
	return pROImask;//*/
}

static CvPoint3D32f cmAveragePoints(vector<CvPoint3D32f> points)
{
	CvPoint3D32f avg = cvPoint3D32f(0, 0, 0);
	for (int p = 0; p < points.size(); p++)
	{
		avg.x += points[p].x;
		avg.y += points[p].y;
		avg.z += points[p].z;
	}
	avg.x /= points.size();
	avg.y /= points.size();
	avg.z /= points.size();

	return avg;
}