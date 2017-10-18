#include "Cfg.h"

CCfg::CCfg()
{
	m_oFrmSz = cv::Size(640, 480);
	m_fFrmRt = 10.0f;
	//std::strcpy(m_acInVdoPth, ".\\data\\vdo.avi");	// in Windows
	std::strcpy(m_acInVdoPth, "./data/vdo.avi");	// in Linux
	//std::strcpy(m_acInFrmPth, ".\\data\\frm.jpg");	// in Windows
	std::strcpy(m_acInFrmPth, "./data/frm.jpg");	// in Linux
	//std::strcpy(m_acInRoiPth, ".\\data\\roi.jpg");	// in Windows
	std::strcpy(m_acInRoiPth, "./data/roi.jpg");	// in Linux
	//std::strcpy(m_acInDetTxtPth, ".\\data\\det\\det.txt");	// in Windows
	std::strcpy(m_acInDetTxtPth, "./data/det/det.txt");	// in Linux
	//std::strcpy(m_acOutDetTxtPth, ".\\data\\det\\det.txt");	// in Windows
	std::strcpy(m_acOutDetTxtPth, "./data/det/det.txt");	// in Linux
	//std::strcpy(m_acOutVdoPth, ".\\data\\outVdo.avi");	// in Windows
	std::strcpy(m_acOutVdoPth, "./data/outVdo.avi");	// in Linux
	//std::strcpy(m_acOutImgFlrPth, ".\\data\\outImg1\\");	// in Windows
	std::strcpy(m_acOutImgFlrPth, "./data/outImg1/");	// in Linux
	//std::strcpy(m_acOutTstpPth, ".\\data\\tstp.txt");	// in Windows
	std::strcpy(m_acOutTstpPth, "./data/tstp.txt");	// in Linux
	m_nInVdoTyp = 0;
	m_nInDetTyp = 0;
	m_bOutDetFlg = false;
	m_bOutVdoFlg = false;
	m_bOutImgFlg = false;
	m_bSelRoiFlg = false;
	m_bPltDetFlg = false;
	m_nProcStFrmCnt = 0;
	m_nProcFrmNum = -1;
	m_fOutFrmRt = 10.0f;
	m_nRszFrmHei = -1;
	m_fDetRszRat = 1.0f;
	m_fDetScrThld = 30.0f;
	m_vstrDetObjCls.push_back("person");
	m_vovDetAspRatRng.push_back(cv::Vec2f(-1.0f, -1.0f));
    m_bDetMltSclTstFlg = false;
	m_fDetHogHitThld = 0.0f;
	m_nDetHogWinStrdSz = 8;
	m_nDetHogPadSz = 32;
	m_fDetHogScl0 = 1.05f;
	m_nDetHogGrpThld = 2;
	std::strcpy(m_acDetYoloDknetPth, "./data/det/darknet");	// in Linux
	std::strcpy(m_acDetYoloDataPth, "./cfg/coco.data");	// in Linux
	std::strcpy(m_acDetYoloCfgPth, "./cfg/yolo.cfg");	// in Linux
	std::strcpy(m_acDetYoloWgtPth, "./yolo.weights");	// in Linux
	//std::strcpy(m_acDetC4Mdl1Pth, ".\\data\\det\\mdl\\combined1.txt.model");	// in Windows
	//std::strcpy(m_acDetC4Mdl1Pth, "./data/det/mdl/combined1.txt.model");	// in Linux
	//std::strcpy(m_acDetC4Mdl2Pth, ".\\data\\det\\mdl\\combined2.txt.model");	// in Windows
	//std::strcpy(m_acDetC4Mdl2Pth, "./data/det/mdl/combined2.txt.model");	// in Linux
	//m_nDetC4WidThld = 36;
	//m_nDetC4HeiThld = 108;
	//m_nDetC4XDiv = 9;
	//m_nDetC4YDiv = 4;
}

CCfg::~CCfg()
{

}

