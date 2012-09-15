#include "CProcessBase.h"


CCProcessBase::CCProcessBase(void) {
    mScale = 1;
    mIsComplete = true;
    mIsPause = false;
    mCurrentFrame = 0;

	mAnimationInternal = CCDirector::sharedDirector()->getAnimationInterval();
}


CCProcessBase::~CCProcessBase(void) {
}


void CCProcessBase::remove() {
    mScale = 1;
    mIsComplete = true;
    mIsPause = false;
    mCurrentFrame = 0;
}
void CCProcessBase::pause() {
    mIsPause = true;
}


void CCProcessBase::resume() {
    mIsPause = false;
}

void CCProcessBase::stop() {
    mIsComplete = true;
    mCurrentFrame = 0;
}

void CCProcessBase::playTo(void * _to, int _durationTo, int _durationTween,  bool _loop, int _ease) {
    mIsComplete = false;
    mIsPause = false;
    mCurrentFrame = 0;
    mTotalFrames = _durationTo;
    mEase = _ease;
}

void CCProcessBase::update(float dt) {
    if (mIsComplete || mIsPause) {
        return;
    }
    mCurrentFrame += mScale * (dt/mAnimationInternal);
    if( mTotalFrames == 0 ) {
        CCLOG( "mTotalFrames is zero!" );
        return;
    }
	
    mCurrentPrecent = mCurrentFrame / mTotalFrames;
	
	if( mCurrentFrame > mTotalFrames ){
		mCurrentFrame = (int)(mCurrentFrame) % mTotalFrames;
	}
    
    updateHandler();
}

void CCProcessBase::updateHandler() {
}