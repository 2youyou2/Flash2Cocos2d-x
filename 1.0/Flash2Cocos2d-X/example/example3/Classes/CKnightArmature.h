#pragma once

#include "CArmature.h"

class CCKnightArmature : public CCArmature,
						 public CCAnimationEventListener
{
public:
	static CCKnightArmature *create(std::string _name, std::string _animationName, std::string _plistPath, std::string _imagePath, CCSpriteBatchNode *_batchNode, int _index = 0);
public:
	CCKnightArmature();
	~CCKnightArmature(void);

	virtual bool init(std::string _name, std::string _animationName, std::string _plistPath, std::string _imagePath, CCSpriteBatchNode *_batchNode, int _index = 0);

	void randomRun();
	void update(float dt); 
	void animationHandler(std::string _aniType, std::string _aniID, std::string _frameID);
private:
	float speedX;
	float speedY;
	int face;
};

