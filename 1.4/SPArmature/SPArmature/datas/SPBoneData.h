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

#include "SPNode.h"

namespace SPII{
namespace SPArmature{

/**
 *  @brief  BoneData used to init a Bone.
 *  BoneData keeps a DisplayData list, a Bone can have many display to change.
 *  The display information saved in the DisplayData
 */
class BoneData : public Node
{
public:
	static BoneData *create();
public:
	BoneData(void);
	~BoneData(void);

    /*
     *  @brief   create CCArray m_pDisplayDataList
     */
	virtual bool init();
    
    /*
     *  @brief   depth copy the information from the given data to self
     *
     *  @param  the data you want to copy
     */
	void copy(BoneData *_data);

public:
    //! the bone's name
	std::string m_strName;
    //! the bone parent's name
	std::string m_strParent;

//	float m_fPivotX;
//	float m_fPivotY;
//	float m_fWidth;
//	float m_fHeight;

    //! save DisplayData informations for the Bone
	CCArray *m_pDisplayDataList;
};


}
}