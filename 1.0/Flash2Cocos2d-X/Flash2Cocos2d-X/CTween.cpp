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
	mNode->rotation = (int)(mNode->rotation) % 360;
	mFrom->copy(mNode);
	mTweenList = (CCFrameNodeList*)_to;
	if (mTweenList->getLength() == 1) {
		//普通过渡
		mLoop = -4;
		mTo->copy(mTweenList->getFrame(0));
	}else {
		if (_loop) {
			//循环过渡
			mLoop = -2;
			mNoScaleListFrames = mTweenList->getFrame();
		}else {
			//列表过渡
			mLoop = -3;
			mNoScaleListFrames = mTweenList->getFrame() - 1;
		}
		mListFrames = _listFrames * mTweenList->getScale();
		if (_loop && mTweenList->getDelay() != 0) {
			float _playedFrames = mNoScaleListFrames * (1 - mTweenList->getDelay());
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
		case -1:
			mCurrentPrecent = 1;
			mIsComplete = true;
			break;
		case -2:
			//循环开始
			mLoop = 0;
			mTotalFrames = mListFrames;
			if (mTweenList->getDelay() != 0) {
				mCurrentFrame = (1 - mTweenList->getDelay()) * mTotalFrames;
				mCurrentPrecent += mCurrentFrame / mTotalFrames;
			}
			mCurrentPrecent = (int)(mCurrentPrecent) % 1;
			mListEndFrame = 0;
			break;
		case -3:
			//列表过渡
			mLoop = -1;
			mCurrentPrecent = (mCurrentPrecent - 1) * mTotalFrames / mListFrames;
			if (mCurrentPrecent >= 1) {
				//
			}else {
				mTotalFrames = mListFrames;
				mCurrentPrecent = (int)(mCurrentPrecent) % 1;
				mListEndFrame = 0;
				break;
			}
		case -4:
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
	float _playedFrames = mNoScaleListFrames * mCurrentPrecent;

	if (_playedFrames <= mListEndFrame-mBetweenFrame || _playedFrames > mListEndFrame) {
		mListEndFrame = 0;
		mToFrameID = 0;
		int _prevFrameID;
		
		do {
			mBetweenFrame = mTweenList->getFrame(mToFrameID)->frame;
			mListEndFrame += mBetweenFrame;
			_prevFrameID = mToFrameID;
			if (++mToFrameID >= mTweenList->getLength()) {
				mToFrameID = 0;
			}
		}while (_playedFrames >= mListEndFrame);

		mFrom->copy(mTweenList->getFrame(_prevFrameID));
		mTo->copy(mTweenList->getFrame(mToFrameID));
		mNode->betweenValue(mFrom, mTo);
	}

	mCurrentPrecent = 1 - (mListEndFrame - _playedFrames) / mBetweenFrame;
	if (mEase == 2) {
		mCurrentPrecent = 0.5 * (1 - cos(mCurrentPrecent * M_PI ));
	}else if (mEase != 0) {
		mCurrentPrecent = mEase > 0?sin(mCurrentPrecent * HALF_PI):(1 - cos(mCurrentPrecent * HALF_PI));
	}
}