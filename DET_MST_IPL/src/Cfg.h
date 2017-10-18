#pragma once

#include <iostream>
//#include <windows.h>	// in Windows
//#include <sys/timeb.h>	// in Windows
#include <ctime>	// in Linux
#include <opencv2/core/core.hpp>

class CCfg
{
public:
	//! full constructor
	CCfg();
	//! default destructor
	~CCfg();

	//! loads configuration file from directory
	void loadCfgFile(void);

	inline cv::Size getFrmSz(void) { return m_oFrmSz; }
	void setFrmSz(cv::Size oFrmSz) { m_oFrmSz = oFrmSz; };
	inline float getFrmRt(void) { return m_fFrmRt; }
	inline void setFrmRt(float fFrmRt) { m_fFrmRt = fFrmRt; }
	inline int getRoiArea(void) { return m_nRoiArea; }
	inline void setRoiArea(int nRoiArea) { m_nRoiArea = nRoiArea; }
	//inline SYSTEMTIME getStSysTm() { return m_tStSysTm; }	// in Windows
	//inline void setStSysTm(SYSTEMTIME tSysTm) { m_tStSysTm = tSysTm; }	// in Windows
	//inline struct _timeb getStTstp() { return m_stbStTstp; }	// in Windows
	//inline void setStTstp(struct _timeb stbStTstp) { m_stbStTstp = stbStTstp; }	// in Windows
	inline struct timespec getStTstp() { return m_stsStTstp; }	// in Linux
	inline void setStTstp(struct timespec stsStTstp) { m_stsStTstp = stsStTstp; }	// in Linux

	inline char* getInVdoPth(void) { return m_acInVdoPth; }
	inline char* getInFrmPth(void) { return m_acInFrmPth; }
	inline char* getInRoiPth(void) { return m_acInRoiPth; }
	inline char* getInDetTxtPth(void) { return m_acInDetTxtPth; }
	inline char* getOutDetTxtPth(void) { return m_acOutDetTxtPth; }
	inline char* getOutVdoPth(void) { return m_acOutVdoPth; }
	inline char* getOutImgFlrPth(void) { return m_acOutImgFlrPth; }
	inline char* getOutTstpPth(void) { return m_acOutTstpPth; }
	inline int getInVdoTyp(void) { return m_nInVdoTyp; }
	inline int getInDetTyp(void) { return m_nInDetTyp; }
	inline bool getOutDetFlg(void) { return m_bOutDetFlg; }
	inline bool getOutVdoFlg(void) { return m_bOutVdoFlg; }
	inline bool getOutImgFlg(void) { return m_bOutImgFlg; }
	inline bool getSelRoiFlg(void) { return m_bSelRoiFlg; }
	inline bool getPltDetFlg(void) { return m_bPltDetFlg; }
	inline int getProcStFrmCnt(void) { return m_nProcStFrmCnt; }
	inline int getProcFrmNum(void) { return m_nProcFrmNum; }
	inline double getOutFrmRt(void) { return m_fOutFrmRt; }
	inline int getRszFrmHei(void) { return m_nRszFrmHei; }
	inline float getDetRszRat(void) { return m_fDetRszRat; }
	inline float getDetScrThld(void) { return m_fDetScrThld; }
	inline std::vector<std::string> getDetObjCls(void) { return m_vstrDetObjCls; }
	inline std::vector<cv::Vec2f> getDetAspRatRng(void) { return m_vovDetAspRatRng; }
	inline bool getDetMltSclTstFlg(void) { return m_bDetMltSclTstFlg; }
	inline float getDetHogHitThld(void) { return m_fDetHogHitThld; }
	inline int getDetHogWinStrdSz(void) { return m_nDetHogWinStrdSz; }
	inline int getDetHogPadSz(void) { return m_nDetHogPadSz; }
	inline float getDetHogScl0(void) { return m_fDetHogScl0; }
	inline int getDetHogGrpThld(void) { return m_nDetHogGrpThld; }
	inline char* getDetYoloDknetPth(void) { return m_acDetYoloDknetPth; }	// in Linux
	inline char* getDetYoloDataPth(void) { return m_acDetYoloDataPth; }	// in Linux
	inline char* getDetYoloCfgPth(void) { return m_acDetYoloCfgPth; }	// in Linux
	inline char* getDetYoloWgtPth(void) { return m_acDetYoloWgtPth; }	// in Linux
	//inline char* getDetC4Mdl1Pth(void) { return m_acDetC4Mdl1Pth; }
	//inline char* getDetC4Mdl2Pth(void) { return m_acDetC4Mdl2Pth; }
	//inline int getDetC4WidThld(void) { return m_nDetC4WidThld; }
	//inline int getDetC4HeiThld(void) { return m_nDetC4HeiThld; }
	//inline int getDetC4XDiv(void) { return m_nDetC4XDiv; }
	//inline int getDetC4YDiv(void) { return m_nDetC4YDiv; }

private:
	//! reads char array
	std::string readCharArr(std::string strCfg, int nParamPos);
	//! reads integer number
	int readInt(std::string strCfg, int nParamPos);
	//! reads float number
	float readFlt(std::string strCfg, int nParamPos);
	//! reads bool value
	bool readBool(std::string strCfg, int nParamPos);
	//! reads vector of char arrays
	void readVecStr(std::string strCfg, int nParamPos, std::vector<std::string>& vstrCharArr);
	//! reads 2D point
	cv::Point read2dPt(std::string strCfg, int nParamPos);
	//! reads vector of pairs of float numbers
	std::vector<cv::Vec2f> readVecFltPr(std::string strCfg, int nParamPos);

