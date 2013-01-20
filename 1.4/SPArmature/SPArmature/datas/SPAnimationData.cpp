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

#include "SPAnimationData.h"

namespace SPII
{
namespace SPArmature
{

AnimationData *AnimationData::create()
{
    AnimationData *pArmatureAniData = new AnimationData();
    if (pArmatureAniData && pArmatureAniData->init())
    {
        pArmatureAniData->autorelease();
        return pArmatureAniData;
    }
    CC_SAFE_DELETE(pArmatureAniData);
    return NULL;
}

AnimationData::AnimationData(void)
{
}


AnimationData::~AnimationData(void)
{
    CC_SAFE_DELETE( m_pMovDataDic );
}

bool AnimationData::init()
{
    bool bRet = false;
    do
    {

        m_pMovDataDic = new CCDictionary();
        CCAssert(m_pMovDataDic, "create AnimationData::m_pMovDataDic fail!");

        bRet = true;
    }
    while (0);

    return bRet;
}

void AnimationData::addMovment(const char* _movName, MovementData* _movData)
{
    m_pMovDataDic->setObject(_movData, _movName);
    m_vecMovNames.push_back(_movName);
}

MovementData *AnimationData::getMovment(const char* _movName)
{
    return (MovementData *)m_pMovDataDic->objectForKey( _movName );
}

std::vector<std::string>  AnimationData::getMovNames()
{
    return m_vecMovNames;
}


}
}