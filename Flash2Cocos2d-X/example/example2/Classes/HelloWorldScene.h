#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "CKnightArmature.h"
#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::CCLayerColor
{
public:
	~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
 

    // implement the "static node()" method manually
    LAYER_CREATE_FUNC(HelloWorld);

	void update(float dt);

	void draw();

	void visit();

	void registerWithTouchDispatcher();

	virtual void addKnight(CCObject* pSender);

	virtual void removeKnight(CCObject* pSender);

private:
	CCArray *armatures;

	CCSpriteBatchNode *batchNode;

};

#endif  // __HELLOWORLD_SCENE_H__