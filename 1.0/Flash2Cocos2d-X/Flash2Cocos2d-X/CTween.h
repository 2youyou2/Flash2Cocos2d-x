#pragma once

#include "CProcessBase.h"
#include "CTweenNode.h"
#include "CFrameNodeList.h"
#include <math.h>

using namespace cocos2d;

#define  HALF_PI  (M_PI * 0.5f)

//�����������ƣ�����ؼ�֡���ݲ���Ⱦ������
class CCTween : public CCProcessBase
{
public:
	/*���� Tween ʵ����Tween ��Ϊ���õ�ʵ�����ö���ع���������*/
	static CCTween* create();

	static void removePoolObject();
private:
	static void recycle(CCTween* _tween);

public:
	CCTween(void);
	~CCTween(void);

	virtual bool init();
	
	/*ɾ��������*/
	void remove();
	/*���� mNode*/
	void setNode(CCTweenNode* _node);
	/**
	* ���Ų��䶯��
	* @param _to FrameNodeList ʵ��
	* @param _toFrames ���ɵ� FrameNodeList ��Ҫ���ѵ�֡
	* @param _listFrames FrameNodeList ���ų�����֡
	* @param _loop �Ƿ�ѭ��
	* @param _ease FrameNodeList �����ؼ���Ļ�����ʽ��-1:SineOut��0:Line��1:SineIn��2:SineInOut
	*/
	void playTo(void * _to, int _toFrames, int _listFrames = 0, bool _loop = false, int _ease = 0);
	/*���� mNode*/
	void updateHandler();
	

private:
	
	void updateCurrentPrecent();

private:
	/* Bone.tweenNode ������ */
	CCTweenNode* mNode;

	CCTweenNode* mFrom;

	CCTweenNode* mTo;

	CCFrameNodeList* mTweenList;


private:	
	static CCArray	*sObjectPool;

	static int		sPoolIndex;
};

