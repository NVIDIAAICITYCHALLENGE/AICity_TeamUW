#pragma once


#include "GeneralDef.h"
#include "FgObjInfo.h"
#include "DeformableModel.h"


// kernel id for 3D vehicle
enum { K_I = 1, K_II, K_III, K_IV, K_V, K_VI, K_VII, K_VIII, K_IX, K_X, K_XI };


class CTrackObj;


//===== CKernel class =====================================================//
class CKernel
{
public:
	CKernel(int id) 
	{
		m_ID = id;
		m_Similar = 0;
		m_Weight = 1;
		m_HistModel = NULL;
	}
	~CKernel(void) 
	{
		if (m_HistModel) 
			cvReleaseMat(&m_HistModel);
	}

	inline void setSimi(float simi) { m_Similar = simi; }
	inline void setWeight(float weight) { m_Weight = weight; }
	inline void setHistModel(CvMat* hist)
	{ 
		if (m_HistModel) cvReleaseMat(&m_HistModel);
		m_HistModel = cvCloneMat(hist); 
	}

	inline int getID() { return m_ID; }
	inline float getSimi() { return m_Similar; }
	inline float getWeight() { return m_Weight; }
	inline CvMat* getHistModel() { return m_HistModel; }

protected:
	int m_ID;

	float m_Similar;
	float m_Weight;
	
	CvMat* m_HistModel;
};


//===== C2DCKernel class ==================================================//
class C2DCKernel : public CKernel
{
public:
	C2DCKernel(int id) : CKernel(id)
	{
		m_Center = cvPoint2D32f(0, 0);
		m_BMask = NULL;
	}
	~C2DCKernel(void) 
	{
		if (m_BMask) 
			cvReleaseImage(&m_BMask);
	}

	inline void setCenter2d(CvPoint2D32f center) { m_Center = center; }
	inline void setBMask(IplImage* bmask) 
	{
		if (m_BMask) cvReleaseImage(&m_BMask);
		m_BMask = cvCloneImage(bmask);
	}
	void setPlane2d(vector<CvPoint2D32f> plane2d);

	inline CvRect getRect() { return m_Rect; }
	inline CvPoint2D32f getCenter2d() { return m_Center; }
	inline IplImage* getBMask() { return m_BMask; }
	inline vector<CvPoint2D32f> getPlane2d() { return m_Plane2d; }

	void shift(CvPoint2D32f shift, float scale = 1);

private:
	CvRect m_Rect;
	
	CvPoint2D32f m_Center;

	vector<CvPoint2D32f> m_Plane2d;

	IplImage* m_BMask;
};


//===== C3DCKernel class ==================================================//
class C3DCKernel : public CKernel
{
public:
	C3DCKernel(int id) : CKernel(id) {}
	~C3DCKernel() {}

	inline void setPlaneSize(CvSize2D32f size) { m_PlaneSize = size; }
	void setCamParam(CCameraParam cam_param) { m_CamParam = cam_param; }
	void setPlane2d(vector<CvPoint2D32f> plane2d);
	void setPlane3d(vector<CvPoint3D32f> plane3d);

	inline CvRect getRect() { return m_Rect; }
	inline CvSize2D32f getPlaneSize() { return m_PlaneSize; }
	inline vector<CvPoint2D32f> getPlane2d() { return m_Plane2d; }
	inline vector<CvPoint3D32f> getPlane3d() { return m_Plane3d; }
	inline CvPoint3D32f getOrthog() { return m_Orthog; }
	inline CvPoint2D32f getCenter2d() { return m_Center2d; }
	inline CvPoint3D32f getCenter3d() { return m_Center3d; }
	inline CvScalar getPlaneCoeffs() { return m_PlaneCoeffs; }

	void shift(CvPoint3D32f shift);
	CvPoint3D32f backProjection(CvPoint2D32f p, CvScalar plane);

private:
	CvRect m_Rect;
	CvSize2D32f m_PlaneSize;

	CCameraParam m_CamParam;

	CvPoint2D32f m_Center2d;
	CvPoint3D32f m_Center3d;
	CvPoint3D32f m_Normal;
	CvPoint3D32f m_Orthog;

	CvScalar m_PlaneCoeffs;

	vector<CvPoint2D32f> m_Plane2d;
	vector<CvPoint3D32f> m_Plane3d;
};


//===== CConstraint class =================================================//
class CConstraint
{
public:
	typedef float  (*CvecFunc)(void* constraint, void* object);
	typedef CvMat* (*CdevFunc)(void* constraint, void* object, void* kernel);

	CConstraint();
	~CConstraint();

	bool init(unsigned int kernel_num);
	void deinit();

	inline void setCvecFuncPtr(CvecFunc func) { m_CvecFunc = func; }
	inline void setCdevFuncPtr(CdevFunc func) { m_CdevFunc = func; }
	inline void setConstValue(float value) { m_ConstValue = value; }

	inline CvecFunc getCvecFuncPtr() { return m_CvecFunc; }
	inline CdevFunc getCdevFuncPtr() { return m_CdevFunc; }
	inline float getConstValue() { return m_ConstValue; }

	inline void addRelatedKernel(int id) { m_RelatedKernelIDs.push_back(id); }
	inline int getRelatedKernelNum() { return int(m_RelatedKernelIDs.size()); }
	inline int getRelatedKernelIdx(int idx) { return m_RelatedKernelIDs[idx]; }
	bool isKernelValid(int id);
	void eraseRelatedKernel(int id);

private:
	CvecFunc m_CvecFunc;
	CdevFunc m_CdevFunc;

