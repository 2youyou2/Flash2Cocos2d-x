#include "CArmature.h"
#include "CConnectionData.h"


CCArmature *CCArmature::create(const char* _name, const char* _animationName, const char* _plistPath, const char* _imagePath,  CCSpriteBatchNode *_batchNode, int _index, bool _isRadian, const char* _eachImageSuffix) {
    //CCXLOG("Enter CCArmature::createArmature");

    CCArmature *_armature = new CCArmature();
    if (_armature && _armature->init(_name, _animationName, _plistPath, _imagePath, _batchNode, _index, _isRadian, _eachImageSuffix)) {
        _armature->autorelease();
        return _armature;
    }
    CC_SAFE_DELETE(_armature);
    return NULL;

    //CCXLOG("Finish CCArmature::createArmature");
}


CCSprite* CCArmature::createBoneDisplay(const char* _boneName) {
    if (cache) {
		std::string _boneNameStr = _boneName;
		_boneNameStr.append(mEachImageSuffix);
        return getTextureDisplay(cache, _boneNameStr.c_str() );
    }
    return NULL;
}

CCSprite* CCArmature::getTextureDisplay(CCSpriteFrameCache* _cache, const char* _boneName) {
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
	cache					= NULL;
	mEachImageSuffix		= ".png";
}


CCArmature::~CCArmature(void) {
	remove();
	CC_SAFE_DELETE(mBoneDic);
	CC_SAFE_DELETE(mBoneList);
}

bool CCArmature::init(const char* _name, const char* _animationName, const char* _plistPath, const char* _imagePath, CCSpriteBatchNode *_batchNode, int _index, bool _isRadian, const char* _eachImageSuffix) {
    bool bRet = false;
    do {

        mAnimation	= CCARmatureAnimation::create();
        CCAssert(mAnimation, "create CCArmature::mAnimation fail!");
        addChild(mAnimation);

		mIsRadian	= _isRadian;

		mBoneDic	= new CCDictionary();
		CCAssert(mBoneDic, "create CCArmature::mBoneDic fail!");

		mBoneList	= new CCArray();
		CCAssert(mBoneList->init(), "create CCArmature::boneList fail!");

        mEachImageSuffix = _eachImageSuffix;

        cache =  CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile(_plistPath, _imagePath);


		mDisPlayBatchNode = _batchNode;
		mDisPlayBatchNode->retain();


        CCArray *_armatureData = CCConnectionData::sharedConnectionData()->getArmatureData(_name);
        if(_armatureData->count()<=0) {
            return NULL;
        }

        CCSprite* _armatureDisplay = NULL;

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



        mName = _name;

        CCArmatureAniData* _animationData = CCConnectionData::sharedConnectionData()->getAnimationData(_animationName);
        if (_animationData) {
            mAnimation->setData(_animationData);
        }

        CCBone			*_bone;
        PCCArmatureData _boneData;
        const char*		_boneName;
        const char*		_parentName;
        CCSprite		*_boneDisplay;
        int				_indexZ;
        int				_length = _armatureData->count();

        for(int indexI = 0; indexI < _length; indexI++) {
            _boneData = (PCCArmatureData)_armatureData->objectAtIndex(indexI);
            _boneName = _boneData->name.c_str();
            _parentName = _boneData->parent.c_str();
            _indexZ = _boneData->z;

            if( !mBoneDisplayFactory ) {
                mBoneDisplayFactory = this;
            }
            do {
                _boneDisplay = mBoneDisplayFactory->createBoneDisplay(_boneName);
                CCAssert(_boneDisplay, "create CCArmature::init::_boneDisplay fail.");

            } while (0);


            _bone = addBone(_boneName, _parentName, _boneDisplay, _indexZ);

            float _anchorPointX = -_boneData->registrationPointX / _boneData->width;
            float _anchorPointY = (_boneData->height - _boneData->registrationPointY) / _boneData->height;
            _bone->getDisplay()->setAnchorPoint(ccp(_anchorPointX, _anchorPointY));
            _bone->setLockPosition( _boneData->x, _boneData->y );

            //CCLOG( "%s: %f, %f, %f, %f" , _boneData->name.c_str(), _anchorPointX, _anchorPointY, _boneData->x, _boneData->y);

        }

        scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

/**
* ¸üÐÂ Tween¡¢Bone¡¢Animation
*/
void CCArmature::update(float dt) {
    int _len = mBoneList->count();

	CCObject *_object = NULL;
	CCARRAY_FOREACH(mBoneList, _object){
		CCBone *_bone = (CCBone*)_object;
		mAnimation->updateTween(&_bone->getName(), dt);
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
        CCBone *_boneParent = (CCBone*)mBoneDic->objectForKey(_parentName);
        if(_boneParent) {
            _boneParent->addBoneChild(_bone);
        }
        mAnimation->addTween(_bone);
    }
    if(_display) {
        CCNode* _displayOld = _bone->getDisplay();
        _bone->setDisplay(_display);
        if(_displayOld) {
            mDisplay->addChild(_display, _displayOld->getZOrder() - 1);
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