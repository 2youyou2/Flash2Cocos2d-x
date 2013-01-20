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

#include "SPArmature.h"
#include "SPArmatureDataManager.h"
#include "SPDisplayData.h"
#include "SPBatchNodeManager.h"
#include "SPArmatureDefine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif

namespace SPII
{
namespace SPArmature
{

    
std::map<int, Armature*> Armature::m_sArmatureIndexDic;
CCDictionary *Armature::m_sArmatureMap = new CCDictionary();
    
int Armature::m_siMaxArmatureZorder = 0;
   
    
Armature *Armature::create(const char* _name)
{
    Armature *_armature = new Armature();
    if (_armature && _armature->init(_name))
    {
        _armature->autorelease();
        return _armature;
    }
    CC_SAFE_DELETE(_armature);
    return NULL;
}
    
    
// Armature *Armature::createWithComArmature(const char* _name, Component::Armature *_comArmature)
// {
//     Armature *_armature = new Armature();
//     if (_armature && _armature->initWithComArmature(_name, _comArmature))
//     {
//         _armature->autorelease();
//         return _armature;
//     }
//     CC_SAFE_DELETE(_armature);
//     return NULL;
// }


Armature::Armature()
    :m_pAnimation(NULL)
    ,m_pBoneDic(NULL)
    ,m_pBoneList(NULL)
    ,m_pDisPlayBatchNode(NULL)
	,m_fZOrder(0)
	,m_fInternalZOrder(0)
	,m_fActualZOrder(0)
    ,m_bBonesIndexChanged(false)
	,m_pAnimationCallback(NULL)
{
}


Armature::~Armature(void)
{
    // remove this Armature's m_fInternalZOrder from m_sArmatureMap, so other Armature can use this internal zorder
    m_sArmatureMap->removeObjectForKey(m_fInternalZOrder);

    if(NULL != m_pBoneDic)
    {
        m_pBoneDic->removeAllObjects();
        CC_SAFE_DELETE(m_pBoneDic);
    }
    if(NULL != m_pBoneList)
    {
        m_pBoneList->removeAllObjects();
        CC_SAFE_DELETE(m_pBoneList);
    }
    
    CC_SAFE_DELETE(m_pAnimation);
}
// bool Armature::initWithComArmature(const char *_name, Component::Armature *_comArmature)
// {
//     bool bRet = false;
//     do
//     {
// 		if (!Armature::init(_name))
// 		{
// 			break;
// 		}
// 		        
//         m_pComArmature = _comArmature;
//         
//         bRet = true;
//     }while (0);
//     
//     return bRet;
// }
    
bool Armature::init(const char* _name)
{
    bool bRet = false;
    do
    {
        cocos2d::CCLog("Armature (%s)  create.", _name);
        m_pAnimation = Animation::create(this);
        CCAssert(m_pAnimation, "create Armature::m_pAnimation fail!");
        m_pAnimation->retain();

        m_pBoneDic	= CCDictionary::create();
        CCAssert(m_pBoneDic, "create Armature::m_pBoneDic fail!");
        m_pBoneDic->retain();
        
        m_pBoneList = CCArray::createWithCapacity(4);
        CCAssert(m_pBoneList, "create Armature::m_pBoneList fail!");
        m_pBoneList->retain();
        
        m_strName = _name;

        ArmatureFileInfo *_fileInfo = ArmatureDataManager::sharedArmatureDataManager()->getArmatureFileInfo(_name);
        CCAssert(_fileInfo, "_fileInfo should not be null!");
        
        
        if(_fileInfo->useExistFileInfo.compare("") != 0)
        {
            _fileInfo = ArmatureDataManager::sharedArmatureDataManager()->getArmatureFileInfo(_fileInfo->useExistFileInfo.c_str());
        }
        
        
        m_pDisPlayBatchNode = BatchNodeManager::sharedBatchNodeManager()->getBatchNode(_fileInfo->imagePath.c_str());
       
        do
        {
            m_pDisplay = CCSprite::create();
            CCAssert(m_pDisplay, "create Armature::init::m_pDisplay fail.");
            if( m_pDisPlayBatchNode )
            {
                CCSprite *_sprite = (CCSprite*)m_pDisplay;
                _sprite->initWithTexture(m_pDisPlayBatchNode->getTexture(), CCRectZero);
                
                m_pDisPlayBatchNode->addChild(_sprite);
                
            }

        }
        while (0);


        AnimationData* _animationData = ArmatureDataManager::sharedArmatureDataManager()->getAnimationData(_name);
        CCAssert(_animationData, "AnimationData not exist! ");
       
        m_pAnimation->setData(_animationData);


        ArmatureData *_armatureData = ArmatureDataManager::sharedArmatureDataManager()->getArmatureData(_name);
        CCAssert(_armatureData, "");
        
        m_pArmatureData = _armatureData;


        CCDictElement *_element = NULL;
        CCDictionary *_boneDataDic = _armatureData->getBoneDatas();
        CCDICT_FOREACH(_boneDataDic, _element)
        {
            createBone(_element->getStrKey());
        }

		internalSort();

		setZOrder(0);
        
        bRet = true;
    }
    while (0);

    return bRet;
}
    
CCSprite *Armature::getDisplay()
{
    return (CCSprite*)m_pDisplay;
}

void Armature::createBone(const char *_boneName)
{
    
    
	if(getBone(_boneName) != NULL)
	{
		return;
	}
    
    BoneData *_boneData = (BoneData*)m_pArmatureData->getBoneDatas()->objectForKey(_boneName);
    std::string _parentName = _boneData->m_strParent;

    Bone *_bone = NULL;
    
    if( _parentName.compare("") !=0 )
    {
        createBone(_parentName.c_str());
        _bone = Bone::create(_boneName);
        addBone(_bone, _parentName.c_str());
    }else{
        _bone = Bone::create(_boneName);
        addBone(_bone, "");
    }
    
    _bone->setData(_boneData);
	_bone->changeDisplayByIndex(0);
}

    
void Armature::addBone(Bone *_bone, const char *_parentName)
{
    CCAssert( _bone != NULL, "Argument must be non-nil");
    CCAssert(m_pBoneDic->objectForKey(_bone->getName()) == NULL, "_bone already added. It can't be added again");

    if (NULL == _parentName)
    {
        addChildBone(_bone);
    }
    else
    {
        Bone *_boneParent = (Bone*)m_pBoneDic->objectForKey(_parentName);
        
        if (_boneParent)
        {
            _boneParent->addChildBone(_bone);
        }
        else
        {
            addChildBone(_bone);
        }
    }
    
    
    _bone->setArmature(this);
    
    m_pBoneDic->setObject(_bone, _bone->getName());
    m_pBoneList->addObject(_bone);
}

void Armature::removeBone(const char* _name)
{
    Bone *_bone = (Bone*)m_pBoneDic->objectForKey(_name);
    CCAssert(_bone != NULL, "_bone must be added to the bone dictionary!");
    
    _bone->setArmature(NULL);
    _bone->removeFromParent();
    
    
    m_pBoneDic->removeObjectForKey(_name);
    m_pBoneList->removeObject(_bone);
}

void Armature::onMovementEvent(const char *_eventType, const char *_movementID)
{
    if(NULL != m_pAnimationCallback)
    {
        m_pAnimationCallback->onMovementEvent(this, _eventType, _movementID);
    }
}
    
Bone *Armature::getBone(const char* _name)
{
    return (Bone*)m_pBoneDic->objectForKey(_name);
}
    
void Armature::setZOrder(int _zOrder)
{

	m_fZOrder = _zOrder;

	/*
	 *	Calculate the actial zorder of this armature every time the m_iZOrder be set.
     *  Every m_fZOrder have at most ARMATURE_MAX_COUNT Armatures
     *  It's like a two dimensional table, m_fInternalZOrder is column number, and m_fZOrder is row number. Then m_fActualZOrder is the actual index.
     *  It can ensure every Armature have different vertexZ
	 */
	if(m_fZOrder >= 0)
	{
		m_fActualZOrder = m_fZOrder * ARMATURE_MAX_COUNT;
		m_fActualZOrder += m_fInternalZOrder;
	}else{
		m_fActualZOrder = m_fZOrder * ARMATURE_MAX_COUNT;
		m_fActualZOrder -= m_fInternalZOrder;
	}
	
    if(m_pDisPlayBatchNode->getRenderType() == BATCHNODE_VERTEXZ)
    {
        /*
         *  also every Armature have max child count, we allocate ARMATURE_MAX_CHILD vertexz number for each Armature
         */
        m_pDisplay->setVertexZ(m_fActualZOrder * ARMATURE_MAX_CHILD);
        
        int _i = 0;
        sortBoneHelper(m_pDisplay->getVertexZ(), _i);
    }
    else
    {
        /*
         *  also every Armature have max child count, we allocate ARMATURE_MAX_CHILD vertexz number for each Armature
         */
        m_pDisplay->_setZOrder(m_fActualZOrder * ARMATURE_MAX_CHILD);
        
        int _i = 0;
        sortBoneHelper(m_pDisplay->getZOrder(), _i);
    }
    
    return;
}
    
int Armature::getZOrder()
{
    return m_fZOrder;
}

    
CCDictionary *Armature::getBoneDic()
{
    return m_pBoneDic;
}
    
void Armature::update(float dt)
{

    if(m_bIsDirty || m_pArmature)
    {
        m_pTransformMatrixForChildren = m_pDisplay->nodeToParentTransform();
        m_bIsDirty = false;
    }

    m_pAnimation->update( dt );
    
	Bone::update(dt);
	
	
    if (m_bBonesIndexChanged)
    {
        if (m_pArmature != NULL)
        {
            m_pArmature->setBonesIndexChanged(true);
        }
        else
        {
            int i = 0;
            
            if(m_pDisPlayBatchNode->getRenderType() == BATCHNODE_VERTEXZ)
            {
                sortBoneHelper(m_pDisplay->getVertexZ(), i);
            }
            else
            {
                sortBoneHelper(m_pDisplay->getZOrder(), i);
            }
            
        }
        
        m_bBonesIndexChanged = false;
    }
    
}



void Armature::internalSort()
{
    if (m_sArmatureMap->count() >= ARMATURE_MAX_COUNT)
    {
        cocos2d::CCLog("warnning : current Armature count is more than ARMATURE_MAX_COUNT(%i), we will do not sort this Armature!!! ", ARMATURE_MAX_COUNT);
        m_fInternalZOrder = 0;
        return;
    }
    
    /*
     *  if m_siMaxArmatureZorder is more than ARMATURE_MAX_COUNT, we will find from 0
     */
    if(m_siMaxArmatureZorder >= ARMATURE_MAX_COUNT)
    {
        m_siMaxArmatureZorder = 0;
    }
    
    /*
     *  Stop until find out a index is not used. 
     */
    while (m_sArmatureMap->objectForKey(m_siMaxArmatureZorder) != NULL) {
        m_siMaxArmatureZorder ++;
    }
    
	m_fInternalZOrder = m_siMaxArmatureZorder;
    
    cocos2d::CCLog("Armature internal zorder : %f", m_fInternalZOrder);
    
    m_sArmatureMap->setObject(this, m_fInternalZOrder);
}

    
void Armature::sortBoneHelper(int _baseVertexz, int &_index)
{
    int i,j,length = m_pBoneList->data->num;
    Bone **x = (Bone**)m_pBoneList->data->arr;
    Bone *tempItem;
    
    // insertion sort
    for(i=1; i<length; i++)
    {
        tempItem = x[i];
        j = i-1;
        
        // if x[i] or x[j] is a Armature, then continue.
        if(dynamic_cast<Armature*>(x[j]) != NULL || dynamic_cast<Armature*>(x[i]) != NULL)
        {
            continue;
        }
        
        // if display of x[i] or x[j] is null, then continue.
        if(x[j]->getDisplay() == NULL || x[i]->getDisplay() == NULL)
        {
            continue;
        }
        
        
        int tempItemZ = tempItem->getZOrder();
        int jZ = x[j]->getZOrder();

        
        int tempOrderArrival = tempItem->getDisplay()->getOrderOfArrival();
        int jOrderArrival = x[j]->getDisplay()->getOrderOfArrival();
        
        //continue moving element downwards while zOrder is smaller or when zOrder is the same but mutatedIndex is smaller
        while(j>=0 && ( tempItemZ < jZ || ( tempItemZ== jZ &&  tempOrderArrival < jOrderArrival ) ) )
        {
            x[j+1] = x[j];
            j = j-1;
            
            
            if (j >= 0) {
                
                jZ = x[j]->getZOrder();
                
                // if display of x[j] is null, then continue.
                if(x[j]->getDisplay() == NULL)
                {
                    continue;
                }
                
                jOrderArrival = x[j]->getDisplay()->getOrderOfArrival();
            }
        }
        x[j+1] = tempItem;
    }
    
    // sort children children recursively
    for (i=0; i<length; i++)
    {
        if(dynamic_cast<Armature*>(x[i]) != NULL)
        {
            Armature *_armature = (Armature*)x[i];
            _armature->sortBoneHelper(_baseVertexz, _index);
            
            continue;
        }
        
        if(x[i]->getDisplay() != NULL)
        {
            if(m_pDisPlayBatchNode->getRenderType() == BATCHNODE_VERTEXZ)
            {
                x[i]->getDisplay()->setVertexZ((float)(_baseVertexz + _index++) / 10000);
            }
            else
            {
                x[i]->getDisplay()->_setZOrder(_baseVertexz + _index++);
            }
            
            //cocos2d::CCLog("%s : %f,  %i", x[i]->getName().c_str(), x[i]->getDisplay()->getVertexZ(), x[i]->getDisplay()->getZOrder());
        }
                
    }
    
}

    
void Armature::setPosition(float _x, float _y)
{
    setDirty(true);
    m_pDisplay->setPosition(ccp(_x, _y));
}

void Armature::setPositionX(float _x)
{
    setDirty(true);
    m_pDisplay->setPositionX(_x);
}

void Armature::setPositionY(float _y)
{
    setDirty(true);
    m_pDisplay->setPositionY(_y);
}

void Armature::setRotation(float _r)
{
    setDirty(true);
    m_pDisplay->setRotation(_r);
}


void Armature::setScale(float _scale)
{
    setDirty(true);
    m_pDisplay->setScale(_scale);
}

void Armature::setScaleX(float _scaleX)
{
    setDirty(true);
    m_pDisplay->setScaleX(_scaleX);
}

void Armature::setScaleY(float _scaleY)
{
    
    setDirty(true);
    m_pDisplay->setScaleY(_scaleY);
}

    
}
}