	vector<int> m_RelatedKernelIDs;

	float m_ConstValue; 
};


//===== CKernelTracker class ==============================================//
class CKernelTracker
{
public:
	CKernelTracker(void);
	~CKernelTracker(void);

	// weighting type
	enum { UNIFORM, GAUSSIAN, ROOF, EPANECHNIKOV };

	// similar criterion
	enum { CORRELATION, KL_DISTANCE, DUAL_KL_DISTANCE };

	// histogram type
	enum { HIST_SEP, HIST_3D };

	// color space 
	enum { RGB, HSV, GRAY };

	virtual bool init() { return true;}
	virtual void deinit();

	virtual bool initConstraints(CTrackObj* pObj) = 0;
	virtual void tracking(IplImage* frame) = 0;

	inline void addKernel(CKernel* kernel) { m_Kernels.push_back(kernel); }
	inline CKernel* getKernel(int idx) { return m_Kernels[idx]; }
	inline int getKernelNum() { return int(m_Kernels.size()); }
	inline CConstraint* getConstraint(int idx) { return &m_Constraints[idx]; }
	inline int getConstraintNum() { return int(m_Constraints.size()); }

	inline void setWidth(unsigned int width) { m_Width = width; }
	inline void setHeight(unsigned int height) { m_Height = height; }
	
	inline unsigned int getWidth() { return m_Width; }
	inline unsigned int getHeight() { return m_Height; }
	inline float getScale() { return m_Scale; }

	IplImage* cvtColorSpace(IplImage* src);

protected:
	vector<CKernel*> m_Kernels;
	vector<CConstraint> m_Constraints;

	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_Iteration;

	float m_Scale;

	CKernel* findKernel(int id);

	float calKernelWeight(CvPoint2D32f center, CvPoint2D32f pos, CvSize2D32f size, int type);
	float calKerneldevWeight(CvPoint2D32f center, CvPoint2D32f pos, CvSize2D32f size, int type);
	float calMeanShiftWeight(CvMat* target, CvMat* candidate, int index, int criterion);
	float calSimi(CvMat* target, CvMat* candidate, int criterion);
};


//===== C2DCKernelTracker class ===========================================//
class C2DCKernelTracker : public CKernelTracker
{
public:
	C2DCKernelTracker(void);
	~C2DCKernelTracker(void);

	bool init();
	void deinit();

	bool initConstraints(CTrackObj* pObj);

	CvMat* buildHistModel(IplImage* src, IplImage* bmask, CvRect rect);

	void tracking(IplImage* frame);

	void setScaleBase(float scale_base) { m_ScaleBase = scale_base; }

private:
	float m_CostValue;
	float m_ConstrainedValue;
	float m_ScaleBase;

	CvMat* calDeltaA(IplImage* frame);
	CvMat* calDeltaB(IplImage* frame);
	void adaptiveWeighting(CvMat* &Jx);
	CvBox2D combineKernels();
	void eraseRelatedKernels(int id);

	static float calCvecFunc1(void* constraint, void* object);
	static CvMat* calCdevFunc1(void* constraint, void* object, void* kernel);
	static float calCvecFunc2(void* constraint, void* object);
	static CvMat* calCdevFunc2(void* constraint, void* object, void* kernel);
	static float calCvecFunc3(void* constraint, void* object);
	static CvMat* calCdevFunc3(void* constraint, void* object, void* kernel);
};


//===== C3DCKernelTracker class ===========================================//
class C3DCKernelTracker : public CKernelTracker
{
public:
	C3DCKernelTracker(void);
	~C3DCKernelTracker(void);

	bool init();
	void deinit();

	bool initConstraints(CTrackObj* pObj);

	CvMat* buildHistModel(IplImage* src, IplImage* bmask, C3DCKernel* kernel3dc);

	void tracking(IplImage* frame);

	inline void setPosition3d(CvPoint3D32f p3d) { m_Postition3d = p3d; }
	inline void setCamParam(CCameraParam cam_param) { m_CamParam = cam_param; }

	inline CvPoint3D32f getPosition3d() { return m_Postition3d; }
 
private:
	float m_CostValue;
	float m_ConstrainedValue;

	CvPoint3D32f m_Postition3d;

	CCameraParam m_CamParam;

	struct Segment{
		CvPoint2D32f pt;
		CvBox2D block;
	};
	struct ProjLine {
		CvPoint2D32f start;
		CvPoint2D32f end;

		vector<Segment> vSegments;
	};

	CvMat* calDeltaA(IplImage* frame);
	CvMat* calDeltaB(IplImage* frame);
	CvMat* calDeltaC(IplImage* frame);
	void adaptiveWeighting(CvMat* &Jx);
	CvBox2D combineKernels();
	void eraseRelatedKernels(int id);

	bool isOnLine(CvPoint2D32f p2d, cvut::CvLine2D32f line);
	ProjLine createSegments(CvPoint2D32f start, CvPoint2D32f end, int segL, int segW);
	float calFESl(IplImage* frame, float grad_thresh, int segW);

	static float calCvecFunc1(void* constraint, void* object);
	static CvMat* calCdevFunc1(void* constraint, void* object, void* kernel);
	static float calCvecFunc2(void* constraint, void* object);
	static CvMat* calCdevFunc2(void* constraint, void* object, void* kernel);
	static float calCvecFunc3(void* constraint, void* object);
	static CvMat* calCdevFunc3(void* constraint, void* object, void* kernel);
};