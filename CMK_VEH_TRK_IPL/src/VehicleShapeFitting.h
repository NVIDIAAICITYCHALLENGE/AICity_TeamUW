#pragma once


#include "ShapeFitting.h"
#include "DeformableModel.h"


using namespace cvut;


class CShapeModel 
{
public:
	CShapeModel() {}
	~CShapeModel() {}

	inline void addPoint(CvPoint2D32f pt) { m_Points.push_back(pt); }
	inline void addLine(CvLine2D32f l) { m_Lines.push_back(l); }

	inline CvPoint2D32f getPoint(int index) { return m_Points[index]; }
	inline CvLine2D32f getLine(int index) { return m_Lines[index]; }

	inline int getPointNum() { return int(m_Points.size()); }
	inline int getLineNum() { return int(m_Lines.size()); }

protected:
	vector<CvPoint2D32f> m_Points;
	vector<CvLine2D32f> m_Lines; 
};


class CVehicleShapeFitting : public CShapeFitting
{
public:
	// view type
	enum { 
		NONE, 
		FRONT,
		REAR,
		RIGHT,
		LEFT,
		FRONT_RIGHT, 
		FRONT_LEFT, 
		REAR_RIGHT, 
		REAR_LEFT
	};

	struct ProjModel2D {
		ProjModel2D () {
			p3d = cvPoint3D32f(0, 0, 0);
			p2d = cvPoint2D32f(0, 0);
			bVisible = false;
		}
		CvPoint3D32f p3d;
		CvPoint2D32f p2d;

		bool bVisible;
	};

	CVehicleShapeFitting(void);
	~CVehicleShapeFitting(void);

	bool init(void) { return true; }
	void deinit(void) {}
	
	//CEDAVariable* ShapeFitting(IplImage* pImg, void* param1 = NULL, void* param2 = NULL);

	//加入一个参数
	CEDAVariable* ShapeFitting(IplImage* pImg, void* param1 = NULL, void* param2 = NULL, int param3 = NULL, int param4 = NULL);

	CDeformableModel PoseEstimation(IplImage* pImg, CDeformableModel* pModel3d_est);

	int ProjectModeltoImage(IplImage* pImg, 
		                    CCameraParam cam_aram,
		                    CDeformableModel* pModel3d, 
		                    vector<ProjModel2D>& vProjModel2d);

	int buildShape(IplImage* pImg, CCameraParam cam_aram, CDeformableModel* pModel3d,
		           CShapeModel& ShapeModel);
	
	//int drawShape(IplImage* pImg, CCameraParam cam_aram, CDeformableModel* pModel3d, 
	//	          CvScalar color = CV_RGB(255, 0, 0), int thickness = 1);

	int drawShape(IplImage* pImg, CCameraParam cam_aram, CDeformableModel* pModel3d, 
		          CvScalar color = CV_RGB(0, 255, 0), int thickness = 1, int param1 = NULL);

	CvScalar GetVehicleBodyColor(IplImage* pImg, CDeformableModel* pModel3d);
	inline int GetViewType() { return m_ViewType; }

	void Terminate() { m_bTerminate = true; }

	
	// configurations
	inline void setSegmentL(int l) { mSegmentL = l; }
	inline void setSegmentW(int w) { mSegmentW = w; }
	inline void setGradientThreshSF(float gradient) { mGradientThreshSF = gradient; }
	inline void setGradientThreshPE(float gradient) { mGradientThreshPE = gradient; }
	inline void setContrastSF(int contrast) { mContrastSF = contrast;}
	inline void setContrastPE(int contrast) { mContrastPE = contrast;}
	inline void setIterationSF(int r, int n) { mRsf = r; mNsf = n; }
	inline void setCriterionThreshSF(float thresh) { mSF_CriterionThresh = thresh; }
	inline void setIterationPE(int r, int n) { mRpe = r; mNpe = n; }
	inline void setCriterionThreshPE(float thresh) { mPE_CriterionThresh = thresh; }
	inline void setScaleRatio(float l, float w, float h) { m_ScaleLength = l; m_ScaleWidth = w; m_ScaleHeight = h; }