	//! video frame size
	cv::Size m_oFrmSz;
	//! video frame rate
	float m_fFrmRt;
	//! ROI area
	int m_nRoiArea;
	////! starting timestamp
	//SYSTEMTIME m_tStSysTm;	// in Windows
	////! starting timestamp
	//struct _timeb m_stbStTstp;	// in Windows
	//! starting timestamp
	struct timespec m_stsStTstp;	// in Linux
	//! path of input video stream
	char m_acInVdoPth[256];
	//! path of input frame image, necessary when m_nInVdoTyp == 3
	char m_acInFrmPth[256];
	//! path of input ROI image
	char m_acInRoiPth[256];
	//! path of input text file of object detection, necessary when m_bInDetTyp == 0
	char m_acInDetTxtPth[256];
	//! path of output text file of object detection, necessary when m_bOutDetFlg == true
	char m_acOutDetTxtPth[256];
	//! path of output video file, necessary when m_bOutVdoFlg == true
	char m_acOutVdoPth[256];
	//! path of folder for output image files, necessary when m_bOutImgFlg == true
	char m_acOutImgFlrPth[256];
	//! path of output timestamp text file, necessary when m_nInVdoTyp > 0
	char m_acOutTstpPth[256];
	//! type of input video source: 0: video file; 1: USB camera; 2: IP camera; 3: updating image
	int m_nInVdoTyp;
	//! type of input detection: 0: text file; 1: online HOG (person only); 2: online YOLO v2
	int m_nInDetTyp;
	//! flag of output detection results as txt file
	bool m_bOutDetFlg;
	//! flag of output video file
	bool m_bOutVdoFlg;
	//! flag of output image files
	bool m_bOutImgFlg;
	//! flag of selecting ROI image
	bool m_bSelRoiFlg;
	//! flag of plotting object detection
	bool m_bPltDetFlg;
	//! starting frame count to process
	int m_nProcStFrmCnt;
	//! number of frames to process (-1: till the end of the video source)
	int m_nProcFrmNum;
	//! frame rate of the output video, necessary when m_bOutVdoFlg = true and m_nInVdoTyp > 0
	double m_fOutFrmRt;
	//! resized video frame height (-1: original size)
	int m_nRszFrmHei;
	//! the ratio to resize the original image for human detection, necessary when m_nInDetTyp > 0
	float m_fDetRszRat;
	//! threshold of detection score (in percentage), necessary when m_nInDetTyp == 0 or 2
	float m_fDetScrThld;
	//! list of object classes, necessary when m_nInDetTyp == 0 or 2
	std::vector<std::string> m_vstrDetObjCls;
	//! list of aspect ratio ranges for object classes, necessary when m_nInDetTyp == 0 or 2
	std::vector<cv::Vec2f> m_vovDetAspRatRng;
	 //! flag of multi-scale testing, necessary when m_nInDetTyp > 0
    bool m_bDetMltSclTstFlg;
	//! threshold for the feature distance with SVM classifying plane for HOG human detection, necessary when m_nInDetTyp == 1
	float m_fDetHogHitThld;
	//! size of window stride for HOG human detection, necessary when m_nInDetTyp == 1
	int m_nDetHogWinStrdSz;
	//! size of the mock parameter for HOG human detection, necessary when m_nInDetTyp == 1
	int m_nDetHogPadSz;
	//! coefficient of the detection window increase for HOG human detection, necessary when m_nInDetTyp == 1
	float m_fDetHogScl0;
	//! coefficient to regulate the similarity threshold for HOG human detection, necessary when m_nInDetTyp == 1
	int m_nDetHogGrpThld;
	//! path of darknet for YOLO v2 detection, necessary when m_nInDetTyp == 2
    char m_acDetYoloDknetPth[256];	// in Linux
	//! path of data file for YOLO v2 detection, necessary when m_nInDetTyp == 2
    char m_acDetYoloDataPth[256];	// in Linux
    //! path of configuration file for YOLO v2 detection, necessary when m_nInDetTyp == 2
    char m_acDetYoloCfgPth[256];	// in Linux
    //! path of weights file for YOLO v2 detection, necessary when m_nInDetTyp == 2
    char m_acDetYoloWgtPth[256];	// in Linux
	////! path of the 1st model for C4 human detection, necessary when m_nInDetTyp == 2
	//char m_acDetC4Mdl1Pth[256];
	////! path of the 2nd model for C4 human detection, necessary when m_nInDetTyp == 2
	//char m_acDetC4Mdl2Pth[256];
	////! width threshold of detected object for C4 human detection, necessary when m_nInDetTyp == 2
	//int m_nDetC4WidThld;
	////! height threshold of detected object for C4 human detection, necessary when m_nInDetTyp == 2
	//int m_nDetC4HeiThld;
	////! division along x direction for C4 human detection, necessary when m_nInDetTyp == 2
	//int m_nDetC4XDiv;
	////! division along y direction for C4 human detection, necessary when m_nInDetTyp == 2
	//int m_nDetC4YDiv;
};
