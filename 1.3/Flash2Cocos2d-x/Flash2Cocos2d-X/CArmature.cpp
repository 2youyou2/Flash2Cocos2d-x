#include "CArmature.h"
#include "CConnectionData.h"


CCArmature *CCArmature::create(const char* _name, const char* _animationName, CCSpriteBatchNode *_batchNode, int _index, bool _isRadian, const char* _eachImageSuffix) {
    //CCXLOG("Enter CCArmature::createArmature");

    CCArmature *_armature = new CCArmature();
    if (_armature && _armature->init(_name, _animationName, _batchNode, _index, _isRadian, _eachImageSuffix)) {
        _armature->autorelease();
        return _armature;
    }
    CC_SAFE_DELETE(_armature);
    return NULL;

    //CCXLOG("Finish CCArmature::createArmature");
}


CCSprite* CCArmature::createBoneDisplay(const char* _boneName) {
	std::string _boneNameStr = _boneName;
	_boneNameStr.append(mEachImageSuffix);
	return getTextureDisplay(_boneNameStr.c_str() );
    return NULL;
}

CCSprite* CCArmature::getTextureDisplay(const char* _boneName) {
    CCSprite *_display = CCSprite::createWithSpriteFrameName(_boneName);
    return _display;
}

CCSprite* CCArmature::createArmatureDisplay(const char* _armatureName) {
    return CCSprite::create();
}


CCArmature::CCArmature() {
    mAnimation				= NULL;
    mDisplay				= NULL;
    mIsRadian				= false;
    mArmaureDisplayFactory	= NULL;
    mBoneDisplayFactory		= NULL;
	mBoneDic				= NULL;
	mBoneList				= NULL;
	mDisPlayBatchNode		= NULL;
	mEachImageSuffix		= ".png";
}


CCArmature::~CCArmature(void) {
	remove();
	CC_SAFE_DELETE(mBoneDic);
	CC_SAFE_DELETE(mBoneList);
}

