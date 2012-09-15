#include "HelloWorldScene.h"
#include "CConnectionData.h"
#include "CXLog.h"
#include "cocos2d.h"
#include "CMathHelper.h"
//#include "vld.h"

using namespace cocos2d;


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

		
		CCConnectionData::sharedConnectionData()->addData("skeleton.xml");

		// add texture to CCSpriteFrameCache
		CCSpriteFrameCache *_cache =  CCSpriteFrameCache::sharedSpriteFrameCache();
		_cache->addSpriteFramesWithFile("skeleton.plist", "skeleton.png");

		batchNode = CCSpriteBatchNode::create( "skeleton.png" );
		addChild( batchNode );

		setTouchEnabled(true);
		CCLayer
		scheduleUpdate();

    } while (0);

    return bRet;
}


void HelloWorld::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN+1, true);
}


void HelloWorld::addItem(float x, float y){

	CCDictionary *_armatureDatas = CCConnectionData::sharedConnectionData()->getArmarureDatas();
	CCXLOG("%f, %d", CCRANDOM_0_1(), _armatureDatas->count());
	int _index = CCRANDOM_0_1()*(_armatureDatas->count()-1);
	CCString *_name = (CCString*)_armatureDatas->allKeys()->objectAtIndex(_index);
	const char *cName = _name->getCString();

	CCArmature *_armature = CCArmature::create(cName, _name->getCString(), batchNode);

	CCArmatureAniData * _aniData = CCConnectionData::sharedConnectionData()->getAnimationData(cName);
	std::vector<std::string> _animationNames = _aniData->getAnimationNames();
	_index = CCRANDOM_0_1() * (_animationNames.size()-1);

	_armature->getAnimation()->playTo((void*)_animationNames.at(_index).c_str());
	_armature->getDisplay()->setPosition(x, y);
	
	addChild(_armature);
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	return true;
}

void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint location = pTouch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	addItem(location.x, location.y);
}