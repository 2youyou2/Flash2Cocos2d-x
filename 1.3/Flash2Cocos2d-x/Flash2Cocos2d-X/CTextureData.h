#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class CCTexutreData : public CCObject
{
public :
	inline static CCTexutreData *create(){
		CCTexutreData *pTexutreData = new CCTexutreData();
		if (pTexutreData && pTexutreData->init()){
			pTexutreData->autorelease();
			return pTexutreData;
		}
		CC_SAFE_DELETE(pTexutreData);
		return NULL;
	}; 
public:

	inline bool init(){
		bool bRet = false;
		do{
			pivotX	= 0;
			pivotY	= 0;
			width	= 0;
			height	= 0;

			bRet = true;
		} while (0);

		return bRet;
	}

public:
	float pivotX;
	float pivotY;
	float width;
	float height;
	std::string name;
};