#pragma once
#include "Tracking.h"
#include <inttypes.h>
#include <stdint.h>

class CObjectClass {
public:
	CObjectClass(void) {}
	~CObjectClass(void) {}

	inline void setObjPosition(CvPoint pose) { m_Position = pose; }
	inline void setObjRect(CvRect r) { m_Rect = r; }
	inline void setObjEllipse(CvBox2D ellipse) { m_Ellipse = ellipse; }
	inline void setObjID(int id) { m_ID = id; }

	inline CvPoint getObjPosition() { return m_Position; }
	inline CvRect getObjRect() { return m_Rect; }
	inline CvBox2D getObjEllipse() { return m_Ellipse; }
	inline int getObjID() { return m_ID; }

	inline int getObjPathNodeNum() { return int(m_ObjPath.size()); }
	inline void addObjPathNode(CvPoint pt) { m_ObjPath.push_back(pt); }
	inline CvPoint getObjPathNode(int index) { return m_ObjPath[index]; }

protected:
	CvPoint m_Position;
	CvRect m_Rect;
	CvBox2D m_Ellipse;

	int m_ID;

	vector<CvPoint> m_ObjPath;
};


class CVehicleClass : public CObjectClass
{
public:
	CVehicleClass(void) { m_bModelValid = false; }
	~CVehicleClass(void) {}

	inline void setModelValid(bool valid) { m_bModelValid = valid; }
	inline void set3dModel(CDeformableModel model) { m_3dModel = model; };
	inline void setShapeModel(CShapeModel model) { m_ShapeModel = model; }

	inline bool isModelVaild() { return m_bModelValid; }
	inline CDeformableModel get3dModel(void) { return m_3dModel; };
	inline CShapeModel getShapeModel(void) { return m_ShapeModel; }

protected:
	CDeformableModel m_3dModel;
	CShapeModel m_ShapeModel;

	bool m_bModelValid;
};


class CIndVehicleTracking : public CTracking
{
public:
	CIndVehicleTracking(int Ncamera);
	virtual ~CIndVehicleTracking(void);

	bool IndVehicleTracking_Init(int iCameraID, int iVideoWidth, int iVideoHeight, const char* ConfigPath);
	void IndVehicleTracking_Uninit(int iCameraID);

	void IndVehicleTracking_SetBgImg(int iCameraID, BYTE* pbBgImgRGB24);
	void IndVehicleTracking_SetRoiImg(int iCameraID, BYTE* pbRoiImgRGB24);

	int IndVehicleTracking_GetObjectCnt(int iCameraID);
	CVehicleClass IndVehicleTracking_GetObject(int iCameraID, int iObjectIdx);
	BOOL IndVehicleTracking_Process(int iCameraID, BYTE* pbCurrentFrameRGB24, __int64_t iTimeStamp);

private:
	int m_nNcamera;

	SurCamInfo* m_SurCam;

	int m_nCamIDIdx;
	int* m_CamIDmap;
	int* m_CpType;
	int* m_SegType;
	int* m_SfType;

	int getCamIDIndex(int id);

	IplImage* getIplImage(BYTE* pRGBdata);

	bool initSurCamInfo(SurCamInfo& sur_cam, const char* inifile_path);
};
