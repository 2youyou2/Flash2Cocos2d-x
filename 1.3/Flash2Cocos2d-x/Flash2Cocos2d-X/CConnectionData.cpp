#include "CConnectionData.h"

CCConnectionData *CCConnectionData::sSharedConnectionData = NULL;

CCConnectionData *CCConnectionData::sharedConnectionData() {
    if (sSharedConnectionData == NULL) {
        sSharedConnectionData = new CCConnectionData();
        if (!sSharedConnectionData || !sSharedConnectionData->init()) {
            CC_SAFE_DELETE(sSharedConnectionData);
        }
    }
    return sSharedConnectionData;
}

CCConnectionData::CCConnectionData(void) {
}


CCConnectionData::~CCConnectionData(void) {
}

bool CCConnectionData::init() {
    bool bRet = false;
    do {

        mArmarureDatas = new CCDictionary();
        CCAssert(mArmarureDatas, "create CCConnectionData::mArmarureDatas fail!");

        mAnimationDatas = new CCDictionary();
        CCAssert(mAnimationDatas, "create CCConnectionData::mAnimationDatas fail!");

		mTextureDatas = new CCDictionary();
		CCAssert(mTextureDatas, "");

        bRet = true;
    } while (0);

    return bRet;
}

CCArray *CCConnectionData::getArmatureData(const char* _id) {
    return (CCArray *)mArmarureDatas->objectForKey(_id);
}

CCArmatureAniData* CCConnectionData::getAnimationData(const char* _id) {
    return (CCArmatureAniData*)mAnimationDatas->objectForKey(_id);
}

void CCConnectionData::addData(const char* _xml) {
    //CCXLOG("Enter CCConnectionData::addData");


    unsigned long	_size;
    char			*_pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(_xml , "r", &_size);


    TiXmlDocument	_document;
    _document.Parse(_pFileContent, 0, TIXML_ENCODING_UTF8);

    TiXmlElement	*_root = _document.RootElement();
	CCAssert(_root, "XML error  or  XML is empty.");

	TiXmlElement *_texturesXML = _root->FirstChildElement(TEXTURE_ATLAS);
	decodeTextures(_texturesXML);

    TiXmlElement *_armaturesXML = _root->FirstChildElement(ARMATURES);
    decodeArmatures(_armaturesXML);

	TiXmlElement *_animationsXML = _root->FirstChildElement(ANIMATIONS);
	decodeAnimations(_animationsXML);

    
    //CCXLOG("Finish CCConnectionData::addData");
}

void CCConnectionData::decodeTextures(TiXmlElement *_texturesXML){
	TiXmlElement *_textureXML = _texturesXML->FirstChildElement(SUB_TEXTURE);

	while(_textureXML){
		CCTexutreData *_texture = CCTexutreData::create();
		_texture->name = _textureXML->Attribute(A_NAME);
		_textureXML->QueryFloatAttribute(A_PIVOT_X, &_texture->pivotX);
		_textureXML->QueryFloatAttribute(A_PIVOT_Y, &_texture->pivotY);
		_textureXML->QueryFloatAttribute(A_WIDTH, &_texture->width);
		_textureXML->QueryFloatAttribute(A_HEIGHT, &_texture->height);


		mTextureDatas->setObject(_texture, _texture->name);

		_textureXML = _textureXML->NextSiblingElement(SUB_TEXTURE);
	}
}

