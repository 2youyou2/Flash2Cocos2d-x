#pragma once

#include "CEventFrame.h"
#include "CFrameNodeList.h"

/*单个动作动画数据*/
class CCBoneAniData : public CCObject {
public:
    static CCBoneAniData *create();
public:

    CCBoneAniData(void);
    ~CCBoneAniData(void);

    virtual bool init();

    /**
    * 添加骨骼的动画数据
    * @param _frameNodeList
    * @param _boneName
    */
    void addAnimation(CCFrameNodeList* _frameNodeList, const char* _boneName);
    /**
    * 获取骨骼动画数据
    * @param _boneName
    * @return 返回骨骼动画数据
    */
    CCFrameNodeList* getAnimation(const char* _boneName);
    //返回的字符很多，直接使用CCLOG打印可能会出错，原因为分配的内存太小。
    std::string toString();

public:

    /**
    * 事件帧列表
    */
    CC_SYNTHESIZE_RETAIN(CCArray *, mEventList, EventList);

private:
    CCDictionary *mFrameNodeListDic;

public:
	/**
    * 动画数据实际持续的帧数
    */
	CC_SYNTHESIZE_PASS_BY_REF(int, mDuration, Duration);
	CC_SYNTHESIZE_PASS_BY_REF(int, mDurationTo, DurationTo);
	CC_SYNTHESIZE_PASS_BY_REF(int, mDurationTween, DurationTween);
	CC_SYNTHESIZE_PASS_BY_REF(bool, mLoop, Loop);
	CC_SYNTHESIZE_PASS_BY_REF(float, mTweenEasing, TweenEasing);
};

