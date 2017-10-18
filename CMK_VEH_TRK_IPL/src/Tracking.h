#pragma once


#include "GeneralDef.h"
#include "Tracker.h"
#include "Segmentation.h"
#include "VehicleShapeFitting.h"


class CTracking;


class CamInfo
{
public:
	CamInfo(void)
	{
		m_CamSrc = NULL;
		m_CamParam = NULL;
		m_SegEngine = NULL;
		m_SFEngine = NULL;

		mCamID = -1;
		m_Width = 0;
		m_Height = 0;

		m_TrackObjList.clear();
	}
	virtual ~CamInfo(void) {}

	virtual bool init(void) { return false; }
	virtual void deinit(void) {}
	virtual void startTracking(IplImage* frame) {}
	virtual void stopTracking(void) {}

	inline void setTrackingSystem(CTracking* pSys) { m_Tracking = pSys; }
	inline void setCameraID(int id) { mCamID = id; }
	inline void setCameraSrc(CCamera* cam_src) { m_CamSrc = cam_src; }
	inline void setCameraParam(CCameraParam* cam_aram) { m_CamParam = cam_aram; }
	inline void setSegEngine(CSegmentation* oe_engine) { m_SegEngine = oe_engine; }
	inline void setSFEngine(CShapeFitting* sf_engine) { m_SFEngine = sf_engine; }

	inline int getCameraID(void) { return mCamID; }
	inline unsigned int getWidth(void) { return m_Width; }
	inline unsigned int getHeight(void) { return m_Height; }
	inline int getNumTrackObj(void) { return int(m_TrackObjList.size()); }
	inline CTrackObj getTrackObj(int index) { return m_TrackObjList[index]; }
	inline CCamera* getCameraSrc(void) { return m_CamSrc; }
	inline CCameraParam* getCameraParam(void) { return m_CamParam; }
	inline CSegmentation* getSegEngine(void) { return m_SegEngine; }
	inline CShapeFitting* getSFEngine(void) { return m_SFEngine; }


protected:
	CTracking* m_Tracking;            // tracking system

	// camera info & engines
	CCamera* m_CamSrc;                // camera/video source
	CCameraParam* m_CamParam;         // camera(s) parameters
	CSegmentation* m_SegEngine;       // object extraction engine
	CShapeFitting* m_SFEngine;        // shape fitting engine

	int mCamID;                       // camera ID
	unsigned int m_Width;             // camera width
	unsigned int m_Height;            // camera height

	vector<CTrackObj> m_TrackObjList;  // tracked object list
};


class SurCamInfo : public CamInfo
{
public:
	SurCamInfo(void);
	~SurCamInfo(void);

	bool init(void);
	void deinit(void);
	void startTracking(IplImage* frame);
	void stopTracking(void);

	// system configurations
	inline void enableVCD(bool enable) { m_bEnableVCD = enable; }
	inline void setKernelTrackingType(int type) { m_KernelTrackingType = type; }
	inline void setResizeRatio(float ratio) { m_ResizeRatio = ratio; }

	inline void setMarginThreshFar(float thresh) { m_Tracker.setMarginThreshFar(thresh); }
	inline void setMarginThreshMid(float thresh) { m_Tracker.setMarginThreshMid(thresh); }
	inline void setMarginThreshNear(float thresh) { m_Tracker.setMarginThreshNear(thresh); }
	inline void setEnlargeFactor(float factor) { m_Tracker.setEnlargeFactor(factor); }
	inline void setScaleBase(float scale_base) { m_Tracker.setScaleBase(scale_base); }
	inline void setValidVel2dThresh(float vel2d) { m_Tracker.setValidVel2dThresh(vel2d); }
	inline void setValidVel3dThresh(float vel3d) { m_Tracker.setValidVel3dThresh(vel3d); }
	inline void setEstVelocityMethod(int method) { m_Tracker.setEstVelocityMethod(method); }
	inline void setEstVelocityFrames(int frame) { m_Tracker.setEstVelocityFrames(frame); }
	inline void setEnterFrameThresh(int thresh) { m_Tracker.setEnterFrameThresh(thresh); }
	inline void set3DModelUpdateDuration(unsigned int duration) { m_Tracker.set3DModelUpdateDuration(duration); }
	inline void setKernelUpdateDuration(unsigned int duration) { m_Tracker.setKernelUpdateDuration(duration); }
	inline void setInitPoseSftFar(CvPoint2D32f sft) { m_Tracker.setInitPoseSftFar(sft); }
	inline void setInitPoseSftMid(CvPoint2D32f sft) { m_Tracker.setInitPoseSftMid(sft); }
	inline void setInitPoseSftNear(CvPoint2D32f sft) { m_Tracker.setInitPoseSftNear(sft); }
	inline void setScaleRatioFar(float l, float w, float h) { m_Tracker.setScaleRatioFar(cvPoint3D32f(l, w, h)); }
	inline void setScaleRatioMid(float l, float w, float h) { m_Tracker.setScaleRatioMid(cvPoint3D32f(l, w, h)); }
	inline void setScaleRatioNear(float l, float w, float h) { m_Tracker.setScaleRatioNear(cvPoint3D32f(l, w, h)); }