void CCConnectionData::decodeArmatures(TiXmlElement *_armaturesXML){
	TiXmlElement *_armatureXML = _armaturesXML->FirstChildElement(ARMATURE);
	while (_armatureXML) {
		const char*	_name = _armatureXML->Attribute(A_NAME);
		
		CCArray *_newDatas = CCArray::create();
		mArmarureDatas->setObject(_newDatas, _name);

		TiXmlElement* _child = _armatureXML->FirstChildElement(BONE);

		while( _child ) {
			CCArmatureData *_armatureData = CCArmatureData::create();

			if( _child->Attribute(A_NAME) ){
				_armatureData->name = _child->Attribute(A_NAME);
			}
			if( _child->Attribute(A_BONE_IMAGE) ){
				_armatureData->boneImage = _child->Attribute(A_BONE_IMAGE);
			}
			if( _child->Attribute(A_PARENT) ) {
				_armatureData->parent = _child->Attribute(A_PARENT);
			}

			_child->QueryFloatAttribute(A_X, &(_armatureData->x));
			_child->QueryFloatAttribute(A_Y, &(_armatureData->y));
			_child->QueryFloatAttribute(A_SKEW_X, &(_armatureData->skewX));
			_child->QueryFloatAttribute(A_SKEW_Y, &(_armatureData->skewY));
			_child->QueryFloatAttribute(A_Z, &(_armatureData->z));

			_armatureData->y = -_armatureData->y;
			_armatureData->skewY = -_armatureData->skewY;
			_armatureData->rotation = _armatureData->skewX;

			CCTexutreData *_textureData = (CCTexutreData *)mTextureDatas->objectForKey(_armatureData->boneImage);
			CCAssert(_textureData, "");

			_armatureData->pivotX = _textureData->pivotX;
			_armatureData->pivotY = _textureData->pivotY;
			_armatureData->width = _textureData->width;
			_armatureData->height = _textureData->height;

			//CCXLOG( "%s, %f, %f, %f, %f" , _armatureData->name.c_str(), _armatureData->pivotX, _armatureData->pivotY, _armatureData->x, _armatureData->y);
			//CCXLOG( "%s, %f, %f, %f, %f" , _armatureData->name.c_str(), _armatureData->width, _armatureData->height, _armatureData->pivotX, _armatureData->pivotY);

			_newDatas->addObject( _armatureData );	
		
			_child = _child->NextSiblingElement(BONE);
		}
		_armatureXML = _armatureXML->NextSiblingElement(ARMATURE);
	}
}

void CCConnectionData::decodeAnimations(TiXmlElement *_animationsXML){
	TiXmlElement *_animationXML = _animationsXML->FirstChildElement(ANIMATION);
	while (_animationXML) {
		const char*	_name = _animationXML->Attribute(A_NAME);

		CCArmatureAniData	*_aniData = (CCArmatureAniData*)mAnimationDatas->objectForKey(_name);
		if (_aniData) {
			CCXLOG("CCArmatureAniData (%s) is already exit", _name);
			return;
		}

		_aniData =  CCArmatureAniData::create();

		mAnimationDatas->setObject( _aniData, _name);



		TiXmlElement* _movement = _animationXML->FirstChildElement(MOVEMENT);

		while( _movement ) {

			const char* _aniName;
			const char* _boneName;
			CCBoneAniData* _boneAniData = NULL;

			_aniName = _movement->Attribute(A_NAME);

			_boneAniData = CCBoneAniData::create();
			_aniData->addAnimation(_aniName, _boneAniData);

			int _duration, _durationTo, _durationTween, _loop = 0;
			float _tweenEasing = 0.0f;

			if( _movement->QueryIntAttribute(A_DURATION, &(_duration)) == TIXML_SUCCESS){
				_boneAniData->setDuration(_duration);
			}
			if( _movement->QueryIntAttribute(A_DURATION_TO, &(_durationTo)) == TIXML_SUCCESS){
				_boneAniData->setDurationTo(_durationTo);
			}
			if( _movement->QueryIntAttribute(A_DURATION_TWEEN, &(_durationTween)) == TIXML_SUCCESS){
				_boneAniData->setDurationTween(_durationTween);
			}
			if( _movement->QueryIntAttribute(A_LOOP, &(_loop)) == TIXML_SUCCESS){
				_boneAniData->setLoop((bool)_loop);
			}
			if( _movement->QueryFloatAttribute(A_TWEEN_EASING, &(_tweenEasing)) == TIXML_SUCCESS){
				_boneAniData->setTweenEasing(_tweenEasing);
			}

			TiXmlElement *_bone = _movement->FirstChildElement(BONE);
			while(_bone) {
				_boneName = _bone->Attribute(A_NAME);
				//CCXLOG(_boneName);
				if (_boneAniData->getAnimation(_boneName)) {
					_bone = _bone->NextSiblingElement();
					continue;
				}

				_boneAniData->addAnimation(getFrameNodeList(_bone, _boneName), _boneName);
				
				_bone = _bone->NextSiblingElement(BONE);
			}


			/*TiXmlElement *_eventFrameElement = _movement->FirstChildElement(EVENT_FRAME);
			if(_eventFrameElement) {
				CCEventFrame* _eventFrame;
				int _frame = 0;
				while( _eventFrameElement ) {
					int _frames;
					_eventFrameElement->QueryIntAttribute(FRAME, &_frames);
					_eventFrame = CCEventFrame::create(_eventFrameElement->Attribute(A_NAME), _frames);
					_boneAniData->getEventList()->addObject(_eventFrame);
					_frame += _eventFrame->getFrame();

					_eventFrameElement = _eventFrameElement->NextSiblingElement(EVENT_FRAME);
				}

				_boneAniData->getEventList()->insertObject(CCEventFrame::create("init", _boneAniData->getFrame() - _frame), 0);
			}*/

			_movement = _movement->NextSiblingElement(MOVEMENT);

		}
		_animationXML = _animationXML->NextSiblingElement(ANIMATION);

	}
}

