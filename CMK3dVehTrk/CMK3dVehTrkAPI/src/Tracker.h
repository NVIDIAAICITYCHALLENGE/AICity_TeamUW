#pragma once


#include "FgObjInfo.h"
#include "KalmanFilter.h"
#include "KernelTracker.h"
#include "DeformableModel.h"
#include "VehicleShapeFitting.h"


#define KF_STATE_NUM                 6
#define KF_MEASUREMENT_NUM           4


enum TrackingType {
	TT_NORMAL = 0,
	TT_MERGE,
	TT_MERGE_UNKNOWN,
	TT_OCCLUDED
};


class CTrackObj 
{
public:
	CTrackObj(void);
	~CTrackObj(void);

	bool Create(int id, unsigned int enter_time, CFgObjInfo info, int kt_type);
	void Delete();

	void resetTrackingStatus();
	void reversePreviousStatus();

	inline void setEnterTime(unsigned int enter_time) { m_EnterTime = enter_time; }
	inline void setLeaveTime(unsigned int leave_time) { m_LeaveTime = leave_time; }
	inline void setObjInfo(CFgObjInfo obj) { m_ObjInfo = obj; }
	inline void setObjInfo_pre(CFgObjInfo obj_pre) { m_ObjInfo_pre = obj_pre; }
	inline void setPosition3d(CvPoint3D32f pos) { m_Position3d = pos; }
	inline void setVelocity2d(CvPoint2D32f vel) { m_Velocity2d = vel; }
	inline void setVelocity3d(CvPoint3D32f vel) { m_Velocity3d = vel; }
	inline void setScale(float scale) { m_Scale = scale; }
	inline void setPk(CvMat* m) { cvCopy(m, m_Pk); }
	inline void setXk(CvMat* m) { cvCopy(m, m_Xk); }
	inline void setKernelTrackingTime(int time) { m_nKernelTrackingTime = time; }
	inline void set3dModelBuildTime(int time) { m_3dModelBuildTime = time; }
	inline void setViewType(int type) { m_ViewType = type; }
	inline void set3dModelVaild (bool valid) { m_b3dModelValid = valid; }
	inline void set3dModel(CDeformableModel model) { m_3dModel = model; m_Position3d = m_3dModel.getTranslation(); }
	inline void setShapeModel(CShapeModel model) { m_ShapeModel = model; }
	inline void setTrackingType(TrackingType type) { m_TrackingType_pre = mTrackingType; mTrackingType = type; }
	inline void setVehicleColor(CvScalar color) { m_VehicleColor = color; }

	inline int getID() { return m_ID; }
	inline int getInitArea() { return m_InitArea; }
	inline unsigned int getEnterTime() { return m_EnterTime; }
	inline unsigned int getLeaveTime() { return m_LeaveTime; }
	inline CFgObjInfo getObjInfo() { return m_ObjInfo; }
	inline CFgObjInfo getObjInfo_pre() { return m_ObjInfo_pre; }
	inline CvPoint3D32f getPosition3d() { return m_Position3d; }
	inline CvPoint2D32f getVelocity2d() { return m_Velocity2d; }
	inline CvPoint3D32f getVelocity3d() { return m_Velocity3d; }
	inline float getScale() { return m_Scale; }
	inline CvMat* getPk() { return m_Pk; }
	inline CvMat* getXk() { return m_Xk; }
	inline bool isKernelTracking() { return (m_nKernelTrackingTime >= 0); }
	inline int getKernelTrackingTime() { return m_nKernelTrackingTime; }
	inline int get3dModelBuildTime() { return m_3dModelBuildTime; }
	inline int getViewType() { return m_ViewType; }
	inline bool is3dModelValid() { return m_b3dModelValid; }
	inline CDeformableModel get3dModel(void) { return m_3dModel; }
	inline CShapeModel getShapeModel(void) { return m_ShapeModel; }
	inline TrackingType getTrackingType() { return mTrackingType; }
	inline TrackingType getTrackingType_pre() { return m_TrackingType_pre; }
	inline CvScalar getVehicleColor() { return m_VehicleColor; }

	inline void addKernel(CKernel* kernel) { m_KernelTracker->addKernel(kernel); }
	inline CKernelTracker* getKernelTracker() { return m_KernelTracker; }

	inline void addPath2dNode(CFgObjInfo node) { m_Path2dNodes.push_back(node); }
	inline CFgObjInfo getPath2dNode(int i) { return m_Path2dNodes[i]; }
	inline int getPath2dSize() { return int(m_Path2dNodes.size()); }

