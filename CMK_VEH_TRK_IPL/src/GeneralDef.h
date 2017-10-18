#pragma once


#ifndef __GEOMETRY_DEF_H__
#define __GEOMETRY_DEF_H__


#include "cvut.h"
#include <linux/limits.h>

const int LENGTH_UNIT = 1000;     // 1 cm = 10 mm, 1m = 1000mm


//===== Camera parameter class ============================================//
class CCameraParam {
public:
	CCameraParam() {}
	~CCameraParam() {}

	bool ReadCameraParamFile(const char* file_path) {
		FILE* fCam = fopen(file_path, "r");
		if (!fCam) return false;

		//---- Get projective matrix -------------------------------------------//
		fscanf(fCam, "%f %f %f %f %f %f %f %f %f\n",
			     &K[0], &K[1], &K[2], &K[3], &K[4], &K[5], &K[6], &K[7], &K[8]);
		fscanf(fCam, "%f %f %f %f %f %f %f %f %f\n",
			     &R[0], &R[1], &R[2], &R[3], &R[4], &R[5], &R[6], &R[7], &R[8]);
		fscanf(fCam, "%f %f %f\n",
			     &C[0], &C[1], &C[2]);

		CvScalar value;

		CvMat* mK = cvCreateMat(3, 3, CV_32FC1);
		cvSetData(mK, K, mK->step);
		CvMat* mR = cvCreateMat(3, 3, CV_32FC1);
		cvSetData(mR, R, mR->step);

		// T = -RC
		CvMat* mT = cvCreateMat(3, 1, CV_32FC1);
		value.val[0] = -C[0]; cvSet1D(mT, 0, value);
		value.val[0] = -C[1]; cvSet1D(mT, 1, value);
		value.val[0] = -C[2]; cvSet1D(mT, 2, value);
		cvMatMul(mR, mT, mT);

		// P = K[R | T]
		CvMat* mP = cvCreateMat(3, 4, CV_32FC1);
		value.val[0] = R[0]; cvSet2D(mP, 0, 0, value);
		value.val[0] = R[1]; cvSet2D(mP, 0, 1, value);
		value.val[0] = R[2]; cvSet2D(mP, 0, 2, value);
		value.val[0] = R[3]; cvSet2D(mP, 1, 0, value);
		value.val[0] = R[4]; cvSet2D(mP, 1, 1, value);
		value.val[0] = R[5]; cvSet2D(mP, 1, 2, value);
		value.val[0] = R[6]; cvSet2D(mP, 2, 0, value);
		value.val[0] = R[7]; cvSet2D(mP, 2, 1, value);
		value.val[0] = R[8]; cvSet2D(mP, 2, 2, value);

		cvSet2D(mP, 0, 3, cvGet1D(mT, 0));
		cvSet2D(mP, 1, 3, cvGet1D(mT, 1));
		cvSet2D(mP, 2, 3, cvGet1D(mT, 2));

		cvMatMul(mK, mP, mP);

		P[0]  = (float)cvGet2D(mP, 0, 0).val[0];
		P[1]  = (float)cvGet2D(mP, 0, 1).val[0];
		P[2]  = (float)cvGet2D(mP, 0, 2).val[0];
		P[3]  = (float)cvGet2D(mP, 0, 3).val[0];
		P[4]  = (float)cvGet2D(mP, 1, 0).val[0];
		P[5]  = (float)cvGet2D(mP, 1, 1).val[0];
		P[6]  = (float)cvGet2D(mP, 1, 2).val[0];
		P[7]  = (float)cvGet2D(mP, 1, 3).val[0];
		P[8]  = (float)cvGet2D(mP, 2, 0).val[0];
		P[9]  = (float)cvGet2D(mP, 2, 1).val[0];
		P[10] = (float)cvGet2D(mP, 2, 2).val[0];
		P[11] = (float)cvGet2D(mP, 2, 3).val[0];

		cvReleaseMat(&mK);
		cvReleaseMat(&mR);
		cvReleaseMat(&mT);
		cvReleaseMat(&mP);
		//----------------------------------------------------------------------//

		fclose(fCam);

		return true;
	}
	bool ReadIntrinParamFile(const char* file_path) {
		FILE* fCam;
		fopen(file_path,"r");
		if (!fCam) return false;

		fscanf(fCam, "%f %f %f %f %f %f %f %f %f\n",
			     &K[0], &K[1], &K[2], &K[3], &K[4], &K[5], &K[6], &K[7], &K[8]);

		return true;
	}