	inline void setDisplayThickness(int thickness) { m_DisplayThickness = thickness; }
	inline void setDisplayProjection(bool flag) { mbDisplayProjection = flag; }
	inline void setDisplayIteration(bool flag) { mbDisplayIteration = flag; }
	
	


private:
	struct Segment{
		CvPoint2D32f pt;
		CvBox2D block;
	};

	struct ReprojLine {
		ReprojLine() {
			start = cvPoint2D32f(0, 0);
			end = cvPoint2D32f(0, 0);
			start_idx = -1;
			end_idx = -1;
		}
		CvPoint2D32f start;
		CvPoint2D32f end;

		int start_idx;
		int end_idx;

		vector<Segment> vSegments;
	};

	struct Model3dAngle {
		float angle0_1_3;
		float angle0_1_4;
		float angle0_3_4;
		float angle1_0_2;
		float angle1_0_5;
		float angle1_2_5;
		float angle2_1_3;
		float angle2_1_6;
		float angle2_3_6;
		float angle3_0_2;
		float angle3_0_7;
		float angle3_2_7;
	};
	
	void DrawProjectedModel(IplImage* pImg, CvSize szShift,
		                    vector<ProjModel2D>* pProjModel2d, vector<ReprojLine>* pReproLine,
		                    CvScalar color, int thickness = 1, int wait_time = 0, 
		                    bool bShowSegment = false);

	bool IsViewFront(Model3dAngle model3d_angle);
	bool IsViewRear(Model3dAngle model3d_angle);
	bool IsViewRight(Model3dAngle model3d_angle);
	bool IsViewLeft(Model3dAngle model3d_angle);
	bool IsViewFrontRight(Model3dAngle model3d_angle);
	bool IsViewFrontLeft(Model3dAngle model3d_angle);
	bool IsViewRearRight(Model3dAngle model3d_angle);
	bool IsViewRearLeft(Model3dAngle model3d_angle);
	
	bool IsPointHidden(CvPoint2D32f pt, vector<CDeformableModel::Plane> vPlane, vector<ProjModel2D> vProjModel2d);

	bool InterpolateLine(vector<ProjModel2D>* pProjModel2d, 
		                 vector<ReprojLine>& vReprojLine,
		                 CvPoint lineIdx1, CvPoint lineIdx2, int start_idx);

	ReprojLine CreateSegments(CvPoint2D32f start, CvPoint2D32f end);

	/*vector<string> CVehicleShapeFitting::LineSegmentation(int eView,
	                                        CDeformableModel* pModel3d, 
										    vector<ProjModel2D>* pProjModel2d, 
										    vector<ReprojLine>& vReprojLine);*/

	void LineSegmentation(int eView,
		                  CDeformableModel* pModel3d, 
		                  vector<ProjModel2D>* vProjModel2d, 
		                  vector<ReprojLine>& vReprojLine);
						  
	
	float CalculateFESl(IplImage* pImg, float grad_thresh);
	
	float CalculateFES(IplImage* pImg, CvSize szShift, vector<ReprojLine>* pReprojLine, float grad_thresh);
	
	CDefModRange EstimateMND_SF(CDeformableModel* Dset, int N);
	CPoseEstRange EstimateMND_PE(CDeformableModel* Dset, int N);

	vector<CvPoint2D32f> CreatePolygon(CDeformableModel::Plane plane, 
		                               vector<CVehicleShapeFitting::ProjModel2D>* pProjModel2d);

	Model3dAngle GetModel3dAngle(vector<ProjModel2D>* pProjModel2d);

	float GetAngle(CvPoint2D32f o, CvPoint2D32f a, CvPoint2D32f b);

	int m_ViewType;
	float mRatio_ext;
	float* m_MNDmean;
	bool m_bTerminate;

	// engine configurations
	int mSegmentL;
	int mSegmentW;
	float mGradientThreshSF;
	float mGradientThreshPE;
	int mContrastSF;
	int mContrastPE;
	int mRsf;
	int mNsf;
	float mSF_CriterionThresh;
	int mRpe;
	int mNpe;
	float mPE_CriterionThresh;
	float m_ScaleLength;
	float m_ScaleWidth;
	float m_ScaleHeight;

	// display options
	int m_DisplayThickness;
	bool mbDisplayProjection;
	bool mbDisplayIteration;

 
};





