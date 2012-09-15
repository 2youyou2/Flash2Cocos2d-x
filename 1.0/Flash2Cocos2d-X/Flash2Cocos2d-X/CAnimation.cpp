#include "CAnimation.h"
#include "CEventFrame.h"
using namespace std;

CCARmatureAnimation *CCARmatureAnimation::create() {
    CCARmatureAnimation *pARmatureAnimation = new CCARmatureAnimation();
    if (pARmatureAnimation && pARmatureAnimation->init()) {
        pARmatureAnimation->autorelease();
        return pARmatureAnimation;
    }
    CC_SAFE_DELETE(pARmatureAnimation);
    return NULL;
}

CCARmatureAnimation::CCARmatureAnimation() {
    mEventListener		= NULL;
    mArmatureAniData	= NULL;
    mBoneAniData		= NULL;
}

CCARmatureAnimation::~CCARmatureAnimation(void) {
    CC_SAFE_DELETE( mTweens );
}

bool CCARmatureAnimation::init() {
    bool bRet = false;
    do {

		mAniIDNow = "";

        mTweens		= new CCDictionary();
        CCAssert(mTweens, "create CCARmatureAnimation::mTweens fail!");

        bRet = true;
    } while (0);

    return bRet;
}

void CCARmatureAnimation::remove() {
    stop();
    CCProcessBase::remove();

    CCDictElement *_element = NULL;
    CCDICT_FOREACH(mTweens, _element) {
        CCTween* _tween = (CCTween*)_element->getObject();
        _tween->remove();
    }
	mArmatureAniData = NULL;
	mBoneAniData = NULL;
    mAniIDNow = "";

	mTweens->removeAllObjects();
}

void CCARmatureAnimation:: pause() {
    CCProcessBase::pause();

    CCDictElement *_element = NULL;
    CCDICT_FOREACH(mTweens, _element) {
        CCTween* _tween = (CCTween*)_element->getObject();
        _tween->pause();
    }
}

void CCARmatureAnimation::resume() {
    CCProcessBase::resume();

    CCDictElement *_element = NULL;
    CCDICT_FOREACH(mTweens, _element) {
        CCTween* _tween = (CCTween*)_element->getObject();
        _tween->resume();
    }
}

void CCARmatureAnimation::stop() {
    CCProcessBase::stop();

    CCDictElement *_element = NULL;
    CCDICT_FOREACH(mTweens, _element) {
        CCTween* _tween = (CCTween*)_element->getObject();
        _tween->stop();
    }
}

CCArmatureAniData* CCARmatureAnimation::getData() {
    return mArmatureAniData;
}

void CCARmatureAnimation::setData(CCArmatureAniData* _aniData) {
    remove();
    mArmatureAniData = _aniData;
}

void CCARmatureAnimation::setAnimationScale(float _scale, const char* _boneName ) {
    CCTween* _tween;
    if (_boneName != "") {
        _tween = (CCTween*)mTweens->objectForKey(_boneName);
        if (_tween) {
            _tween->setScale(_scale);
        }
    } else {
        mScale = _scale;

        CCDictElement *_element = NULL;
        CCDICT_FOREACH(mTweens, _element) {
            CCTween* _tween = (CCTween*)_element->getObject();
            _tween->setScale(_scale);
        }
    }
}

void CCARmatureAnimation::addTween(CCBone* _bone) {
    const char* _boneID = _bone->getName().c_str();
    CCTween		*_tween = (CCTween*)mTweens->objectForKey(_boneID);
    if (!_tween) {
        _tween = CCTween::create();
        mTweens->setObject(_tween, _boneID);
    }
    _tween->setNode(_bone->getTweenNode());
}

void CCARmatureAnimation::removeTween(CCBone* _bone) {
    const char* _boneID = _bone->getName().c_str();
    CCTween		*_tween = (CCTween*)mTweens->objectForKey(_boneID);
    if (_tween) {
        _tween->remove();
    }
}

CCTween* CCARmatureAnimation::getTween(const char* _boneID) {
    return (CCTween*)mTweens->objectForKey(_boneID);
}

void CCARmatureAnimation::updateTween(const std::string *_boneID, float dt) {
	CCTween* _tween = (CCTween*)mTweens->objectForKey(*_boneID);
    if (_tween) {
        _tween->update( dt );
    }
}