	void SetK(float fx, float fy, float cx, float cy) {
		K[0] = fx;
		K[1] = 0;
		K[2] = cx;
		K[3] = 0;
		K[4] = fy;
		K[5] = cy;
		K[6] = 0;
		K[7] = 0;
		K[8] = 1;
	}
	void SetR(float roll, float pitch, float yaw) {
		CvMat* mRpre;
		CvMat* mCameraR;
		CvMat* mRpost;

		mRpre = cvCreateMat(3, 3, CV_32FC1);
		float Rpre[9] = {0};
		Rpre[0] = 0; Rpre[1] = -1; Rpre[2] = 0;
		Rpre[3] = 0; Rpre[4] = 0;  Rpre[5] = -1;
		Rpre[6] = 1; Rpre[7] = 0;  Rpre[8] = 0;
		cvSetData(mRpre, Rpre, mRpre->step);

		mRpost = cvCreateMat(3, 3, CV_32FC1);
		float Rpost[9] = {0};
		Rpost[0] =  cos(yaw)*cos(pitch);
		Rpost[1] = -sin(yaw)*cos(pitch);
		Rpost[2] =  sin(pitch);
		Rpost[3] =  cos(yaw)*sin(pitch)*sin(roll) + sin(yaw)*cos(roll);
		Rpost[4] =  sin(yaw)*sin(pitch)*sin(roll) + cos(yaw)*cos(roll);
		Rpost[5] = -cos(pitch)*sin(roll);
		Rpost[6] = -cos(yaw)*sin(pitch)*cos(roll) + sin(yaw)*sin(roll);
		Rpost[7] =  sin(yaw)*sin(pitch)*cos(roll) + cos(yaw)*sin(roll);
		Rpost[8] =  cos(pitch)*cos(roll);
		cvSetData(mRpost, Rpost, mRpost->step);

		mCameraR = cvCreateMat(3, 3, CV_32FC1);
		cvSetData(mCameraR, R, mCameraR->step);

		cvMatMul(mRpre, mRpost, mCameraR);

		cvReleaseMat(&mRpre);
		cvReleaseMat(&mRpost);
		cvReleaseMat(&mCameraR);
	}
	void SetC(float x, float y, float z) {
		C[0] = x;
		C[1] = y;
		C[2] = z;
	}
	void calculateP() {
		CvScalar value;

		CvMat* mK = cvCreateMat(3, 3, CV_32FC1);
		cvSetData(mK, K, mK->step);
		CvMat* mR = cvCreateMat(3, 3, CV_32FC1);
		cvSetData(mR, R, mR->step);

		// T = -RC
		CvMat* mT = cvCreateMat(3, 1, CV_32FC1);
		value.val[0] = -C[0]; cvSet1D(mT, 0, value);
		value.val[0] = -C[1]; cvSet1D(mT, 1, value);
		value.val[0] = -C[2]; cvSet1D(mT, 2, value);
		cvMatMul(mR, mT, mT);

		// P = K[R | T]
		CvMat* mP = cvCreateMat(3, 4, CV_32FC1);
		value.val[0] = R[0]; cvSet2D(mP, 0, 0, value);
		value.val[0] = R[1]; cvSet2D(mP, 0, 1, value);
		value.val[0] = R[2]; cvSet2D(mP, 0, 2, value);
		value.val[0] = R[3]; cvSet2D(mP, 1, 0, value);
		value.val[0] = R[4]; cvSet2D(mP, 1, 1, value);
		value.val[0] = R[5]; cvSet2D(mP, 1, 2, value);
		value.val[0] = R[6]; cvSet2D(mP, 2, 0, value);
		value.val[0] = R[7]; cvSet2D(mP, 2, 1, value);
		value.val[0] = R[8]; cvSet2D(mP, 2, 2, value);

		cvSet2D(mP, 0, 3, cvGet1D(mT, 0));
		cvSet2D(mP, 1, 3, cvGet1D(mT, 1));
		cvSet2D(mP, 2, 3, cvGet1D(mT, 2));

		cvMatMul(mK, mP, mP);

		P[0]  = (float)cvGet2D(mP, 0, 0).val[0];
		P[1]  = (float)cvGet2D(mP, 0, 1).val[0];
		P[2]  = (float)cvGet2D(mP, 0, 2).val[0];
		P[3]  = (float)cvGet2D(mP, 0, 3).val[0];
		P[4]  = (float)cvGet2D(mP, 1, 0).val[0];
		P[5]  = (float)cvGet2D(mP, 1, 1).val[0];
		P[6]  = (float)cvGet2D(mP, 1, 2).val[0];
		P[7]  = (float)cvGet2D(mP, 1, 3).val[0];
		P[8]  = (float)cvGet2D(mP, 2, 0).val[0];
		P[9]  = (float)cvGet2D(mP, 2, 1).val[0];
		P[10] = (float)cvGet2D(mP, 2, 2).val[0];
		P[11] = (float)cvGet2D(mP, 2, 3).val[0];

		cvReleaseMat(&mK);
		cvReleaseMat(&mR);
		cvReleaseMat(&mT);
		cvReleaseMat(&mP);
	}

