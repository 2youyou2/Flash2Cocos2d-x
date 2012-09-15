#include "CTweenNode.h"
#include <limits>

CCTweenNode *CCTweenNode::create(){
	CCTweenNode *pTweenNode = new CCTweenNode();
	if (pTweenNode && pTweenNode->init()){
		pTweenNode->autorelease();
		return pTweenNode;
	}
	CC_SAFE_DELETE(pTweenNode);
	return NULL;
} 

CCTweenNode::CCTweenNode( ){
	reset();
}


CCTweenNode::~CCTweenNode(void){
	
}

void CCTweenNode::reset(){
	CCFrameNode::reset();
	sR = sX = sY = sSX = sSY = sA = dR = dX = dY = dSX = dSY = dA = 0;
}

void CCTweenNode::betweenValue(CCFrameNode* _from, CCFrameNode* _to) {
	sR = _from->rotation;
	sX = _from->x;
	sY = _from->y;
	sSX = _from->scaleX;
	sSY = _from->scaleY;
	sA = _from->alpha;

	if (_to->offR) {
		dR = _to->rotation + _to->offR * 360 - sR;
	}else {
		dR = _to->rotation - sR;
	}

	dX = _to->x - sX;
	dY = _to->y - sY;
	dSX = _to->scaleX - sSX;
	dSY = _to->scaleY - sSY;
	dA = _to->alpha - sA;
}


void CCTweenNode::tweenTo(float _k) {
	rotation = sR + dR * _k;
	x = sX + dX * _k;
	y = sY + dY * _k;

	if (dSX) {
		scaleX = sSX + dSX * _k;
	}else {
		scaleX = std::numeric_limits<float>::quiet_NaN();
	}

	if (dSY) {
		scaleY = sSY + dSY * _k;
	}else {
		scaleY = std::numeric_limits<float>::quiet_NaN();
	}

	if (dA) {
		alpha = sA + dA * _k;
	}else {
		alpha = std::numeric_limits<float>::quiet_NaN();
	}
}

void CCTweenNode::remove(){
	reset();
}