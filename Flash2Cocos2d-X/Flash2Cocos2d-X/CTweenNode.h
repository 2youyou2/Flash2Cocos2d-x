#pragma once

#include "CFrameNode.h"


//������Ƶ�
class CCTweenNode : public CCFrameNode
{
public:
	static CCTweenNode *create();
public:
	CCTweenNode();
	~CCTweenNode(void);

	/* �� TweenNode �������ؼ�����ȡֵ */
	void betweenValue(CCFrameNode* _from, CCFrameNode* _to);
	/**
	* �� TweenNode �������ؼ�����ȡֵ
	* param _k 0~1 ��ֵ������ֵΪ 0��TweenNode ȡ from ��ֵ������ֵΪ 1��TweenNode ȡ to ��ֵ
	*/
	void tweenTo(float _k);

	void remove();

protected:
	void reset();
protected:
	float sR;
	float sX;
	float sY;
	float sSX;
	float sSY;
	float sA;
	float dR;
	float dX;
	float dY;
	float dSX;
	float dSY;
	float dA;
};