	float* getK() { return K; }
	float* getR() { return R; }
	float* getC() { return C; }
	float* getP() { return P;}

	CvPoint2D32f reproject3DTo2D(CvPoint3D32f p3d)
	{
		CvMat* mP = cvCreateMat(3, 4, CV_32FC1);
		cvSetData(mP, P, mP->step);

		CvMat* mX = cvCreateMat(4, 1, CV_32FC1);
		CvScalar value;
		value.val[0] = p3d.x; cvSet2D(mX, 0, 0, value);
		value.val[0] = p3d.y; cvSet2D(mX, 1, 0, value);
		value.val[0] = p3d.z; cvSet2D(mX, 2, 0, value);
		value.val[0] = 1;     cvSet2D(mX, 3, 0, value);

		CvMat* mx = cvCreateMat(3, 1, CV_32FC1);

		cvMatMul(mP, mX, mx);

		CvPoint2D32f p2d = cvPoint2D32f(cvGet2D(mx, 0, 0).val[0]/cvGet2D(mx, 2, 0).val[0],
			                            cvGet2D(mx, 1, 0).val[0]/cvGet2D(mx, 2, 0).val[0]);

		cvReleaseMat(&mP);
		cvReleaseMat(&mX);
		cvReleaseMat(&mx);

		return p2d;
	}

private:
	float K[9];
	float R[9];
	float C[3];

	float P[12];
};


//===== Camera category class =============================================//
class CCamera {
public:
	CCamera(void) {}
	~CCamera(void) {}

	virtual bool init(void)
	{
		m_Width = 0;
		m_Height = 0;
		mFrameCount = -1;

		mCameraID = -1;

		mpCapture = NULL;

		return true;
	}
	virtual void deinit(void) {};

	inline unsigned int GetWidth() { return m_Width; }
	inline unsigned int GetHeight() { return m_Height; }
	inline unsigned int GetFrameCount() { return mFrameCount; }
	inline int GetCameraID() { return mCameraID; }
	inline IplImage* getCurrentFrame() { return cvut::cvGetCurrentFrame(mpCapture); }

	inline void SetWidth(int width) { m_Width = width; }
	inline void SetHeight(int height) { m_Height = height; }
	inline void SetFrameCount(int frame_count) { mFrameCount = frame_count; }
	inline void SetCameraID(int id) { mCameraID = id; }

protected:
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int mFrameCount;

	int mCameraID;

	CvCapture* mpCapture;
};


class CVideoClip : public CCamera
{
public:
	CVideoClip(void) {}
	~CVideoClip(void) {}

	bool init(void)
	{
		mpCapture = cvCaptureFromAVI(mFilePath);
		if (!mpCapture) return false;

		m_Width = (unsigned int)cvGetCaptureProperty(mpCapture, CV_CAP_PROP_FRAME_WIDTH);
		m_Height = (unsigned int)cvGetCaptureProperty(mpCapture, CV_CAP_PROP_FRAME_HEIGHT);
		mFrameCount = (unsigned int)cvGetCaptureProperty(mpCapture, CV_CAP_PROP_FRAME_COUNT);

		return true;
	}
	void deinit(void)
	{
		if (mpCapture) cvReleaseCapture(&mpCapture);
	}

	inline void SetVideoFilePath(const char* file_path) { strcpy(mFilePath, file_path); }

private:
//changed MAX_PATH to 260
	char mFilePath[260];
};


class CWebCamera : public CCamera
{
public:
	CWebCamera(void) {}
	~CWebCamera(void) {}

	bool init(void) {}
	void deinit(void) {}

private:

};


#endif // __GEOMETRY_DEF_H__