bool CCArmature::init(const char* _name, const char* _animationName, CCSpriteBatchNode *_batchNode, int _index, bool _isRadian, const char* _eachImageSuffix) {
    bool bRet = false;
    do {

		// init attributes
        mAnimation	= CCArmatureAnimation::create();
        CCAssert(mAnimation, "create CCArmature::mAnimation fail!");
        addChild(mAnimation);

		mIsRadian	= _isRadian;

		mBoneDic	= new CCDictionary();
		CCAssert(mBoneDic, "create CCArmature::mBoneDic fail!");

		mBoneList	= new CCArray();
		CCAssert(mBoneList->init(), "create CCArmature::boneList fail!");

        mEachImageSuffix = _eachImageSuffix;

		mDisPlayBatchNode = _batchNode;
		mDisPlayBatchNode->retain();

		mName = _name;
		

		// create armature display , which is the top parent of the bone display
        if( !mArmaureDisplayFactory ) {
            mArmaureDisplayFactory = this;
        }

        do {
            mDisplay = mArmaureDisplayFactory->createArmatureDisplay(_name);
            CCAssert(mDisplay, "create CCArmature::init::mDisplay fail.");
			if( _batchNode ){
				if( static_cast<CCSprite*>(mDisplay) ){
					CCSprite *_sprite = (CCSprite*)mDisplay;
					_sprite->initWithTexture(mDisPlayBatchNode->getTexture(), CCRectZero);
				}

				mDisPlayBatchNode->addChild(mDisplay, _index);
			}

        } while (0);


        CCArmatureAniData* _animationData = CCConnectionData::sharedConnectionData()->getAnimationData(_animationName);
        CCAssert(_animationData, "CCArmatureAniData not exist! ");
		mAnimation->setData(_animationData);


		CCArray *_armatureDatas = CCConnectionData::sharedConnectionData()->getArmatureData(_name);
		CCAssert(_armatureDatas->count()>0, "_armatureData->count() should not <=0");
		
		createBones(_armatureDatas);
        
        scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void CCArmature::createBones(CCArray *_armatureDatas){
	CCBone			*_bone;
	PCCArmatureData _boneData;
	const char		*_boneName;
	const char		*_parentName;
	const char		*_imageName;
	CCSprite		*_boneDisplay;
	int				_indexZ;
	int				_length = _armatureDatas->count();

	for(int indexI = 0; indexI < _length; indexI++) {
		_boneData = (PCCArmatureData)_armatureDatas->objectAtIndex(indexI);
		_boneName = _boneData->name.c_str();
		_parentName = _boneData->parent.c_str();
		_imageName = _boneData->boneImage.c_str();
		_indexZ = _boneData->z;

		if( !mBoneDisplayFactory ) {
			mBoneDisplayFactory = this;
		}
		do {
			_boneDisplay = mBoneDisplayFactory->createBoneDisplay(_imageName);
			CCAssert(_boneDisplay, "create CCArmature::init::_boneDisplay fail.");

		} while (0);


		_bone = addBone(_boneName, _parentName, _boneDisplay, _indexZ);

		float _anchorPointX = _boneData->pivotX / _boneData->width;
		float _anchorPointY = (_boneData->height - _boneData->pivotY) / _boneData->height;
		_bone->getDisplay()->setAnchorPoint(ccp(_anchorPointX, _anchorPointY));
		_bone->setLockPosition( _boneData->x, _boneData->y , _boneData->skewX, _boneData->skewY);
	}


	resetBonesParent();
}

void CCArmature::resetBonesParent(){

	CCDictElement *_element = NULL;
	CCDICT_FOREACH(mBoneDic, _element){
		CCBone *_bone = (CCBone *)_element->getObject();
		CCBone *_boneParent = (CCBone *)mBoneDic->objectForKey(_bone->getParentName());
		if(_boneParent) {
			_bone->setParent(_boneParent);
		}
	}
}


void CCArmature::update(float dt) {
    int _len = mBoneList->count();

	CCObject *_object = NULL;
	CCARRAY_FOREACH(mBoneList, _object){
		CCBone *_bone = (CCBone*)_object;
		mAnimation->updateTween(_bone->getName(), dt);
		_bone->update( dt );
	}

    mAnimation->update( dt );
}

void CCArmature::remove() {

	CCObject *_object = NULL;
	CCBone *_bone;
	CCARRAY_FOREACH(mBoneList, _object){
		_bone = (CCBone*)_object;
		_bone->remove();
	}

	mAnimation->remove();
    mAnimation = NULL;

	if( mDisPlayBatchNode ){
		mDisPlayBatchNode->removeChild(mDisplay, true);
		mDisPlayBatchNode->release();
	}
	mDisPlayBatchNode = NULL;
	mDisplay = NULL;

    mBoneDic->removeAllObjects();
    mBoneList->removeAllObjects();
}


CCBone* CCArmature::addBone(const char* _name, const char* _parentName, CCNode* _display, int _index) {
    CCBone *_bone = (CCBone*)mBoneDic->objectForKey(_name);
    if(!_bone) {
        _bone = CCBone::create(mIsRadian);
        _bone->setName(_name);
        mBoneList->addObject(_bone);
        mBoneDic->setObject(_bone, _name);
        mAnimation->addTween(_bone);
		_bone->setParentName(_parentName);
    }
    if(_display) {
        CCNode* _displayOld = _bone->getDisplay();
        _bone->setDisplay(_display);
        if(_displayOld) {
            mDisplay->addChild(_display, _displayOld->getZOrder());
        } else if (_index == -1) {
            mDisplay->addChild(_display);
        } else if (_index >= 0) {
            mDisplay->addChild(_display, _index);
        }
    }
    return _bone;
}

CCBone *CCArmature::getBone(const char* _name) {
    return (CCBone*)mBoneDic->objectForKey(_name);
}

void CCArmature::removeBone(const char* _name) {
    CCBone *_bone = (CCBone*)mBoneDic->objectForKey(_name);
    if(_bone) {
        if(_bone->getDisplay() /*&& display->contains(_bone->display)*/) {
            mDisplay->removeChild(_bone->getDisplay(), false);
        }
        mAnimation->removeTween(_bone);
        _bone->remove();
    }
}