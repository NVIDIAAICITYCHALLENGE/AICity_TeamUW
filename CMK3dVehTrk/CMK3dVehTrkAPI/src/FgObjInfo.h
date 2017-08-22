#pragma once


#include "cvut.h"


class CFgObjInfo
{
public:
	CFgObjInfo();
	~CFgObjInfo();

	inline CvBox2D getBox2D() { return m_ObjBox; }
	inline CvPoint2D32f getBoxCenter() { return m_ObjBox.center; }
	inline float getBoxAngle() { return m_ObjBox.angle; }
	inline float getBoxWidth() { return m_ObjBox.size.width; }
	inline float getBoxHeight() { return m_ObjBox.size.height; }

	inline CvRect getRect() { return m_ObjRect; }
	inline int getRectLeft() { return m_ObjRect.x; }
	inline int getRectTop() { return m_ObjRect.y; }
	inline int getRectWidth() { return m_ObjRect.width; }
	inline int getRectHeight() { return m_ObjRect.height; }
	inline int getArea() { return m_ObjArea; }

	inline void setBox2D(CvBox2D box) { m_ObjBox = box; }
	inline void setBoxCenter(CvPoint2D32f pt) { m_ObjBox.center = pt; } 
	inline void setBoxAngle(float angle) { m_ObjBox.angle = angle; }
	inline void setBoxWidth(float width) { m_ObjBox.size.width = width; }
	inline void setBoxHeight(float height) { m_ObjBox.size.height = height; }
	inline void setRect(CvRect rect) { m_ObjRect = rect; }
	inline void setArea(int area) { m_ObjArea = area; }

	void shift(CvPoint2D32f shift);
	void updateRect();

private:
	CvBox2D m_ObjBox;
	CvRect m_ObjRect;

	int m_ObjArea;
};