void CCARmatureAnimation::playTo(void * _to, int _toFrames, int _listFrames, bool _loop, int _ease) {
    //CCXLOG("Enter CCARmatureAnimation::playTo");
    if (!mArmatureAniData) {
        return;
    }
    mBoneAniData = mArmatureAniData->getAnimation( (const char *)_to );
    if (!mBoneAniData) {
        return;
    }
    CCProcessBase::playTo(_to, _toFrames, _listFrames, _loop, _ease);
    mAniIDNow = (const char *)_to;
    CCFrameNodeList *_eachA;
    CCDictElement	*_element = NULL;
    CCTween			*_tween;

    CCDICT_FOREACH(mTweens, _element) {
        _tween = (CCTween*)_element->getObject();
        _eachA = mBoneAniData->getAnimation(_element->getStrKey());
        if (_eachA) {
            _tween->playTo(_eachA, _toFrames, _listFrames, _loop, _ease);
        }
    }

    mNoScaleListFrames = mBoneAniData->getFrame();

    if (mNoScaleListFrames == 1) {
        //普通过渡
        mLoop = -4;
    } else {
        if (_loop) {
            //循环过渡
            mLoop = -2;
        } else {
            //列表过渡
            mLoop = -3;
            mNoScaleListFrames --;
        }
        mListFrames = _listFrames;
    }
    //CCXLOG("Finish CCARmatureAnimation::playTo");
}

void CCARmatureAnimation::updateHandler() {
    if (mCurrentPrecent >= 1) {
        switch(mLoop) {
        case -3:
            //列表过渡
            mLoop = -1;
            mCurrentPrecent = (mCurrentPrecent - 1) * mTotalFrames / mListFrames;
            if (mCurrentPrecent >= 1) {
                //
            } else {
                mCurrentPrecent = (int)(mCurrentPrecent) % 1;
                mTotalFrames = mListFrames;
                mListEndFrame = 0;
                if (mEventListener!=NULL) {
                    mEventListener->animationHandler(START, mAniIDNow, "");
                }
                break;
            }
        case -1:
        case -4:
            mCurrentPrecent = 1;
            mIsComplete = true;
            if (mEventListener!=NULL) {
                mEventListener->animationHandler(COMPLETE, mAniIDNow, "");
            }
            break;
        case -2:
            //循环开始
            mLoop = 0;
            mCurrentPrecent = 0;
            mTotalFrames = mListFrames;
            mListEndFrame = 0;
            if (mEventListener!=NULL) {
                mEventListener->animationHandler(START, mAniIDNow, "");
            }
            break;
        default:
            //继续循环
            mLoop += int(mCurrentPrecent);
            mCurrentPrecent = (int)(mCurrentPrecent) % 1;
            if (mEventListener != NULL) {
                mEventListener->animationHandler(LOOP_COMPLETE, mAniIDNow, "");
            }
            break;
        }
    }

    if (mLoop >= -1 && mBoneAniData->getEventList()->count() > 0) {
        //多关键帧动画过程
        updateCurrentPrecent();
    }
}

void CCARmatureAnimation::updateCurrentPrecent() {
    float _playedFrames = mNoScaleListFrames * mCurrentPrecent;
    if (_playedFrames <= mListEndFrame-mBetweenFrame || _playedFrames > mListEndFrame) {
        mToFrameID = 0;
        mListEndFrame = 0;

        CCEventFrame *_eventFrame = (CCEventFrame*)(mBoneAniData->getEventList()->objectAtIndex(mToFrameID));
        int _prevFrameID;
        do {
            mBetweenFrame = _eventFrame->getFrame();
            mListEndFrame += mBetweenFrame;
            _prevFrameID = mToFrameID;
            if (++mToFrameID >= mBoneAniData->getEventList()->count()) {
                mToFrameID = 0;
            }
        } while (_playedFrames >= mListEndFrame);

        if (mEventListener != NULL) {
            mEventListener->animationHandler(IN_FRAME, mAniIDNow, _eventFrame->getName());
        }
    }

    mCurrentPrecent = 1 - (mListEndFrame - _playedFrames) / mBetweenFrame;
}