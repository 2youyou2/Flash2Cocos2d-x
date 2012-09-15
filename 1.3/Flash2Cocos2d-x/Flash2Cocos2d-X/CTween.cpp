#include "CTween.h"

cocos2d::CCArray * CCTween::sObjectPool = NULL;
int CCTween::sPoolIndex = -1;

CCTween* CCTween::create() {

	if( !sObjectPool ){
		sObjectPool = new CCArray();
		CCAssert(sObjectPool->init(), "create CCTween fail!");
	}

	sPoolIndex ++;

	if ( sPoolIndex < sObjectPool->count()) {
		CCTween* _tween =(CCTween*) sObjectPool->objectAtIndex(sPoolIndex);
		return _tween;
	}

	CCTween *pTween = new CCTween();
	if (pTween && pTween->init()){
		pTween->autorelease();
		sObjectPool->addObject(pTween);
		return pTween;
	}
	CC_SAFE_DELETE(pTween);
	return NULL;

}

void CCTween::removePoolObject(){
	if( sObjectPool ){
		sObjectPool->removeAllObjects();
		CC_SAFE_DELETE( sObjectPool );
	}
}


void CCTween::recycle(CCTween* _tween) {
	sPoolIndex --;
}

CCTween::CCTween(){
	mFrom		= NULL;
	mTo			= NULL;
	mNode		= NULL;
	mTweenList	= NULL;
}


CCTween::~CCTween(void){
	CC_SAFE_DELETE( mFrom );
	CC_SAFE_DELETE( mTo );
}


bool CCTween::init(){
	bool bRet = false;
	do {

		if( !mFrom ){
			mFrom = new CCTweenNode();
		}
		if( !mTo ){
			mTo = new CCTweenNode();
		}
		
		bRet = true;
	} while (0);

	return bRet;
}



void CCTween::remove(){
	stop();
	CCProcessBase::remove();
	mNode = NULL;
	mTweenList = NULL;
	mFrom->remove();
	mTo->remove();
	recycle(this);
}

void CCTween::setNode(CCTweenNode* _node) {
	mNode = _node;
}

void CCTween::playTo(void * _to, int _toFrames, int _listFrames, bool _loop, int _ease){
	CCProcessBase::playTo(_to, _toFrames, _listFrames, _loop, _ease);

	setActive(true);

	mNode->rotation = (int)(mNode->rotation) % 360;
	mFrom->copy(mNode);
	mTweenList = (CCFrameNodeList*)_to;

	if (mTweenList->getLength() == 1) {
		//普通过渡
		mLoop = SINGLE;
		mTo->copy(mTweenList->getFrame(0));
	}else {
		if (_loop) {
			//循环过渡
			mLoop = LIST_LOOP_START;
			mDuration = mTweenList->getFrame();
		}else {
			//列表过渡
			mLoop = LIST_START;
			mDuration = mTweenList->getFrame() - 1;
		}
		mDurationTween = _listFrames * mTweenList->getScale();
		if (_loop && mTweenList->getDelay() != 0) {
			float _playedFrames = mDuration * (1 - mTweenList->getDelay());
			int _prevFrameID = 0;
			int _toFrameID = 0;
			int _listEndFrame = 0;
			int _betweenFrame = 0;
			do {
				_betweenFrame = mTweenList->getFrame(_toFrameID)->frame;
				_listEndFrame += _betweenFrame;
				_prevFrameID = _toFrameID;
				if (++_toFrameID >= mTweenList->getLength()) {
					_toFrameID = 0;
				}
			}while (_playedFrames >= _listEndFrame);

			mTo->betweenValue(mTweenList->getFrame(_prevFrameID), mTweenList->getFrame(_toFrameID));

			float _currentPrecent = 1 - (_listEndFrame - _playedFrames) / _betweenFrame;
			if (mEase == 2) {
				_currentPrecent = 0.5 * (1 - cos(_currentPrecent * M_PI ));
			}else if (mEase != 0) {
				_currentPrecent = mEase > 0?sin(_currentPrecent * HALF_PI):(1 - cos(_currentPrecent * HALF_PI));
			}
			mTo->tweenTo(_currentPrecent);
		}else {
			mTo->copy(mTweenList->getFrame(0));
		}
	}
	mNode->betweenValue(mFrom, mTo);
}


void CCTween::updateHandler() {
	if (mCurrentPrecent >= 1) {
		switch(mLoop) {
		case LIST:
			mCurrentPrecent = 1;
			mIsComplete = true;
			break;
		case LIST_LOOP_START:
			//循环开始
			mLoop = 0;
			mTotalFrames = mDurationTween;
			if (mTweenList->getDelay() != 0) {
				mCurrentFrame = (1 - mTweenList->getDelay()) * mTotalFrames;
				mCurrentPrecent += mCurrentFrame / mTotalFrames;
			}
			mCurrentPrecent = (int)(mCurrentPrecent) % 1;
			mTotalDuration = 0;
			break;
		case LIST_START:
			//列表过渡
			mLoop = -1;
			mCurrentFrame = mCurrentPrecent * mTotalFrames;
			mCurrentPrecent = mCurrentFrame / mDurationTween;
			if (mCurrentPrecent >= 1) {
				//
			}else {
				mTotalFrames = mDurationTween;
				//mCurrentPrecent = (int)(mCurrentPrecent) % 1;
				mTotalDuration = 0;
				break;
			}
		case SINGLE:
			mCurrentPrecent = 1;
			mIsComplete = true;
			break;
		
		default:
			//继续循环
			mLoop += int(mCurrentPrecent);
			mCurrentPrecent = (int)(mCurrentPrecent) % 1;
			break;
		}
	}else if (mLoop < -1) {
		mCurrentPrecent = sin(mCurrentPrecent * HALF_PI);
	}

	if (mLoop >= -1) {
		//多关键帧动画过程
		updateCurrentPrecent();
	}
	mNode->tweenTo(mCurrentPrecent);
}

void CCTween::updateCurrentPrecent(){
	float _playedFrames = mDuration * mCurrentPrecent;

	if (_playedFrames <= mTotalDuration-mBetweenDuration || _playedFrames > mTotalDuration) {
		mTotalDuration = 0;
		mToFrameID = 0;
		int _prevFrameID;
		
		do {
			mBetweenDuration = mTweenList->getFrame(mToFrameID)->frame;
			mTotalDuration += mBetweenDuration;
			_prevFrameID = mToFrameID;
			if (++mToFrameID >= mTweenList->getLength()) {
				mToFrameID = 0;
			}
		}while (_playedFrames >= mTotalDuration);

		mFrom->copy(mTweenList->getFrame(_prevFrameID));
		mTo->copy(mTweenList->getFrame(mToFrameID));
		mNode->betweenValue(mFrom, mTo);
	}

	mCurrentPrecent = 1 - (mTotalDuration - _playedFrames) / mBetweenDuration;
	if (mEase == 2) {
		mCurrentPrecent = 0.5 * (1 - cos(mCurrentPrecent * M_PI ));
	}else if (mEase != 0) {
		mCurrentPrecent = mEase > 0?sin(mCurrentPrecent * HALF_PI):(1 - cos(mCurrentPrecent * HALF_PI));
	}
}

void CCTween::setActive(bool _active){
	mActive = _active;
	mNode->visible = _active;
}

bool CCTween::getActive(){
	return mActive;
}