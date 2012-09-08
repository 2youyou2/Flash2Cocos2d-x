#pragma once

#include "cocos2d.h"
#include "CXLog.h"

using namespace cocos2d;

/* 基本帧数据 */
class CCBaseNode : public CCObject
{
public:
	static CCBaseNode *create( float _x = 0, float _y = 0, float _rotation = 0 );
public:
	CCBaseNode();
	~CCBaseNode(void);

	virtual bool init( float _x = 0, float _y = 0, float _rotation = 0 );

	void copy( CCBaseNode* _fV );

protected:
	void reset();

public:
	float rotation;
	float x;
	float y;
};

