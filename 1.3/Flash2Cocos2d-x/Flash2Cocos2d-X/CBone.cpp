#include "CBone.h"
#include <math.h>

cocos2d::CCArray * CCBone::sObjectPool = NULL;
int CCBone::sPoolIndex = -1;


CCBone* CCBone::create(bool _isRadian) {

	if( !sObjectPool ){
		sObjectPool = new CCArray();
		CCAssert(sObjectPool->init(), "create CCBone fail!");
	}
	sPoolIndex++;
	if ( sPoolIndex < sObjectPool->count()) {
		CCBone* _bone =(CCBone*) sObjectPool->objectAtIndex(sPoolIndex);
		return _bone;
	}

	CCBone *pBone = new CCBone();
	if (pBone && pBone->init(_isRadian)){
		sObjectPool->addObject(pBone);
		pBone->autorelease();
		return pBone;
	}
	CC_SAFE_DELETE(pBone);
	return NULL;

}

void CCBone::recycle(CCBone* _bone) {
	sPoolIndex--;
}

void CCBone::removePoolObject(){
	if( sObjectPool ){
		sObjectPool->removeAllObjects();
		CC_SAFE_DELETE( sObjectPool );
	}
}


CCBone::CCBone(){
	reset();
	mNode = NULL;
	mTweenNode = NULL;
}



CCBone::~CCBone(void){
	CC_SAFE_DELETE( mNode );
	CC_SAFE_DELETE( mTweenNode );	
}


bool CCBone::init(bool _isRadian){
	bool bRet = false;
	do{
		mIsRadian	= _isRadian;

		if( !mNode ){
			mNode		= new CCBaseNode();
		}
		if( !mTweenNode ){
			mTweenNode	= new CCTweenNode();
		}
		

		bRet		= true;
	} while (0);

	return bRet;
}


void CCBone::remove() {
	reset();
	CCBone::recycle(this);
}


void CCBone::reset(){
	mIsRadian	= false;
	mTransformX	= 0;
	mTransformY	= 0;
	mTransformSkewX = 0;
	mTransformSkewY = 0;
	mTransformRotation = 0;
	mParentX	= 0;
	mParentY	= 0;
	mParentR	= 0;
	mLockX		= 0;
	mLockY		= 0;
	mLockR		= 0;

	mDisplay		= NULL;
	mParent		= NULL;
	/*CC_SAFE_DELETE(mNode);
	CC_SAFE_DELETE(mTweenNode);*/
}

void CCBone::addBoneChild(CCBone* _child){
	_child->mParent = this;
}

void CCBone::setLockPosition(float _x, float _y, float _skewX, float _skewY) {
	mLockX = _x;
	mLockY = _y;
	mLockSkewX = _skewX;
	mLockSkewY = _skewY;
}

void CCBone::update( float dt ) {
	if( !mTweenNode->active || !mTweenNode->visible){
		if(mDisplay->isVisible()){
			mDisplay->setVisible(false);
		}
		return;
	}else{
		if( mDisplay && !mDisplay->isVisible()){
			mDisplay->setVisible(true);
		}
	}

	mTransformX = mNode->x + mLockX + mTweenNode->x;
	mTransformY = mNode->y + mLockY + mTweenNode->y;
	mTransformRotation = mNode->rotation + mTweenNode->rotation;
	mTransformSkewX = mNode->skewX + mLockSkewX + mTweenNode->skewX + mTransformRotation;
	mTransformSkewY = mNode->skewY + mLockSkewY + mTweenNode->skewY - mTransformRotation;

	//CCXLOG("%s, %f, %f", mName, mTransformX, mTransformY);
	if (mParent) {
		mParentX = mParent->getGlobalX();
		mParentY = mParent->getGlobalY();
		mParentR = mParent->getGlobalR();
		mParentSkewX = mParent->getGlobalSkewX();
		mParentSkewY = mParent->getGlobalSkewY();


		//atan2f(-_dY, _dX) 与 atan2f(_dY, _dX) 产生不一样的结果
		float _r = atan2f(-mTransformY, mTransformX) + (mParentSkewX + mParentR) * ANGLE_TO_RADIAN;
		_r = -_r;
		//CCXLOG("%s: %f, %f, %f, %f", name.c_str(), _dY, _dX, mParentR, _r);
		float _len = sqrt(mTransformX * mTransformX + mTransformY * mTransformY);
		mTransformX = _len * cos(_r) + mParentX;
		mTransformY = _len * sin(_r) + mParentY;

		//mTransformRotation += mParentR; 

		mTransformSkewX = mTransformSkewX + mParentSkewX + mParentR;
		mTransformSkewY = mTransformSkewY + mParentSkewY - mParentR;
		//CCXLOG("%s, %f, %f, %f, %f", mName, mTransformX , mParentX, mTransformY , mParentY);
	}

	updateDisplay();

}

void CCBone::updateDisplay(){
	if (mDisplay) {
		

		float _ckx = cos( CC_DEGREES_TO_RADIANS(mTransformSkewX)); 
		float _cky = cos( CC_DEGREES_TO_RADIANS(mTransformSkewY));
		float _skx = sin( CC_DEGREES_TO_RADIANS(mTransformSkewX)); 
		float _sky = sin( CC_DEGREES_TO_RADIANS(mTransformSkewY));

		if( mTweenNode->img.compare("") != 0 )
		{
			
			CCSprite *_newSprite = CCSprite::createWithSpriteFrameName(mTweenNode->img.c_str());
			_newSprite->setAnchorPoint(mDisplay->getAnchorPoint());

			mDisplay->getParent()->addChild(_newSprite);
			mDisplay->removeFromParentAndCleanup(true);
			

			mDisplay = _newSprite;
		}

		mDisplay->setPositionX	( mTransformX );
		mDisplay->setPositionY	( mTransformY );
		mDisplay->setScaleX		( mTweenNode->scaleX * _cky );
		mDisplay->setScaleY		( mTweenNode->scaleY * _ckx );
		mDisplay->setSkewX		( CC_RADIANS_TO_DEGREES(atanf(mTweenNode->scaleY * _skx / mDisplay->getScaleX())) );
		mDisplay->setSkewY		( CC_RADIANS_TO_DEGREES(atanf(mTweenNode->scaleX * _sky / mDisplay->getScaleY())) );
	
		

		if( static_cast<CCSprite*>(mDisplay) ){
			( (CCSprite*)mDisplay )->setOpacity( mTweenNode->alpha );
		}

		
	}

	//CCLOG("%s: x:%f, y:%f, scaleX:%f, scaleY:%f, alpha:%d", name.c_str(), mDisplay->getPositionX(), mDisplay->getPositionY(), mDisplay->getScaleX(), mDisplay->getScaleY(), mDisplay->getOpacity());
}

float CCBone::getGlobalX() {
	return mTransformX;
}

float CCBone::getGlobalY() {
	return mTransformY;
}

float CCBone::getGlobalR(){
	return mTransformRotation;
}

float CCBone::getGlobalSkewX(){
	return mTransformSkewX;
}

float CCBone::getGlobalSkewY(){
	return mTransformSkewY;
}