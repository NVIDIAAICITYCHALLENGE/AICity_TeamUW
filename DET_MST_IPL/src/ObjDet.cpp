#include "ObjDet.h"

CDetNd::CDetNd(void)
{
	setFrmCnt(-1);
	setDetScr(0.0f);
	setDetCls(DFLT_DET_CLS);
}

CDetNd::CDetNd(cv::Rect2f oDetBBox, float fDetScr, char* acDetCls, int nFrmCnt)
{
	setDetBBox(oDetBBox);
	setDetScr(fDetScr);
	setDetCls(acDetCls);
	setFrmCnt(nFrmCnt);
}

CDetNd::~CDetNd(void)
{

}

CObjDet::CObjDet(void)
{
    m_bYoloInit = false;	// in Linux
}

CObjDet::~CObjDet(void)
{
	if (0 == m_oCfg.getInDetTyp())
		m_ifsInDetTxt.close();
}

void CObjDet::initialize(CCfg oCfg, cv::Mat oImgRoi)
{
	// configuration parameters
	m_oCfg = oCfg;

	// ROI image
	m_oImgRoi = oImgRoi.clone();

	// next detected node
	m_oNxtDetNd = CDetNd();

	// text file for reading detection results
	if (0 == m_oCfg.getInDetTyp())
	{
		m_ifsInDetTxt.close();
		m_ifsInDetTxt.open(oCfg.getInDetTxtPth());
	}
	// HOG descriptor
	else if (1 == m_oCfg.getInDetTyp())
		m_oHogDesc.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    // YOLO v2 detector
    else if ((2 == m_oCfg.getInDetTyp()) && !m_bYoloInit)	// in Linux
    {
        m_bYoloInit = true;

        std::string strYoloDknetPth = m_oCfg.getDetYoloDknetPth();
        std::string strYoloDataPth = m_oCfg.getDetYoloDataPth();
        std::string strYoloCfgPth = m_oCfg.getDetYoloCfgPth();
        std::string strYoloWgtPth = m_oCfg.getDetYoloWgtPth();
        //std::string strCdDknetCmd;
        std::string  strDknetTstCmd;

        //strCdDknetCmd.append("cd ").append(strYoloDknetPth);
        //system(strCdDknetCmd.c_str());

        strDknetTstCmd.append("./darknet detector test ").append(strYoloDataPth).append(" ");
        strDknetTstCmd.append(strYoloCfgPth).append(" ").append(strYoloWgtPth);
        //system(strDknetTstCmd.c_str());

        std::cout << "(1) open a new terminal" << std::endl;
        std::cout << "(2) cd to the directory of ./darknet" << std::endl;
        std::cout << "(3) execute the following command in the terminal: " << std::endl;
        std::cout << strDknetTstCmd << std::endl;
        std::cout << std::endl;
        cv::waitKey(0);
    }
	//// C4 scanner
	//else if (2 == m_oCfg.getInDetTyp())
	//{
	//	m_poC4Scnr = new DetectionScanner(m_oCfg.getDetC4HeiThld(), m_oCfg.getDetC4WidThld(),
	//		m_oCfg.getDetC4XDiv(), m_oCfg.getDetC4YDiv(), 256, 0.8);
	//	LoadCascade(*m_poC4Scnr, m_oCfg.getDetC4Mdl1Pth(), m_oCfg.getDetC4Mdl2Pth());
	//}

	// out text file of object detection
	if (m_oCfg.getOutDetFlg())
	{
		FILE* pfOutDet = std::fopen(m_oCfg.getOutDetTxtPth(), "w");
		std::fclose(pfOutDet);
	}
}

void CObjDet::process(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd, int nFrmCnt)
{
	// read results of object detection
	if (0 == m_oCfg.getInDetTyp())
		rdObjDet(oImgFrm, voDetNd, nFrmCnt);
	// run object detection
	else
		rnObjDet(oImgFrm, voDetNd, nFrmCnt);

	if (NMS_DET_FLG)
		nonMaxSuppr(voDetNd);

	rmvOutBBox(voDetNd);
}

