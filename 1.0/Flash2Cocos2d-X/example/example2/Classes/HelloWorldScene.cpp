#include "HelloWorldScene.h"
#include "CConnectionData.h"
#include "CXLog.h"
#include "cocos2d.h"
#include "CMathHelper.h"
//#include "vld.h"

using namespace cocos2d;


HelloWorld::~HelloWorld(){
	CC_SAFE_DELETE(armatures);
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

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCConnectionData::sharedConnectionData()->addData("ExportTool_skeleton.xml");
		batchNode = CCSpriteBatchNode::create( "knight.png" );
		
		armatures = new CCArray();


		for(int i=0; i<50; i++){
			addKnight(NULL);
		}

		addChild( batchNode );



		CCLabelTTF *pAddLabel = CCLabelTTF::create("+", "Arial", 48);
		CC_BREAK_IF(! pAddLabel);

		CCLabelTTF *pRemoveLabel = CCLabelTTF::create("-", "Arial", 48);
		CC_BREAK_IF(! pRemoveLabel);

		CCMenuItemLabel *pAddKnightItem = CCMenuItemLabel::create(
			pAddLabel,
			this,
			(SEL_MenuHandler)&HelloWorld::addKnight);

		CC_BREAK_IF(! pAddKnightItem);

		pAddKnightItem->setPosition(size.width-50, 100);

		CCMenuItemLabel *pRemoveKnightItem = CCMenuItemLabel::create(
			pRemoveLabel,
			this,
			(SEL_MenuHandler)&HelloWorld::removeKnight);

		CC_BREAK_IF(! pRemoveKnightItem);

		pRemoveKnightItem->setPosition(size.width-50, 50);

		
		CCMenu* pMenu = CCMenu::create(pAddKnightItem, pRemoveKnightItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		this->addChild(pMenu, 1);

		scheduleUpdate();

    } while (0);

    return bRet;
}


void HelloWorld::update(float dt){
	CCObject *_object = NULL;

	//long _begin = millisecondNow();
	
	CCARRAY_FOREACH(armatures, _object){
		_object->update(dt);
	}

	/*long _end = millisecondNow();

	CCXLOG("update cost : %d", _end-_begin);

	CCXLOG("dt : %f", dt);*/
}

void HelloWorld::draw(){
	CCLayerColor::draw();
}

void HelloWorld::visit(){
	//long _begin = millisecondNow();
	CCLayerColor::visit();
	/*long _end = millisecondNow();

	CCXLOG("visit cost : %d", _end-_begin);*/
}

void HelloWorld::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN+1, true);
}


void HelloWorld::addKnight(CCObject* pSender){
	
	CCKnightArmature*  _armature= NULL;
	_armature = CCKnightArmature::create("knight", "knight", "knight.plist", "knight.png", batchNode, armatures->count());
	_armature->randomRun();
	_armature->unscheduleUpdate();

	addChild( _armature );

	armatures->addObject(_armature);

	
}

void HelloWorld::removeKnight(CCObject* pSender){
	if( armatures->count()>0 ){
		removeChild( (CCKnightArmature*)armatures->lastObject() , true);
		armatures->removeLastObject(true);
	}
}