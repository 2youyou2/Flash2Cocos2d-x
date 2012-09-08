#include "HelloWorldScene.h"
#include "CConnectionData.h"
using namespace cocos2d;


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
		
		left = right = attack = false;

		moveCommand = 0;

		speedX = 0;

		CCSize size = CCDirector::sharedDirector()->getWinSize();

		// step 1: add your skeleton xml file
		CCConnectionData::sharedConnectionData()->addData("ExportTool_skeleton.xml");

		// step 2: create a CCSpriteBatchNode 
		CCSpriteBatchNode *batchNode = CCSpriteBatchNode::create( "knight.png" );
		
		// step 3: create a armature just need armature name, plist, picture, and a CCSpriteBatchNode
		armature = CCArmature::create("knight", "knight", "knight.plist", "knight.png", batchNode);
		//armature = CCCocos2dCommand::createArmature("zombie", "zombie", "Zombie.plist", "Zombie.png");
		
		armature->getDisplay()->setPosition(ccp(size.width/2, size.height/2));
		armature->getDisplay()->setScale( 1 );
		
		// step 4: play the animation
		armature->getAnimation()->playTo("stand", 1, 60, true);



		addChild( batchNode );

		scheduleUpdate();

    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender){
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

long millisecondNow(){ 
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL); 
	return (now.tv_sec * 1000 + now.tv_usec / 1000); 
} 

void HelloWorld::update(float dt){
	attack = left = right = false;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000)==0x8000)
		left = true;
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000)==0x8000)
		right = true;
	if ((GetAsyncKeyState(VK_UP) & 0x8000)==0x8000){
		armature->getAnimation()->setScale( armature->getAnimation()->getScale() + 0.1 );
		armature->getAnimation()->setAnimationScale( armature->getAnimation()->getScale() );
	}		
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000)==0x8000){
		armature->getAnimation()->setScale( armature->getAnimation()->getScale() - 0.1 );
		armature->getAnimation()->setAnimationScale( armature->getAnimation()->getScale() );
	}
	if((GetAsyncKeyState(VK_SPACE) & 0x8000)==0x8000){
		attack = true;
	}
		
#endif
	

	if( left && right ){
		move( moveCommand );
	}else if( left ){
		move( -1 );
	}else if( right ){
		move( 1 );
	}else if( attack ){
		armature->getAnimation()->playTo((void *)"attack", 4, 60, true, 1);
	}else{
		move( 0 );
	}

	updateSpeed();
	armature->update(dt);
}

void HelloWorld::move( int _dir ){
	if (moveCommand == _dir) {
		return;
	}

	if (_dir == 0) {
		speedX = 0;
		armature->getAnimation()->playTo("stand", 1, 60, true);
	}else if (moveCommand == 0) {
		face = -_dir;
		armature->getDisplay()->setScaleX( -face * 1 );
		speedX = 0.8 * -face;
		armature->getAnimation()->playTo("run", 1, 30, true);
	}
	moveCommand = _dir;
}

void HelloWorld::updateSpeed(){
	if (speedX != 0) {
		armature->getDisplay()->setPositionX(  armature->getDisplay()->getPositionX() + speedX );
		if (armature->getDisplay()->getPositionX() < 0) {
			armature->getDisplay()->setPositionX( 0 );
		}else if (armature->getDisplay()->getPositionX() > CCDirector::sharedDirector()->getWinSize().width) {
			armature->getDisplay()->setPositionX( CCDirector::sharedDirector()->getWinSize().width );
		}
	}
}

void HelloWorld::draw()
{
	CCLayerColor::draw();

}

