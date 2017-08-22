#include "IniFile.h"
#include "CommonDef.h"
#include "BasicBgSub.h"
#include "VehicleShapeFitting.h"
#include "IndVehicleTracking.h"


CIndVehicleTracking::CIndVehicleTracking(int Ncamera)
{
	m_nNcamera = Ncamera;
	m_nCamIDIdx = -1;

	if (m_nNcamera > 0) {
		m_SurCam = new SurCamInfo[m_nNcamera]();
		m_CamIDmap = new int[m_nNcamera]();
		m_CpType = new int[m_nNcamera]();
		m_SegType = new int[m_nNcamera]();
		m_SfType = new int[m_nNcamera]();
	}
}

CIndVehicleTracking::~CIndVehicleTracking(void)
{
	if (m_nNcamera > 0) {
		for (int i = 0; i < m_nNcamera; i++)
			delete m_SurCam->getCameraSrc();
		delete [] m_SurCam;
		delete [] m_CamIDmap;
	}
}

BOOL CIndVehicleTracking::IndVehicleTracking_Init(int iCameraID, int iVideoWidth, int iVideoHeight, const char* ConfigPath)
{
	BOOL res = S_FALSE;

	m_CamIDmap[++m_nCamIDIdx] = iCameraID;
	m_CpType[m_nCamIDIdx] = NONE_PARAM;
	m_SegType[m_nCamIDIdx] = BASIC_BG_SUB;
	m_SfType[m_nCamIDIdx] = DEFORM_MODEL;


	// load camera configurations from file
	if (!initSurCamInfo(m_SurCam[m_nCamIDIdx], ConfigPath)) {
		printStatus(SL_FATAL, "load camera parameters fail.\r\n"); 
		goto error;
	}


	// initialize camera/video
	if (!m_SurCam[m_nCamIDIdx].init()) {
		printStatus(SL_FATAL, "camera(%d) init() fail.\r\n", getCamIDIndex(m_nCamIDIdx));
		goto error;
	}
	m_SurCam[m_nCamIDIdx].getCameraSrc()->SetWidth(iVideoWidth);
	m_SurCam[m_nCamIDIdx].getCameraSrc()->SetHeight(iVideoHeight);


	// set system parameter 
	printStatus(SL_INFO, "Camera ID = %d\r\n", iCameraID);
	m_SurCam[m_nCamIDIdx].setCameraID(iCameraID); 
	m_SurCam[m_nCamIDIdx].setTrackingSystem(this);


	m_CamInfos.push_back(&m_SurCam[m_nCamIDIdx]);
	res = true;

error:
	return res;
}

void CIndVehicleTracking::IndVehicleTracking_Uninit(int iCameraID)
{
	int CamIDIdx = getCamIDIndex(iCameraID);

	m_CamInfos[CamIDIdx] = NULL;

	m_SurCam[CamIDIdx].deinit();
}

void CIndVehicleTracking::IndVehicleTracking_SetBgImg(int iCameraID, BYTE* pbBgImgRGB24)
{
	IplImage* pFrame = NULL;
	pFrame = getIplImage(pbBgImgRGB24);
	if (!pFrame) return;

	switch (m_SegType[m_nCamIDIdx]) {
	default:
		printStatus(SL_PRINT, "no object extraction engine.\r\n");
		break;
	case BASIC_BG_SUB: 
		{
			CBasicBgSub* pBgSub = (CBasicBgSub*)m_CamInfos[getCamIDIndex(iCameraID)]->getSegEngine();
			pBgSub->setBgImg(pFrame);
		}
		break;
	/*case SUB_SENSE:
		{
		}
		break;
		*/
	}

	cvReleaseImage(&pFrame);
}

void CIndVehicleTracking::IndVehicleTracking_SetRoiImg(int iCameraID, BYTE* pbRoiImgRGB24)
{
	IplImage* pFrame = NULL;
	pFrame = getIplImage(pbRoiImgRGB24);

	//cvShowImage("ROI", pFrame);
	//cvWaitKey(0);

	if (!pFrame) return;

	switch (m_SegType[m_nCamIDIdx]) {
	default:
		printStatus(SL_PRINT, "no object extraction engine.\r\n");
		break;
	case BASIC_BG_SUB: 
		{
			CBasicBgSub* pBgSub = (CBasicBgSub*)m_CamInfos[getCamIDIndex(iCameraID)]->getSegEngine();
			pBgSub->setRoiImg(pFrame);
		}
		break;
	}

	cvReleaseImage(&pFrame);
}

