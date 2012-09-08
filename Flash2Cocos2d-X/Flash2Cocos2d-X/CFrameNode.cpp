#include "CFrameNode.h"

CCFrameNode *CCFrameNode::create(float _x, float _y, float _rotation ){
	CCFrameNode *pFrameNode = new CCFrameNode();
	if (pFrameNode && pFrameNode->init(_x, _y, _rotation)){
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

	scaleX = 1;
	scaleY = 1;
	alpha = 255;
	frame = 1;
	offR = 0;
}

void CCFrameNode::copy(CCBaseNode* _fv) {
	CCBaseNode::copy(_fv);
	CCFrameNode* _nV = (CCFrameNode*)_fv;
	if (_nV) {
		scaleX	= _nV->scaleX;
		scaleY	= _nV->scaleY;
		alpha	= _nV->alpha;
		frame	= _nV->frame;
		offR	= _nV->offR;
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