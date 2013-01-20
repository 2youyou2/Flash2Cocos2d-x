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

#include "SPBatchNode.h"
#include "SPAnimation.h"
#include "SPBoneData.h"
#include "SPDisplayData.h"
#include "SPArmatureData.h"
#include "SPBone.h"
#include "cocos2d.h"

using namespace cocos2d;



namespace SPII{
    
namespace Component{
    class Armature;
}

namespace SPArmature{

class ArmatureAnimationCallback
{
public:
    virtual void onMovementEvent(Armature *_armature, const char *_eventType, const char *_movementID) = 0;
};

typedef struct _ArmatureFileInfo : public CCObject
{
    static _ArmatureFileInfo *create()
    {
        _ArmatureFileInfo *_afi = new _ArmatureFileInfo();
        if(_afi)
        {
            _afi->autorelease();
            return _afi;  
        }
        CC_SAFE_DELETE(_afi);
        return NULL;
    }

    std::string armatureName;
    std::string useExistFileInfo;
    std::string imagePath;
    std::string plistPath;
    std::string xmlFilePath;
}ArmatureFileInfo;
    
class Armature : public Bone
{

public:
    /**
     *  @brief    create a armature with specified name
     *
     *  @param   _name   armature name
     */
    static Armature *create(const char* _name);
    
    /**
     *  @brief    create a armature with Component Armature
     *
     *  @param   _name   armature name
     *
     *  @param   _comArmature   component Armature bind to
     */
    //static Armature *createWithComArmature(const char* _name, Component::Armature *_comArmature);

public:
    Armature();
    ~Armature(void);

    /**
     *  @brief    init a armature with specified name
     *
     *  @param   _name   armature name
     */
    virtual bool init(const char *_name);
    
    /**
     *  @brief    init a armature with Component Armature
     *
     *  @param   _name   armature name
     *
     *  @param   _comArmature   component Armature bind to
     */
    //virtual bool initWithComArmature(const char *_name, Component::Armature *_comArmature);
    
    /**
     *  @brief    add a Bone to this Armature, 
     *
     *  @param   _name   armature name
     *
     *  @param   _comArmature   component Armature bind to
     */
    void addBone(Bone *_bone, const char* _parentName);
	
    /**
     *  @brief    get a bone with the specified name
     *
     *  @param   _name  the bone's name you want to get
     *
     */
    Bone *getBone(const char *_name);
    
    /**
     *  @brief   remove a bone with the specified name. It will also remove child Bone recursionly.
     *
     *  @param  _name  the bone's name you want to remove
     *
     */
    void removeBone(const char *_name);

    /**
     *  @brief   Reorder the Armature. This will redistribution all child bones vertexz or zorder.
     *          You should note that this zorder is different from zorder of cocos2dx.
     *          It is used to calculate child bones vertexz or zorder.
     *          
     *          We do not use cocos2dx zorder, because we use batchnode to improve speed and batchnode zorder do not support two batchnode can't mixed use their sprite's zorder
     *
     *  @param  _zOrder  the zorder you want to set
     *
     */
    void setZOrder(int _zOrder);
	
    /**
     *  @brief   Get current Armature's zorder
     *
     *  @return Current Armature's zorder  
     *
     */
    int getZOrder();

    /**
     *  @brief   Get Armature's bone dictionary
     *
     *  @return Armature's bone dictionary
     *
     */
	CCDictionary *getBoneDic();
    
    /**
     *  @brief   Get Armature's display. It's a little different from  Bone's display. Armature's display is only CCSprite
     *
     *  @return Armature's display
     *
     */
    CCSprite *getDisplay();
    
    void setPosition(float _x, float _y);
    void setPositionX(float _x);
    void setPositionY(float _y);
    
    void setRotation(float _r);
    
    void setScale(float _scale);
    void setScaleX(float _scaleX);
    void setScaleY(float _scaleY);
    
    
    /**
     *  @brief   Animation event call back, When Animation is start, complete or other, Animation will call this method to dispatch Animation event.
     *
     *  @param  _eventType 
     *
     */
    void onMovementEvent(const char *_eventType, const char *_movementID);
    
    
    void update(float dt);
private:
    /*
     *  @brief   sort Bones in this Armature, if m_pDisPlayBatchNode used BATCHNODE_VERTEXZ, then order use vertexz, else use cocos2dx zorder 
     */
    void sortBoneHelper(int _baseVertexz, int &_index);
    
    /*
     *  @brief used to create Bone internal
     */
	void createBone(const char *_boneName );

	/*
	 *	When a new armature is created, internal sort is called. This sort will calculate the current max armature internal zorder, 
	 *	and give the zorder to the new created armature. This zorder is not be changed after.
	 *	It's different from the zorder used for users.
	 */
	void internalSort();
public:
	CC_SYNTHESIZE(Animation *, m_pAnimation, Animation);

	CC_SYNTHESIZE_RETAIN( BatchNode*, m_pDisPlayBatchNode, DisPlayBatchNode);

	CC_SYNTHESIZE_PASS_BY_REF(bool, m_bBonesIndexChanged, BonesIndexChanged);
    
    CC_SYNTHESIZE(ArmatureData *, m_pArmatureData, ArmatureData);

	CC_SYNTHESIZE(ArmatureAnimationCallback *, m_pAnimationCallback, AnimationCallback);
protected:
    
    
    /*
     *  the dictionary of the bones, include all bones in the armature, no matter it is the direct bone or the indirect 
     *  bone. It is different from m_pChindren.
     */
    CCDictionary *m_pBoneDic;
    
    //! a CCArray include all bones
    CCArray *m_pBoneList;
    
    /*
     *  use to save armature zorder info, 
     */
    static std::map<int, Armature*> m_sArmatureIndexDic;
    
protected:
    //! this is used to sign the index of the Armature being created
	float m_fInternalZOrder;
    
    //! zorder for user to set
	float m_fZOrder;
	
	/*
	 *	m_iInternalZOrder combine m_fZOrder is the m_fActualZOrder.
	 */
	float m_fActualZOrder;

private:
	static int m_siMaxArmatureZorder;
    static CCDictionary *m_sArmatureMap;
    
//    Component::Armature *m_pComArmature;
    
};


}
}