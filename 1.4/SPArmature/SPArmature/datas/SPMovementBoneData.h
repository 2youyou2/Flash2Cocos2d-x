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

#include "SPFrameData.h"

namespace SPII{
namespace SPArmature{


class MovementBoneData : public CCObject
{
public:
	static MovementBoneData *create();
public:
	MovementBoneData();
	~MovementBoneData(void);

	virtual bool init();

	/*
     *  @brief   add FrameData to Bone's movement. When add a FrameData, length add 1 and duration add FrameData's duration
     *
     *  @param  _frameData  the FrameData you want to add to this mevement
     */
	void addFrame(FrameData *_frameData);
    
    /*
     *  @brief   get specified frame according to the index
     *
     *  @param  _index  frame index
     */
	FrameData *getFrame(int _index);
    
    /*
     *  @brief   get all frames including in this MovementData
     *
     *  @return a CCArray* include all frames
     */
	CCArray *getFrames();


public:
	/**
	 *	@brief	movement delay percent, this value can produce a delay effect
     */
	CC_SYNTHESIZE_PASS_BY_REF(float, m_fDelay, Delay);

	/**
	 *	@brief	scale this movement
     */
	CC_SYNTHESIZE_PASS_BY_REF(float, m_fScale, Scale);

	/**
	 *	@brief	this Bone in this movement will last m_iDuration frames
     */
	CC_SYNTHESIZE_PASS_BY_REF(int, m_iDuration, Duration);

	/**
	 *	@brief	frame list length
     */
	CC_SYNTHESIZE_PASS_BY_REF(int, m_iLength, Length);

private:
	CCArray *m_pFrameList;
};

}
}