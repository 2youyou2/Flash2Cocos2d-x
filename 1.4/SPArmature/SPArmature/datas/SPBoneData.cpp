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

#include "SPBoneData.h"
#include "SPDisplayData.h"

namespace SPII
{
namespace SPArmature
{


BoneData *BoneData::create()
{
    BoneData *pBoneData = new BoneData();
    if (pBoneData && pBoneData->init())
    {
        pBoneData->autorelease();
        return pBoneData;
    }
    CC_SAFE_DELETE(pBoneData);
    return NULL;
}

BoneData::BoneData(void)
    :m_pDisplayDataList(NULL)
    ,m_strName("")
    ,m_strParent("")

//    ,m_fPivotX(0)
//    ,m_fPivotY(0)
//    ,m_fWidth(0)
//    ,m_fHeight(0)
{

}


BoneData::~BoneData(void)
{
    CC_SAFE_DELETE( m_pDisplayDataList );
}

bool BoneData::init()
{
    bool bRet = false;
    do
    {

        m_pDisplayDataList = CCArray::create();
        CCAssert(m_pDisplayDataList , "");
		m_pDisplayDataList->retain();

        bRet = true;
    }
    while(0);
    return bRet;
}

void BoneData::copy(BoneData *_data)
{
	Node::copy(_data);
    
	m_strName = _data->m_strName;
	m_strParent = _data->m_strParent;

//	m_fPivotX = _copy->m_fPivotX;
//	m_fPivotY = _copy->m_fPivotY;
//	m_fWidth = _copy->m_fWidth;
//	m_fHeight = _copy->m_fHeight;

    m_pDisplayDataList->removeAllObjects();
	CCObject *_object = NULL;
	CCARRAY_FOREACH(_data->m_pDisplayDataList, _object)
	{
        DisplayData *_displayData = (DisplayData*)_object;
        DisplayData *_newData = DisplayData::create();
        
        _newData->m_strImageName = _displayData->m_strImageName;
        _newData->m_strDisplayName = _displayData->m_strDisplayName;
        _newData->m_eDisplayType = _displayData->m_eDisplayType;
        
		m_pDisplayDataList->addObject(_newData);
	}

}

}
}