void CCfg::loadCfgFile()
{
	FILE * poCfgFile;
	long nlFileSz, nlRdRst;
	char * pcBuf;

	//poCfgFile = std::fopen(".\\data\\cfg.json", "r");	// in Windows
	poCfgFile = std::fopen("./data/cfg.json", "r");	// in Linux
	if (poCfgFile == NULL) { std::fputs("Error: configuration file not opened", stderr); exit(1); }

	// obtain file size:
	fseek(poCfgFile, 0, SEEK_END);
	nlFileSz = ftell(poCfgFile);
	rewind(poCfgFile);

	// allocate memory to contain the whole file:
	pcBuf = (char*)malloc(sizeof(char)*nlFileSz);
	if (pcBuf == NULL) { fputs("Memory error", stderr); exit(2); }

	// copy the file into the buffer:
	nlRdRst = fread(pcBuf, 1, nlFileSz, poCfgFile);
	//if (nlRdRst != nlFileSz) { fputs("Reading error", stderr); exit(3); }

	std::string strCfg(pcBuf);
	//strCfg.erase(std::remove_if(strCfg.begin(), strCfg.end(), [](char c) { return c >= 0 && isspace(c); }), strCfg.end());	// in Windows
    strCfg.erase(std::remove_if(strCfg.begin(), strCfg.end(), ::isspace), strCfg.end());	// in Linux

	int nParamPos = strCfg.find("\"inVdoPth\"");
	if (nParamPos != std::string::npos)
		std::strcpy(m_acInVdoPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"inFrmPth\"");
	if (nParamPos != std::string::npos)
		std::strcpy(m_acInFrmPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"inRoiPth\"");
	if (nParamPos != std::string::npos)
		std::strcpy(m_acInRoiPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"inDetTxtPth\"");
	if (nParamPos != std::string::npos)
		std::strcpy(m_acInDetTxtPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"outDetTxtPth\"");
	if (nParamPos != std::string::npos)
		std::strcpy(m_acOutDetTxtPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"outVdoPth\"");
	if (nParamPos != std::string::npos)
		std::strcpy(m_acOutVdoPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"outImgFlrPth\"");
	if (nParamPos != std::string::npos)
		std::strcpy(m_acOutImgFlrPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"outTstpPth\"");
	if (nParamPos != std::string::npos)
		std::strcpy(m_acOutTstpPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"inVdoTyp\"");
	if (nParamPos != std::string::npos)
		m_nInVdoTyp = readInt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"inDetTyp\"");
	if (nParamPos != std::string::npos)
		m_nInDetTyp = readInt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"outDetFlg\"");
	if (nParamPos != std::string::npos)
		m_bOutDetFlg = readBool(strCfg, nParamPos);

	nParamPos = strCfg.find("\"outVdoFlg\"");
	if (nParamPos != std::string::npos)
		m_bOutVdoFlg = readBool(strCfg, nParamPos);

	nParamPos = strCfg.find("\"outImgFlg\"");
	if (nParamPos != std::string::npos)
		m_bOutImgFlg = readBool(strCfg, nParamPos);

	nParamPos = strCfg.find("\"selRoiFlg\"");
	if (nParamPos != std::string::npos)
		m_bSelRoiFlg = readBool(strCfg, nParamPos);

	nParamPos = strCfg.find("\"pltDetFlg\"");
	if (nParamPos != std::string::npos)
		m_bPltDetFlg = readBool(strCfg, nParamPos);

	nParamPos = strCfg.find("\"procStFrmCnt\"");
	if (nParamPos != std::string::npos)
		m_nProcStFrmCnt = readInt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"procFrmNum\"");
	if (nParamPos != std::string::npos)
		m_nProcFrmNum = readInt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"outFrmRt\"");
	if (nParamPos != std::string::npos)
		m_fOutFrmRt = readFlt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"rszFrmHei\"");
	if (nParamPos != std::string::npos)
		m_nRszFrmHei = readInt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"detRszRat\"");
	if (nParamPos != std::string::npos)
		m_fDetRszRat = readFlt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"detScrThld\"");
	if (nParamPos != std::string::npos)
		m_fDetScrThld = readFlt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"detObjCls\"");
	if (nParamPos != std::string::npos)
		readVecStr(strCfg, nParamPos, m_vstrDetObjCls);

    nParamPos = strCfg.find("\"detAspRatRng\"");
	if (nParamPos != std::string::npos)
		m_vovDetAspRatRng = readVecFltPr(strCfg, nParamPos);

    nParamPos = strCfg.find("\"detMltSclTstFlg\"");
	if (nParamPos != std::string::npos)
		m_bDetMltSclTstFlg = readBool(strCfg, nParamPos);

	nParamPos = strCfg.find("\"detHogHitThld\"");
	if (nParamPos != std::string::npos)
		m_fDetHogHitThld = readFlt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"detHogWinStrdSz\"");
	if (nParamPos != std::string::npos)
		m_nDetHogWinStrdSz = readInt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"detHogPadSz\"");
	if (nParamPos != std::string::npos)
		m_nDetHogPadSz = readInt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"detHogScl0\"");
	if (nParamPos != std::string::npos)
		m_fDetHogScl0 = readFlt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"detHogGrpThld\"");
	if (nParamPos != std::string::npos)
		m_nDetHogGrpThld = readInt(strCfg, nParamPos);

	nParamPos = strCfg.find("\"detYoloDknetPth\"");	// in Linux
	if (nParamPos != std::string::npos)	// in Linux
		std::strcpy(m_acDetYoloDknetPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"detYoloDataPth\"");	// in Linux
	if (nParamPos != std::string::npos)	// in Linux
		std::strcpy(m_acDetYoloDataPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"detYoloCfgPth\"");	// in Linux
	if (nParamPos != std::string::npos)	// in Linux
		std::strcpy(m_acDetYoloCfgPth, readCharArr(strCfg, nParamPos).c_str());

	nParamPos = strCfg.find("\"detYoloWgtPth\"");	// in Linux
	if (nParamPos != std::string::npos)	// in Linux
		std::strcpy(m_acDetYoloWgtPth, readCharArr(strCfg, nParamPos).c_str());

	//nParamPos = strCfg.find("\"detC4Mdl1Pth\"");
	//if (nParamPos != std::string::npos)
	//	std::strcpy(m_acDetC4Mdl1Pth, readCharArr(strCfg, nParamPos).c_str());

	//nParamPos = strCfg.find("\"detC4Mdl2Pth\"");
	//if (nParamPos != std::string::npos)
	//	std::strcpy(m_acDetC4Mdl2Pth, readCharArr(strCfg, nParamPos).c_str());

	//nParamPos = strCfg.find("\"detC4WidThld\"");
	//if (nParamPos != std::string::npos)
	//	m_nDetC4WidThld = readInt(strCfg, nParamPos);

	//nParamPos = strCfg.find("\"detC4HeiThld\"");
	//if (nParamPos != std::string::npos)
	//	m_nDetC4HeiThld = readInt(strCfg, nParamPos);

	//nParamPos = strCfg.find("\"detC4XDiv\"");
	//if (nParamPos != std::string::npos)
	//	m_nDetC4XDiv = readInt(strCfg, nParamPos);

	//nParamPos = strCfg.find("\"detC4YDiv\"");
	//if (nParamPos != std::string::npos)
	//	m_nDetC4YDiv = readInt(strCfg, nParamPos);

	// terminate
	fclose(poCfgFile);
	free(pcBuf);
}