void CObjDet::output(cv::Mat& oImgFrm, std::vector<CDetNd>& voDetNd)
{
	if (m_oCfg.getOutDetFlg())
		outTxt(voDetNd);

	if (m_oCfg.getPltDetFlg())
		pltDetBBox(oImgFrm, voDetNd);
}

void CObjDet::rdObjDet(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd, int nFrmCnt)
{
	bool bNxtFrmFlg = true;
	std::vector<CDetNd>().swap(voDetNd);
    std::vector<std::string> vstrDetObjCls = m_oCfg.getDetObjCls();

	while ((!m_ifsInDetTxt.eof()) && ((nFrmCnt == m_oNxtDetNd.getFrmCnt()) || (-1 == m_oNxtDetNd.getFrmCnt())))
	{
		// at the first frame with objects
		if (-1 == m_oNxtDetNd.getFrmCnt())
			bNxtFrmFlg = false;

		// push back the extra node read from the last iteration
		// not at the first frame with objects
		if (bNxtFrmFlg && (nFrmCnt == m_oNxtDetNd.getFrmCnt()))
		{
			voDetNd.push_back(m_oNxtDetNd);
			bNxtFrmFlg = false;
		}

		char acInDetBuf[256] = { 0 };
		int nDetFrmCnt = -1, nDetId = -1;
		float fDetScr = 0.0f;
		cv::Rect2f oDetBBox;
		cv::Point3f oDet3dCoord;
		char acDetCls[32];

		// read from the input txt file
		m_ifsInDetTxt.getline(acInDetBuf, 256);
		std::sscanf(acInDetBuf, "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%s", &nDetFrmCnt, &nDetId,
			&oDetBBox.x, &oDetBBox.y, &oDetBBox.width, &oDetBBox.height,
			&fDetScr, &oDet3dCoord.x, &oDet3dCoord.y, &oDet3dCoord.z, acDetCls);

        char * acDetClsTmp;
        acDetClsTmp = std::strstr(acInDetBuf,acDetCls);
        std::strncpy(acDetCls, acDetClsTmp, ((acInDetBuf + std::strlen(acInDetBuf)) - acDetClsTmp));
        if (!std::isalnum(acDetCls[std::strlen(acDetCls)-1]))
            acDetCls[std::strlen(acDetCls)-1] = '\0';
        std::string strDetCls(acDetCls);
        //strDetCls.erase(std::remove_if(strDetCls.begin(), strDetCls.end(), [](char c) { return c >= 0 && isspace(c); }), strDetCls.end());	// in Windows
        strDetCls.erase(std::remove_if(strDetCls.begin(), strDetCls.end(), ::isspace), strDetCls.end());	// in Linux

        if (nDetFrmCnt >= nFrmCnt)
        {
            m_oNxtDetNd = CDetNd(oDetBBox, fDetScr, acDetCls, nDetFrmCnt);

            // if there is object detected
            if (0 <= nDetFrmCnt)
            {
                // validate the detected bounding box
                if (valBBox(oDetBBox, m_oCfg.getFrmSz(), m_oImgRoi))
                {
                    m_oNxtDetNd.setDetBBox(oDetBBox);

                    // only objects in the current frame are pushed back
                    // filter away detected objects with low score
                    if ((nFrmCnt >= nDetFrmCnt) && (m_oCfg.getDetScrThld() < fDetScr))
                    {
                        // only specific object classes are accepted
                        for (std::vector<std::string>::iterator it = vstrDetObjCls.begin(); it != vstrDetObjCls.end(); ++it)
                        {
                            if (0 == it->compare(strDetCls));
                            {
                                voDetNd.push_back(m_oNxtDetNd);
                                break;
                            }
                        }
                    }
                }
            }
        }
	}
}

