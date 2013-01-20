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

#include "SPTextureData.h"


namespace SPII{
namespace SPArmature{

TextureData *TextureData::create()
{
	TextureData *pTextureData = new TextureData();
	if (pTextureData && pTextureData->init())
	{
		pTextureData->autorelease();
		return pTextureData;
	}
	CC_SAFE_DELETE(pTextureData);
	return NULL;
}

TextureData::TextureData()
	:m_fHeight(0.0f)
	,m_fWidth(0.0f)
	,m_fPivotX(0.0f)
	,m_fPivotY(0.0f)
	,m_strName("")
{

}


TextureData::~TextureData()
{
    CC_SAFE_DELETE(m_pContourDataList);
}
    
bool TextureData::init()
{
    m_pContourDataList = CCArray::createWithCapacity(3);
    CCAssert(m_pContourDataList, "create m_pContourDataList failed!");
    m_pContourDataList->retain();
    
    return true;
}
    
void TextureData::addContourData(ContourData *_contourData)
{
    m_pContourDataList->addObject(_contourData);
}


void TextureData::removeContourData(ContourData *_contourData)
{
    m_pContourDataList->removeObject(_contourData);
}


ContourData *TextureData::getContourData(int _index)
{
    return (ContourData*)m_pContourDataList->objectAtIndex(_index);
}

CCArray *TextureData::getContourDatas()
{
    return m_pContourDataList;
}
    
int TextureData::getContourCount()
{
    return m_pContourDataList->count();
}

}
}