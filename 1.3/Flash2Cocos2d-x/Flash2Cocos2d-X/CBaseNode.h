#pragma once

#include "cocos2d.h"
#include "CXLog.h"

using namespace cocos2d;

/* 基本帧数据 */
class CCBaseNode : public CCObject
{
public:
	static CCBaseNode *create();
public:
	CCBaseNode();
	~CCBaseNode(void);

	virtual bool init( float _x = 0, float _y = 0, float _rotation = 0, float _scaleX = 1, float _scaleY = 1,  float _z = 0, float _skewX = 0, float _skewY = 0);

	void copy( CCBaseNode* _fV );

protected:
	void reset();

public:
	float rotation;
	float x;
	float y;
	float z;
	float scaleX;
	float scaleY;
	float alpha;
	float skewX;
	float skewY;
	float tweenRotate;
	bool  visible;
};

