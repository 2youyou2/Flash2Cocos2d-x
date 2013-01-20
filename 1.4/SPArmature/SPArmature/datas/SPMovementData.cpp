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

#include "SPMovementData.h"

namespace SPII
{
namespace SPArmature
{


MovementData *MovementData::create()
{
    MovementData *pMovementData = new MovementData();
    if (pMovementData->init())
    {
        pMovementData->autorelease();
        return pMovementData;
    }
    CC_SAFE_DELETE(pMovementData);
    return NULL;
}

MovementData::MovementData(void)
    :m_pMovBoneDataDic(NULL)
    ,m_pMovFrameDataArr(NULL)
{
}


MovementData::~MovementData(void)
{
    CC_SAFE_DELETE( m_pMovBoneDataDic );
    CC_SAFE_DELETE( m_pMovFrameDataArr );
}

bool MovementData::init()
{
    bool bRet = false;
    do
    {

        m_pMovBoneDataDic = new CCDictionary();
        CCAssert(m_pMovBoneDataDic, "create MovementData::m_pMovBoneDataDic fail!");

        m_pMovFrameDataArr = new CCArray();
        CCAssert(m_pMovFrameDataArr, "create MovementData::m_pMovFrameDataArr fail!");

        bRet = true;
    }
    while (0);

    return bRet;
}


void MovementData::addMovementBoneData(MovementBoneData* _movementData, const char* _boneName)
{
    m_pMovBoneDataDic->setObject(_movementData, _boneName );
}

MovementBoneData* MovementData::getMovementBoneData(const char* _boneName)
{
    return (MovementBoneData*)(m_pMovBoneDataDic->objectForKey(_boneName));
}

void MovementData::addMovFrameData(MovementFrameData *_movFrameData)
{
	m_pMovFrameDataArr->addObject(_movFrameData);
}

MovementFrameData *MovementData::getMoveFrameData(int _index)
{
	return (MovementFrameData*)m_pMovFrameDataArr->objectAtIndex(_index);
}
    
CCDictionary *MovementData::getMovBoneDataDic()
{
    return m_pMovBoneDataDic;
}
    
CCArray *MovementData::getMovFrameDataArr()
{
    return m_pMovFrameDataArr;
}

}
}