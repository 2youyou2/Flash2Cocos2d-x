#pragma once

#include "CProcessBase.h"
#include "CArmatureAniData.h"
#include "CTween.h"
#include "CBone.h"

class CCArmature;

/**
* ���ſ�ʼ
*/
static const char* START = "start";
/**
* ���Ž���
*/
static const char* COMPLETE = "complete";
/**
* ѭ������
*/
static const char* LOOP_COMPLETE = "loopComplete";
/**
* �����¼�֡
*/
static const char* IN_FRAME = "inFrame";


class CCAnimationEventListener{
public:
	virtual void animationHandler(const char* _aniType, const char* _aniID, const char* _frameID) = 0;
};


/*Ϊ�Ǽܹ���Tween*/
class CCArmatureAnimation : public CCProcessBase
{
public:
    static CCArmatureAnimation *create();
public:
    CCArmatureAnimation();
    ~CCArmatureAnimation(void);

    virtual bool init();

    /**
    * ɾ��������
    */
    void remove();
    void pause();
    void resume();
    void stop();
    /**
    * ��ȡ ���� ��ǰ ArmatureAniData
    */
    CCArmatureAniData* getData();
    void setData(CCArmatureAniData* _aniData);
    /**
    * �Թ����������ٶȽ�������
    * @param _scale ����ֵ��Ϊϵ���˵�ԭ���Ķ���ֵ֡
    * @param _boneName ָ��ĳ�����ŵĹ���ID����ָ����Ĭ��Ϊ���й���
    */
    void setAnimationScale(float _scale, const char* _boneName = "");
    /**
    * Ϊ������� Tween ʵ��
    */
    void addTween(CCBone* _bone);
    /**
    * ɾ�������� Tween ʵ��
    */
    void removeTween(CCBone* _bone);
    /**
    * ��ȡ������ Tween ʵ��
    */
    CCTween *getTween(const char* _boneID);
    /**
    * ����ָ���� Tween ʵ��
    */
    void updateTween(const char *_boneID, float dt);
    /**
    * ���Ŷ���
    * @param _to ArmatureAniData �� ConnectionData �е��ַ�������
    * @param _toFrames ���ɵ� _to ������Ҫ���ѵ�֡
    * @param _listFrames _to �������ų�����֡
    * @param _loop �Ƿ�ѭ��
    * @param _ease FrameNodeList �����ؼ���Ļ�����ʽ��-1:SineOut��0:Line��1:SineIn��2:SineInOut
    */
    void playTo(void * _to, int _durationTo = -1, int _durationTween = -1,  bool _loop = true, int _ease = -2);

    void updateHandler();

    void updateCurrentPrecent();
public:
    /**
    * �¼��ص�
    */
	CC_SYNTHESIZE(CCAnimationEventListener *, mEventListener, AniEventListener);

protected:
    CCArmatureAniData	*mArmatureAniData;
    CCBoneAniData		*mBoneAniData;
	
private:
	
    // <const char*, CCTween*>
    CCDictionary	*mTweens;
	
    const char*		mAniIDNow;

};