std::string CCfg::readCharArr(std::string strCfg, int nParamPos)
{
	int nValPos, nValLen;

	nValPos = strCfg.find(":", (nParamPos + 1)) + 2;
	nValLen = strCfg.find("\"", (nValPos + 1)) - nValPos;

	return strCfg.substr(nValPos, nValLen);
}

int CCfg::readInt(std::string strCfg, int nParamPos)
{
	int nValPos, nValLen, nValEnd1, nValEnd2;

	nValPos = strCfg.find(":", (nParamPos + 1)) + 1;
	nValEnd1 = strCfg.find(",", (nValPos + 1));
	nValEnd2 = strCfg.find("}", (nValPos + 1));
	nValLen = (nValEnd1 <= nValEnd2) ? (nValEnd1 - nValPos) : (nValEnd2 - nValPos);

	return std::atoi(strCfg.substr(nValPos, nValLen).c_str());
}

float CCfg::readFlt(std::string strCfg, int nParamPos)
{
	int nValPos, nValLen, nValEnd1, nValEnd2;

	nValPos = strCfg.find(":", (nParamPos + 1)) + 1;
	nValEnd1 = strCfg.find(",", (nValPos + 1));
	nValEnd2 = strCfg.find("}", (nValPos + 1));
	nValLen = (nValEnd1 <= nValEnd2) ? (nValEnd1 - nValPos) : (nValEnd2 - nValPos);

	return std::atof(strCfg.substr(nValPos, nValLen).c_str());
}