int CIndVehicleTracking::IndVehicleTracking_GetObjectCnt(int iCameraID)
{
	return m_CamInfos[getCamIDIndex(iCameraID)]->getNumTrackObj();
}

CVehicleClass CIndVehicleTracking::IndVehicleTracking_GetObject(int iCameraID, int iObjectIdx)
{
	CVehicleClass xObj;
	CTrackObj tObj = m_CamInfos[getCamIDIndex(iCameraID)]->getTrackObj(iObjectIdx);
	CFgObjInfo obj_info = tObj.getObjInfo();

	xObj.setObjID(tObj.getID());
	xObj.setObjPosition(cvPointFrom32f(obj_info.getBoxCenter()));
	xObj.setObjRect(obj_info.getRect());
	xObj.setObjEllipse(obj_info.getBox2D());
	for (int n = 0; n < tObj.getPath2dSize(); n++)
		xObj.addObjPathNode(cvPointFrom32f(tObj.getPath2dNode(n).getBoxCenter()));

	if (tObj.is3dModelValid()) {
		xObj.setModelValid(true);
		xObj.set3dModel(tObj.get3dModel());
		xObj.setShapeModel(tObj.getShapeModel());
	}
	
	return xObj;
}

BOOL CIndVehicleTracking::IndVehicleTracking_Process(int iCameraID, BYTE* pbCurrentFrameRGB24, __int64 iTimeStamp)
{
	printStatus(SL_PRINT, "timestamp = %d\r\n", iTimeStamp);
	IplImage* pFrame = NULL;
	pFrame = getIplImage(pbCurrentFrameRGB24);
	if (!pFrame) return FALSE;

	// do tracking
	m_GlobalTimeStamp = unsigned long(iTimeStamp);

	m_CamInfos[getCamIDIndex(iCameraID)]->startTracking(pFrame);
	
	cvReleaseImage(&pFrame);

	return TRUE;
}

int CIndVehicleTracking::getCamIDIndex(int id)
{
	for (int n = 0; n < m_nNcamera; n++) {
		if (id == m_CamIDmap[n])
			return n;
	}
	return -1;
}

IplImage* CIndVehicleTracking::getIplImage(BYTE* pbRGBdata) 
{
	IplImage* pImg = NULL;
	int width = m_SurCam[m_nCamIDIdx].getCameraSrc()->GetWidth(),
		height = m_SurCam[m_nCamIDIdx].getCameraSrc()->GetHeight();

	pImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	if (!pImg) { printStatus(SL_FATAL, "insufficient memory.\r\n"); return NULL; }

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			CvScalar pxl;
			pxl.val[2] = pbRGBdata[(j*width+i)*3];
			pxl.val[1] = pbRGBdata[(j*width+i)*3+1];
			pxl.val[0] = pbRGBdata[(j*width+i)*3+2];
			cvSet2D(pImg, j, i, pxl);
		}
	}

	return pImg;
}