void CObjDet::rnObjDet(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd, int nFrmCnt)
{
	float fDetRszRat = m_oCfg.getDetRszRat();

	if (1.0f  != fDetRszRat)	// enlarge the image and do the detection
	{
		cv::Mat oImgRsz;
		cv::resize(oImgFrm, oImgRsz, cv::Size((oImgFrm.cols * fDetRszRat), (oImgFrm.rows * fDetRszRat)));

        if (m_oCfg.getDetMltSclTstFlg())
            detMltSclTst(oImgRsz, voDetNd);
        else
        {
            if (1 == m_oCfg.getInDetTyp())
                detHog(oImgRsz, voDetNd);
            else if (2 == m_oCfg.getInDetTyp()) // in Linux
                detYolo(oImgRsz, voDetNd);
            //else if (2 == m_oCfg.getInDetTyp())
            //	detC4(oImgRsz, voDetNd);
            else
                std::cout << "Error: Unknown object detector" << std::endl;
        }

		for (int i = 0; i < voDetNd.size(); i++)	// for each detected object
		{
			cv::Rect2f oDetBBox = cv::Rect2f((voDetNd[i].getDetBBox().x / fDetRszRat),
				(voDetNd[i].getDetBBox().y / fDetRszRat),
				(voDetNd[i].getDetBBox().width / fDetRszRat),
				(voDetNd[i].getDetBBox().height / fDetRszRat));
			if (valBBox(oDetBBox, m_oCfg.getFrmSz(), m_oImgRoi))
			{
				voDetNd[i].setFrmCnt(nFrmCnt);
				voDetNd[i].setDetBBox(oDetBBox);
			}
		}
	}
	else
	{
        if (m_oCfg.getDetMltSclTstFlg())
            detMltSclTst(oImgFrm, voDetNd);
        else
        {
            if (1 == m_oCfg.getInDetTyp())
                detHog(oImgFrm, voDetNd);
            else if (2 == m_oCfg.getInDetTyp()) // in Linux
                detYolo(oImgFrm, voDetNd);
            //else if (2 == m_oCfg.getInDetTyp())
            //	detC4(oImgFrm, voDetNd);
            else
                std::cout << "Error: Unknown object detector" << std::endl;
        }

		for (int i = 0; i < voDetNd.size(); i++)	// for each detected object
		{
			cv::Rect2f oDetBBox = voDetNd[i].getDetBBox();
			if (valBBox(oDetBBox, m_oCfg.getFrmSz(), m_oImgRoi))
			{
				voDetNd[i].setDetBBox(oDetBBox);
				voDetNd[i].setFrmCnt(nFrmCnt);
			}
		}
	}
}

void CObjDet::detHog(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd)
{
	std::vector<cv::Rect> voRectFound;
	std::vector<cv::Rect2f> voRectFiltFound;
	std::vector<CDetNd>().swap(voDetNd);

	// run the detector with default parameters. to get a higher hit-rate
    // (and more false alarms, respectively), decrease the hitThreshold and
    // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
	float fHitThld = m_oCfg.getDetHogHitThld();
	int nWinStrdSz = m_oCfg.getDetHogWinStrdSz();
	int nPadSz = m_oCfg.getDetHogPadSz();
	float fScale0 = m_oCfg.getDetHogScl0();
	int fGrpThld = m_oCfg.getDetHogGrpThld();

	m_oHogDesc.detectMultiScale(oImgFrm, voRectFound, fHitThld, cvSize(nWinStrdSz,nWinStrdSz), cvSize(nPadSz,nPadSz), fScale0, fGrpThld);

	for(int i = 0; i < voRectFound.size(); i++)
	{
		int j;

		cv::Rect oRect = voRectFound[i];

		for (j = 0; j < voRectFound.size(); j++)
			if (j != i && (oRect & voRectFound[j]) == oRect)
				break;

		if (j == voRectFound.size())
			voRectFiltFound.push_back(oRect);
	}

	for(int i = 0; i < voRectFiltFound.size(); i++)
	{
		cv::Rect2f oRect = voRectFiltFound[i];
		// the HOG detector returns slightly larger rectangles than the real objects.
		// so we slightly shrink the rectangles to get a nicer out.
		oRect.x += oRect.width * 0.1f;
		oRect.width = oRect.width * 0.8f;
		oRect.y += oRect.height * 0.07f;
		oRect.height = oRect.height * 0.8f;
		voDetNd.push_back(CDetNd(oRect, DFLT_DET_SCR, DFLT_DET_CLS));
	}

	std::vector<cv::Rect>().swap(voRectFound);
	std::vector<cv::Rect2f>().swap(voRectFiltFound);
}

