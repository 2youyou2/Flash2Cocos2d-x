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

#include "SPArmatureDataManager.h"
#include "SPTransformHelp.h"

namespace SPII
{
namespace SPArmature
{


ArmatureDataManager *ArmatureDataManager::m_sSharedArmatureDataManager = NULL;

ArmatureDataManager *ArmatureDataManager::sharedArmatureDataManager()
{
    if (m_sSharedArmatureDataManager == NULL)
    {
        m_sSharedArmatureDataManager = new ArmatureDataManager();
        if (!m_sSharedArmatureDataManager || !m_sSharedArmatureDataManager->init())
        {
            CC_SAFE_DELETE(m_sSharedArmatureDataManager);
        }
    }
    return m_sSharedArmatureDataManager;
}

ArmatureDataManager::ArmatureDataManager(void)
{
    m_pAnimationDatas = NULL;
    m_pArmarureDatas = NULL;
    m_pTextureDatas = NULL;
    m_pArmatureFileInfoDic = NULL;
}


ArmatureDataManager::~ArmatureDataManager(void)
{
    
    CC_SAFE_DELETE(m_pAnimationDatas);
    CC_SAFE_DELETE(m_pArmarureDatas);
    CC_SAFE_DELETE(m_pTextureDatas);
    CC_SAFE_DELETE(m_pArmatureFileInfoDic);
}

bool ArmatureDataManager::init()
{
    bool bRet = false;
    do
    {

        m_pArmarureDatas = CCDictionary::create();
        CCAssert(m_pArmarureDatas, "create ArmatureDataManager::m_pArmarureDatas fail!");
        m_pArmarureDatas->retain();
        
        m_pAnimationDatas = CCDictionary::create();
        CCAssert(m_pAnimationDatas, "create ArmatureDataManager::m_pAnimationDatas fail!");
        m_pAnimationDatas->retain();

        m_pTextureDatas = CCDictionary::create();
        CCAssert(m_pTextureDatas, "create ArmatureDataManager::m_pTextureDatas fail!");
        m_pTextureDatas->retain();
        
        m_pArmatureFileInfoDic = CCDictionary::create();
        CCAssert(m_pArmatureFileInfoDic, "create ArmatureDataManager::m_pArmatureFileInfoDic fail!");
        m_pArmatureFileInfoDic->retain();

        bRet = true;
    }
    while (0);

    return bRet;
}

ArmatureData *ArmatureDataManager::getArmatureData(const char* _id)
{
    ArmatureData * _armatureData = NULL;
    if (m_pArmarureDatas)
    {
        _armatureData = (ArmatureData*)m_pArmarureDatas->objectForKey(_id);
    }
    return _armatureData;
}

AnimationData* ArmatureDataManager::getAnimationData(const char* _id)
{
    AnimationData *_animationData = NULL;
    if (m_pAnimationDatas)
    {
        _animationData = (AnimationData*)m_pAnimationDatas->objectForKey(_id);
    }
    return _animationData;
}

TextureData *ArmatureDataManager::getTextureData(const char* _id)
{
    TextureData *_textureData = NULL;
    if (m_pTextureDatas)
    {
        _textureData = (TextureData*)m_pTextureDatas->objectForKey(_id);
    }
    return _textureData;
}
    
ArmatureFileInfo *ArmatureDataManager::getArmatureFileInfo(const char *_id)
{
    ArmatureFileInfo *_armatureFileInfo = NULL;
    if (m_pArmatureFileInfoDic)
    {
        _armatureFileInfo = (ArmatureFileInfo*)m_pArmatureFileInfoDic->objectForKey(_id);
    }
    return _armatureFileInfo;
}

void ArmatureDataManager::addDataFromXML(const char* _xml)
{
    /*
     *	check if xml is already added to ArmatureDataManager, if then return.
     */
    for(int _i = 0; _i<m_arrXMLFileList.size(); _i++)
    {
        if (m_arrXMLFileList[_i].compare(_xml) == 0)
        {
            return;
        }
    }

    /*
     *  Need to get the full path of the xml file, or the Tiny XML can't find the xml at IOS
     */
    const char *fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(_xml);

    /*
     *  Need to read the tiny xml into memory first, or the Tiny XML can't find the xml at IOS
     */
    unsigned long	_size;
    const char *_pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullPath , "r", &_size);


