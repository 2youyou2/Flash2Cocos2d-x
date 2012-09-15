#pragma once

#include "CFrameNode.h"
#include <vector>

/* 关键帧数据列表 */
class CCFrameNodeList : public CCObject
{
public:
	static CCFrameNodeList *create(float _delay = 0, float _scale = 1);
public:
	/**
	* @param _delay 播放延时百分比
	* @param _scale 周期缩放百分比
	*/
	CCFrameNodeList(float _delay = 0, float _scale = 1);
	~CCFrameNodeList(void);

	virtual bool init();

	/**
	* 添加关键帧
	* @param _value 关键帧数据
	*/
	void			addFrame(CCFrameNode* _value);
	/**
	* 获取关键帧
	* @param _id index
	* @return 返回关键帧数据
	*/
	CCFrameNode		*getFrame(int _id);

	CCArray			*getFrames();

	std::string		toString();

public:
	/*播放延时百分比*/
	CC_SYNTHESIZE_PASS_BY_REF(float, mDelay,  Delay);

	/*周期缩放百分比*/
	CC_SYNTHESIZE_PASS_BY_REF(float, mScale,  Scale);

	/*实际持续帧数*/
	CC_SYNTHESIZE_PASS_BY_REF(int,   mFrame,  Frame);

	/*列表长度*/
	CC_SYNTHESIZE_PASS_BY_REF(int,	 mLength, Length);

private:
	CCArray *mList;
};