void CObjDet::detYolo(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd)	// in Linux
{
	std::vector<CDetNd>().swap(voDetNd);

    char cSgl = ' ';
	FILE* pfCommTxt;

    char acTstImgNm[128] = { 0 };
    std::sprintf(acTstImgNm, "/test.jpg");
    char acTstImgPth[128] = { 0 };
    std::sprintf(acTstImgPth, m_oCfg.getDetYoloDknetPth());
    std::strcat(acTstImgPth, acTstImgNm);
    cv::imwrite(acTstImgPth, oImgFrm);

    char acCommTxtNm[128] = { 0 };
    std::sprintf(acCommTxtNm, "/comm.txt");
    char acCommTxtPth[128] = { 0 };
    std::sprintf(acCommTxtPth, m_oCfg.getDetYoloDknetPth());
    std::strcat(acCommTxtPth, acCommTxtNm);

	// e means darknet is ready to process image
	while ('e' != cSgl)
	{
		pfCommTxt = std::fopen(acCommTxtPth, "r");
		cSgl = std::fgetc(pfCommTxt);
		std::fclose(pfCommTxt);
	}

	// tell darknet to process test.jpg
	pfCommTxt = std::fopen(acCommTxtPth, "w");
    std::fprintf(pfCommTxt, "s");  // s is start signal
	std::fclose(pfCommTxt);

	cv::Rect oDetBBox;
	float fDetScr;
	char acDetCls[128];

	cSgl = 's';

	// wait for processing to be finished and results to be written to out.txt
	while ('e' != cSgl)
	{
		pfCommTxt = std::fopen(acCommTxtPth, "r");
		cSgl = std::fgetc(pfCommTxt);
		std::fclose(pfCommTxt);
	}

    char acOutTxtNm[128] = { 0 };
    std::sprintf(acOutTxtNm, "/out.txt");
    char acOutTxtPth[128] = { 0 };
    std::sprintf(acOutTxtPth, m_oCfg.getDetYoloDknetPth());
    std::strcat(acOutTxtPth, acOutTxtNm);
	std::ifstream ifsOutTxt(acOutTxtPth);

    std::vector<std::string> vstrDetObjCls = m_oCfg.getDetObjCls();

    char acBuf[256] = { 0 };
    ifsOutTxt.getline(acBuf, 256);
	while (!ifsOutTxt.eof())
	{
        std::sscanf(acBuf, "%d %d %d %d %f %s",
            &oDetBBox.x, &oDetBBox.width, &oDetBBox.y, &oDetBBox.height, &fDetScr, acDetCls);

        oDetBBox.width -= oDetBBox.x;
        oDetBBox.height -= oDetBBox.y;
        fDetScr *= 100.0f;
        char * acDetClsTmp;
        acDetClsTmp = std::strstr(acBuf,acDetCls);
        std::strncpy(acDetCls, acDetClsTmp, ((acBuf + std::strlen(acBuf)) - acDetClsTmp));
        if (!std::isalnum(acDetCls[std::strlen(acDetCls)-1]))
            acDetCls[std::strlen(acDetCls)-1] = '\0';
        std::string strDetCls(acDetCls);
        //strDetCls.erase(std::remove_if(strDetCls.begin(), strDetCls.end(), [](char c) { return c >= 0 && isspace(c); }), strDetCls.end());	// in Windows
        strDetCls.erase(std::remove_if(strDetCls.begin(), strDetCls.end(), ::isspace), strDetCls.end());	// in Linux

        // check that bounding box class is one we want
        // only specific object classes are accepted
        for (std::vector<std::string>::iterator it = vstrDetObjCls.begin(); it != vstrDetObjCls.end(); ++it)
        {
            if ((0 == it->compare(strDetCls)) && (m_oCfg.getDetScrThld() < fDetScr))
            {
                voDetNd.push_back(CDetNd(oDetBBox, fDetScr, acDetCls));
                break;
            }
        }

        ifsOutTxt.getline(acBuf, 256);
	}
}

