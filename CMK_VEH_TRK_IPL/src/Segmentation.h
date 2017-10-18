#pragma once


#include "FgObjInfo.h"

class CSegmentation 
{
public:
	CSegmentation(void);
	virtual ~CSegmentation(void);

	virtual bool init(void);
	virtual void deinit(void);

	virtual bool isFar(CvPoint2D32f pt) { return false; }
	virtual bool isMiddle(CvPoint2D32f pt) { return false; }
	virtual bool isNear(CvPoint2D32f pt) { return false; }

	virtual IplImage* getBMask(void) { return NULL; }
	virtual vector<CFgObjInfo> doSegmentation(IplImage* pFrame);

protected:
	IplImage* m_Background;
	IplImage* m_BMask;
	IplImage* m_Roi;
};