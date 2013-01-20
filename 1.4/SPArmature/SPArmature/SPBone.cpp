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

#include "SPBone.h"
#include "SPUtilMath.h"
#include "SPArmature.h"
#include "SPArmatureDataManager.h"
#include "SPBatchNodeManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif

//using namespace SP_JSBind;

namespace SPII
{
namespace SPArmature
{
 
// INIT_JS_CLASS_ROOT(Bone)
//  
//  
// JS_FUNCTION_BEGIN(Bone)
// JS_FUNCTION_BIND(init, 1)
// JS_FUNCTION_BIND(addSpriteDisplay, 3)
// //JS_FUNCTION_BIND(changeDisplay, 3)
// JS_FUNCTION_BIND(changeDisplayByIndex, 1)
// JS_FUNCTION_BIND(removeFromParent, 0)
//     
// JS_FUNCTION_BIND(setPosition, 2)
// JS_FUNCTION_BIND(setPositionX, 1)
// JS_FUNCTION_BIND(setPositionY, 1)
// JS_FUNCTION_BIND(setRotation, 1)
// JS_FUNCTION_BIND(setScale, 1)
// JS_FUNCTION_BIND(setScaleX, 1)
// JS_FUNCTION_BIND(setScaleY, 1)
// JS_FUNCTION_END
// 
// JS_PROPERTY_GET_BEGIN(Bone)
// JS_PROPERTY_GET_END
// 
// JS_PROPERTY_SET_BEGIN(Bone)
// JS_PROPERTY_SET_END
// 
// JS_PROPERTY_BEGIN(Bone)
// JS_PROPERTY_END

    
Bone* Bone::create()
{

    Bone *pBone = new Bone();
    if (pBone && pBone->init())
    {
        pBone->autorelease();
        return pBone;
    }
    CC_SAFE_DELETE(pBone);
    return NULL;

}
    
Bone* Bone::create(const char *_name)
{
    
    Bone *pBone = new Bone();
    if (pBone && pBone->init(_name))
    {
        pBone->autorelease();
        return pBone;
    }
    CC_SAFE_DELETE(pBone);
    return NULL;
}

// Bone::Bone(JSObject* obj)
// {
//     m_pNode = NULL;
//     m_pTweenNode = NULL;
//     m_pDisplay = NULL;
//     m_pParent = NULL;
//     m_pArmature = NULL;
//     m_pChildArmature = NULL;
//     m_iDisplayIndex = -1;
//     m_pBoneData = NULL;
//     m_pTween = NULL;
//     m_pTween = NULL;
//     m_pChildren = NULL;
//     m_iZOrder = 0;
//     m_pContourSprite = NULL;
// }
    
Bone::Bone()
{
//    CREATE_JS_OBJECT(Bone)
    
    m_pNode = NULL;
    m_pTweenNode = NULL;
    m_pDisplay = NULL;
    m_pParent = NULL;
    m_pArmature = NULL;
    m_pChildArmature = NULL;
    m_iDisplayIndex = -1;
    m_pBoneData = NULL;
    m_pTween = NULL;
    m_pTween = NULL;
    m_pChildren = NULL;
    m_iZOrder = 0;
    m_pContourSprite = NULL;
}



Bone::~Bone(void)
{

	if( m_pDisplay )
	{
		m_pDisplay->removeFromParentAndCleanup(true);
        m_pDisplay = NULL;
	}

    CC_SAFE_DELETE(m_pChildren);
    CC_SAFE_DELETE(m_pBoneData);
    CC_SAFE_DELETE(m_pTween);
    CC_SAFE_DELETE(m_pNode);
}

bool Bone::init()
{
    return Bone::init(NULL);
}


bool Bone::init(const char *_name)
{
    bool bRet = false;
    do
    {

		// init m_pNode
        m_pNode = Node::create();
        m_pNode->retain();
        m_pNode->m_fScaleX = 0;
        m_pNode->m_fScaleY = 0;
        
        
        if(NULL != _name)
        {
            m_strName = _name;
        }


		m_pBoneData = BoneData::create();
		m_pBoneData->retain();

		m_pTween = Tween::create(this);
		m_pTween->retain();

		m_pTweenNode = m_pTween->m_pNode;
        
        bRet = true;
    }
    while (0);

    return bRet;
}

void Bone::setData(BoneData *_boneData)
{
    CCAssert(NULL != _boneData, "_boneData must not be NULL");
	
    m_pBoneData->copy(_boneData);
    
    //init m_strName and m_strParentName
    m_strName = m_pBoneData->m_strName;
    m_strParentName = m_pBoneData->m_strParent;
    
    m_iZOrder = m_pBoneData->m_iZOrder;
    
    initDisplayList();
}
    
void Bone::initDisplayList()
{
	
	CCObject *_object = NULL;
	CCARRAY_FOREACH(m_pBoneData->m_pDisplayDataList, _object)
	{
		DisplayData *_displayData = (DisplayData*)_object;
        
        CCObject *_display = NULL;
        
        switch (_displayData->m_eDisplayType) {
            case SP_SPRITE:
            {
                
                _display = CCSprite::createWithSpriteFrameName((_displayData->m_strDisplayName + ".png").c_str());
                
                /*
                 *  init display anchorPoint， every Texture have a anchor point
                 */
                TextureData *_textureData = ArmatureDataManager::sharedArmatureDataManager()->getTextureData(_displayData->m_strDisplayName.c_str());
                
                CC_BREAK_IF(!_textureData);
                
                float _anchorPointX = _textureData->m_fPivotX / _textureData->m_fWidth;
                float _anchorPointY = (_textureData->m_fHeight - _textureData->m_fPivotY) / _textureData->m_fHeight;
                ((CCSprite*)_display)->setAnchorPoint(ccp(_anchorPointX, _anchorPointY));
                
#if SP_DEBUG_SHOW_CONTOUR
                if (_textureData->getContourCount() > 0)
                {
                    
                    ContourSprite *_sprite = ContourSprite::create();
                    _sprite->addContourDataList(_textureData->getContourDatas());
                    
                    _displayData->setContourSprite(_sprite);
                    
                }
#endif
            }
                
                break;
            case SP_ARMATURE:
                
            {
                Armature *_armature = Armature::create(_displayData->m_strDisplayName.c_str());
                
                /*
                 *  here use the whole Armature to be a display
                 */
                _display = _armature;
                
                /*
                 *  because this bone have called this name, so _armature should change it's name, or it can't add to
                 *  Armature's bone children.
                 */
                _armature->setName(_armature->getName() + "_child");
            }
                
                break;
            default:
                break;
        }

        _displayData->setDisplay(_display);
	}
}

void Bone::addSpriteDisplay(const char *_displayName, const char *_imageName, int _index)
{
    CCAssert(NULL != _displayName && NULL != _imageName, "_displayName and _imageName must not be null");
    
    std::string _displayNameStr = _displayName;
    std::string _imageNameStr = _imageName;
    
    CCAssert(_imageNameStr.compare("") != 0  && _displayNameStr.compare("") != 0, "_displayName and _imageName must have content");
    
    
    CCSprite *_newDisplay = CCSprite::createWithSpriteFrameName(_displayName);
    
    CCSpriteBatchNode *_batchNode = NULL;
    
    if(NULL != _imageName)
    {
        _batchNode = BatchNodeManager::sharedBatchNodeManager()->getBatchNode(_imageName);
    }else{
        _batchNode = m_pArmature->getDisplay()->getBatchNode();
    }
 
    
    DisplayData *_displayData = NULL;
    
    if(_index >= 0 && _index < m_pBoneData->m_pDisplayDataList->count())
    {
        _displayData = (DisplayData*)m_pBoneData->m_pDisplayDataList->objectAtIndex(_index);
        
        
#if SP_DEBUG_SHOW_CONTOUR
        //! if old display data have contour, remove it
        ContourSprite *_contourSprite = _displayData->getContourSprite();
        if(_contourSprite)
        {
            BatchNodeManager::sharedBatchNodeManager()->getCurrentLayer()->removeChild(_contourSprite, true);
        }
#endif
        
    }
    else
    {
        _displayData = DisplayData::create();
        
        m_pBoneData->m_pDisplayDataList->addObject(_displayData);
    }
    
    _displayData->m_eDisplayType = SP_SPRITE;
    _displayData->setDisplay(_newDisplay);
	_displayData->m_strDisplayName = _displayName;
    _displayData->m_strImageName = _imageName;
    
    
    // remove .xxx
    std::string _textureName = _displayName;
    size_t startPos = _textureName.find_last_of(".");
    _textureName = _textureName.erase(startPos);
    
    // if the display have a TextureData, then use it to init display anchor point
	TextureData *_textureData = ArmatureDataManager::sharedArmatureDataManager()->getTextureData(_textureName.c_str());
    
    if(NULL != _textureData)
    {
        float _anchorPointX = _textureData->m_fPivotX / _textureData->m_fWidth;
        float _anchorPointY = (_textureData->m_fHeight - _textureData->m_fPivotY) / _textureData->m_fHeight;
        _newDisplay->setAnchorPoint(ccp(_anchorPointX, _anchorPointY));
        
#if SP_DEBUG_SHOW_CONTOUR
        if (_textureData->getContourCount() > 0)
        {
            
            ContourSprite *_sprite = ContourSprite::create();
            _sprite->addContourDataList(_textureData->getContourDatas());
            _displayData->setContourSprite(_sprite);
            
        }
#endif
    }
    
    // if changed display index is current display index, then change current display to the new display
    if(_index == m_iDisplayIndex)
    {
        m_iDisplayIndex = -1;
        changeDisplayByIndex(_index);
    }
}
    
//void Bone::changeDisplay(const char *_displayName, const char *_imageName)
//{
//    CCSprite *_newDisplay = CCSprite::createWithSpriteFrameName(_displayName);
//    
//    CCSpriteBatchNode *_batchNode = NULL;
//    
//    if(_imageName != NULL)
//    {
//        _batchNode = BatchNodeManager::sharedBatchNodeManager()->getBatchNode(_imageName);
//    }else{
//        _batchNode = m_pArmature->getDisplay()->getBatchNode();
//    }
//    
//    
//	if(m_pDisplay != NULL)
//	{
//        _newDisplay->setAnchorPoint(m_pDisplay->getAnchorPoint());
//        _newDisplay->setVertexZ(m_pDisplay->getVertexZ());
//	}
//    
//    _batchNode->addChild(_newDisplay);
//    
//    setDisplay(_newDisplay, true);
//}

void Bone::changeDisplayByIndex(int _index)
{
    
    CCAssert(_index>=-1 && (m_pBoneData ? _index<(int)m_pBoneData->m_pDisplayDataList->count() : true), "the _index value is out of range");
    
    // Armature do not nedd to change the display, Armature's display is just used to do transform for child Bones, it will not display on the screen.
    if(dynamic_cast<Armature*>(this) != NULL)
    {
        return;
    }

	/*
	 *	if _index is equal to current display index,then do nothing  
	 */
	if ( m_iDisplayIndex == _index )
	{
		return;
	}

    
	m_iDisplayIndex = _index;

	/* 
	 *	m_iDisplayIndex == -1, it means you want to hide you display  
	 */
	if (-1 == m_iDisplayIndex)
	{
        if(m_pDisplay)
        {
            m_pDisplay->removeFromParentAndCleanup(true);
            m_pDisplay = NULL;
            
        }
		return;
	}


    DisplayData  *_displayData = (DisplayData*)m_pBoneData->m_pDisplayDataList->objectAtIndex(m_iDisplayIndex);
    m_pCurrentDisplayData = _displayData;
    
    CCSprite *_display = NULL;
    
    m_pContourSprite = NULL;
    
    switch (_displayData->m_eDisplayType) {
        case SP_SPRITE:
        {
            _display = (CCSprite*)_displayData->getDisplay();
            
            /**
             *  if _displayData->m_strImageName is "", then the 
             */
            if(_displayData->m_strImageName.compare("") == 0)
            {
                m_pArmature->getDisplay()->getBatchNode()->addChild(_display/*, m_pBoneData->m_iZOrder*/);
            }
            else
            {
                BatchNode *_batchNode = BatchNodeManager::sharedBatchNodeManager()->getBatchNode(_displayData->m_strImageName);
                _batchNode->addChild(_display);

                
                if (m_pArmature->getDisPlayBatchNode()->getRenderType() == BATCHNODE_VERTEXZ)
                {
                    _batchNode->setRenderType(BATCHNODE_VERTEXZ);
                    cocos2d::CCLog("%s  change display and RenderType is BATCHNODE_VERTEXZ!", m_strName.c_str());
                }
            }


#if SP_DEBUG_SHOW_CONTOUR
			ContourSprite *_contourSprite = _displayData->getContourSprite();
			if(_contourSprite)
			{
				_contourSprite->removeFromParentAndCleanup(true);
				BatchNodeManager::sharedBatchNodeManager()->getCurrentLayer()->addChild(_contourSprite);

				CCLOG("layer add child!");
				setContourSprite(_contourSprite);
			}
#endif
            
            if(m_pChildArmature)
            {
                /*
                 *  remove child armature from armature's bone list
                 */
                m_pChildArmature->removeFromParent();
            }
            
            m_pChildArmature = NULL;
        }
            break;
        case SP_ARMATURE:
        {
            //Armature *_armature = (Armature*)m_pDisplayList->objectAtIndex(_index);
            Armature *_armature = (Armature*)_displayData->getDisplay();
            
            _armature->getAnimation()->playByIndex(0);
            
            _display = (CCSprite*)_armature->getDisplay();
            
            m_pChildArmature = _armature;
            
            /*
             *  add child armature to armature's bone list
             */
            if(m_pArmature)
            {
                m_pArmature->addBone(_armature, m_strName.c_str());
            }
        }
            break;
        default:
            break;
    }

    
    /*
     *  if already have a display we can just use the display's vertexz, or we should update the armature's children order
     */
    if (m_pDisplay)
    {
        if (m_pArmature->getDisPlayBatchNode()->getRenderType() == BATCHNODE_VERTEXZ)
        {
            _display->setVertexZ(m_pDisplay->getVertexZ());
        }
        else
        {
            _display->_setZOrder(m_pDisplay->getZOrder());
        }
    }else{
        m_pArmature->setBonesIndexChanged(true);
    }
    
    setDisplay(_display);
    
}
    
void Bone::update(float dt)
{
    
    if (m_pArmature && dynamic_cast<Armature*>(this) == NULL)
    {
        
		
        m_pTween->update(dt);
        
        if(-1 == m_iDisplayIndex)
		{
			return;
		}

        
        float _transformX = m_pBoneData->m_fX + m_pNode->m_fX + m_pTweenNode->m_fX;
        float _transformY = m_pBoneData->m_fY + m_pNode->m_fY + m_pTweenNode->m_fY;
        float _transformSkewX = m_pBoneData->m_fSkewX + m_pNode->m_fSkewX + m_pTweenNode->m_fSkewX;
        float _transformSkewY = m_pBoneData->m_fSkewY + m_pNode->m_fSkewY + m_pTweenNode->m_fSkewY;
        float _transformScaleX = m_pNode->m_fScaleX + m_pTweenNode->m_fScaleX;
        float _transformScaleY = m_pNode->m_fScaleX + m_pTweenNode->m_fScaleY;
        


        float cosX = cos(_transformSkewX);
        float sinX = sin(_transformSkewX);
        float cosY = cos(_transformSkewY);
        float sinY = sin(_transformSkewY);

        if (m_pDisplay)
        {
            m_pGlobalTransformMatrix.a = _transformScaleX * cosY;
            m_pGlobalTransformMatrix.b = _transformScaleX * sinY;
            m_pGlobalTransformMatrix.c = _transformScaleY * sinX;
            m_pGlobalTransformMatrix.d = _transformScaleY * cosX;
            m_pGlobalTransformMatrix.tx = _transformX;
            m_pGlobalTransformMatrix.ty = _transformY;
        }

        if (m_pChildren && m_pChildren->count() > 0)
        {
            /*
             *  m_pTransformMatrixForChildren is used for children, and it don't contain 
             *  the parent scale value
             */
            m_pTransformMatrixForChildren.a = cosY;
            m_pTransformMatrixForChildren.b = sinY;
            m_pTransformMatrixForChildren.c = sinX;
            m_pTransformMatrixForChildren.d = cosX;
            m_pTransformMatrixForChildren.tx = _transformX;
            m_pTransformMatrixForChildren.ty = _transformY;
            
        }

        if(m_pParent)
        {
            m_pGlobalTransformMatrix = CCAffineTransformConcat(m_pGlobalTransformMatrix, m_pParent->m_pTransformMatrixForChildren);
            
            if (m_pChildren && m_pChildren->count() > 0)
            {
                m_pTransformMatrixForChildren = CCAffineTransformConcat(m_pTransformMatrixForChildren, m_pParent->m_pTransformMatrixForChildren);
            }
        }
        
        if (m_pDisplay)
        {
            updateDisplay();
        }
        
    }
 

    CCObject *_object = NULL;
    CCARRAY_FOREACH(m_pChildren, _object)
    {
        Bone *_bone = (Bone*)_object;
        _bone->update(dt);
    }

}

void Bone::updateDisplay()
{
   if(m_pDisplay)
   {
      m_pDisplay->translateFormOtherNode(m_pGlobalTransformMatrix); 
   }
    
   
    if(m_pCurrentDisplayData->m_eDisplayType == SP_SPRITE)
    {
        CCSprite *_sprite = (CCSprite*)m_pDisplay;
        _sprite->setDirtyRecursively(true);
        
#if SP_DEBUG_SHOW_CONTOUR
        if(m_pContourSprite)
        {
            m_pContourSprite->translateFormOtherNode(m_pGlobalTransformMatrix);
        }
#endif
        
        if(m_pArmature->getDisPlayBatchNode()->getRenderType() != BATCHNODE_VERTEXZ)
        {
            _sprite->setOpacity(m_pTweenNode->m_iAlpha);
            _sprite->setColor(ccc3(m_pTweenNode->m_iRed, m_pTweenNode->m_iGreen, m_pTweenNode->m_iBlue));
        }
    }
	
}


void Bone::addChildBone(Bone* _child)
{
	CCAssert( NULL != _child, "Argument must be non-nil");
	CCAssert( NULL == _child->m_pParent, "child already added. It can't be added again");
    
	if(!m_pChildren)
	{
		childrenAlloc();
	}

	if (m_pChildren->indexOfObject(_child) == UINT_MAX)
    {
        m_pChildren->addObject(_child);
        _child->setParentBone(this);
    }
}

void Bone::removeChildBone(Bone *_bone)
{
    if ( m_pChildren->indexOfObject(_bone) != UINT_MAX )
    {
        
        CCArray *_ccbones = _bone->m_pChildren;
        CCObject *_object = NULL;
        CCARRAY_FOREACH(_ccbones, _object)
        {
            Bone *_ccBone = (Bone*)_object;
            _bone->removeChildBone(_ccBone);
        }
        
        _bone->setParentBone(NULL);
        _bone->setDisplay(NULL);
        m_pChildren->removeObject(_bone);
 
    }
}

void Bone::removeFromParent()
{
    if (NULL != m_pParent)
    {
        m_pParent->removeChildBone(this);
    }
}

void Bone::setParentBone(Bone *_parent)
{
    m_pParent = _parent;
    
    if(NULL != m_pParent)
    {
        m_strParentName = m_pParent->getName();
    }
    else
    {
        if(NULL != m_pArmature)
        {
            m_pArmature->removeBone(m_strName.c_str());
            m_pArmature = NULL;
        }
		m_strParentName = "";
		 
    }
}

Bone *Bone::getParentBone()
{
    return m_pParent;
}

void Bone::childrenAlloc(void)
{
	CC_SAFE_DELETE(m_pChildren);
	m_pChildren = CCArray::createWithCapacity(4);
	m_pChildren->retain();
}

void Bone::setDisplay(CCNode *_display)
{
    if (m_pDisplay)
    {
        m_pDisplay->removeFromParentAndCleanup(true);
    }
    
    m_pDisplay = _display;
}
    
CCNode *Bone::getDisplay()
{
    return m_pDisplay;
}
    
void Bone::setContourSprite(ContourSprite *_contourSprite)
{
    if (m_pContourSprite)
    {
        m_pContourSprite->removeFromParentAndCleanup(true);
    }
    
    m_pContourSprite = _contourSprite;
}
        
void Bone::setZOrder(int _zorder)
{
    /*
	 *	if _zorder is equal to m_iZOrder,then do nothing
	 */
    if (_zorder == m_iZOrder) {
        return;
    }
    
    m_iZOrder = _zorder;
    
    m_pArmature->setBonesIndexChanged(true);
}
    
int Bone::getZOrder()
{
    return m_iZOrder;
}
    
void Bone::setPosition(float x, float y)
{
    setDirty(true);
    m_pNode->m_fX = x;
    m_pNode->m_fY = y;
}
    
void Bone::setPositionX(float x)
{
    setDirty(true);
    m_pNode->m_fX = x;
}
    
void Bone::setPositionY(float y)
{
    setDirty(true);
    m_pNode->m_fY = y;
}

void Bone::setRotation(float r)
{
    setDirty(true);
    m_pNode->m_fSkewX = CC_DEGREES_TO_RADIANS(r);
    m_pNode->m_fSkewY = CC_DEGREES_TO_RADIANS(-r);
}


void Bone::setScale(float scale)
{
    setDirty(true);
    m_pNode->m_fScaleX = scale;
    m_pNode->m_fScaleY = scale;
}
    
void Bone::setScaleX(float scaleX)
{
    setDirty(true);
    m_pNode->m_fScaleX = scaleX;
}
    
void Bone::setScaleY(float scaleY)
{
    
    setDirty(true);
    m_pNode->m_fScaleY = scaleY;
}

}
}
