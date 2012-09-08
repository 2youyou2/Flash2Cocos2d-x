#pragma once
#include "CBaseNode.h"

//��������
class CCProcessBase:public CCNode
{
public:
	CCProcessBase(void);
	~CCProcessBase(void);

	/* ɾ�������� */
	void remove();
	/* ��ͣ */
	void pause();
	/* ���� */
	void resume();
	/* ֹͣ */
	void stop();
	/* ���ŵ� */
	void playTo(void * _to, int _toFrames, int _listFrames = 0, bool _loop = false, int _ease = 0);
	/* ���� */
	void update(float dt);

	virtual void updateHandler();

public:
	/* ���������ٶ�����ֵ��Ĭ��Ϊ1 */
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

