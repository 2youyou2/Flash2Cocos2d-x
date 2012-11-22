#pragma once

#include "CProcessBase.h"
#include "CArmatureAniData.h"
#include "CTween.h"
#include "CBone.h"

class CCArmature;

/**
* 播放开始
*/
static const char* START = "start";
/**
* 播放结束
*/
static const char* COMPLETE = "complete";
/**
* 循环结束
*/
static const char* LOOP_COMPLETE = "loopComplete";
/**
* 进入事件帧
*/
static const char* IN_FRAME = "inFrame";


class CCAnimationEventListener{
public:
	virtual void animationHandler(const char* _aniType, const char* _aniID, const char* _frameID) = 0;
};


/*为骨架管理Tween*/
class CCArmatureAnimation : public CCProcessBase
{
public:
    static CCArmatureAnimation *create();
public:
    CCArmatureAnimation();
    ~CCArmatureAnimation(void);

    virtual bool init();

    /**
    * 删除，回收
    */
    void remove();
    void pause();
    void resume();
    void stop();
    /**
    * 获取 设置 当前 ArmatureAniData
    */
    CCArmatureAniData* getData();
    void setData(CCArmatureAniData* _aniData);
    /**
    * 对骨骼动画的速度进行缩放
    * @param _scale 缩放值做为系数乘到原来的动画帧值
    * @param _boneName 指定某个缩放的骨骼ID，不指定则默认为所有骨骼
    */
    void setAnimationScale(float _scale, const char* _boneName = "");
    /**
    * 为骨骼添加 Tween 实例
    */
    void addTween(CCBone* _bone);
    /**
    * 删除骨骼的 Tween 实例
    */
    void removeTween(CCBone* _bone);
    /**
    * 获取骨骼的 Tween 实例
    */
    CCTween *getTween(const char* _boneID);
    /**
    * 更新指定的 Tween 实例
    */
    void updateTween(const char *_boneID, float dt);
    /**
    * 播放动画
    * @param _to ArmatureAniData 在 ConnectionData 中的字符串索引
    * @param _toFrames 过渡到 _to 动画需要花费的帧
    * @param _listFrames _to 动画播放持续的帧
    * @param _loop 是否循环
    * @param _ease FrameNodeList 各个关键点的缓动方式，-1:SineOut，0:Line，1:SineIn，2:SineInOut
    */
    void playTo(void * _to, int _durationTo = -1, int _durationTween = -1,  bool _loop = true, int _ease = -2);

    void updateHandler();

    void updateCurrentPrecent();
public:
    /**
    * 事件回调
    */
	CC_SYNTHESIZE(CCAnimationEventListener *, mEventListener, AniEventListener);

protected:
    CCArmatureAniData	*mArmatureAniData;
    CCBoneAniData		*mBoneAniData;
	
private:
	
    // <const char*, CCTween*>
    CCDictionary	*mTweens;
	
    const char*		mAniIDNow;

};

