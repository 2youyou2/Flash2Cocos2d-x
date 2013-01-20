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

#include "SPMovementData.h"

namespace SPII{
namespace SPArmature{

/**
 *  @brief  AnimationData include all movement infomation for the Armature
 *
 *  The struct is AnimationData -> MovementData -> MovementBoneData -> FrameData
 *                                              -> MovementFrameData
 */
class AnimationData : public CCObject {
public:
    static AnimationData *create();
public:
    AnimationData(void);
    ~AnimationData(void);

    /**
     *	@brief create CCDictionary mBoneDatas
     */
    virtual bool init();

    /**
    *	@brief	add single movment£¬like run£¬stand 
	*
    *	@param	_movData	single movment data
    *
	*	@param	_movName	movment's name
    */
    void addMovment(const char* _movName, MovementData* _movData);

    /**
    *	@brief	get single movment£¬like run£¬stand 
	*
    *	@param	_movName	movment's name
    *
	*	@return	single movment data
    */
    MovementData *getMovment(const char* _movName);

	 /**
    *	@brief	get movment name list 
    *
	*	@return	std::vector<std::string>, the movement name vector
    */
    std::vector<std::string> getMovNames();
private:
	/**
	 *	@brief	save movment datas
	 *  @key	std::string
	 *  @value	MovmentData *
     */
    CCDictionary *m_pMovDataDic;
    
    /**
	 *	@brief	a vector to save all movement names in this animation
     */
    std::vector<std::string> m_vecMovNames;

    /*
     *  @brief	AnimationData Name used to mark this Aimation is used for which Armature
     */
	CC_SYNTHESIZE_PASS_BY_REF(std::string, m_strName, Name);

};


}
}