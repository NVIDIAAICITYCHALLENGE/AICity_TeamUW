#pragma once

#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include "utils.h"
#include "Cfg.h"
/*#include "ObjDetC4\Pedestrian.h"*/

//! default score for object detection (for HOG and C4) (default: 50.0f)
#define DFLT_DET_SCR (50.0f)
//! default class for object detection (for HOG and C4) (default: "person")
#define DFLT_DET_CLS ("person")
//! define the flag of non-maximum suppression in object detection (default: false)
#define NMS_DET_FLG (false)
//! define the threshold for the IOU of non-maximum suppression in object detection (default: 0.50f)
#define NMS_DET_IOU_THLD (0.50f)
//! define the threshold for overlapping area ratio for removing outliers (default: 0.50f)
#define OVLP_AREA_RAT_THLD (0.50f)

// detected node
class CDetNd
{
public:
	CDetNd(void);
	CDetNd(cv::Rect2f oDetBBox, float fDetScr, char* acDetCls = "person", int nFrmCnt = -1);
	~CDetNd(void);

	inline int getFrmCnt(void) { return m_nFrmCnt; }
	inline void setFrmCnt(int nFrmCnt) { m_nFrmCnt = nFrmCnt; }
	inline cv::Rect2f getDetBBox(void) { return m_oDetBBox; }
	inline void setDetBBox(cv::Rect2f oDetBBox) { m_oDetBBox = oDetBBox; }
	inline float getDetScr(void) { return m_fDetScr; }
	inline void setDetScr(float fDetScr) { m_fDetScr = fDetScr; }
	inline char* getDetCls(void) { return m_acDetCls; }
	inline void setDetCls(char* acDetCls) { std::sprintf(m_acDetCls, acDetCls); }

protected:
	//! frame count associated with object detection
	int m_nFrmCnt;
	//! bounding box of object detection
	cv::Rect2f m_oDetBBox;
	//! score of object detection
	float m_fDetScr;
	//! class of object detection
	char m_acDetCls[32];
};

class CObjDet
{
public:
	CObjDet(void);
	~CObjDet(void);

	//! initializes object detector
	void initialize(CCfg oCfg, cv::Mat oImgRoi);
	//! performs object detection
	void process(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd, int nFrmCnt);
	//! outputs object detection as txt file and/or images
	void output(cv::Mat& oImgFrm, std::vector<CDetNd>& voDetNd);

private:
	//! reads object detection results
	void rdObjDet(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd, int nFrmCnt);
	//! runs object detection
	void rnObjDet(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd, int nFrmCnt);
	//! detects object(s) using HOG
	void detHog(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd);
	//! detects object(s) using YOLO v2
	void detYolo(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd);	// in Linux
	////! detects object(s) using C4
	//void detC4(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd);
	//! detects object(s) through multi-scale testing
	void detMltSclTst(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd);
	//! performs non-maximum suppression
	void nonMaxSuppr(std::vector<CDetNd>& voDetNd);
	//! post-process to remove outliers
	void rmvOutBBox(std::vector<CDetNd>& voDetNd);
	//! outputs object detection as txt file
	void outTxt(std::vector<CDetNd>& voDetNd);
	//! outputs object detection as images
	void pltDetBBox(cv::Mat& oImgFrm, std::vector<CDetNd>& voDetNd);

	//! configuration file
	CCfg m_oCfg;
	//! ROI image
	cv::Mat m_oImgRoi;
	//! next detected node
	CDetNd m_oNxtDetNd;
	//! text file for reading detection results
	std::ifstream m_ifsInDetTxt;
	//! HOG descriptor
	cv::HOGDescriptor m_oHogDesc;
	//! flag of YOLO initialization
	bool m_bYoloInit;   // in Linux
	////! C4 scanner
	//DetectionScanner* m_poC4Scnr;
};
