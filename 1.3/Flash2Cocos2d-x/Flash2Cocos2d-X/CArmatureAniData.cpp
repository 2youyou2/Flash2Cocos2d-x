#include "CArmatureAniData.h"


CCArmatureAniData *CCArmatureAniData::create() {
    CCArmatureAniData *pArmatureAniData = new CCArmatureAniData();
    if (pArmatureAniData && pArmatureAniData->init()) {
        pArmatureAniData->autorelease();
        return pArmatureAniData;
    }
    CC_SAFE_DELETE(pArmatureAniData);
    return NULL;
}

CCArmatureAniData::CCArmatureAniData(void) {
}


CCArmatureAniData::~CCArmatureAniData(void) {
    CC_SAFE_DELETE( mBoneAniDataDic );
}

bool CCArmatureAniData::init() {
    bool bRet = false;
    do {

        mBoneAniDataDic = new CCDictionary();
        CCAssert(mBoneAniDataDic, "create CCArmatureAniData::boneAniDataDic fail!");

        bRet = true;
    } while (0);

    return bRet;
}

void CCArmatureAniData::addAnimation(const char* _animationName, CCBoneAniData* _boneAniData) {
    mBoneAniDataDic->setObject(_boneAniData, _animationName);
    mAnimationNames.push_back(_animationName);
}

CCBoneAniData *CCArmatureAniData::getAnimation(const char* _animationName) {
    return (CCBoneAniData *)mBoneAniDataDic->objectForKey( _animationName );
}

std::vector<std::string>  CCArmatureAniData::getAnimationNames() {
    return mAnimationNames;
}