	// display options
	inline void setDisplayThickness(int thickness) { m_DisplayThickness = thickness; }
	inline void setDisplayBlock(bool enable) { m_bDisplayBlock = enable; }
	inline void setDisplaySegment(bool enable) { m_bDisplaySegment = enable; }
	inline void setDisplayPath(bool enable) { m_bDisplayPath = enable; }
	inline void setDisplay3dModel(bool enable) { m_bDisplay3dModel = enable; }
	inline void setDisplayKernels(bool enable) { m_bDisplayKernels = enable; }
	inline void setDisplaySize(CvSize size) { m_DisplaySize = size; }
	inline void setSaveDebugFrames(bool enable) { m_bSaveDebugFrames = enable; }
	inline void setBeginFrame(unsigned long frame) { m_BeginFrame = frame; }
	inline CvSize getDisplaySize() { return m_DisplaySize; }


private:
	IplImage* initFrame(IplImage* frame);
	void checkMergeStatus(vector<CFgObjInfo>& vFgObj);

	void EnterDetection(vector<CFgObjInfo>& vFgObj);
	void LeaveDetection(vector<CFgObjInfo>& vFgObj);

	void KalmanPredict(CTrackObj* pObj);
	void KalmanUpdate(CTrackObj* pObj);
	void updateFgObjInfo(CTrackObj* pObj);

	bool initKernels2DC(CTrackObj* pObj);
	bool initKernels3DC(CTrackObj* pObj);
	bool initPlaneKernel2DC(C2DCKernel& kernel2dc, vector<CvPoint2D32f> plane2d,
		                    C2DCKernelTracker* knlTkr, IplImage* pFrame_pre_cvt);
	bool initPlaneKernel3DC(C3DCKernel& kernel3dc, C3DCKernelTracker* knl_tkr, IplImage* frame_cvt);
	void shiftKernels2DC(CTrackObj* pObj);
	void shiftKernels3DC(CTrackObj* pObj);
	CDeformableModel KernelTracking2DC(CTrackObj* pObj, int upd);
	CDeformableModel KernelTracking3DC(CTrackObj* pObj, int upd);
	CDeformableModel build3dModelByKernel2DC(CTrackObj* pObj);
	CDeformableModel build3dModelByKernel3DC(CTrackObj* pObj);

	void build3dVehicleModel(CTrackObj* pObj);
	void adopt3DVehicleModel(CTrackObj* pObj);
	CDeformableModel VehicleShapeFitting(CTrackObj* pObj, IplImage* pImg);
	CDeformableModel VehiclePoseEstimation(CTrackObj* pObj, IplImage* pImg);
	CvPoint3D32f getScaleRatio(CTrackObj* pObj);


	CvBox2D getPlane3DInBox2D(CvPoint3D32f p3d1, CvPoint3D32f p3d2, CvPoint3D32f p3d3, CvPoint3D32f p3d4);
	IplImage* getFeatureSide(CTrackObj* pObj);
	//bool LicensePlateRecognition(CTrackObj* pObj);
	bool VehicleColorDetection(CTrackObj* pObj);

	CTrackObj* isOccluded(CTrackObj* pObj1, CTrackObj* pObj2);
	bool isInSameBlob(CFgObjInfo fgobj1, CFgObjInfo fgobj2, vector<CFgObjInfo>* vpFgObj);
	bool isObjectLeft(CTrackObj* pObj, vector<CFgObjInfo>& vpFgObj);
	bool isTotalObjectInFrame(CTrackObj* pTrkObj);
	bool isNewObjEnterAndMerge(CFgObjInfo* pBlob, CTrackObj* pTrkObj, int overlap);
	bool isBlobValid(CFgObjInfo obj);
	bool isPathNodeValid(CTrackObj* pObj);
	bool is3dPredicable(CTrackObj* pObj);
	bool is3dModelUpdate(CTrackObj* pObj);
	bool isVelocity3dValid(CTrackObj* pObj);
	bool isDebug();

	int addNewObj(CFgObjInfo fgobj, CFgObjInfo fgobj_pre, TrackingType type = TT_NORMAL);
	CFgObjInfo getRoughNewObjInfo(CFgObjInfo* pBlob, CTrackObj* pTrkObj);