    TiXmlDocument	_document;
    _document.Parse(_pFileContent, 0, TIXML_ENCODING_UTF8);

    TiXmlElement	*_root = _document.RootElement();
    CCAssert(_root, "XML error  or  XML is empty.");

    /*
     *  begin decode armature data from xml
     */
    TiXmlElement *_armaturesXML = _root->FirstChildElement(ARMATURES);
    TiXmlElement *_armatureXML = _armaturesXML->FirstChildElement(ARMATURE);
    while(_armatureXML)
    {
        decodeArmature(_armatureXML);
        _armatureXML = _armatureXML->NextSiblingElement(ARMATURE);
    }

    /*
     *  begin decode animation data from xml
     */
    TiXmlElement *_animationsXML = _root->FirstChildElement(ANIMATIONS);
    TiXmlElement *_animationXML = _animationsXML->FirstChildElement(ANIMATION);
    while(_animationXML)
    {
        decodeAnimation(_animationXML);
        _animationXML = _animationXML->NextSiblingElement(ANIMATION);
    }

    /*
     *  begin decode texture data from xml
     */
    TiXmlElement *_texturesXML = _root->FirstChildElement(TEXTURE_ATLAS);
    TiXmlElement *_textureXML = _texturesXML->FirstChildElement(SUB_TEXTURE);
    while(_textureXML)
    {
        decodeTexture(_textureXML);
        _textureXML = _textureXML->NextSiblingElement(SUB_TEXTURE);
    }


    _document.Clear();

