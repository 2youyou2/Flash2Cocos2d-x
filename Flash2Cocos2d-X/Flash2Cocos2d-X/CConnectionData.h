#pragma once

#include "CArmature.h"
#include "CBone.h"
#include "tinyxml.h"

static const char* BONE = "bone";

static const char* ANIMATION = "animation";

static const char* NAME = "name";

static const char* PARENT = "parent";

static const char* ROTATION = "rotation";

static const char* OFF_R = "offR";

static const char* X = "x";

static const char* Y = "y";

static const char* Z = "z";

static const char* REGISTRATION_POINT_X = "registrationPointX";

static const char* REGISTRATION_POINT_Y = "registrationPointY";

static const char* WIDTH = "width";

static const char* HEIGHT = "height";

static const char* SCALE_X = "scaleX";

static const char* SCALE_Y = "scaleY";

static const char* ALPHA = "alpha";

static const char* DELAY = "delay";

static const char* SCALE = "scale";

static const char* FRAME = "frame";

static const char* EVENT_FRAME = "eventFrame";


//��ʽ������������������������
class CCConnectionData : public CCObject {
public:
    static CCConnectionData *sharedConnectionData();
private:
    CCConnectionData(void);
    ~CCConnectionData(void);

public:
    virtual bool init();
    /**
    * @param _id
    * @return ���عǼ�����
    */
    CCArray * getArmatureData(const char* _id);
    /**
    * @param _id
    * @return ���عǼܶ�������
    */
    CCArmatureAniData* getAnimationData(const char* _id);
    /**
    * ��XML����ת������������ ArmatureAniData ��
    * @param _xml XML����
    */
    void addData(const char* _xml);

    void removeAll(bool _deletMe = false);

private:
    CCFrameNodeList	*getFrameNodeList(TiXmlElement* _frameXMLList, const char* _boneName);

    CCFrameNode		*getFrameNode(TiXmlElement* _nodeXML);

private:
    //�洢�Ǽ����� <const char*, std::vector<PCCArmatureData> > ;
    CCDictionary *mArmarureDatas;

    //�洢�Ǽܶ������� std::map<const char*, CCArmatureAniData*> mAnimationDatas;
    CCDictionary *mAnimationDatas;
private:
    static CCConnectionData *sSharedConnectionData;
};

