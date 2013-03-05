#include "HelloWorldScene.h"
#include "SPArmatureDataManager.h"
#include "SPBatchNodeManager.h"
#include "SPSpriteDisplayData.h"

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
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
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(200, 200, 200, 255)));
		//		CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(0, 0, 0, 255)));

		CCSize size = CCDirector::sharedDirector()->getWinSize();

		weaponIndex = 0;

		BatchNodeManager::sharedBatchNodeManager()->initWithLayer(this, "TEST_ARMATURE");

		ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Zombie_f/Zombie", "", "zombie.png", "zombie.plist", "zombie.xml");
		ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Knight_f/Knight", "", "knight.png", "knight.plist", "knight.xml");
		ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("cyborg", "", "cyborg.png", "cyborg.plist", "cyborg.xml");


 		ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Zombie_ladder", "", "zombie_ladder.png", "zombie_ladder.plist", "zombie_ladder.xml");
		ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("bird", "", "bird.png", "bird.plist", "bird.xml");
 		ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("weapon", "", "weapon.png", "weapon.plist", "weapon.xml");

		armatures = new CCArray();

		Armature *armature = NULL;

		//! use BATCHNODE_VERTEXZ you can change display in different batchnode
		BatchNodeManager::sharedBatchNodeManager()->getBatchNode("weapon.png")->setRenderType(BATCHNODE_VERTEXZ);

		armature = Armature::create("Knight_f/Knight");
		armature->getAnimation()->play("run");
		armature->setPosition(size.width/2 + 50, size.height/2);
		armatures->addObject(armature);


		armature = Armature::create("cyborg");
		armature->getAnimation()->play("run");
		armature->setPosition(size.width/2 + 130, size.height/2);
		armature->getAnimation()->MovementEventSignal.connect(this, &HelloWorld::onMovementEvent);	
		armature->setZOrder(2);
		armatures->addObject(armature);
// 				 
		armature = Armature::create("Zombie_f/Zombie");
		armature->getAnimation()->play("run");
		armature->setPosition(size.width/2 + 100, size.height/2);
		armature->setZOrder(1);
		armatures->addObject(armature);
				 		
		
				 
// 		armature = Armature::create("Knight_f/Knight");
// 		armature->getAnimation()->play("run");
// 		armature->setPosition(size.width/2 - 150, size.height/2);
// 
// 		SpriteDisplayData *param = SpriteDisplayData::create();
// 		param->setParam("weapon_f-sword2.png");
// 		
// 		armature->getBone("weapon")->addDisplay(param, 0);
// 		armature->setRenderType(BATCHNODE_VERTEXZ);
// 		armatures->addObject(armature);
// 
		
		armature = Armature::create("Zombie_ladder");
		armature->getAnimation()->play("anim_placeladder", 0, 50, 1, TWEEN_EASING_MAX);
		armature->getAnimation()->setAnimationScale(3);
		armature->setPosition(size.width/2 - 230, size.height/2 -100);		
		armatures->addObject(armature);
				 
		armature = Armature::create("bird");
		armature->getAnimation()->play("changeColor");
		armature->setPosition(size.width/2 - 230, size.height/2 + 100);		
		armatures->addObject(armature);



// 		for (int i = 0; i<600; i++)
// 		{
// 			Armature *armature = Armature::create("Knight_f/Knight");
// 			armature->getAnimation()->play("run");
// 			armature->setPosition(30 + (i%20)*(size.width/21), 30 + (i/20)*20);
// 			armatures->addObject(armature);
// 		}


		scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}



void HelloWorld::update(float dt)
{
	CCObject *_object = NULL;
	CCARRAY_FOREACH(armatures, _object)
	{
		Armature *armature = (Armature*)_object;
		armature->update(dt);
	}



}

void HelloWorld::draw()
{
	CCLayerColor::draw();

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float width = size.width;
	float height = size.height;

	ccDrawLine(ccp(0, height/2), ccp(width, height/2));
	ccDrawLine(ccp(width/2, height), ccp(width/2, 0));
}

void HelloWorld::onMovementEvent(Armature *_armature, const char *_eventType, const char *_movementID)
{
	weaponIndex++;

	if (weaponIndex >= 4)
	{
		weaponIndex = 0;
	}

	std::string typesStr = _eventType;
	if (typesStr.compare(LOOP_COMPLETE) == 0)
	{
		_armature->getBone("armInside")->getChildArmature()->getAnimation()->playByIndex(weaponIndex);
		_armature->getBone("armOutside")->getChildArmature()->getAnimation()->playByIndex(weaponIndex);
	}
}