	inline void addPath3dNode(CvPoint3D32f node) { m_Path3dNodes.push_back(node); }
	inline CvPoint3D32f getPath3dNode(int i) { return m_Path3dNodes[i]; }
	inline int getPath3dSize() { return int(m_Path3dNodes.size()); }

	
private:
	int m_ID;
	int m_InitArea;
	unsigned int m_EnterTime;
	unsigned int m_LeaveTime;

	CFgObjInfo m_ObjInfo;
	CFgObjInfo m_ObjInfo_pre;

	CvPoint3D32f m_Position3d;
	CvPoint2D32f m_Velocity2d;
	CvPoint3D32f m_Velocity3d;

	float m_Scale;

	CvMat* m_Pk;
	CvMat* m_Xk;

	CKernelTracker* m_KernelTracker;

	int m_nKernelTrackingTime;
	int m_3dModelBuildTime;
	int m_ViewType;
	bool m_b3dModelValid;
	CDeformableModel m_3dModel;
	CShapeModel m_ShapeModel;

	TrackingType mTrackingType;
	TrackingType m_TrackingType_pre;

	vector<CFgObjInfo> m_Path2dNodes;
	vector<CvPoint3D32f> m_Path3dNodes;

	CvScalar m_VehicleColor;
};


class CTracker 
{
public:
	CTracker(void) {}
	~CTracker(void) {}

	virtual bool init() { return false; }
	virtual void deinit() {}
};


class CSurTracker : public CTracker
{
public:
	CSurTracker(void);
	~CSurTracker(void);

	bool init();
	void deinit();

	inline void setPreviousBMask(IplImage* pFg) 
	{
		if (pFg)
		{printf("success.\r\n");}
		else
		{printf("fail.\r\n");}
		if (m_BMask_pre) cvReleaseImage(&m_BMask_pre);

		m_BMask_pre = cvCloneImage(pFg); //pFg is after post processing bounding box
	}
	inline void setCurrentBMask(IplImage* pFg) 
	{ 
		if (m_BMask_cur) cvReleaseImage(&m_BMask_cur);
		m_BMask_cur = cvCloneImage(pFg); 
	}
	inline void setPreviousFrame(IplImage* pFrame) 
	{ 
		if (m_Frame_pre) cvReleaseImage(&m_Frame_pre);
		m_Frame_pre = cvCloneImage(pFrame); 
	}
	inline void setCurrentFrame(IplImage* pFrame) 
	{
		if (m_Frame_cur) cvReleaseImage(&m_Frame_cur);
		m_Frame_cur = cvCloneImage(pFrame); 
	}
	inline void setPreviousOrigin(IplImage* pOrigin) 
	{
		if (m_Origin_cur) cvReleaseImage(&m_Origin_cur);
		m_Origin_cur = cvCloneImage(pOrigin); 
	}
	inline void setCurrentOrigin(IplImage* pOrigin) 
	{
		if (m_Origin_pre) cvReleaseImage(&m_Origin_pre);
		m_Origin_pre = cvCloneImage(pOrigin); 
	}
	inline void setNewObjNum(int count) { m_NewObjNum = count; }	

	inline void setMarginThreshFar(float thresh) { m_MarginThreshFar = thresh; }
	inline void setMarginThreshMid(float thresh) { m_MarginThreshMid = thresh; }
	inline void setMarginThreshNear(float thresh) { m_MarginThreshNear = thresh; }
	inline void setEnlargeFactor(float factor) { m_EnlargeFactor = factor; }
	inline void setScaleBase(float scale_base) { m_ScaleBase = scale_base; }
	inline void setValidVel2dThresh(float valid_vel2d) { m_ValidVel2dThresh = valid_vel2d; }
	inline void setValidVel3dThresh(float valid_vel3d) { m_ValidVel3dThresh = valid_vel3d; }
	inline void setEstVelocityMethod(int method) { m_EstVelocityMethod = (unsigned int)method; }
	inline void setEstVelocityFrames(int frames) { m_EstVelocityFrames = (unsigned int)frames; }
	inline void setEnterFrameThresh(int thresh) { m_EnterFrameThresh = (unsigned int)thresh; }
	inline void set3DModelUpdateDuration(unsigned int duration) { m_3DModelUpdateDuration = duration; }
	inline void setKernelUpdateDuration(unsigned int duration) { m_KernelUpdateDuration = duration; }
	inline void setInitPoseSftFar(CvPoint2D32f sft) { m_InitPoseSftFar = sft; }
	inline void setInitPoseSftMid(CvPoint2D32f sft) { m_InitPoseSftMid = sft; }
	inline void setInitPoseSftNear(CvPoint2D32f sft) { m_InitPoseSftNear = sft; }
	inline void setScaleRatioFar(CvPoint3D32f scale) { m_ScaleRatioFar = scale; }
	inline void setScaleRatioMid(CvPoint3D32f scale) { m_ScaleRatioMid = scale; }
	inline void setScaleRatioNear(CvPoint3D32f scale) { m_ScaleRatioNear = scale; }