CCFrameNodeList* CCConnectionData::getFrameNodeList(TiXmlElement* _frameXMLList, const char* _boneName) {
    CCFrameNodeList* _nodeList = CCFrameNodeList::create();
    float _scale, _delay;

    if( _frameXMLList ) {
        if( _frameXMLList->QueryFloatAttribute(A_MOVEMENT_SCALE, &_scale) == TIXML_SUCCESS ) {
            _nodeList->setScale(_scale);
        }
        if( _frameXMLList->QueryFloatAttribute(A_MOVEMENT_DELAY, &_delay) == TIXML_SUCCESS ) {
            _nodeList->setDelay(_delay);
        }
    }

	TiXmlElement *_boneFrame= _frameXMLList->FirstChildElement(FRAME);

    while( _boneFrame ) {
        _nodeList->addFrame( getFrameNode( _boneFrame ) );
        _boneFrame = _boneFrame->NextSiblingElement(FRAME);
    }
    return _nodeList;
}

CCFrameNode* CCConnectionData::getFrameNode(TiXmlElement* _nodeXML) {
    float _x, _y, _scale_x, _scale_y, _skew_x, _skew_y, _rotation, _off_r, _alpha = 0;
    int _frame = 0;
	int _hide = 0;

    CCFrameNode* _node = CCFrameNode::create();
	
	if( _nodeXML->Attribute(A_IMAGE) != NULL )
	{
		_node->img = _nodeXML->Attribute(A_IMAGE);
	}
	

	if(_nodeXML->QueryFloatAttribute(A_ROTATION, &_rotation) == TIXML_SUCCESS){
		_node->rotation = _rotation;
	}
	if(_nodeXML->QueryFloatAttribute(A_X, &_x) == TIXML_SUCCESS){
		_node->x = _x;
	}
	if(_nodeXML->QueryFloatAttribute(A_Y, &_y) == TIXML_SUCCESS){
		_node->y = -_y;
	}
	if( _nodeXML->QueryFloatAttribute(A_SCALE_X, &_scale_x) == TIXML_SUCCESS ) {
        _node->scaleX = _scale_x;
    }
    if( _nodeXML->QueryFloatAttribute(A_SCALE_Y, &_scale_y) == TIXML_SUCCESS ) {
        _node->scaleY = _scale_y;
    }
	if( _nodeXML->QueryFloatAttribute(A_SKEW_X, &_skew_x) == TIXML_SUCCESS ) {
		_node->skewX = _skew_x;
		//_node->rotation = _skew_x;
	}
	if( _nodeXML->QueryFloatAttribute(A_SKEW_Y, &_skew_y) == TIXML_SUCCESS ) {
		_node->skewY = -_skew_y;
	}
	if( _nodeXML->QueryIntAttribute(A_DURATION, &_frame) == TIXML_SUCCESS ) {
		_node->frame = _frame;
	}
    if( _nodeXML->QueryFloatAttribute(A_ALPHA, &_alpha) == TIXML_SUCCESS ) {
        _node->alpha = _alpha * 255;
    }
	if(  _nodeXML->QueryIntAttribute(A_HIDE, &_hide) == TIXML_SUCCESS ) {
		_node->visible = !(bool)_hide;
	}
	

	/*if( _nodeXML->QueryFloatAttribute(A_OFF_R, &_off_r) == TIXML_SUCCESS ) {
		_node->offR = _off_r;
	}*/


    

    return _node;
}

void CCConnectionData::removeAll(bool _deletMe) {
    if( mAnimationDatas ) {
        mAnimationDatas->removeAllObjects();
		delete mAnimationDatas;
		mAnimationDatas = NULL;
    }
    if( mArmarureDatas ) {
        mArmarureDatas->removeAllObjects();
		delete mArmarureDatas;
		mArmarureDatas = NULL;
    }
	if( mTextureDatas ){
		mTextureDatas->removeAllObjects();
		delete mTextureDatas;
		mTextureDatas = NULL;
	}
    
}