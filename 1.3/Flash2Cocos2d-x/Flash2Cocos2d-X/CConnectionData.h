#pragma once

#include "CArmature.h"
#include "CBone.h"
#include "tinyxml.h"
#include "CConstValue.h"
#include "CTextureData.h"

//��ʽ������������������������
class CCConnectionData : public CCObject {
public:
    static CCConnectionData *sharedConnectionData();
private:
    CCConnectionData(void);
    ~CCConnectionData(void);

public:
    virtual bool init();
    
    CCArray *getArmatureData(const char* _id);
    
    CCArmatureAniData *getAnimationData(const char* _id);

	CCTexutreData *getTextureData(const char *_textureName);
    /**
    * ��XML����ת������������ ArmatureAniData ��
    * @param _xml XML����
    */
    void addData(const char* _xml);

    void removeAll(bool _deletMe = false);

private:
    CCFrameNodeList	*getFrameNodeList(TiXmlElement* _frameXMLList, const char* _boneName);

    CCFrameNode	*getFrameNode(TiXmlElement* _nodeXML);

	void decodeTextures(TiXmlElement *_texturesXML);
	void decodeArmatures(TiXmlElement *_armaturesXML);
	void decodeAnimations(TiXmlElement *_animationsXML);


private:
    //save armature datas.			 <const char*, std::vector<PCCArmatureData> > ;
	CC_SYNTHESIZE_READONLY(CCDictionary *, mArmarureDatas, ArmarureDatas);

    //save armature animation datas. <const char*, CCArmatureAniData*> ;
	CC_SYNTHESIZE_READONLY(CCDictionary *, mAnimationDatas, AnimationDatas);

	//save texture datas			 <const char*, CCTextureData*>
	CC_SYNTHESIZE_READONLY(CCDictionary *, mTextureDatas, TextureDatas);
private:
    static CCConnectionData *sSharedConnectionData;
};

