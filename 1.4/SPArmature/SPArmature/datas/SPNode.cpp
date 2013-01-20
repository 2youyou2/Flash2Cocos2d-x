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

#include "SPNode.h"
#include "SPUtilMath.h"

namespace SPII
{
namespace SPArmature
{


Node *Node::create()
{
    Node *pBaseNode = new Node();
    if (pBaseNode)
    {
        pBaseNode->autorelease();
        return pBaseNode;
    }
    CC_SAFE_DELETE(pBaseNode);
    return NULL;
}


Node::Node()
    :m_fX(0.0f)
    ,m_fY(0.0f)
    ,m_iZOrder(0)

    ,m_fSkewX(0.0f)
    ,m_fSkewY(0.0f)
    ,m_fScaleX(1.0f)
    ,m_fScaleY(1.0f)

    ,m_fTweenRotate(0.0f)

    ,m_iAlpha(255)
    ,m_iRed(255)
    ,m_iGreen(255)
    ,m_iBlue(255)

{
}

Node::~Node()
{

}



void Node::copy( Node* _node )
{
    m_fX = _node->m_fX;
    m_fY = _node->m_fY;
    m_iZOrder = _node->m_iZOrder;
    

    m_fScaleX = _node->m_fScaleX;
    m_fScaleY = _node->m_fScaleY;
    m_fSkewX = _node->m_fSkewX;
    m_fSkewY = _node->m_fSkewY;

    m_fTweenRotate = _node->m_fTweenRotate;
    
    m_iAlpha = _node->m_iAlpha;
    m_iRed = _node->m_iRed;
    m_iGreen = _node->m_iGreen;
    m_iBlue = _node->m_iBlue;

}

void Node::subtract(Node *_from, Node *_to)
{
    m_fX = _to->m_fX - _from->m_fX;
    m_fY = _to->m_fY - _from->m_fY;
    m_fScaleX = _to->m_fScaleX - _from->m_fScaleX;
    m_fScaleY = _to->m_fScaleY - _from->m_fScaleY;
    m_fSkewX = _to->m_fSkewX - _from->m_fSkewX;
    m_fSkewY = _to->m_fSkewY - _from->m_fSkewY;
	m_iAlpha = _to->m_iAlpha - _from->m_iAlpha;
    m_iRed = _to->m_iRed - _from->m_iRed;
    m_iGreen = _to->m_iGreen - _from->m_iGreen;
    m_iBlue = _to->m_iBlue - _from->m_iBlue;

    if (m_fSkewX > SP_PI)
    {
        m_fSkewX -= SP_DOUBLE_PI;
    }
    if (m_fSkewX < -SP_PI)
    {
        m_fSkewX += SP_DOUBLE_PI;
    }

    if (m_fSkewY > SP_PI)
    {
        m_fSkewY -= SP_DOUBLE_PI;
    }
    if (m_fSkewY < -SP_PI)
    {
        m_fSkewY += SP_DOUBLE_PI;
    }

    if (_to->m_fTweenRotate)
    {
        m_fSkewX += _to->m_fTweenRotate;
        m_fSkewY += _to->m_fTweenRotate;
    }
}

}
}