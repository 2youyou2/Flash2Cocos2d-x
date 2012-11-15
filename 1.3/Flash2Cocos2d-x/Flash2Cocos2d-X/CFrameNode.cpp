#include "CFrameNode.h"

CCFrameNode *CCFrameNode::create(){
	CCFrameNode *pFrameNode = new CCFrameNode();
	if (pFrameNode && pFrameNode->init()){
		pFrameNode->autorelease();
		return pFrameNode;
	}
	CC_SAFE_DELETE(pFrameNode);
	return NULL;
}

CCFrameNode::CCFrameNode( ){
	reset();
}


CCFrameNode::~CCFrameNode(void){
}

void CCFrameNode::reset(){
	CCBaseNode::reset();
	frame = 1;
	offR = 0;
	img = "";
}

void CCFrameNode::copy(CCBaseNode* _fv) {
	CCBaseNode::copy(_fv);
	if (static_cast<CCFrameNode*>(_fv)){
		CCFrameNode* _nV = (CCFrameNode*)_fv;
		if (_nV) {
			frame	= _nV->frame;
			offR	= _nV->offR;
			img		= _nV->img;
		}
	}
	
}

std::string CCFrameNode::toString(){
	std::string str;
	CCString	*cstr = CCString::createWithFormat("x;%f, y:%f, rotation:%f, scaleX:%f, scaleY:%f, alpha:%f, frame:%d, offR:%f;", 
		x, y, rotation, scaleX, scaleY, alpha, frame, offR);

	//CCLOG( cstr->getCString() );
	
	str.append( cstr->getCString() );
	return str;
}