	int calOverlapArea(CFgObjInfo obj1, IplImage* mask1, CFgObjInfo obj2, IplImage* mask2);
	float calKLDistance(float a, float b);
	float cal3dModelSimi(CDeformableModel* pModel3d1, CDeformableModel* pModel3d2);

	void updateMergeGroup(CvPoint idx);

	CTrackObj findTrackObjByID(int id);
	void reversePreviousStatus(int id);

	void drawPlane(IplImage* img, vector<CvPoint2D32f> plane);
	IplImage* createTargetImage(CTrackObj* pObj);
	IplImage* createTargetImageBy3dModel(CTrackObj* pObj);
	void getValidFgObj(CFgObjInfo& fgobj, CvRect bound);

	CvBox2D get3dModelBox2D(CShapeModel* model2d);

	void updateObjectsArea();

	CCameraParam resizeCamParam(CCameraParam cam_aram_ori, float ratio_rsz);

	CvPoint3D32f estPosition3d(CTrackObj* pObj, bool bUseVelocity3d = false);
	CvPoint2D32f estVelocity2D(CTrackObj* pObj);
	CvPoint3D32f estVelocity3d(CTrackObj* pObj, bool b3dPredicable = false);
	CvPoint2D32f rectifyInitPose(CvPoint2D32f p2d);
	CvPoint3D32f estGroundPose3d(CvPoint2D32f p2d);
	CvPoint3D32f estOrientation3d(CvPoint3D32f v3d);
	CvPoint3D32f estPosition3dbyZ(CvPoint2D32f p2d, float Z);

	// for 3d model
	void save3dModelFile(CTrackObj* pObj, CDeformableModel* model3d);
	bool read3dModelFile(CTrackObj* pObj, CDeformableModel& model3d);

	// for displaying results
	void DisplaySegment(vector<CFgObjInfo>* vFgObj);
	void DisplayPath(CTrackObj* pTrkObj);
	void DisplayTracking();


	//---- Class member -----------------------------------------------------------//
	CSurTracker m_Tracker;
	CVehicleShapeFitting* m_VSFengine;

	bool m_bStopTracking;

	int m_TotalLeftIndex;

	// system configurations
	float m_ResizeRatio;
	bool m_bEnableVCD;
	int m_KernelTrackingType;

	// display options
	IplImage* m_Display;
	bool m_bDisplayBlock;
	bool m_bDisplay3dModel;
	bool m_bDisplayKernels;
	bool m_bDisplaySegment;
	bool m_bDisplayPath;
	bool m_bSaveDebugFrames;
	int m_DisplayThickness;
	unsigned long m_BeginFrame;
	CvSize m_DisplaySize;
};


class VelCamInfo : public CamInfo
{
public:
	VelCamInfo(void);
	~VelCamInfo(void);

	bool init(void);
	void deinit(void);
	void startTracking(IplImage* frame);
	void stopTracking(void);

private:
	CVelTracker tracker;
};


class CTracking
{
public:
	CTracking(void);
	virtual ~CTracking(void);

	virtual bool addCamera(CamInfo* pCam, int id) { return false; }
	virtual void startTracking(IplImage* frame) {}
	virtual void stopTracking(void) {}
	virtual void deinit(void) {}

	int getObjGUID() { return ++m_ObjGUID; }
	unsigned int getGlobalTimeStamp() { return m_GlobalTimeStamp; }

	void addLeftObj(CTrackObj trkobj) { m_LeftObjList.push_back(trkobj); }
	void setLeftObj(CTrackObj trkobj, int idx) { m_LeftObjList[idx] = trkobj; }
	inline int getLeftObjNum() { return int(m_LeftObjList.size()); }
	inline CTrackObj getLeftObj(int idx) { return m_LeftObjList[idx]; }

protected:
	vector<CamInfo*> m_CamInfos;

	vector<CTrackObj> m_LeftObjList;

	unsigned int m_NumCamera;         // number of cameras.
	unsigned long m_GlobalTimeStamp;  // time stamp of the whole system.
	unsigned int m_ValidFrameCount;   // valid count of frames.
	int m_ObjGUID;                    // GUID for objects.

	bool m_bStopTracking;             // flag for stop tracking.
};


class CHumanTracking : public CTracking
{
public:
	CHumanTracking(void);
	~CHumanTracking(void);

	bool addCamera(CamInfo* pCam, int id);
	void startTracking(IplImage* frame);
	void stopTracking(void);
	void deinit(void);
};


class CVehicleTracking : public CTracking
{
public:
	CVehicleTracking(void);
	~CVehicleTracking(void);

	bool addCamera(CamInfo* pCam, int id);
	void startTracking(IplImage* frame);
	void stopTracking(void);
	void deinit(void);
};
