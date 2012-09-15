#include "CBoneAniData.h"

CCBoneAniData *CCBoneAniData::create() {
    CCBoneAniData *pBoneAniData = new CCBoneAniData();
    if (pBoneAniData && pBoneAniData->init()) {
        pBoneAniData->autorelease();
        return pBoneAniData;
    }
    CC_SAFE_DELETE(pBoneAniData);
    return NULL;
}

CCBoneAniData::CCBoneAniData(void) {
}


CCBoneAniData::~CCBoneAniData(void) {
    CC_SAFE_DELETE( mEventList );
    CC_SAFE_DELETE( mFrameNodeListDic );
}

bool CCBoneAniData::init() {
    bool bRet = false;
    do {
        mEventList = new CCArray();
        CCAssert(mEventList->init(), "create CCBoneAniData::mEventList fail!");

        mFrameNodeListDic = new CCDictionary();
        CCAssert(mFrameNodeListDic, "create CCBoneAniData::mFrameNodeListDic fail!");

        bRet = true;
    } while (0);

    return bRet;
}


void CCBoneAniData::addAnimation(CCFrameNodeList* _frameNodeList, const char* _boneName) {
    mFrameNodeListDic->setObject(_frameNodeList, _boneName );
}

CCFrameNodeList* CCBoneAniData::getAnimation(const char* _boneName) {
    return (CCFrameNodeList*)(mFrameNodeListDic->objectForKey(_boneName));
}

std::string CCBoneAniData::toString() {
    std::string _str;
    CCDictElement *_element = NULL;
    CCDICT_FOREACH(mFrameNodeListDic, _element) {
        CCFrameNodeList *_nodeList = (CCFrameNodeList*)_element->getObject();
        _str.append( "boneName: " );
        _str.append( _element->getStrKey() );
        _str.append( "\n" );
        _str.append( _nodeList->toString() );
    }

    return _str;
}