//void CObjDet::detC4(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd)
//{
//	std::vector<CvRect> voRectFound;
//	cv::Rect2f oRect;
//	std::vector<CDetNd>().swap(voDetNd);
//
//	voRectFound = DetectHumanFromIplImage(cvCloneImage(&(IplImage)oImgFrm), *m_poC4Scnr);
//
//	for( int i = 0; i < voRectFound.size(); i++ )
//	{
//		oRect.x = voRectFound[i].x;
//		oRect.y = voRectFound[i].y;
//		oRect.width = voRectFound[i].width;
//		oRect.height = voRectFound[i].height;
//		voDetNd.push_back(CDetNd(oRect, DFLT_DET_SCR, DFLT_DET_CLS));
//	}
//
//	std::vector<CvRect>().swap(voRectFound);
//}

void CObjDet::detMltSclTst(cv::Mat oImgFrm, std::vector<CDetNd>& voDetNd)
{
	std::vector<CDetNd>().swap(voDetNd);
    cv::Size oFrmSz = oImgFrm.size();

    cv::Mat oSubImg, oSubRoiImg;
	std::vector<CDetNd> voSubImgDetNd;
    std::vector<cv::Rect> voSubImgRoi;
    std::vector<cv::Point> voTopLftPt;

    for (int i = 0; i < 9; i++)
    {
        voTopLftPt.push_back(cv::Point(((i % 3) * (oFrmSz.width / 4)), ((i / 3) * (oFrmSz.height / 4))));
        voSubImgRoi.push_back(cv::Rect(voTopLftPt[i].x, voTopLftPt[i].y, (oFrmSz.width / 2), (oFrmSz.height / 2)));
    }

    // perform object detection in each individual subregion
    for (int i = 0; i < 9; i++)
    {
        oSubImg = cv::Mat(oImgFrm, voSubImgRoi[i]);
        cv::resize(oSubImg, oSubImg, oFrmSz);

        oSubRoiImg = cv::Mat(m_oImgRoi, voSubImgRoi[i]);
        cv::resize(oSubRoiImg, oSubRoiImg, oFrmSz);

        if (1 == m_oCfg.getInDetTyp())
			detHog(oSubImg, voSubImgDetNd);
		else if (2 == m_oCfg.getInDetTyp()) // in Linux
			detYolo(oSubImg, voSubImgDetNd);
		//else if (2 == m_oCfg.getInDetTyp())
		//	detC4(oSubImg, voSubImgDetNd);
		else
			std::cout << "Error: Unknown object detector" << std::endl;

        for (int j = 0; j < voSubImgDetNd.size(); j++)	// for each detected object
		{
			cv::Rect2f oSubImgDetBBox = voSubImgDetNd[j].getDetBBox();
			if (valBBox(oSubImgDetBBox, oSubImg.size(), oSubRoiImg))
			{
                oSubImgDetBBox.x /= 2;
                oSubImgDetBBox.y /= 2;
                oSubImgDetBBox.width /= 2;
                oSubImgDetBBox.height /= 2;
                oSubImgDetBBox.x += voTopLftPt[i].x;
                oSubImgDetBBox.y += voTopLftPt[i].y;
                voSubImgDetNd[j].setDetBBox(oSubImgDetBBox);
                voDetNd.push_back(voSubImgDetNd[j]);
			}
		}
    }

    nonMaxSuppr(voDetNd);
}

void CObjDet::nonMaxSuppr(std::vector<CDetNd>& voDetNd)
{
	std::vector<int> viNonMaxDetNd;
	std::vector<int>::iterator it;

	for (int i = voDetNd.size() - 1; i >= 0; i--)
	{
		// a pair of bounding boxes overlap with each other: erase the one with lower detection score
		for (int j = i - 1; j >= 0; j--)
		{
			if (NMS_DET_IOU_THLD < calcBBoxIou(voDetNd[i].getDetBBox(), voDetNd[j].getDetBBox()))
			{
				int iNonMaxDetNd = (voDetNd[i].getDetScr() < voDetNd[j].getDetScr()) ? i : j;
				it = find(viNonMaxDetNd.begin(), viNonMaxDetNd.end(), iNonMaxDetNd);
				if (viNonMaxDetNd.end() == it)
					viNonMaxDetNd.push_back(iNonMaxDetNd);
			}
		}
	}

	for (int i = voDetNd.size() - 1; i >= 0; i--)
	{
		it = find(viNonMaxDetNd.begin(), viNonMaxDetNd.end(), i);
		if (viNonMaxDetNd.end() != it)
		{
			voDetNd.erase(voDetNd.begin() + i);
			viNonMaxDetNd.erase(it);
		}
	}
}