	inline CKalmanFilter* getKalmanFilter() { return &m_Kalman; }
	inline IplImage* getPreviousBMask() { return m_BMask_pre; }
	inline IplImage* getCurrentBMask() { return m_BMask_cur; }
	inline IplImage* getPreviousFrame() { return m_Frame_pre; }
	inline IplImage* getCurrentFrame() { return m_Frame_cur; }
	inline IplImage* getPreviousOrigin() { return m_Origin_pre; }
	inline IplImage* getCurrentOrigin() { return m_Origin_cur; }
	inline int getNewObjNum() { return m_NewObjNum; }

	inline float getMarginThreshFar() { return m_MarginThreshFar; }
	inline float getMarginThreshMid() { return m_MarginThreshMid; }
	inline float getMarginThreshNear() { return m_MarginThreshNear; }
	inline float getEnlargeFactor() { return m_EnlargeFactor; }
	inline float getScaleBase() { return m_ScaleBase; }
	inline float getValidVel2dThresh() { return m_ValidVel2dThresh; }
	inline float getValidVel3dThresh() { return m_ValidVel3dThresh; }
	inline unsigned int getEstVelocityMethod() { return m_EstVelocityMethod; }
	inline unsigned int getEstVelocityFrames() { return m_EstVelocityFrames; }
	inline unsigned int getEnterFrameThresh() { return m_EnterFrameThresh; }
	inline unsigned int get3DModelUpdateDuration() { return m_3DModelUpdateDuration; }
	inline unsigned int getKernelUpdateDuration() { return m_KernelUpdateDuration; }
	inline CvPoint2D32f getInitPoseSftFar() { return m_InitPoseSftFar; }
	inline CvPoint2D32f getInitPoseSftMid() { return m_InitPoseSftMid; }
	inline CvPoint2D32f getInitPoseSftNear() { return m_InitPoseSftNear; }
	inline CvPoint3D32f getScaleRatioFar() { return m_ScaleRatioFar; }
	inline CvPoint3D32f getScaleRatioMid() { return m_ScaleRatioMid; }
	inline CvPoint3D32f getScaleRatioNear() { return m_ScaleRatioNear; }
	

	inline void addMergeGroup(vector<int> group) { m_MergeGroups.push_back(group); }
	inline vector<int> getMergeGroup(int igrp) { return m_MergeGroups.at(igrp); }
	inline int getMergeGroupObj(int igrp, int iobj) { return m_MergeGroups.at(igrp).at(iobj); }
	
	void addObjToGroup(int obj_id, int igrp); 
	int eraseMergeGroupObj(int igrp, int iobj); // true means erase group(igrp) at the same time
	CvPoint findMergeGroupObj(int obj_id); // (group idx, object idx)

	void showMergeGroupStatus();

private:
	CKalmanFilter m_Kalman;

	IplImage* m_Origin_pre;
	IplImage* m_Origin_cur;
	IplImage* m_Frame_pre;
	IplImage* m_Frame_cur;
	IplImage* m_BMask_pre;
	IplImage* m_BMask_cur;

	int m_NewObjNum;

	// threshold
	float m_MarginThreshFar;
	float m_MarginThreshMid;
	float m_MarginThreshNear;
	float m_EnlargeFactor;
	float m_ScaleBase;
	float m_ValidVel2dThresh;
	float m_ValidVel3dThresh;
	unsigned int m_EstVelocityMethod;
	unsigned int m_EstVelocityFrames;
	unsigned int m_EnterFrameThresh;
	unsigned int m_3DModelUpdateDuration;
	unsigned int m_KernelUpdateDuration;
	CvPoint2D32f m_InitPoseSftFar;
	CvPoint2D32f m_InitPoseSftMid;
	CvPoint2D32f m_InitPoseSftNear;
	CvPoint3D32f m_ScaleRatioFar;
	CvPoint3D32f m_ScaleRatioMid;
	CvPoint3D32f m_ScaleRatioNear;

	vector<vector<int>> m_MergeGroups;
};


class CVelTracker : public CTracker
{
public:
	CVelTracker(void);
	~CVelTracker(void);

	bool init();
	void deinit();
};