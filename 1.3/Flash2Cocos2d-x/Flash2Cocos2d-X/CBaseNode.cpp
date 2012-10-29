#include "CBaseNode.h"

CCBaseNode *CCBaseNode::create(){
	CCBaseNode *pBaseNode = new CCBaseNode();
	if (pBaseNode && pBaseNode->init()){
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
	z = 0;
	rotation = 0;

	skewX = 0;
	skewY = 0;
	scaleX = 1;
	scaleY = 1;

	tweenRotate = 0;

	alpha = 255;
	visible = true;
	active = true;
}

bool CCBaseNode::init( float _x, float _y, float _rotation, float _scaleX, float _scaleY,  float _z, float _skewX, float _skewY){
	bool bRet = false;
	do{
		x = _x;
		y = _y;
		rotation = _rotation;

		scaleX = _scaleX;
		scaleY = _scaleY;
		z = _z;
		skewX = _skewX;
		skewY = _skewY;


		bRet = true;
	} while (0);

	return bRet;
}

void CCBaseNode::copy( CCBaseNode* _fV ) {
	rotation = _fV->rotation;
	x = _fV->x;
	y = _fV->y;
	z = _fV->z;
	scaleX = _fV->scaleX;
	scaleY = _fV->scaleY;
	skewX = _fV->skewX;
	skewY = _fV->skewY;
	tweenRotate = _fV->tweenRotate;
	alpha = _fV->alpha;
	visible = _fV->visible;
}