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


#include "tinyxml.h"
#include "SPConstValue.h"
#include "SPDisplayData.h"
#include "SPBoneData.h"
#include "SPAnimationData.h"
#include "SPMovementBoneData.h"
#include "SPMovementFrameData.h"
#include "SPMovementData.h"
#include "SPArmatureData.h"
#include "SPTextureData.h"
#include "SPArmature.h"
#include "SPContourData.h"

namespace SPII{
namespace SPArmature{


/**
 *	@brief	format and manage armature configuration and armature animation
 */
class ArmatureDataManager : public CCObject {
public:
    static ArmatureDataManager *sharedArmatureDataManager();
private:
    ArmatureDataManager(void);
    ~ArmatureDataManager(void);

public:
	/**
     *	@brief	init ArmatureDataManager
     */
    virtual bool init();
    
	/**
     *	@brief	get armature data from m_pArmarureDatas(CCDictionary)
     *
     *	@param	_id the id of the armature data you want to get
	 *  
	 *  @return	ArmatureData *
     */
    ArmatureData *getArmatureData(const char* _id);

	/**
     *	@brief	get animation data from m_pAnimationDatas(CCDictionary)
     *
     *	@param 	_id the id of the animation data you want to get
	 *  
	 *  @return AnimationData *
     */
    AnimationData *getAnimationData(const char* _id);

	/**
     *	@brief	get texture data from m_pTextureDatas(CCDictionary)
     *
     *	@param 	_id the id of the texture data you want to get
	 *  
	 *  @return TextureData *
     */
    TextureData *getTextureData(const char* _id);
    
    /**
     *	@brief	get ArmatureFileInfo from m_pArmatureFileInfoDic(CCDictionary)
     *
     *	@param 	_id the id of the ArmatureFileInfo you want to get
	 *
	 *  @return ArmatureFileInfo *
     */
    ArmatureFileInfo *getArmatureFileInfo(const char *_id);
	
	/**
	 *	@brief	Clear the data in the m_pArmarureDatas and m_pAnimationDatas, and set m_pArmarureDatas and m_pAnimationDatas to NULL
     */
    void removeAll();

public:
    
	/**
	 *	@brief	Translate XML export from Dragon Bone flash tool to datas, and save them. 
	 *          When you add a new xml, the data already saved will be keeped.
     *
     *	@param 	_xmlPath path of xml file
     */
    void addDataFromXML(const char *_xmlPath);
    
    /**
	 *	@brief	Just decode the TexutreData info in the xml, used for other texture info
     *
     *	@param 	_xmlPath path of xml file
     */
    void addTextureDataFromXML(const char *_xmlPath);
    
    /**
	 *	@brief	Add ArmatureFileInfo, it is managed by ArmatureDataManager.
     *
     *	@param 	_fileInfo ArmatureFileInfo you want to add
     */
    void addArmatureFileInfo(ArmatureFileInfo *_fileInfo);

	void addArmatureFileInfo(const char *_armatureName, const char *_useExistFileInfo, const char *_imagePath, const char *_plistPath, const char *_xmlFilePath);
private:
    /**
	 *	@brief	decode Armature Datas from xml export from Dragon Bone flash tool
     */
	void decodeArmature(TiXmlElement *_armatureXML);
	void decodeBone(TiXmlElement *_boneXML, TiXmlElement *_parentXML, ArmatureData *_armatureData);
	void decodeBoneDisplay(TiXmlElement *_displayXML, BoneData *_boneData);



	/**
	 *	@brief	decode Animation Datas from xml export from Dragon Bone flash tool
     */
	void decodeAnimation(TiXmlElement *_animationXML);
	void decodeMovement(TiXmlElement *_movementXML, AnimationData *_aniData);
	void decodeMovementBone(TiXmlElement* _movBoneXml, TiXmlElement* _parentXml, BoneData *_boneData, MovementData *_movementData);
	FrameData *decodeFrame(TiXmlElement* _frameXML, TiXmlElement* _parentFrameXml, BoneData *_boneData, MovementBoneData *_movBoneData);

	
    /**
	 *	@brief	decode Texture Datas from xml export from Dragon Bone flash tool
     */
	void decodeTexture(TiXmlElement *_textureXML);
    
    /**
	 *	@brief	decode Contour Datas from xml export from Dragon Bone flash tool
     */
    void decodeContour(TiXmlElement *_contourXML, TextureData *_textureData);
    
   
private:
    /**
	 *	@brief	save amature datas
	 *  @key	std::string
	 *  @value	ArmatureData *
     */
	CC_SYNTHESIZE_READONLY(CCDictionary *, m_pArmarureDatas, ArmarureDatas);

    /**
	 *	@brief	save animation datas
	 *  @key	std::string
	 *  @value	AnimationData *
     */
	CC_SYNTHESIZE_READONLY(CCDictionary *, m_pAnimationDatas, AnimationDatas);

	/**
	 *	@brief	save texture datas
	 *  @key	std::string
	 *  @value	TextureData *
     */
	CC_SYNTHESIZE_READONLY(CCDictionary *, m_pTextureDatas, TextureDatas);

private:
    static ArmatureDataManager *m_sSharedArmatureDataManager;
    
    //! a std::vector save the xml file already added
    std::vector<std::string> m_arrXMLFileList;
    
    /**
	 *	@brief	save ArmatureFileInfo datas
	 *  @key	std::string
	 *  @value	ArmatureFileInfo*
     */
    CCDictionary *m_pArmatureFileInfoDic;
};

}
}