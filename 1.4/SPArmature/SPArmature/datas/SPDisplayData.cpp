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

#include "SPDisplayData.h"


namespace SPII{
namespace SPArmature{

DisplayData *DisplayData::create()
{
	DisplayData *pDisplayData = new DisplayData();
	if (pDisplayData && pDisplayData->init()){
		pDisplayData->autorelease();
		return pDisplayData;
	}
	CC_SAFE_DELETE(pDisplayData);
	return NULL;
}

DisplayData::DisplayData(void)
:m_eDisplayType(SP_SPRITE)
,m_pDisplay(NULL)
,m_strDisplayName("")
,m_strImageName("")
,m_pContourSprite(NULL)
{
}


DisplayData::~DisplayData(void)
{
    if(NULL != m_pDisplay)
    {
        m_pDisplay->release();
    }
    
    m_pDisplay = NULL;
    
    if (NULL != m_pContourSprite)
    {
        m_pContourSprite->release();
    }
    
    m_pContourSprite = NULL;
}
    
bool DisplayData::init()
{
    
    return true;
}

void DisplayData::setDisplay(CCObject *_display)
{
    if (NULL != m_pDisplay)
    {
        m_pDisplay->release();
    }
    
    m_pDisplay = _display;
    m_pDisplay->retain();
}
    
CCObject *DisplayData::getDisplay()
{
    return m_pDisplay;
}
    
void DisplayData::setContourSprite(SPII::SPArmature::ContourSprite *_contourSprite)
{
    if(m_pContourSprite != NULL)
    {
        m_pContourSprite->release();
    }
    
    m_pContourSprite = _contourSprite;
    m_pContourSprite->retain();
}
    
ContourSprite *DisplayData::getContourSprite()
{
    return m_pContourSprite;
}
    
}
}