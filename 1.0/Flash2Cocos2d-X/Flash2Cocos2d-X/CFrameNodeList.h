#pragma once

#include "CFrameNode.h"
#include <vector>

/* �ؼ�֡�����б� */
class CCFrameNodeList : public CCObject
{
public:
	static CCFrameNodeList *create(float _delay = 0, float _scale = 1);
public:
	/**
	* @param _delay ������ʱ�ٷֱ�
	* @param _scale �������Űٷֱ�
	*/
	CCFrameNodeList(float _delay = 0, float _scale = 1);
	~CCFrameNodeList(void);

	virtual bool init();

	/**
	* ��ӹؼ�֡
	* @param _value �ؼ�֡����
	*/
	void			addFrame(CCFrameNode* _value);
	/**
	* ��ȡ�ؼ�֡
	* @param _id index
	* @return ���عؼ�֡����
	*/
	CCFrameNode		*getFrame(int _id);

	CCArray			*getFrames();

	std::string		toString();

public:
	/*������ʱ�ٷֱ�*/
	CC_SYNTHESIZE_PASS_BY_REF(float, mDelay,  Delay);

	/*�������Űٷֱ�*/
	CC_SYNTHESIZE_PASS_BY_REF(float, mScale,  Scale);

	/*ʵ�ʳ���֡��*/
	CC_SYNTHESIZE_PASS_BY_REF(int,   mFrame,  Frame);

	/*�б���*/
	CC_SYNTHESIZE_PASS_BY_REF(int,	 mLength, Length);

private:
	CCArray *mList;
};

