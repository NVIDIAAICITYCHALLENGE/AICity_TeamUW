#pragma once


#include "GeneralDef.h"
#include "Segmentation.h"

using namespace std;


// background mode
enum { FST_FRAME, LOAD_IMG };


class CBasicBgSub : public CSegmentation
{
public:
	CBasicBgSub(void);
	~CBasicBgSub(void);

	bool init(void);
	void deinit(void);

	bool isFar(CvPoint2D32f pt);
	bool isMiddle(CvPoint2D32f pt);
	bool isNear(CvPoint2D32f pt);

	void setBgImg(IplImage* img);
	void setRoiImg(IplImage* img);

	inline IplImage* getBMask(void) { return m_BMask; }
	vector<CFgObjInfo> doSegmentation(IplImage* pFrame);

	inline void setBgMode(int mode) { m_BgMode = mode; }
	inline void setHoleFilling(bool enable) { m_bHoleFilling = enable; }
	inline void setOtsuThresh1(float otsu_thresh1) { m_OtsuThresh1 = otsu_thresh1; }
	inline void setOtsuThresh2(float otsu_thresh2) { m_OtsuThresh2 = otsu_thresh2; }
	inline void setTopHat(int size) { m_TopHat = size; }
	inline void setClose(int size) { m_Close = size; }
	inline void setOpen(int size) { m_Open = size; }
	inline void setDilate(int size) { m_Dilate = size; }
	inline void setErode(int size) { m_Erode = size; }
	inline void setFarLineRatio(float far_dis) { m_FarLineRatio = far_dis; }
	inline void setNearLineRatio(float near_dis) { m_NearLineRatio = near_dis; }
	inline void setBlobThreshFar(float blob_thresh) { m_BlobThreshFar = blob_thresh; }
	inline void setBlobThreshMid(float blob_thresh) { m_BlobThreshMid = blob_thresh; }
	inline void setBlobThreshNear(float blob_thresh) { m_BlobThreshNear = blob_thresh; }

	inline void setBgImgPath(const char* bg_img_path) { strcpy(m_BgImgPath, bg_img_path); }
	inline void setBgRoiPath(const char* roi_img_path) { strcpy(m_RoiImgPath, roi_img_path); }
	inline void setDisplayBlobs(bool enable) { m_bDisplayBlobs = enable; }
	inline void setSaveBlobs(bool enable) { m_bSaveBlobs = enable; }

private:
	// parameters
	int m_BgMode;
	int m_Width;
	int m_Height;

	float m_OtsuThresh1;
	float m_OtsuThresh2;

	int m_TopHat;
	int m_Close;
	int m_Open;
	int m_Dilate;
	int m_Erode;

	float m_FarLineRatio;         // threshold of far line ([0, 1] for 2d, length unit for 3d)
	float m_NearLineRatio;        // threshold of near line ([0, 1] for 2d, length unit for 3d)
	float m_BlobThreshFar;       // threshold of blob size (far), [0, 1]
	float m_BlobThreshMid;    // threshold of blob size (middle), [0, 1]
	float m_BlobThreshNear;      // threshold of blob size (near), [0, 1]

	char m_BgImgPath[MAX_PATH]; // path of background image for LOAD_IMG mode
	char m_RoiImgPath[MAX_PATH];

	IplImage* m_Roi;

	bool m_bHoleFilling;
	bool m_bDisplayBlobs;
	bool m_bSaveBlobs;

	int OtsuThreshold(IplImage* pImg);

	bool isBlobTooSmall(CFgObjInfo fgobj);
	bool isInROI(CFgObjInfo fgobj);

	void doDifference(IplImage* pImg, IplImage* pBg, IplImage* pSeg);

	CFgObjInfo getObjInfo(CvSeq* pContour, CvMemStorage* pStorage);

	void HoleFilling(IplImage** src, CvRect rect, int gap_thresh);

	vector<CFgObjInfo> PostProcessing(IplImage* pFrame, IplImage* pSeg);
};
