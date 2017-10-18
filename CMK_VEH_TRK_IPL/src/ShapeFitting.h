#pragma once


#include "GeneralDef.h"
#include "EDAVariable.h"


class CShapeFitting {
public:
	CShapeFitting(void);
	virtual ~CShapeFitting(void);

	virtual bool init(void);
	virtual void deinit(void);
	virtual CEDAVariable* ShapeFitting(IplImage* pImg, void* param1 = NULL, void* param2 = NULL);

	inline void SetCamParam(CCameraParam cam_aram) { m_CamParam = cam_aram; }
	inline CCameraParam GetCamParam() { return m_CamParam; }

protected:
	CCameraParam m_CamParam;
	CEDAVariable* m_Variables;
};
