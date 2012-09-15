#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"

#include "CArmature.h"

class HelloWorld : public cocos2d::CCLayerColor 
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    LAYER_CREATE_FUNC(HelloWorld);

	void update(float dt);

	void move( int _dir );

	void draw();


private:
	void updateSpeed();

private:
	CCArmature *armature;
	
	bool left;
	bool right;
	bool attack;

	int moveCommand;

	float speedX;
	int face;
};

#endif  // __HELLOWORLD_SCENE_H__