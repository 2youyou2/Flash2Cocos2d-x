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


//格式化、管理骨骼配置与骨骼动画
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
    * @return 返回骨架数据
    */
    CCArray * getArmatureData(const char* _id);
    /**
    * @param _id
    * @return 返回骨架动画数据
    */
    CCArmatureAniData* getAnimationData(const char* _id);
    /**
    * 将XML数据转换成内置数据 ArmatureAniData 等
    * @param _xml XML数据
    */
    void addData(const char* _xml);

    void removeAll(bool _deletMe = false);

private:
    CCFrameNodeList	*getFrameNodeList(TiXmlElement* _frameXMLList, const char* _boneName);

    CCFrameNode		*getFrameNode(TiXmlElement* _nodeXML);

private:
    //存储骨架数据 <const char*, std::vector<PCCArmatureData> > ;
    CCDictionary *mArmarureDatas;

    //存储骨架动画数据 std::map<const char*, CCArmatureAniData*> mAnimationDatas;
    CCDictionary *mAnimationDatas;
private:
    static CCConnectionData *sSharedConnectionData;
};

