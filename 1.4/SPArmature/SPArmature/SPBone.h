/*
 * Copyright (c) 2012 Chukong Technologies, Inc.
 *
 * http://www.sweetpome.com
 * http://tools.cocoachina.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "SPNode.h"
#include "SPBoneData.h"
#include "SPTween.h"
//#include "SPJSbind.h"
#include "SPDisplayData.h"
//#include "SPContourSprite.h"


#define SP_DEBUG_SHOW_CONTOUR 1

using namespace cocos2d;
//using namespace SP_JSBind;

namespace SPII{
namespace SPArmature{


class Armature;

class /*SP_EXTERN*/ Bone : public CCObject
{
public:
    /**
	 *	@brief	create a Bone
     */
    static Bone *create();
    /**
	 *	@brief	create a Bone with specified name
     *
	 *	@param 	_name Bone's name.
     *
     */
	static Bone *create(const char *_name);

public:
//	Bone(JSObject* obj);
    Bone();
	virtual ~Bone(void);
    
    virtual bool init();
    /**
	 *	@brief	init a Bone with specified name
     *
	 *	@param 	_name Bone's name.
     *
     */
	virtual bool init(const char *_name);

    /**
	 *	@brief	Add display and set the display's DisplayType to SP_SPRITE. 
     *          If _index already have a display, then replace it.
     *          If _index is current 
     *
	 *	@param 	_displayName display's name in the CCSpriteFrameCache, like "sword.png" etc.
     *
	 *	@param 	_imageName image name, like "weapon.png" etc.
     *          NULL : will use Armature's batchnode to init this display
     *
	 *	@param 	_index the index of the display you want to replace or add to
     *          -1 : append display from back
     */
    void addSpriteDisplay(const char *_displayName, const char *_imageName, int _index);
    
    
//    /**
//	 *	@brief	This method can change the display with a sprite in diffrent CCSpriteBatchNode from origin display.
//     *          It will use method CCSprite::createWithSpriteFrameName(_displayName) to create a new CCSprite for the display.
//     *          So you should make sure you have add Sprite Frames to CCSpriteFrameCache before you use this method.
//     *
//	 *	@param 	_displayName the displayName you want to use
//     *
//     *  @param  _imageName  the 
//     */
//	void changeDisplay(const char *_displayName, const char *_imageName);

	/**
	 *	@brief	Change display by index. You can just use this method to 
	 *          change display in the display list. The display list is just used for this bone,
     *			and it is the displays you may use in every frame.
     *
     *          note : this method can only change display with a sprite in the same CCSpriteBatchNode from origin display
     *	
	 *	@param 	_index the index of the display you want to change
     */
	void changeDisplayByIndex(int _index);
  

    /**
	 *	@brief	Add a child to this bone, and it will let this child call setParent(Bone *_parent) function to set self to it's parent
     *
	 *	@param 	_child  the child you want to add
     */
	void addChildBone(Bone* _child);
    
    /**
	 *	@brief	set parent bone. 
     *          If _parent is NUll, then also remove this bone from armature.
     *          It will not set the Armature, if you want to add the bone to a Armature, you should use Armature::addBone(Bone *_bone, const char* _parentName).
     *
	 *	@param 	_parent  the parent bone.
     *          NULL : remove this bone from armature
     */
    void setParentBone(Bone *_parent);
    
    /**
	 *	@brief	get parent bone
     *
	 *	@return parent bone
     */
    Bone *getParentBone();
    
    
    /**
	 *	@brief	Remove itself from its parent Bone.
     *
	 *	@param 	_cleanup    whether or not cleanup display
     */
    void removeFromParent();
    
    /**
	 *	@brief	Removes a child Bone
     *
	 *	@param 	_bone   the bone you want to remove
     */
	void removeChildBone(Bone *_bone);
    
	/**
	 *	@brief	update the bone data, it will call void initDisplayList() function to update display list
     *
	 *	@param 	_boneData   
     */
    void setData(BoneData *_boneData);

    /**
	 *	@brief	set the bone's zorder. the zorder is different to cocos2dx's zorder, it used to calculate the vertexz
     *
	 *	@param 	_zorder zorder you want to set
     */
    void setZOrder(int _zorder);
    
