#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "CArmature.h"

using namespace cocos2d;

enum MoveType{NOTHING, MOVELEFT = -1, MOVERIGHT = 1, STAND, SQUAT, JUMP};

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

	void registerWithTouchDispatcher();
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	void move(int _dir);
	void jump();
	void squat();
	void updateMovement();
	void updateSpeed();
	void updateWeapon();

private:
	CCArmature *armature;
	
	bool left;
	bool right;
	bool down;

	int moveDir;

	float speedX;
	float speedY;
	int	face;

	float mouseX;
	float mouseY;

	bool isJumping;
	bool isSquat;

	CCSpriteBatchNode *batchNode;

};

#endif  // __HELLOWORLD_SCENE_H__