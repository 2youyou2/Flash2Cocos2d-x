#include "HelloWorldScene.h"
#include "CConnectionData.h"
#include "CXLog.h"
#include "cocos2d.h"
//#include "vld.h"




HelloWorld::~HelloWorld(){

}

CCScene* HelloWorld::scene(){
    CCScene * scene = NULL;
    do{
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init(){

    bool bRet = false;
    do {

        CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(200, 200, 200, 255)));
		

        bRet = true;
		
		left = right = false;

		moveDir = -100;

		mouseX = mouseY = speedX = speedY = 0;
		isJumping = isSquat = false;
		

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCConnectionData::sharedConnectionData()->addData("skeleton.xml");
		batchNode = CCSpriteBatchNode::create( "robotbiped.png" );
		addChild(batchNode);

		armature = CCArmature::create("RobotBiped", "RobotBiped", "robotbiped.plist", "robotbiped.png", batchNode);
		armature->getDisplay()->setPosition(ccp(size.width/2, 150));
		addChild(armature);
		
		jump();
		armature->getAnimation()->playTo("run");

		scheduleUpdate();

		setTouchEnabled(true);

    } while (0);

    return bRet;
}


void HelloWorld::update(float dt){
	left = right = down = false; 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000)==0x8000){
		left = true;
		move(MOVELEFT);
	}		
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000)==0x8000){
		right = true;
		move(MOVERIGHT);
	}
	if ((GetAsyncKeyState(VK_UP) & 0x8000)==0x8000){
		move(JUMP);
	}		
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000)==0x8000){
		down = true;
		move(SQUAT);
	}
#endif
	if( !isJumping ){
		if( right && left ){
			move(moveDir);
		}else if(!right && !left && !down){
			move(STAND);
		}
	}
	

	updateSpeed();
	updateWeapon();
}

void HelloWorld::draw()
{
	CCLayerColor::draw();
}

void HelloWorld::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN+1, true);
}


void HelloWorld::move(int _dir) {
	if (moveDir == _dir) {
		return;
	}

	moveDir = _dir;
	updateMovement();
}

void HelloWorld::jump(){
	
	/*CCJumpBy *mJumpBy  = CCJumpBy::create(1, ccp( 0, 0 ), 200.0f, 1);  
	mJumpBy->setTag(1);

	if( !armature->getDisplay()->getActionByTag(1) ){
		armature->getDisplay()->runAction(mJumpBy);
		armature->getAnimation()->playTo("jump", 8);
	}else{
		if( armature->getDisplay()->getActionByTag(1)->isDone() ){
			armature->getDisplay()->runAction(mJumpBy);
			armature->getAnimation()->playTo("jump", 8);
		}
	}
	*/
	
	if (isJumping) {
		return;
	}
	speedY = 25;
	isJumping = true;
}

void HelloWorld::squat() {
	isSquat = true;
	armature->getAnimation()->playTo("squat");
}

void HelloWorld::updateMovement(){

	if (moveDir == STAND) {
		speedX = 0;
		armature->getAnimation()->playTo("stand");
	}else if (moveDir == JUMP){
		jump();
	}else if (moveDir == SQUAT){
		squat();
	}else {
		if (moveDir * face > 0) {
			speedX = 5* face;
			armature->getAnimation()->playTo("run");
		}else {
			speedX = -3 * face;
			armature->getAnimation()->playTo("runBack");
		}
	}
}

void HelloWorld::updateSpeed(){
	CCXLOG("%d", speedY);
	if (isJumping) {
		if (speedY == 1 ) {
			armature->getAnimation()->playTo("fall");
		}
		speedY -= 1;
	}
	if (speedX != 0) {
		armature->getDisplay()->setPositionX( armature->getDisplay()->getPositionX() + speedX ) ;
		if (armature->getDisplay()->getPositionX() < 0) {
			armature->getDisplay()->setPositionX( 0 );
		}else if (armature->getDisplay()->getPositionX() > 800) {
			armature->getDisplay()->setPositionX( 800 );
		}
	}
	if (speedY != 0) {
		armature->getDisplay()->setPositionY( armature->getDisplay()->getPositionY() + speedY ) ;
		
		if (armature->getDisplay()->getPositionY() <150) {
			armature->getDisplay()->setPositionY( 150 );
			isJumping = false;
			speedY = 0;

			
		}
	}
}

void HelloWorld::updateWeapon(){
	face = mouseX > armature->getDisplay()->getPositionX()?1: -1;
	if (armature->getDisplay()->getScaleX() != face) {
		armature->getDisplay()->setScaleX(face);
		updateMovement();
	}

	float _r = 0;
	if(face>0){
		_r = atan2(mouseY - armature->getDisplay()->getPositionY(), mouseX - armature->getDisplay()->getPositionX()) * 180 / M_PI;
	}else{
		_r = 180 - atan2(mouseY - armature->getDisplay()->getPositionY(), mouseX - armature->getDisplay()->getPositionX()) * 180 / M_PI;
		if (_r > 180) {
			_r -= 360;
		}
	}

	_r = -_r;

	
	
	CCBone *_chest = armature->getBone("chest");
	_chest->getNode()->rotation=_r*0.5;

	CCBone *_head = armature->getBone("head");
	if(_r>0){
		_head->getNode()->rotation=_r*0.2;
	}else{
		_head->getNode()->rotation=_r*0.4;
	}

	CCBone *_armR = armature->getBone("upperarmR");
	CCBone *_armL = armature->getBone("upperarmL");
	_armR->getNode()->rotation = _r*0.5;
	if(_r>0){
		_armL->getNode()->rotation=_r*0.8;
	}else{
		_armL->getNode()->rotation=_r*0.6;
	}
}


void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	return true;
}

void HelloWorld::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint location = pTouch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	
	mouseX = location.x;
	mouseY = location.y;
}