    /**
     *  @brief   Get current bone's zorder
     *
     *  @return Current bone's zorder
     *
     */
    int getZOrder();
    
    /**
	 *	@brief	change current display, and clean up display if _cleanup
     *
	 *	@param 	_display    display you want to change to 
     */
    void setDisplay(CCNode *_display);
    
    /**
     *  @brief   Get Bone's display. 
     *
     *  @return Bone's display
     *
     */
	CCNode *getDisplay();
    
    void setContourSprite(ContourSprite *_contourSprite);

    void setPosition(float x, float y);
    void setPositionX(float x);
    void setPositionY(float y);

    void setRotation(float r);

    void setScale(float scale);
    void setScaleX(float scaleX);
    void setScaleY(float scaleY);
    
    void update(float dt);
    
protected:

	void updateDisplay();
    
    /**
	 *	@brief	Use BoneData to init the display list. 
     *          If display is a sprite, and it have texture info in the TexutreData, then use TexutreData to init the display's anchor point
     *          If the display is a Armature, then create a new Armature
     */
	virtual void initDisplayList();

public:
    
	CC_SYNTHESIZE_PASS_BY_REF(std::string, m_strName, Name);
	
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_strParentName, ParentName);
    
    /*
     *  the origin state of the Bone. Display's state is effected by m_pBoneData, m_pNode, m_pTweenNode
     *  when call setData function, it will copy from the BoneData.
     */
	CC_SYNTHESIZE_RETAIN(BoneData *, m_pBoneData, BoneData);
    
    //! A weak reference to the Armature
	CC_SYNTHESIZE(Armature *, m_pArmature, Armature);
    
    //! A weak reference to the child Armature
    CC_SYNTHESIZE(Armature *, m_pChildArmature, ChildArmature);
    
	CC_SYNTHESIZE(Tween *, m_pTween, Tween);
    
    //! set if is dirdy, if it's dirty, we update the transform
    CC_SYNTHESIZE_PASS_BY_REF(bool, m_bIsDirty, Dirty);
    
	
public:
	//! self Transform, use this to change display's state
	CCAffineTransform m_pGlobalTransformMatrix ;
    //! Transform for children
	CCAffineTransform m_pTransformMatrixForChildren;

private:
    //! used for make tween effect between every frame
	Node *m_pTweenNode;
    //! used for users to change the m_pDisplay's transform
    Node *m_pNode;
    //! current display index
	int m_iDisplayIndex;
    //! include current display information
    DisplayData *m_pCurrentDisplayData;

    /*
     *  the zorder in the Armature, when chane zorder, you need to change Armature's m_bBonesIndexChanged to 
     *  true. It is used for Armature calculate Bone's vertexz
     */
    int m_iZOrder;

protected:
    //! Bone's display object, it can be a sprite, particle etc.
	CCNode *m_pDisplay;
    //! used to show the contour if display have a contour editted
    ContourSprite *m_pContourSprite;
    
    CCArray *m_pChildren;
    //! a weak reference to it's parent
    Bone *m_pParent;
private:
	//! lazy allocs
	void childrenAlloc(void);

public:
//     CREATE_JS_CLASS_ROOT(Bone)
//     JS_NO_RETURN_FUNCTION_1(Bone, init, const char*)
//     JS_NO_RETURN_FUNCTION_3(Bone, addSpriteDisplay, const char*, const char*, int)
// //    JS_NO_RETURN_FUNCTION_2(Bone, changeDisplay, const char*, const char*);
//     JS_NO_RETURN_FUNCTION_1(Bone, changeDisplayByIndex, int);
//     JS_NO_RETURN_FUNCTION_0(Bone, removeFromParent);
//     JS_NO_RETURN_FUNCTION_2(Bone, setPosition, float, float);
//     JS_NO_RETURN_FUNCTION_1(Bone, setPositionX, float);
//     JS_NO_RETURN_FUNCTION_1(Bone, setPositionY, float);
//     JS_NO_RETURN_FUNCTION_1(Bone, setRotation, float);
//     JS_NO_RETURN_FUNCTION_1(Bone, setScale, float);
//     JS_NO_RETURN_FUNCTION_1(Bone, setScaleX, float);
//     JS_NO_RETURN_FUNCTION_1(Bone, setScaleY, float);
};


}
}