bool CIndVehicleTracking::initSurCamInfo(SurCamInfo& sur_cam, const char* inifile_path)
{
	bool res = false;
	IniFile ini(inifile_path);
	

	// load camera information
	ini.setSection("General Options");
	m_SegType[m_nCamIDIdx] = ini.readInt("SegmentationEngine", BASIC_BG_SUB);
	m_SfType[m_nCamIDIdx] = ini.readInt("ShapeFittingEngine", DEFORM_MODEL);

	float resize_ratio = float(ini.readInt("ResizeRatio", 1));
	sur_cam.setKernelTrackingType(ini.readInt("KernelTrackingType", KT_2DC));
	sur_cam.enableVCD(ini.readInt("enableVCD", 0) ? true : false);
	sur_cam.setResizeRatio(resize_ratio);


	// camera source
	CCamera* cam_src = new CCamera();
	sur_cam.setCameraSrc(cam_src);


	// tracking system
	ini.setSection("Tracking System");
	sur_cam.setMarginThreshFar(ini.readInt("MarginThreshFar", 10)/1000.f);
	sur_cam.setMarginThreshMid(ini.readInt("MarginThreshMid", 50)/1000.f);
	sur_cam.setMarginThreshNear(ini.readInt("MarginThreshNear", 100)/1000.f);
	sur_cam.setEnlargeFactor(ini.readInt("EnlargeFactor", 1000)/1000.f);
	sur_cam.setScaleBase(ini.readInt("ScaleBase", int(1e7))/1e7f);
	sur_cam.setValidVel2dThresh(float(ini.readInt("ValidVel2dThresh", 2)));
	sur_cam.setValidVel3dThresh(ini.readInt("ValidVel3dThresh", 30)/100.f*LENGTH_UNIT);
	sur_cam.setEstVelocityMethod(ini.readInt("EstVelocityMethod", 0));
	sur_cam.setEstVelocityFrames(ini.readInt("EstVelocityFrames", 5));
	sur_cam.setEnterFrameThresh(ini.readInt("EnterFrameThresh", 5));
	sur_cam.set3DModelUpdateDuration(ini.readInt("3DModelUpdateDuration", 10));
	sur_cam.setKernelUpdateDuration(ini.readInt("KernelUpdateDuration", 10));
	sur_cam.setInitPoseSftFar(cvPoint2D32f(ini.readInt("InitPoseSftFarX", 0)/1000.f, 
	                                       ini.readInt("InitPoseSftFarY", 0)/1000.f));
	sur_cam.setInitPoseSftMid(cvPoint2D32f(ini.readInt("InitPoseSftMidX", 0)/1000.f, 
	                                       ini.readInt("InitPoseSftMidY", 0)/1000.f));
	sur_cam.setInitPoseSftNear(cvPoint2D32f(ini.readInt("InitPoseSftNearX", 0)/1000.f, 
	                                        ini.readInt("InitPoseSftNearY", 0)/1000.f));


	// camera parameters
	ini.setSection("Camera Parameters");
	m_CpType[m_nCamIDIdx] = ini.readInt("CameraParamType", NONE_PARAM);

	CCameraParam* cam_aram = new CCameraParam();
	switch (m_CpType[m_nCamIDIdx]) {
	default:
	case NONE_PARAM: 
		printStatus(SL_WARNING, "no camera parameters.\r\n");
		break;
	case PROJECTIVE: 
		{
			char InExCamParamPath[MAX_PATH]; InExCamParamPath[0] = '\0';
			strcpy_s(InExCamParamPath, MAX_PATH, ini.readStr("InExCamParamPath","InExCamParam.txt").c_str());
			if (cam_aram->ReadCameraParamFile(InExCamParamPath)) {
				// resize projective matrix
				float* K = cam_aram->getK();
				K[0] /= resize_ratio; K[2] /= resize_ratio; 
				K[4] /= resize_ratio; K[5] /= resize_ratio;
				cam_aram->calculateP();

				sur_cam.setCameraParam(cam_aram);
			}
		}
		break;
	case HOMOGRAPHY: 
		{

		}
		break;
	}


	// object extraction engine
	switch (m_SegType[m_nCamIDIdx]) {
	default:
		printStatus(SL_PRINT, "no object extraction engine.\r\n"); 
		goto error;
	case BASIC_BG_SUB: 
		{
			ini.setSection("Basic Background Subtraction");
			CBasicBgSub* bbg_sub = new CBasicBgSub();
			int bg_mode = ini.readInt("BgMode", FST_FRAME);
			bbg_sub->setBgMode(bg_mode);
			bbg_sub->setHoleFilling(ini.readInt("EnableHoleFilling", 0) ? true : false);
			bbg_sub->setOtsuThresh1((ini.readInt("OtsuThresh1", 100)/100.f));
			bbg_sub->setOtsuThresh2((ini.readInt("OtsuThresh2", 90)/100.f));
			bbg_sub->setTopHat(ini.readInt("TopHat", 50));
			bbg_sub->setClose(ini.readInt("Close", 9));
			bbg_sub->setOpen(ini.readInt("Open", 13));
			bbg_sub->setDilate(ini.readInt("Dilate", 5));
			bbg_sub->setErode(ini.readInt("Erode", 5));
			bbg_sub->setFarLineRatio(ini.readInt("FarLineRatio", 33333)*1e-5f);
			bbg_sub->setNearLineRatio(ini.readInt("NearLineRatio", 66667)*1e-5f);
			bbg_sub->setBlobThreshFar(ini.readInt("BlobThreshFar", 3125)*1e-5f);
			bbg_sub->setBlobThreshMid(ini.readInt("BlobThreshMid", 6250)*1e-5f);
			bbg_sub->setBlobThreshNear(ini.readInt("BlobThreshNear", 12500)*1e-5f);
			char ImgPath[MAX_PATH]; ImgPath[0] = '\0';
			if (bg_mode = LOAD_IMG) {
				strcpy_s(ImgPath, MAX_PATH, ini.readStr("BgImgPath","background.bmp").c_str());
				bbg_sub->setBgImgPath(ImgPath);
			}
			sur_cam.setSegEngine(bbg_sub);
		}
		break;
	}


	// shape fitting engine
	switch (m_SfType[m_nCamIDIdx]) {
	default:
	case DEFORM_MODEL: 
		{
			ini.setSection("Deformable Model");
			CVehicleShapeFitting* vsf = new CVehicleShapeFitting();
			vsf->setSegmentL(ini.readInt("SegmentL", 5));
			vsf->setSegmentW(ini.readInt("SegmentW", 10));
			vsf->setGradientThreshSF(float(ini.readInt("GradientThreshSF", 2)));
			vsf->setContrastSF(ini.readInt("ContrastSF", 0));
			vsf->setIterationSF(ini.readInt("RSF", 2000), ini.readInt("NSF", 40));
			vsf->setCriterionThreshSF(ini.readInt("CriterionThreshSF", 2)/100.f);
			vsf->setGradientThreshPE(float(ini.readInt("GradientThreshPE", 40)));
			vsf->setContrastPE(ini.readInt("ContrastPE", 128));
			vsf->setIterationPE(ini.readInt("RPE", 500), ini.readInt("NPE", 50));
			vsf->setCriterionThreshPE(ini.readInt("CriterionThreshPE", 4)/100.f);
			sur_cam.setSFEngine(vsf);
			sur_cam.setScaleRatioFar(ini.readInt("ScaleLFar", 1000)/1000.f, 
				                     ini.readInt("ScaleWFar", 1000)/1000.f, 
				                     ini.readInt("ScaleHFar", 1000)/1000.f);
			sur_cam.setScaleRatioMid(ini.readInt("ScaleLMid", 1000)/1000.f, 
				                     ini.readInt("ScaleWMid", 1000)/1000.f, 
				                     ini.readInt("ScaleHMid", 1000)/1000.f);
			sur_cam.setScaleRatioNear(ini.readInt("ScaleLNear", 1000)/1000.f, 
				                      ini.readInt("ScaleWNear", 1000)/1000.f, 
				                      ini.readInt("ScaleHNear", 1000)/1000.f);
		}
		break;
	}

	// debug
	ini.setSection("Debug");
	int thickness = ini.readInt("DisplayThickness", 1);
	sur_cam.setDisplayThickness(thickness);
	switch (m_SegType[m_nCamIDIdx]) {
	default: break;
	case BASIC_BG_SUB: 
		((CBasicBgSub*)sur_cam.getSegEngine())->setSaveBlobs(ini.readInt("SaveBlobs", 0) ? true : false);
		((CBasicBgSub*)sur_cam.getSegEngine())->setDisplayBlobs(ini.readInt("DisplayBlobs", 0) ? true : false);
		break;
	}
	sur_cam.setDisplaySegment(ini.readInt("DisplaySegment", 0) ? true : false);
	sur_cam.setDisplayBlock(ini.readInt("DisplayBlock", 0) ? true : false);
	sur_cam.setDisplayPath(ini.readInt("DisplayPath", 0) ? true : false);
	sur_cam.setDisplay3dModel(ini.readInt("Display3dModel", 0) ? true : false);
	sur_cam.setDisplayKernels(ini.readInt("DisplayKernels", 0) ? true : false);
	switch (m_SfType[m_nCamIDIdx]) {
	default: break;
	case DEFORM_MODEL: 
		((CVehicleShapeFitting*)sur_cam.getSFEngine())->setDisplayThickness(thickness);
		((CVehicleShapeFitting*)sur_cam.getSFEngine())->setDisplayProjection(ini.readInt("DisplayProjection", 0) ? true : false);
		((CVehicleShapeFitting*)sur_cam.getSFEngine())->setDisplayIteration(ini.readInt("DisplayIteration", 0) ? true : false);
		break;
	}
	sur_cam.setSaveDebugFrames(ini.readInt("SaveDebugFrames", 0) ? true : false);
	sur_cam.setBeginFrame(unsigned long(ini.readInt("BeginFrame", 0)));

	res = true;




error:
	return res;
}