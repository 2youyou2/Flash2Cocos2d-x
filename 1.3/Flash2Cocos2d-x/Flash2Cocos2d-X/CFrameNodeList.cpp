#include "CFrameNodeList.h"


CCFrameNodeList *CCFrameNodeList::create(float _delay, float _scale){
	CCFrameNodeList *pFrameNodeList = new CCFrameNodeList(_delay, _scale);
	if (pFrameNodeList && pFrameNodeList->init()){
		pFrameNodeList->autorelease();
		return pFrameNodeList;
	}
	CC_SAFE_DELETE(pFrameNodeList);
	return NULL;
}

CCFrameNodeList::CCFrameNodeList(float _delay, float _scale){
	mDelay	= _delay;
	mScale	= _scale;
	mFrame	= 0;
	mLength	= 0;
	mList	= NULL;
}


CCFrameNodeList::~CCFrameNodeList(void){
	CC_SAFE_DELETE(mList);
}

bool CCFrameNodeList::init(){
	bool bRet = false;
	do{
		mList = new CCArray();
		CCAssert(mList->init(), "create CCFrameNodeList::mList fail!");

		bRet = true;
	} while (0);

	return bRet;
}


void CCFrameNodeList::addFrame(CCFrameNode* _value) {
	mList->addObject(_value);
	mFrame += _value->frame;
	mLength++;
}

CCFrameNode* CCFrameNodeList::getFrame(int _id) {
	/*if (_id<0) {
		_id = mLength + _id;
	}*/
	return (CCFrameNode*)mList->objectAtIndex(_id);
}

CCArray *CCFrameNodeList::getFrames(){
	return mList;
}

std::string CCFrameNodeList::toString(){
	std::string str;
	CCObject *obj = NULL;
	
	CCARRAY_FOREACH(mList, obj){
		CCFrameNode *_frame = (CCFrameNode *)obj;
		str.append( _frame->toString() );
		str.append("\n");
	}

	return str;
}