void CObjDet::rmvOutBBox(std::vector<CDetNd>& voDetNd)
{
	std::vector<int> viOutDetNd;
	std::vector<int>::iterator it;

    // remove bounding box(es) of abnormal size(s) or aspect ratio(s)
    std::vector<std::string> vstrDetObjCls = m_oCfg.getDetObjCls();
    std::vector<cv::Vec2f> vovAspRatRng = m_oCfg.getDetAspRatRng();
    for (int i = voDetNd.size() - 1; i >= 0; i--)
	{
        for (std::vector<std::string>::iterator it = vstrDetObjCls.begin(); it != vstrDetObjCls.end(); ++it)
        {
            std::string strDetCls(voDetNd[i].getDetCls());
            //strDetCls.erase(std::remove_if(strDetCls.begin(), strDetCls.end(), [](char c) { return c >= 0 && isspace(c); }), strDetCls.end());	// in Windows
            strDetCls.erase(std::remove_if(strDetCls.begin(), strDetCls.end(), ::isspace), strDetCls.end());	// in Linux

            if (0 == it->compare(strDetCls))
            {
                float fAspRat = voDetNd[i].getDetBBox().width / voDetNd[i].getDetBBox().height;
                if ((vovAspRatRng[it - vstrDetObjCls.begin()][0] > fAspRat) || (vovAspRatRng[it - vstrDetObjCls.begin()][1] < fAspRat))
                    viOutDetNd.push_back(i);
                //break;
            }
        }
	}

	for (int i = voDetNd.size() - 1; i >= 0; i--)
	{
		it = find(viOutDetNd.begin(), viOutDetNd.end(), i);
		if (viOutDetNd.end() != it)
		{
			voDetNd.erase(voDetNd.begin() + i);
			viOutDetNd.erase(it);
		}
	}

    // remove large bounding box(es) that contain other bounding box(es)
	for (int i = voDetNd.size() - 1; i >= 0; i--)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			if ((voDetNd[i].getDetBBox().x <= voDetNd[j].getDetBBox().x) &&
				(voDetNd[i].getDetBBox().y <= voDetNd[j].getDetBBox().y) &&
				((voDetNd[i].getDetBBox().x + voDetNd[i].getDetBBox().width) >=
					(voDetNd[j].getDetBBox().x + voDetNd[j].getDetBBox().width)) &&
				((voDetNd[i].getDetBBox().y + voDetNd[i].getDetBBox().height) >=
					(voDetNd[j].getDetBBox().y + voDetNd[j].getDetBBox().height)))
			{
				it = find(viOutDetNd.begin(), viOutDetNd.end(), i);
				if (viOutDetNd.end() == it)
				{
					if (0 == std::strcmp(voDetNd[i].getDetCls(), voDetNd[j].getDetCls()))
						viOutDetNd.push_back(i);
				}
				break;
			}
			else if ((voDetNd[j].getDetBBox().x <= voDetNd[i].getDetBBox().x) &&
				(voDetNd[j].getDetBBox().y <= voDetNd[i].getDetBBox().y) &&
				((voDetNd[j].getDetBBox().x + voDetNd[j].getDetBBox().width) >=
					(voDetNd[i].getDetBBox().x + voDetNd[i].getDetBBox().width)) &&
				((voDetNd[j].getDetBBox().y + voDetNd[j].getDetBBox().height) >=
					(voDetNd[i].getDetBBox().y + voDetNd[i].getDetBBox().height)))
			{
				it = find(viOutDetNd.begin(), viOutDetNd.end(), j);
				if (viOutDetNd.end() == it)
				{
					if (0 == std::strcmp(voDetNd[j].getDetCls(), voDetNd[i].getDetCls()))
						viOutDetNd.push_back(j);
				}
				break;
			}
		}
	}

	for (int i = voDetNd.size() - 1; i >= 0; i--)
	{
		it = find(viOutDetNd.begin(), viOutDetNd.end(), i);
		if (viOutDetNd.end() != it)
		{
			voDetNd.erase(voDetNd.begin() + i);
			viOutDetNd.erase(it);
		}
	}

    // remove small bounding box(es) included by other bounding box(es)
	for (int i = voDetNd.size() - 1; i >= 0; i--)
	{
		for (int j = i - 1; j >= 0; j--)
		{
            if (0.0 < calcBBoxIou(voDetNd[i].getDetBBox(), voDetNd[j].getDetBBox()))
            {
                float fMinX = std::max(voDetNd[i].getDetBBox().x, voDetNd[j].getDetBBox().x);
                float fMinY = std::max(voDetNd[i].getDetBBox().y, voDetNd[j].getDetBBox().y);
                float fMaxX = std::min((voDetNd[i].getDetBBox().x + voDetNd[i].getDetBBox().width),
                    (voDetNd[j].getDetBBox().x + voDetNd[j].getDetBBox().width));
                float fMaxY = std::min((voDetNd[i].getDetBBox().y + voDetNd[i].getDetBBox().height),
                    (voDetNd[j].getDetBBox().y + voDetNd[j].getDetBBox().height));
                float fOvlpArea = (fMaxX - fMinX) * (fMaxY - fMinY);

                if (OVLP_AREA_RAT_THLD < (fOvlpArea / voDetNd[i].getDetBBox().area()))
                {
                    it = find(viOutDetNd.begin(), viOutDetNd.end(), i);
                    if (viOutDetNd.end() == it)
                    {
                        if (0 == std::strcmp(voDetNd[i].getDetCls(), voDetNd[j].getDetCls()))
                            viOutDetNd.push_back(i);
                    }
                    break;
                }
                else if (OVLP_AREA_RAT_THLD < (fOvlpArea / voDetNd[j].getDetBBox().area()))
                {
                    it = find(viOutDetNd.begin(), viOutDetNd.end(), j);
                    if (viOutDetNd.end() == it)
                    {
                        if (0 == std::strcmp(voDetNd[j].getDetCls(), voDetNd[i].getDetCls()))
                            viOutDetNd.push_back(j);
                    }
                    break;
                }
            }
		}
    }

	for (int i = voDetNd.size() - 1; i >= 0; i--)
	{
		it = find(viOutDetNd.begin(), viOutDetNd.end(), i);
		if (viOutDetNd.end() != it)
		{
			voDetNd.erase(voDetNd.begin() + i);
			viOutDetNd.erase(it);
		}
	}
}

