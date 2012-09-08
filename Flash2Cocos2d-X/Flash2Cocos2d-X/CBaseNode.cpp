#include "CBaseNode.h"

CCBaseNode *CCBaseNode::create( float _x, float _y, float _rotation ){
	CCBaseNode *pBaseNode = new CCBaseNode();
	if (pBaseNode && pBaseNode->init(_x, _y, _rotation)){
		pBaseNode->autorelease();
		return pBaseNode;
	}
	CC_SAFE_DELETE(pBaseNode);
	return NULL;
}


CCBaseNode::CCBaseNode(){
	reset();
}

CCBaseNode::~CCBaseNode(){

}

void CCBaseNode::reset(){
	x = 0;
	y = 0;
	rotation = 0;
}

bool CCBaseNode::init( float _x, float _y, float _rotation ){
	bool bRet = false;
	do{
		x = _x;
		y = _y;
		rotation = _rotation;

		bRet = true;
	} while (0);

	return bRet;
}

void CCBaseNode::copy( CCBaseNode* _fV ) {
	rotation = _fV->rotation;
	x = _fV->x;
	y = _fV->y;
}