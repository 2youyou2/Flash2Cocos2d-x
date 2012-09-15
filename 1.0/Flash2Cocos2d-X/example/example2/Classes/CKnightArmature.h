#pragma once

#include "CArmature.h"

class CCKnightArmature : public CCArmature,
						 public CCAnimationEventListener
{
public:
	static CCKnightArmature *create(const char* _name, const char* _animationName, const char* _plistPath, const char* _imagePath, CCSpriteBatchNode *_batchNode, int _index = 0);
public:
	CCKnightArmature();
	~CCKnightArmature(void);

	virtual bool init(const char* _name, const char* _animationName, const char* _plistPath, const char* _imagePath, CCSpriteBatchNode *_batchNode, int _index = 0);

	void randomRun();
	void update(float dt); 
	void animationHandler(const char* _aniType, const char* _aniID, const char* _frameID);
private:
	float speedX;
	float speedY;
	int face;
};

