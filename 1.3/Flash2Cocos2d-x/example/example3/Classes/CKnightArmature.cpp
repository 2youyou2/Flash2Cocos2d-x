#include "CKnightArmature.h"

CCKnightArmature *CCKnightArmature::create(std::string _name, std::string _animationName, std::string _plistPath, std::string _imagePath, CCSpriteBatchNode *_batchNodeL, int _index){
	CCKnightArmature *pKnightArmature = new CCKnightArmature();
	if (pKnightArmature && pKnightArmature->init(_name, _animationName, _plistPath, _imagePath, _batchNodeL, _index)) {
		pKnightArmature->autorelease();
		return pKnightArmature;
	}
	CC_SAFE_DELETE(pKnightArmature);
	return NULL;
}

CCKnightArmature::CCKnightArmature(){	
}


CCKnightArmature::~CCKnightArmature(void){
}

bool CCKnightArmature::init(std::string _name, std::string _animationName, std::string _plistPath, std::string _imagePath, CCSpriteBatchNode *_batchNodeL, int _index){
	bool bRet = false;
	do {

		CC_BREAK_IF(! CCArmature::init(_name, _animationName, _plistPath, _imagePath, _batchNodeL, _index) );
		face = CCRANDOM_0_1() > 0.5?1: -1;

		mDisplay->setPosition(ccp( -100 * CCRANDOM_0_1() - 100, 70 + CCRANDOM_0_1() * 300 ));
		mDisplay->setScaleX(face);

		mAnimation->setAniEventListener(this);
		//CCXLOG("init success");
		bRet = true;
	} while (0);

	return bRet;
	
}

void CCKnightArmature::randomRun() {
	float _scale = CCRANDOM_0_1() * 0.3 + 0.7;
	speedX = _scale * 3;
	speedY = CCRANDOM_0_1() * 1.5;
	//animation->setAnimationScale(_scale);
	mAnimation->playTo((void *)"run", 8, 40, true);
}

void CCKnightArmature::update(float dt){
	CCArmature::update(dt);

	mDisplay->setPositionX( mDisplay->getPositionX() + speedX * face );
	mDisplay->setPositionY( mDisplay->getPositionY() + speedY );
	
	if (face > 0) {
		if (mDisplay->getPositionX() > CCDirector::sharedDirector()->getWinSize().width + 100) {
			mDisplay->setPositionX(-100);
		}
	}else {
		if (mDisplay->getPositionX() < 0 - 100) {
			mDisplay->setPositionX( CCDirector::sharedDirector()->getWinSize().width + 100 );
		}
	}

	if (mDisplay->getPositionY() < 50) {
		mDisplay->setPositionY(50);
		speedY = CCRANDOM_0_1() * 2;
	}else if (mDisplay->getPositionY() > CCDirector::sharedDirector()->getWinSize().height) {
		speedY = -CCRANDOM_0_1() * 2;
	}
}

void CCKnightArmature::animationHandler(std::string _aniType, std::string _aniID, std::string _frameID){
	
	//CCLOG("%s, %s, %s", _aniType.c_str(),_aniID.c_str(),_frameID.c_str());
	if( _aniType.compare(LOOP_COMPLETE)==0 ){
		if( _aniID.compare("run")==0 ){
			if (CCRANDOM_0_1() > 0.90) {
				speedX = 0;
				speedY = 0;
				if (CCRANDOM_0_1()>0.5) {
					mAnimation->playTo((void *)"stand", 4, 20, true, 2);
				}else {
					mAnimation->playTo((void *)"attack", 4, 50, false, 1);
				}
			}
		}else if(_aniID.compare("stand")==0){
			if (CCRANDOM_0_1() > 0.60) {
				randomRun();
			}
		}
		
	}else if(_aniType.compare(COMPLETE)==0){
		randomRun();
	}
}