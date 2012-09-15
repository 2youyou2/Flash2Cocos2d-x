#pragma once

#include "CEventFrame.h"
#include "CFrameNodeList.h"

/*����������������*/
class CCBoneAniData : public CCObject {
public:
    static CCBoneAniData *create();
public:

    CCBoneAniData(void);
    ~CCBoneAniData(void);

    virtual bool init();

    /**
    * ��ӹ����Ķ�������
    * @param _frameNodeList
    * @param _boneName
    */
    void addAnimation(CCFrameNodeList* _frameNodeList, const char* _boneName);
    /**
    * ��ȡ������������
    * @param _boneName
    * @return ���ع�����������
    */
    CCFrameNodeList* getAnimation(const char* _boneName);
    //���ص��ַ��ֱܶ࣬��ʹ��CCLOG��ӡ���ܻ����ԭ��Ϊ������ڴ�̫С��
    std::string toString();

public:

    /**
    * �¼�֡�б�
    */
    CC_SYNTHESIZE_RETAIN(CCArray *, mEventList, EventList);

private:
    CCDictionary *mFrameNodeListDic;

public:
	/**
    * ��������ʵ�ʳ�����֡��
    */
	CC_SYNTHESIZE_PASS_BY_REF(int, mDuration, Duration);
	CC_SYNTHESIZE_PASS_BY_REF(int, mDurationTo, DurationTo);
	CC_SYNTHESIZE_PASS_BY_REF(int, mDurationTween, DurationTween);
	CC_SYNTHESIZE_PASS_BY_REF(bool, mLoop, Loop);
	CC_SYNTHESIZE_PASS_BY_REF(float, mTweenEasing, TweenEasing);
};

