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
    CC_SAFE_DELETE( mArmarureDatas );
    CC_SAFE_DELETE( mAnimationDatas );
    CC_SAFE_DELETE( sSharedConnectionData );
}

bool CCConnectionData::init() {
    bool bRet = false;
    do {

        mArmarureDatas = new CCDictionary();
        CCAssert(mArmarureDatas, "create CCConnectionData::mArmarureDatas fail!");

        mAnimationDatas = new CCDictionary();
        CCAssert(mAnimationDatas, "create CCConnectionData::mAnimationDatas fail!");

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


    unsigned long	size;
    char			*pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(_xml , "r", &size);


    TiXmlDocument	document;
    document.Parse(pFileContent, 0, TIXML_ENCODING_UTF8);


    TiXmlElement	*root = document.RootElement();
    if( !root ) {
        return;
    }

    TiXmlElement	*skeleton = root->FirstChildElement("skeleton");


    while (skeleton) {
        const char*	_name = skeleton->Attribute(NAME);
        //CCLOG(_name);
        CCArmatureAniData	*_aniData = (CCArmatureAniData*)mAnimationDatas->objectForKey(_name);

        if (_aniData) {
            return;
        }

        _aniData =  CCArmatureAniData::create();

        mAnimationDatas->setObject( _aniData, _name);

        //std::vector<PCCArmatureData> newDatas;
        CCArray *_newDatas = CCArray::create();
        mArmarureDatas->setObject(_newDatas, _name);

        TiXmlElement* child = skeleton->FirstChildElement();

        while( child ) {
            //CCLOG(child->Attribute(NAME));
            const char* value = child->Value();
			std::string valueStr = value;
            if( valueStr.compare("bone")==0 ) {
                CCArmatureData *_armatureData = CCArmatureData::create();
                _armatureData->name = child->Attribute(NAME);

                if( child->Attribute(PARENT) ) {
                    _armatureData->parent = child->Attribute(PARENT);
                }
                child->QueryFloatAttribute(X, &(_armatureData->x));
                child->QueryFloatAttribute(Y, &(_armatureData->y));
                _armatureData->y = -_armatureData->y;
                child->QueryFloatAttribute(Z, &(_armatureData->z));
                child->QueryFloatAttribute(WIDTH, &(_armatureData->width));
                child->QueryFloatAttribute(HEIGHT, &(_armatureData->height));
                child->QueryFloatAttribute(REGISTRATION_POINT_X, &(_armatureData->registrationPointX));
                child->QueryFloatAttribute(REGISTRATION_POINT_Y, &(_armatureData->registrationPointY));
                _armatureData->registrationPointY = -_armatureData->registrationPointY;

                //CCXLOG( "%s, %f, %f, %f, %f" , armatureData->name, armatureData->width, armatureData->height, armatureData->registrationPointX, armatureData->registrationPointY);

                ((CCArray*)mArmarureDatas->objectForKey(_name))->addObject( _armatureData );
            } else if( valueStr.compare("animation")==0 ) {
                const char* _aniName;
                const char* _boneName;
				std::string _boneNameStr;
                CCBoneAniData* _boneAniData;

                _aniName = child->Attribute(NAME);

                _boneAniData = CCBoneAniData::create();
                _aniData->addAnimation(_aniName, _boneAniData);

				int _frame = 0;
				if( child->QueryIntAttribute(FRAME, &(_frame)) == TIXML_SUCCESS){
					_boneAniData->setFrame(_frame);
				}



                TiXmlElement *_eventFrameElement = child->FirstChildElement(EVENT_FRAME);
                if(_eventFrameElement) {
                    CCEventFrame* _eventFrame;
                    int _frame = 0;
                    while( _eventFrameElement ) {
                        int _frames;
                        _eventFrameElement->QueryIntAttribute(FRAME, &_frames);
                        _eventFrame = CCEventFrame::create(_eventFrameElement->Attribute(NAME), _frames);
                        _boneAniData->getEventList()->addObject(_eventFrame);
                        _frame += _eventFrame->getFrame();

                        _eventFrameElement = _eventFrameElement->NextSiblingElement(EVENT_FRAME);
                    }

                    _boneAniData->getEventList()->insertObject(CCEventFrame::create("init", _boneAniData->getFrame() - _frame), 0);
                }

                TiXmlElement *frame = child->FirstChildElement();


                while(frame) {
                    _boneName = frame->Value();
                    _boneNameStr = _boneName;
                    if (_boneNameStr.compare(EVENT_FRAME) != 0) {
                        if (_boneAniData->getAnimation(_boneName)) {
                            frame = frame->NextSiblingElement();
                            continue;
                        }

                        _boneAniData->addAnimation(getFrameNodeList(child, _boneName), _boneName);
                    }
                    frame = frame->NextSiblingElement();
                }
            }

            child = child->NextSiblingElement();

        }

        skeleton = skeleton->NextSiblingElement("skeleton");

    }

    

    //CCXLOG("Finish CCConnectionData::addData");
}

CCFrameNodeList* CCConnectionData::getFrameNodeList(TiXmlElement* _frameXMLList, const char* _boneName) {
    CCFrameNodeList* _nodeList = CCFrameNodeList::create();
    float _scale, _delay;


    TiXmlElement *_boneFrame= _frameXMLList->FirstChildElement(_boneName);

    if( _boneFrame ) {
        if( _boneFrame->QueryFloatAttribute(SCALE, &_scale) == TIXML_SUCCESS ) {
            _nodeList->setScale(_scale);
        }
        if( _boneFrame->QueryFloatAttribute(DELAY, &_delay) == TIXML_SUCCESS ) {
            _nodeList->setDelay(_delay);
        }
    }

    while( _boneFrame ) {
        _nodeList->addFrame( getFrameNode( _boneFrame ) );
        _boneFrame = _boneFrame->NextSiblingElement(_boneName);
    }
    return _nodeList;
}

CCFrameNode* CCConnectionData::getFrameNode(TiXmlElement* _nodeXML) {
    float _x, _y, _scale_x, _scale_y, _rotation, _off_r, _alpha = 0;
    int _frame = 0;

    CCFrameNode* _node = NULL;


    if( _nodeXML->QueryFloatAttribute(ROTATION, &_rotation) == TIXML_SUCCESS &&
            _nodeXML->QueryFloatAttribute(X, &_x) == TIXML_SUCCESS &&
            _nodeXML->QueryFloatAttribute(Y, &_y) == TIXML_SUCCESS) {
        //_rotation = _rotation * Math.PI / 180;
        _node = CCFrameNode::create(_x, -_y, _rotation);
    } else {
        CCLOG( "Create FrameNode:%s fail!", _nodeXML->Value());
    }

    if( _nodeXML->QueryFloatAttribute(SCALE_X, &_scale_x) == TIXML_SUCCESS ) {
        _node->scaleX = _scale_x;
    }
    if( _nodeXML->QueryFloatAttribute(SCALE_Y, &_scale_y) == TIXML_SUCCESS ) {
        _node->scaleY = _scale_y;
    }
    if( _nodeXML->QueryFloatAttribute(OFF_R, &_off_r) == TIXML_SUCCESS ) {
        _node->offR = _off_r;
    }
    if( _nodeXML->QueryFloatAttribute(ALPHA, &_alpha) == TIXML_SUCCESS ) {
        _node->alpha = _alpha * 255;
    }
    if( _nodeXML->QueryIntAttribute(FRAME, &_frame) == TIXML_SUCCESS ) {
        _node->frame = _frame;
    }

    //_node->toString();

    return _node;
}

void CCConnectionData::removeAll(bool _deletMe) {
    if( mAnimationDatas ) {
        mAnimationDatas->removeAllObjects();
    }
    if( mArmarureDatas) {
        mArmarureDatas->removeAllObjects();
    }
    //delete this;
}