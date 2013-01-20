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

#include "SPContourData.h"


namespace SPII{
namespace SPArmature{

ContourData *ContourData::create()
{
	ContourData *pContourData = new ContourData();
	if (pContourData && pContourData->init())
	{
		pContourData->autorelease();
		return pContourData;
	}
	CC_SAFE_DELETE(pContourData);
	return NULL;
}

ContourData::ContourData()
	:m_pVertexList(NULL)
{

}


ContourData::~ContourData()
{
    CC_SAFE_DELETE(m_pVertexList);
}
    
bool ContourData::init()
{
    m_pVertexList = CCArray::create();
    m_pVertexList->retain();
    
    return true;
}
    
void ContourData::addVertex(CCPoint *_vertex)
{
    m_pVertexList->addObject(_vertex);
}
    
void ContourData::insertVertex(CCPoint *_vertex, int _index)
{
    m_pVertexList->insertObject(_vertex, _index);
}
    
void ContourData::removeVertex(cocos2d::CCPoint *_vertex)
{
    m_pVertexList->removeObject(_vertex);
}
    
CCPoint *ContourData::getVertex(int _index)
{
    return (CCPoint*)m_pVertexList->objectAtIndex(_index);
}

int ContourData::indexOfVertex(CCPoint *_vertex)
{
    return m_pVertexList->indexOfObject(_vertex);
}
    
int ContourData::getVertexCount()
{
    return m_pVertexList->count();
}
    
const CCArray *ContourData::getVertexList()
{
    return m_pVertexList;
}
    
}
}