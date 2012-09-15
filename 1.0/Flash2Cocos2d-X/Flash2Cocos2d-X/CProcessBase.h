#pragma once
#include "CBaseNode.h"

//动画基类
class CCProcessBase:public CCNode
{
public:
	CCProcessBase(void);
	~CCProcessBase(void);

	/* 删除，回收 */
	void remove();
	/* 暂停 */
	void pause();
	/* 继续 */
	void resume();
	/* 停止 */
	void stop();
	/* 播放到 */
	void playTo(void * _to, int _toFrames, int _listFrames = 0, bool _loop = false, int _ease = 0);
	/* 更新 */
	void update(float dt);

	virtual void updateHandler();

public:
	/* 动画播放速度缩放值，默认为1 */
	CC_SYNTHESIZE_PASS_BY_REF(float,	mScale,				Scale);
	CC_SYNTHESIZE_PASS_BY_REF(bool,		mIsPause,			IsPause);
	CC_SYNTHESIZE_PASS_BY_REF(bool,		mIsComplete,		IsComplete);
	
	CC_SYNTHESIZE_PASS_BY_REF(int,		mNoScaleListFrames, NoScaleListFrames);
	CC_SYNTHESIZE_PASS_BY_REF(float,	mCurrentPrecent,	CurrentPrecent);
	CC_SYNTHESIZE_PASS_BY_REF(int,		mLoop,				Loop);
	CC_SYNTHESIZE_PASS_BY_REF(int,		mEase,				Ease);
	CC_SYNTHESIZE_PASS_BY_REF(float,	mAnimationInternal, AnimationInternal);
	
protected:
	int	mBetweenFrame;
	int	mListEndFrame;
	int	mToFrameID;
	int mTotalFrames;
	int mListFrames;
	float mCurrentFrame;

};

