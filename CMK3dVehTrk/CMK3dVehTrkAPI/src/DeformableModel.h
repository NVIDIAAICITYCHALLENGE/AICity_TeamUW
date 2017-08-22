#pragma once


#include "GeneralDef.h"
#include "EDAVariable.h"


class CDefModRange : public CEDAVarRange
{
public:
	CDefModRange(float sl = 1, float sw = 1, float sh = 1);
	~CDefModRange(void);

	float L_min,  L_max;
	float W1_min, W1_max;
	float W2_min, W2_max;
	float H1_min, H1_max;
	float H2_min, H2_max;
	float H3_min, H3_max;
	float H4_min, H4_max;
	float X1_min, X1_max;
	float X2_min, X2_max;
	float X3_min, X3_max;
	float X4_min, X4_max;
	float Ta_min, Ta_max;

	CvPoint3D32f init_t;
	CvPoint3D32f range_t;

	CvPoint3D32f init_r;
	CvPoint3D32f range_r;

	float m_ScaleLength;
	float m_ScaleWidth;
	float m_ScaleHeight;
};


class CPoseEstRange : public CEDAVarRange
{
public:
	CPoseEstRange(void);
	~CPoseEstRange(void);

	CvPoint3D32f m_Mean_t;
	CvPoint3D32f m_Range_t;

	CvPoint3D32f m_Mean_r;
	CvPoint3D32f m_Range_r;
};


class CDeformableModel : public CEDAVariable
{
public:
	struct Line {
		Line() { idx1 = -1; idx2 = -1; }
		int idx1;
		int idx2;
	};

	struct Plane {
		Plane() { idx1 = -1; idx2 = -1; idx3 = -1; idx4 = -1; idx5 = -1; idx6 = -1;}
		int idx1;
		int idx2;
		int idx3;
		int idx4;
		int idx5;
		int idx6;
	};

	CDeformableModel(void);
	~CDeformableModel(void);

	bool initVariables(CEDAVarRange* var_range);
	bool initPoseVariables(CEDAVarRange* var_range);

	
	inline float getL()  { return L; }
	inline float getW1() { return W1; }
	inline float getW2() { return W2; }
	inline float getH1() { return H1; }
	inline float getH2() { return H2; }
	inline float getH3() { return H3; }
	inline float getH4() { return H4; }
	inline float getX1() { return X1; }
	inline float getX2() { return X2; }
	inline float getX3() { return X3; }
	inline float getX4() { return X4; }
	inline float getTa() { return Ta; }
	inline CvPoint3D32f getTranslation() { return t_vector; }
	inline CvPoint3D32f getRotation() { return r_radian; }
	inline CDefModRange getDeformableRange() { return m_DefModRange; }

	inline void setTranslation(CvPoint3D32f t_vec) { t_vector = t_vec; }
	inline void setRotation(CvPoint3D32f r_rad) { r_radian = r_rad; }

	inline CvPoint3D32f getPoint(int idx) { return m_Model[idx]; }
	inline void setPoint(int idx, CvPoint3D32f pt) { m_Model.at(idx) = pt; }
	inline Line getLine(int idx) { return m_Lines[idx]; }
	inline int findLine(int idx1, int idx2) 
	{
		for (int l = 0; l < (int)m_Lines.size(); l++)
			if ((m_Lines.at(l).idx1 == idx1 && m_Lines.at(l).idx2 == idx2) ||
				(m_Lines.at(l).idx1 == idx2 && m_Lines.at(l).idx2 == idx1))
				return l;
		return -1;
	}
	inline float getFES() { return FES; }
	inline void setFES(float fes) { FES = fes; }

	inline void addPoint(CvPoint3D32f pt) { m_Model.push_back(pt); }
	inline void addLine(int idx1, int idx2)
	{
		Line line;
		line.idx1 = idx1;
		line.idx2 = idx2;
		m_Lines.push_back(line);
	}
	inline int getPointNum() { return int(m_Model.size()); }
	inline int getLineNum() { return int(m_Lines.size()); }

	void shift(CvPoint3D32f v3d);
	void scale(float s);

private:
	vector<CvPoint3D32f> m_Model;
	vector<Line> m_Lines;

	float FES;

	float L;   // length of whole vehicle
	float W1;  // width of whole vehicle
	float W2;  // width of vehicle's roof
	float H1;  // height of vehicle's rear
	float H2;  // height of vehicle's front
	float H3;  // height of vehicle's windows (vertex 8, 9, 10, 11)
	float H4;  // height of vehicle's roof (vertex 12, 13, 14, 15)
	float X1;  // x-shift from vehicle's center to vertex 8 or 11 
	float X2;  // x-shift from vehicle's center to vertex 9 or 10
	float X3;  // x-shift from vehicle's center to vertex 12 or 15
	float X4;  // x-shift from vehicle's center to vertex 13 or 14
	float Ta;  // height of tire

	CvPoint3D32f t_vector;
	CvPoint3D32f r_radian;

	CDefModRange m_DefModRange;

	void toWCS();
};