    m_arrXMLFileList.push_back(_xml);
}
    
void ArmatureDataManager::addTextureDataFromXML(const char *_xmlPath)
{
    /*
     *	check if xml is already added to ArmatureDataManager, if then return.
     */
    for(int _i = 0; _i<m_arrXMLFileList.size(); _i++)
    {
        if (m_arrXMLFileList[_i].compare(_xmlPath) == 0)
        {
            return;
        }
    }
    
    /*
     *  Need to get the full path of the xml file, or the Tiny XML can't find the xml at IOS
     */
    const char *fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(_xmlPath);
    
    /*
     *  Need to read the tiny xml into memory first, or the Tiny XML can't find the xml at IOS
     */
    unsigned long	_size;
    const char *_pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(fullPath , "r", &_size);
    
    
    TiXmlDocument	_document;
    _document.Parse(_pFileContent, 0, TIXML_ENCODING_UTF8);
    
    TiXmlElement	*_root = _document.RootElement();
    CCAssert(_root, "XML error  or  XML is empty.");
    
    /*
     *  begin decode texture data from xml
     */
    TiXmlElement *_texturesXML = _root->FirstChildElement(TEXTURE_ATLAS);
    TiXmlElement *_textureXML = _texturesXML->FirstChildElement(SUB_TEXTURE);
    while(_textureXML)
    {
        decodeTexture(_textureXML);
        _textureXML = _textureXML->NextSiblingElement(SUB_TEXTURE);
    }
    
    
    _document.Clear();
    
    m_arrXMLFileList.push_back(_xmlPath);
}


void ArmatureDataManager::addArmatureFileInfo(ArmatureFileInfo *_fileInfo)
{
    m_pArmatureFileInfoDic->setObject(_fileInfo, _fileInfo->armatureName);

	if (_fileInfo->useExistFileInfo.compare("") != 0)
	{
		_fileInfo = (ArmatureFileInfo*)m_pArmatureFileInfoDic->objectForKey(_fileInfo->useExistFileInfo);
	}

	addDataFromXML(_fileInfo->xmlFilePath.c_str());
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(_fileInfo->plistPath.c_str(), _fileInfo->imagePath.c_str());
	
}

void ArmatureDataManager::addArmatureFileInfo(const char *_armatureName, const char *_useExistFileInfo, const char *_imagePath, const char *_plistPath, const char *_xmlFilePath)
{
    ArmatureFileInfo *_armatureFileInfo = ArmatureFileInfo::create();
    _armatureFileInfo->armatureName = _armatureName;
    _armatureFileInfo->imagePath = _imagePath;
    _armatureFileInfo->plistPath = _plistPath;
    _armatureFileInfo->xmlFilePath = _xmlFilePath;
    _armatureFileInfo->useExistFileInfo = _useExistFileInfo;

    addArmatureFileInfo(_armatureFileInfo);
}

void ArmatureDataManager::decodeArmature(TiXmlElement *_armatureXML)
{
    const char*	_name = _armatureXML->Attribute(A_NAME);

    ArmatureData	*_armatureData = (ArmatureData*)m_pArmarureDatas->objectForKey(_name);
    if (_armatureData)
    {
        //CCLOG("ArmatureData (%s) is already exit.And we will do nothing .", _name);
        return;
    }

    _armatureData = ArmatureData::create();
    _armatureData->setName(_name);
    m_pArmarureDatas->setObject(_armatureData, _name);


    TiXmlElement* _boneXML = _armatureXML->FirstChildElement(BONE);

    while( _boneXML )
    {
        /*
         *  If this bone have parent, then get the parent bone xml
         */
        const char *_parentName = _boneXML->Attribute(A_PARENT);
        TiXmlElement *_parentXML = NULL;
        if (_parentName)
        {
            _parentXML = _armatureXML->FirstChildElement(BONE);
            std::string _name = _parentName;
            while (_parentXML)
            {
                if (_name.compare(_parentXML->Attribute(A_NAME)) == 0)
                {
                    break;
                }
                _parentXML = _parentXML->NextSiblingElement(BONE);
            }
        }

        decodeBone(_boneXML, _parentXML, _armatureData);
        _boneXML = _boneXML->NextSiblingElement(BONE);
    }

}



void ArmatureDataManager::decodeBone(TiXmlElement *_boneXML, TiXmlElement *_parentXML, ArmatureData *_armatureData)
{

    std::string _name = _boneXML->Attribute(A_NAME);

    CCAssert(_name.compare("") != 0, "");

    if( _armatureData->getBoneDatas()->objectForKey(_name) )
    {
        //CCLOG("ArmatureData (%s) is already exit.And we will do nothing .", _name.c_str());
        return;
    }

    BoneData *_boneData = BoneData::create();

    _boneData->m_strName = _name;

    if( _boneXML->Attribute(A_PARENT) != NULL )
    {
        _boneData->m_strParent = _boneXML->Attribute(A_PARENT);
    }

    _boneXML->QueryFloatAttribute(A_X,		&(_boneData->m_fX));
    _boneXML->QueryFloatAttribute(A_Y,		&(_boneData->m_fY));
    _boneXML->QueryFloatAttribute(A_SKEW_X, &(_boneData->m_fSkewX));
    _boneXML->QueryFloatAttribute(A_SKEW_Y, &(_boneData->m_fSkewY));
    _boneXML->QueryFloatAttribute(A_SCALE_X,&(_boneData->m_fScaleX));
    _boneXML->QueryFloatAttribute(A_SCALE_Y,&(_boneData->m_fScaleY));
    _boneXML->QueryIntAttribute(A_Z,		&(_boneData->m_iZOrder));
//    _boneXML->QueryFloatAttribute(A_PIVOT_X,&(_boneData->m_fPivotX));
//    _boneXML->QueryFloatAttribute(A_PIVOT_Y,&(_boneData->m_fPivotY));
//    _boneXML->QueryFloatAttribute(A_WIDTH,	&(_boneData->m_fWidth));
//    _boneXML->QueryFloatAttribute(A_HEIGHT, &(_boneData->m_fHeight));

    _boneXML->QueryIntAttribute(A_ALPHA,	&(_boneData->m_iAlpha));
    _boneXML->QueryIntAttribute(A_RED,		&(_boneData->m_iRed));
    _boneXML->QueryIntAttribute(A_GREEN,	&(_boneData->m_iGreen));
    _boneXML->QueryIntAttribute(A_BLUE,		&(_boneData->m_iBlue));

    // flash export data is a percent value, so wo change it
    _boneData->m_iAlpha *= 2.55;
    _boneData->m_iRed *= 2.55;
    _boneData->m_iGreen *= 2.55;
    _boneData->m_iBlue *= 2.55;
    
    /*
     *  Note : Flash's Y value contrary to Cocos2dX's Y value
     */
    _boneData->m_fY = -_boneData->m_fY;

    /*
     *  Note : Flash export datas are DEGREE values, and our matrix use RADIAN values
     */
    _boneData->m_fSkewX = CC_DEGREES_TO_RADIANS(_boneData->m_fSkewX);
    _boneData->m_fSkewY = CC_DEGREES_TO_RADIANS(-_boneData->m_fSkewY);


    if(_parentXML)
    {
        /*
         *  recalculate bone data from parent bone data, use for translate matrix
         */
        Node _helpNode;
        _parentXML->QueryFloatAttribute(A_X, &_helpNode.m_fX);
        _parentXML->QueryFloatAttribute(A_Y, &_helpNode.m_fY);
        _parentXML->QueryFloatAttribute(A_SKEW_X, &_helpNode.m_fSkewX);
        _parentXML->QueryFloatAttribute(A_SKEW_Y, &_helpNode.m_fSkewY);

        _helpNode.m_fY = -_helpNode.m_fY;
        _helpNode.m_fSkewX = CC_DEGREES_TO_RADIANS(_helpNode.m_fSkewX);
        _helpNode.m_fSkewY = CC_DEGREES_TO_RADIANS(-_helpNode.m_fSkewY);

        TransformHelp::transformFromParent(*_boneData, _helpNode);
    }


    TiXmlElement *_displayXML = _boneXML->FirstChildElement(DISPLAY);
    while(_displayXML)
    {
        decodeBoneDisplay(_displayXML, _boneData);
        _displayXML = _displayXML->NextSiblingElement(DISPLAY);
    }



    _armatureData->getBoneDatas()->setObject( _boneData, _boneData->m_strName);

}

void ArmatureDataManager::decodeBoneDisplay(TiXmlElement *_displayXML, BoneData *_boneData)
{
    int _isArmature = 0;

    DisplayData *_displayData = DisplayData::create();
    if(_displayXML->Attribute(A_NAME) != NULL )
    {
        _displayData->m_strDisplayName = _displayXML->Attribute(A_NAME);
    }

    if( _displayXML->QueryIntAttribute(A_IS_ARMATURE, &(_isArmature)) == TIXML_SUCCESS )
    {
        if(_isArmature)
        {
            _displayData->m_eDisplayType = SP_ARMATURE;
        }
        else
        {
            _displayData->m_eDisplayType = SP_SPRITE;
        }
        
    }

    _boneData->m_pDisplayDataList->addObject(_displayData);
}



void ArmatureDataManager::decodeAnimation(TiXmlElement *_animationXML)
{
    const char*	_name = _animationXML->Attribute(A_NAME);

    AnimationData	*_aniData = (AnimationData*)m_pAnimationDatas->objectForKey(_name);
    if (_aniData)
    {
        //CCLOG("AnimationData (%s) is already exit", _name);
        return;
    }

    _aniData =  AnimationData::create();

    m_pAnimationDatas->setObject( _aniData, _name);
    _aniData->setName(_name);

    TiXmlElement* _movementXML = _animationXML->FirstChildElement(MOVEMENT);

    while( _movementXML )
    {
        decodeMovement(_movementXML, _aniData);
        _movementXML = _movementXML->NextSiblingElement(MOVEMENT);

    }

}

void ArmatureDataManager::decodeMovement(TiXmlElement *_movementXML, AnimationData *_aniData)
{
    const char* _aniName = _movementXML->Attribute(A_NAME);

    MovementData *_movementData = MovementData::create();
    _aniData->addMovment(_aniName, _movementData);


    int _duration, _durationTo, _durationTween, _loop = 0;
    float _tweenEasing = 0.0f;

    if( _movementXML->QueryIntAttribute(A_DURATION, &(_duration)) == TIXML_SUCCESS)
    {
        _movementData->setDuration(_duration);
    }
    if( _movementXML->QueryIntAttribute(A_DURATION_TO, &(_durationTo)) == TIXML_SUCCESS)
    {
        _movementData->setDurationTo(_durationTo);
    }
    if( _movementXML->QueryIntAttribute(A_DURATION_TWEEN, &(_durationTween)) == TIXML_SUCCESS)
    {
        _movementData->setDurationTween(_durationTween);
    }
    if( _movementXML->QueryIntAttribute(A_LOOP, &(_loop)) == TIXML_SUCCESS)
    {
        _movementData->setLoop((bool)_loop);
    }
    
    const char * _easing= _movementXML->Attribute(A_TWEEN_EASING);
    if(_easing != NULL)
    {
        std::string str = _easing;
        if(str.compare(FL_NAN) != 0)
        {
            if( _movementXML->QueryFloatAttribute(A_TWEEN_EASING, &(_tweenEasing)) == TIXML_SUCCESS)
            {
                _movementData->setTweenEasing(_tweenEasing);
            }
        }else{
            _movementData->setTweenEasing(TWEEN_EASING_MAX);
        }
    }

    TiXmlElement *_movBoneXml = _movementXML->FirstChildElement(BONE);
    while(_movBoneXml)
    {
        const char* _boneName = _movBoneXml->Attribute(A_NAME);

        if (_movementData->getMovementBoneData(_boneName))
        {
            _movBoneXml = _movBoneXml->NextSiblingElement();
            continue;
        }

        ArmatureData *_armatureData = (ArmatureData*)m_pArmarureDatas->objectForKey(_aniData->getName());

        BoneData *_boneData = (BoneData*)_armatureData->getBoneDatas()->objectForKey(_boneName);

        std::string _parentName = _boneData->m_strParent;


        TiXmlElement *_parentXML = NULL;
        if (_parentName.compare("") != 0)
        {
            _parentXML = _movementXML->FirstChildElement(BONE);

            while (_parentXML)
            {
                if (_parentName.compare(_parentXML->Attribute(A_NAME)) == 0)
                {
                    break;
                }
                _parentXML = _parentXML->NextSiblingElement(BONE);
            }
        }

        decodeMovementBone(_movBoneXml, _parentXML, _boneData, _movementData);

        _movBoneXml = _movBoneXml->NextSiblingElement(BONE);
    }


}


void ArmatureDataManager::decodeMovementBone(TiXmlElement* _movBoneXml, TiXmlElement* _parentXml, BoneData *_boneData, MovementData *_movementData)
{
    MovementBoneData* _movBoneData = MovementBoneData::create();
    float _scale, _delay;

    if( _movBoneXml )
    {
        if( _movBoneXml->QueryFloatAttribute(A_MOVEMENT_SCALE, &_scale) == TIXML_SUCCESS )
        {
            _movBoneData->setScale(_scale);
        }
        if( _movBoneXml->QueryFloatAttribute(A_MOVEMENT_DELAY, &_delay) == TIXML_SUCCESS )
        {
            if(_delay > 0)
            {
                _delay -= 1;
            }
            _movBoneData->setDelay(_delay);
        }
    }

    int _length = 0;
    int _i = 0;
    int _parentTotalDuration = 0;
    int _currentDuration = 0;

    TiXmlElement *_parentFrameXML = NULL;

    std::vector<TiXmlElement*> _parentXMLList;

    /*
     *  get the parent frame xml list, we need get the origin data
     */
    if( _parentXml != NULL )
    {
        _parentFrameXML = _parentXml->FirstChildElement(FRAME);
        while (_parentFrameXML)
        {
            _parentXMLList.push_back(_parentFrameXML);
            _parentFrameXML = _parentFrameXML->NextSiblingElement(FRAME);
        }

        _parentFrameXML = NULL;

        _length = _parentXMLList.size();
    }


    int _totalDuration =0;

    std::string name = _movBoneXml->Attribute(A_NAME);

    TiXmlElement *_frameXML= _movBoneXml->FirstChildElement(FRAME);

    while( _frameXML )
    {
        if(_parentXml)
        {
            /*
             *  in this loop we get the corresponding parent frame xml
             */
            while(_i < _length && (_parentFrameXML?(_totalDuration < _parentTotalDuration || _totalDuration >= _parentTotalDuration + _currentDuration):true))
            {
                _parentFrameXML = _parentXMLList[_i];
                _parentTotalDuration += _currentDuration;
                _parentFrameXML->QueryIntAttribute(A_DURATION, &_currentDuration);
                _i++;

            }
        }

        FrameData * _frameData = decodeFrame( _frameXML, _parentFrameXML, _boneData, _movBoneData);

        _totalDuration += _frameData->m_iDuration;

        _frameXML = _frameXML->NextSiblingElement(FRAME);
    }

    _movementData->addMovementBoneData(_movBoneData, name.c_str());
}

FrameData * ArmatureDataManager::decodeFrame(TiXmlElement* _frameXML,  TiXmlElement* _parentFrameXml, BoneData *_boneData, MovementBoneData *_movBoneData)
{
    float _x, _y, _scale_x, _scale_y, _skew_x, _skew_y, _tweenEasing = 0;
    int _duration, _displayIndex, _zOrder, _alpha, _red, _green, _blue = 0;

    FrameData* _node = FrameData::create();


    if(_frameXML->Attribute(A_MOVEMENT) != NULL)
    {
        _node->m_strMovement = _frameXML->Attribute(A_MOVEMENT);
    }
    if(_frameXML->Attribute(A_EVENT) != NULL)
    {
        _node->m_strEvent = _frameXML->Attribute(A_EVENT);
    }
    if(_frameXML->Attribute(A_SOUND) != NULL)
    {
        _node->m_strSound = _frameXML->Attribute(A_SOUND);
    }
    if(_frameXML->Attribute(A_SOUND_EFFECT) != NULL)
    {
        _node->m_strSoundEffect = _frameXML->Attribute(A_SOUND_EFFECT);
    }
    if(_frameXML->QueryFloatAttribute(A_X, &_x) == TIXML_SUCCESS)
    {
        _node->m_fX = _x;
    }
    if(_frameXML->QueryFloatAttribute(A_Y, &_y) == TIXML_SUCCESS)
    {
        _node->m_fY = -_y;
    }
    if( _frameXML->QueryFloatAttribute(A_SCALE_X, &_scale_x) == TIXML_SUCCESS )
    {
        _node->m_fScaleX = _scale_x;
    }
    if( _frameXML->QueryFloatAttribute(A_SCALE_Y, &_scale_y) == TIXML_SUCCESS )
    {
        _node->m_fScaleY = _scale_y;
    }
    if( _frameXML->QueryFloatAttribute(A_SKEW_X, &_skew_x) == TIXML_SUCCESS )
    {
        _node->m_fSkewX = CC_DEGREES_TO_RADIANS(_skew_x);
    }
    if( _frameXML->QueryFloatAttribute(A_SKEW_Y, &_skew_y) == TIXML_SUCCESS )
    {
        _node->m_fSkewY = CC_DEGREES_TO_RADIANS(-_skew_y);
    }
    if( _frameXML->QueryIntAttribute(A_DURATION, &_duration) == TIXML_SUCCESS )
    {
        _node->m_iDuration = _duration;
    }
    if( _frameXML->QueryIntAttribute(A_ALPHA, &_alpha) == TIXML_SUCCESS )
    {
        _node->m_iAlpha = 2.55 * _alpha;
    }
    if( _frameXML->QueryIntAttribute(A_RED, &_red) == TIXML_SUCCESS )
    {
        _node->m_iRed = 2.55 * _red;
    }
    if( _frameXML->QueryIntAttribute(A_GREEN, &_green) == TIXML_SUCCESS )
    {
        _node->m_iGreen = 2.55 * _green;
    }
    if( _frameXML->QueryIntAttribute(A_BLUE, &_blue) == TIXML_SUCCESS )
    {
        _node->m_iBlue = 2.55 * _blue;
    }
    if(  _frameXML->QueryIntAttribute(A_DISPLAY_INDEX, &_displayIndex) == TIXML_SUCCESS )
    {
        _node->m_iDisplayIndex = _displayIndex;
    }
    if(  _frameXML->QueryIntAttribute(A_Z, &_zOrder) == TIXML_SUCCESS )
    {
        _node->m_iZOrder = _zOrder;
    }
    
    const char * _easing= _frameXML->Attribute(A_TWEEN_EASING);
    if(_easing != NULL)
    {
        std::string str = _easing;
        if(str.compare(FL_NAN) != 0)
        {
            if( _frameXML->QueryFloatAttribute(A_TWEEN_EASING, &(_tweenEasing)) == TIXML_SUCCESS)
            {
                _node->m_fTweenEasing = _tweenEasing;
            }
        }else{
            _node->m_fTweenEasing = TWEEN_EASING_MAX;
        }
    }

    if(_parentFrameXml)
    {
        /*
         *  recalculate frame data from parent frame data, use for translate matrix
         */
        Node _helpNode;
        _parentFrameXml->QueryFloatAttribute(A_X, &_helpNode.m_fX);
        _parentFrameXml->QueryFloatAttribute(A_Y, &_helpNode.m_fY);
        _parentFrameXml->QueryFloatAttribute(A_SKEW_X, &_helpNode.m_fSkewX);
        _parentFrameXml->QueryFloatAttribute(A_SKEW_Y, &_helpNode.m_fSkewY);

        _helpNode.m_fY = -_helpNode.m_fY;
        _helpNode.m_fSkewX = CC_DEGREES_TO_RADIANS(_helpNode.m_fSkewX);
        _helpNode.m_fSkewY = CC_DEGREES_TO_RADIANS(-_helpNode.m_fSkewY);

        TransformHelp::transformFromParent(*_node, _helpNode);
    }

    _node->m_fX -= _boneData->m_fX;
    _node->m_fY -= _boneData->m_fY;
    _node->m_fSkewX -= _boneData->m_fSkewX;
    _node->m_fSkewY -= _boneData->m_fSkewY;

    _movBoneData->addFrame(_node);

    return _node;
}

void ArmatureDataManager::decodeTexture(TiXmlElement *_textureXML)
{
    TextureData *_textureData = TextureData::create();

    if( _textureXML->Attribute(A_NAME) != NULL)
    {
        _textureData->m_strName = _textureXML->Attribute(A_NAME);
    }

    _textureXML->QueryFloatAttribute(A_PIVOT_X, &_textureData->m_fPivotX);
    _textureXML->QueryFloatAttribute(A_PIVOT_Y, &_textureData->m_fPivotY);
    _textureXML->QueryFloatAttribute(A_WIDTH, &_textureData->m_fWidth);
    _textureXML->QueryFloatAttribute(A_HEIGHT, &_textureData->m_fHeight);


    m_pTextureDatas->setObject(_textureData, _textureData->m_strName);
    
    TiXmlElement *_contourXML = _textureXML->FirstChildElement(CONTOUR);
    
    while (_contourXML) {
        
        decodeContour(_contourXML, _textureData);
        
        _contourXML = _contourXML->NextSiblingElement(CONTOUR);
    }

}
    
void ArmatureDataManager::decodeContour(TiXmlElement *_contourXML, TextureData *_textureData)
{
    ContourData *_contourData = ContourData::create();
    
    TiXmlElement *_vertexDataXML = _contourXML->FirstChildElement(CONTOUR_VERTEX);
    
    while (_vertexDataXML) {
        CCPoint *_vertex = new CCPoint(0, 0);
        _vertex->autorelease();
        
        _vertexDataXML->QueryFloatAttribute(A_X, &_vertex->x);
        _vertexDataXML->QueryFloatAttribute(A_Y, &_vertex->y);
    
        _vertex->y = -_vertex->y;
        _contourData->addVertex(_vertex);
        
        
        _vertexDataXML = _vertexDataXML->NextSiblingElement(CONTOUR_VERTEX);
    }
    
   CCLOG("vertex count :%i",  _contourData->getVertexCount());
    _textureData->addContourData(_contourData);
}
    


void ArmatureDataManager::removeAll()
{
    if( m_pAnimationDatas )
    {
        m_pAnimationDatas->removeAllObjects();
    }
    if( m_pArmarureDatas )
    {
        m_pArmarureDatas->removeAllObjects();
    }

    if( m_pTextureDatas )
    {
        m_pTextureDatas->removeAllObjects();
    }
    if( m_pArmatureFileInfoDic )
    {
        m_pArmatureFileInfoDic->removeAllObjects();
    }
    m_arrXMLFileList.clear();

}

}
}