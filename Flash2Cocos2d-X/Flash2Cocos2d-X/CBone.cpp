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
	mParentX		= 0;
	mParentY		= 0;
	mParentR		= 0;
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

void CCBone::setLockPosition(float _x, float _y, float _r) {
	mLockX = _x;
	mLockY = _y;
	mLockR = _r;
}

void CCBone::update( float dt ) {
	if (mParent) {
		mParentX = mParent->getGlobalX();
		mParentY = mParent->getGlobalY();
		mParentR = mParent->getGlobalR();

		float _dX = mLockX + mNode->x + mTweenNode->x;
		float _dY = mLockY + mNode->y + mTweenNode->y;
		

		//atan2f(-_dY, _dX) 与 atan2f(_dY, _dX) 产生不一样的结果
		float _r = atan2f(-_dY, _dX) + mParentR * ANGLE_TO_RADIAN;
		_r = -_r;
		//CCXLOG("%s: %f, %f, %f, %f", name.c_str(), _dY, _dX, mParentR, _r);
		float _len = sqrt(_dX * _dX + _dY * _dY);
		mTransformX = _len * cos(_r);
		mTransformY = _len * sin(_r);
	}else {
		mTransformX = mNode->x + mTweenNode->x;
		mTransformY = mNode->y + mTweenNode->y;
		//CCXLOG("%s: %f, %f, %f", name.c_str(), mLockY, mNode->y, mTweenNode->y);
		//CCXLOG("%s, %f, %f, %f", name.c_str(), mTransformX, mTransformY, mNode->rotation + mTweenNode->rotation + mParentR + mLockR);
	}

	updateDisplay();

}

void CCBone::updateDisplay(){
	if (mDisplay) {
		mDisplay->setPositionX( mTransformX + mParentX );
		mDisplay->setPositionY( mTransformY + mParentY );
		
		if (mIsRadian) {
			mDisplay->setRotation( (mNode->rotation + mTweenNode->rotation + mParentR + mLockR) * ANGLE_TO_RADIAN );
		}else {
			mDisplay->setRotation( mNode->rotation + mTweenNode->rotation + mParentR + mLockR );
		}
		
		if (isnan(mTweenNode->scaleX)) {
		}else {
			mDisplay->setScaleX( mTweenNode->scaleX );
		}
		if (isnan(mTweenNode->scaleY)) {
		}else {
			mDisplay->setScaleY( mTweenNode->scaleY );
		}
		if (isnan(mTweenNode->alpha)) {
		}else {
			if (mTweenNode->alpha) {
				mDisplay->setVisible( true );
				if( static_cast<CCSprite*>(mDisplay) ){
					( (CCSprite*)mDisplay )->setOpacity( mTweenNode->alpha );
				}
				
			}else {
				mDisplay->setVisible( false );
			}
		}
	}

	//CCLOG("%s: x:%f, y:%f, scaleX:%f, scaleY:%f, alpha:%d", name.c_str(), mDisplay->getPositionX(), mDisplay->getPositionY(), mDisplay->getScaleX(), mDisplay->getScaleY(), mDisplay->getOpacity());
}

float CCBone::getGlobalX() {
	return mTransformX + mParentX;
}

float CCBone::getGlobalY() {
	return mTransformY + mParentY;
}

float CCBone::getGlobalR(){
	return mNode->rotation + mTweenNode->rotation + mParentR + mLockR;
}
