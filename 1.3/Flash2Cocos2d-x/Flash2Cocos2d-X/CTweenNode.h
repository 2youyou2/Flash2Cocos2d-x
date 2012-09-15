#pragma once

#include "CFrameNode.h"


//补间控制点
class CCTweenNode : public CCFrameNode
{
public:
	static CCTweenNode *create();
public:
	CCTweenNode();
	~CCTweenNode(void);

	/* 让 TweenNode 从两个关键点中取值 */
	void betweenValue(CCFrameNode* _from, CCFrameNode* _to);
	/**
	* 让 TweenNode 从两个关键点中取值
	* param _k 0~1 的值，当此值为 0，TweenNode 取 from 点值，当此值为 1，TweenNode 取 to 点值
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