bool CCfg::readBool(std::string strCfg, int nParamPos)
{
	int nBoolVal, nValPos, nValLen, nValEnd1, nValEnd2;

	nValPos = strCfg.find(":", (nParamPos + 1)) + 1;
	nValEnd1 = strCfg.find(",", (nValPos + 1));
	nValEnd2 = strCfg.find("}", (nValPos + 1));
	nValLen = (nValEnd1 <= nValEnd2) ? (nValEnd1 - nValPos) : (nValEnd2 - nValPos);

	nBoolVal = std::atoi(strCfg.substr(nValPos, nValLen).c_str());
	if (nBoolVal > 0)
		return true;
	else if (nBoolVal <= 0)
		return false;
}

void CCfg::readVecStr(std::string strCfg, int nParamPos, std::vector<std::string>& vstrCharArr)
{
	int nValPos, nValLen, nValEnd;
	std::vector<std::string>().swap(vstrCharArr);

	nValPos = strCfg.find(":", (nParamPos + 1)) + 3;
	nValEnd = strCfg.find("]", nValPos);

	while (nValPos < nValEnd)
	{
		nValLen = strCfg.find("\"", (nValPos + 1)) - nValPos;
		std::string strCharArr = strCfg.substr(nValPos, nValLen);
		vstrCharArr.push_back(strCharArr);
		nValPos = nValPos + nValLen + 3;
	}
}

cv::Point CCfg::read2dPt(std::string strCfg, int nParamPos)
{
	int nPtXPos = nParamPos, nPtXLen, nPtYPos, nPtYLen;

	nPtXPos = strCfg.find(":", (nParamPos + 1)) + 2;
	nPtXLen = strCfg.find(",", (nPtXPos + 1)) - nPtXPos;
	nPtYPos = nPtXPos + nPtXLen + 1;
	nPtYLen = strCfg.find("]", (nPtYPos + 1)) - nPtYPos;
	cv::Point oPt = cv::Point(std::atof(strCfg.substr(nPtXPos, nPtXLen).c_str()), std::atof(strCfg.substr(nPtYPos, nPtYLen).c_str()));

	return oPt;
}

std::vector<cv::Vec2f> CCfg::readVecFltPr(std::string strCfg, int nParamPos)
{
    int nValPos, nValLen, nValEnd, nFlt1Pos = nParamPos, nFlt1Len, nFlt2Pos, nFlt2Len;
	std::vector<cv::Vec2f> vovFlt;

	nValPos = strCfg.find(":", (nParamPos + 1)) + 1;
	nValEnd = strCfg.find("]]", (nValPos + 1));

	while (nValPos < nValEnd)
	{
		nFlt1Pos = strCfg.find("[", (nValPos + 1)) + 1;
		nFlt1Len = strCfg.find(",", (nFlt1Pos + 1)) - nFlt1Pos;
		nFlt2Pos = nFlt1Pos + nFlt1Len + 1;
		nFlt2Len = strCfg.find("]", (nFlt2Pos + 1)) - nFlt2Pos;
		vovFlt.push_back(cv::Vec2f(std::atof(strCfg.substr(nFlt1Pos, nFlt1Len).c_str()), std::atof(strCfg.substr(nFlt2Pos, nFlt2Len).c_str())));
		nValPos = nFlt2Pos + nFlt2Len + 1;
	}

	return vovFlt;
}
