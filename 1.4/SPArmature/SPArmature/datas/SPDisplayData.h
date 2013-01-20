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

#include "SPContourSprite.h"
#include "cocos2d.h"
using namespace cocos2d;

namespace SPII{
namespace SPArmature{
    
/**
 *  @brief  DisplayType distinguish which type you display is.
 */
enum DisplayType{
    SP_SPRITE,          //! display is a single CCSprite
    SP_ARMATURE,        //! display is a Armature
    SP_PARTICLE,        //! display is a CCParticle, this is not support now.
    
    SP_DISPLAY_MAX
};

class DisplayData : public CCObject
{
public:
	static DisplayData *create();
public:
	DisplayData(void);
	~DisplayData(void);
    
    virtual bool init();
    
    /*
     *  @brief   this method will call new display's retain() method to retain a reference, and call old display's release() method to release a reference.
     *          
     *  @param  display you want to set to DisplayData
     */
    void setDisplay(CCObject *_display);
    CCObject *getDisplay();
    
    void setContourSprite(ContourSprite *_contourSprite);
    ContourSprite *getContourSprite();
    
public:
    
	//! mark which type your display is
    DisplayType m_eDisplayType;
    
    /**
     *  @brief  If DisplayType is SP_SPRITE, then Bone will use this image name to create a CCSprite from CCSpriteFrameCache.
     *          It should note that when use this name to create CCSprite from CCSpriteFrameCache, you should use m_strDisplayName + ".png", because when use Texture Packer to pack single image file, the name have ".png".
     *
     *          If DisplayType is SP_ARMATURE, the name is the Armature's name. When Bone init display and type is SP_ARMATURE,
     *          then Bone will create a Armature.
     */
    std::string m_strDisplayName;
    
    /**
     *  @brief  m_strImageName is different from m_strDisplayName, it is used for the display with type of DisplayType.
     *          It is the name of the whole image name.
     *          Example : we have several pngs named 1.png, 2.png, 3.png, these names are display names. And when use texture packer 
     *          to packer them to all.png, the 'all.png' is the image name.
     */
    std::string m_strImageName;
    
    
protected:
    //! the actual display point
    CCObject *m_pDisplay;
    
    ContourSprite *m_pContourSprite;
};

}
}