#pragma once


#include "cvut.h"


class CKalmanFilter
{
public:
	CKalmanFilter();
	~CKalmanFilter();

	bool init(int state_num, int measure_num);
	void deinit();

	inline void setH(CvMat* m) { cvCopy(m, H); cvTranspose(H, H_t); }
	inline void setAk(CvMat* m) { cvCopy(m, Ak); cvTranspose(Ak, Ak_t); }
	inline void setScaledQ(double scale) { cvSetIdentity(Q, cvRealScalar(scale)); }
	inline void setScaledR(double scale) { cvSetIdentity(R, cvRealScalar(scale)); }
	inline void setZk(CvMat* m) { cvCopy(m, Zk); }
	inline void setXk1_k1(CvMat* m) { cvCopy(m, Xk1_k1); }
	inline void setPk1_k1(CvMat* m) { cvCopy(m, Pk1_k1); }

	inline CvMat* getXk_k1() { return Xk_k1; } // prediction result
	inline CvMat* getXk_k() { return Xk_k; }
	inline CvMat* getPk_k() { return Pk_k; }

	void predict();
	void update();

private:
	int mStateNum;           // number of state
	int mMeasurementNum;     // number of measurement

	CvMat* H;                //time invariance; measurement matrix
	CvMat* H_t;	             // transpose of H
	CvMat* Q;	             // cov of the Gaussian for process noise
	CvMat* R;	             // cov of the Gaussian for measurement noise
	CvMat* Ak;	             // time invariance; state transition matrix
	CvMat* Ak_t;             // transpose of Ak
	CvMat* Sk;               // Sk = H*P*H' + R
	CvMat* Sk_inv;           // inverse of Sk
	CvMat* Kgk;              // Kalman gain
	CvMat* Kgk_t;            // transpose of Kgk
	CvMat* Yk;
	CvMat* Zk;
	CvMat* Pk1_k1;	         // initial error cov.
	CvMat* Pk_k1;
	CvMat* Pk_k;
	CvMat* Xk1_k1;           // estimation
	CvMat* Xk_k1;            // estimation
	CvMat* Xk_k;  	         // estimation
};