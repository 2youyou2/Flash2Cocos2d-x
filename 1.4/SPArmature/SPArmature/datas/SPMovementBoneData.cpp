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

#include "SPMovementBoneData.h"

namespace SPII{
namespace SPArmature{

MovementBoneData *MovementBoneData::create()
{
    MovementBoneData *pFrameNodeList = new MovementBoneData();
    if (pFrameNodeList && pFrameNodeList->init())
    {
        pFrameNodeList->autorelease();
        return pFrameNodeList;
    }
    CC_SAFE_DELETE(pFrameNodeList);
    return NULL;
}

MovementBoneData::MovementBoneData()
    :m_fDelay(0.0f)
    ,m_fScale(1.0f)
    ,m_iDuration(0)
    ,m_iLength(0)
    ,m_pFrameList(NULL)
{
}


MovementBoneData::~MovementBoneData(void)
{
    CC_SAFE_DELETE(m_pFrameList);
}

bool MovementBoneData::init()
{
    bool bRet = false;
    do
    {
        m_pFrameList = new CCArray();
        CCAssert(m_pFrameList->init(), "create MovementBoneData::m_pFrameList fail!");

        bRet = true;
    }
    while (0);

    return bRet;
}


void MovementBoneData::addFrame(FrameData* _frameData)
{
    CCAssert(_frameData, "MovementBoneData::addFrame  _frameData can't be null.");

    m_pFrameList->addObject(_frameData);
    m_iDuration += _frameData->m_iDuration;
    m_iLength++;

}

FrameData* MovementBoneData::getFrame(int _index)
{
    return (FrameData*)m_pFrameList->objectAtIndex(_index);
}

CCArray *MovementBoneData::getFrames()
{
    return m_pFrameList;
}


}
}