void CObjDet::outTxt(std::vector<CDetNd>& voDetNd)
{
    FILE* pfOutDet = std::fopen(m_oCfg.getOutDetTxtPth(), "a");

    for (std::vector<CDetNd>::iterator it = voDetNd.begin(); it != voDetNd.end(); ++it)
		std::fprintf(pfOutDet, "%d,-1,%.3f,%.3f,%.3f,%.3f,%.3f,-1,-1,-1,%s\n",
			it->getFrmCnt(), it->getDetBBox().x, it->getDetBBox().y, it->getDetBBox().width, it->getDetBBox().height,
            it->getDetScr(), it->getDetCls());

	std::fclose(pfOutDet);
}

void CObjDet::pltDetBBox(cv::Mat& oImgFrm, std::vector<CDetNd>& voDetNd)
{
	for (std::vector<CDetNd>::iterator it = voDetNd.begin(); it != voDetNd.end(); ++it)
	{
        // object bounding box
		cv::rectangle(oImgFrm, it->getDetBBox(), cv::Scalar(0, 0, 0), 2);
		// object class label
		cv::Rect2f oDetClsLbl(it->getDetBBox().x, (it->getDetBBox().y - 20.0f), it->getDetBBox().width, 20.0f);
		cv::rectangle(oImgFrm, oDetClsLbl, cv::Scalar(0, 0, 0), -1);
		cv::putText(oImgFrm, it->getDetCls(), cv::Point(it->getDetBBox().x, (it->getDetBBox().y - 5.0f)), CV_FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	}

	//cv::imshow("object detection", oImgFrm);
	//cv::waitKey(0);
}
