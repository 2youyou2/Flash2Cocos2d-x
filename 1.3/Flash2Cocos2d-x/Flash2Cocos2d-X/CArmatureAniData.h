#pragma once

#include "CBoneAniData.h"

/*骨架动画数据*/
class CCArmatureAniData : public CCObject {
public:
    static CCArmatureAniData *create();
public:
    CCArmatureAniData(void);
    ~CCArmatureAniData(void);

    virtual bool init();

    /**
    * 添加单个动作动画，比如 run，stand 等
    * @param _boneAniData
    * @param _animationName
    */
    void addAnimation(const char* _animationName, CCBoneAniData* _boneAniData);
    /**
    * 获取单个动作动画，比如 run，stand 等
    * @param _animationName
    * @return 返回 BoneAniData 实例
    */
    CCBoneAniData *getAnimation(const char* _animationName);
    /**
    * 获取拥有的动作动画列表
    * @return 动画列表数组
    */
    std::vector<std::string> getAnimationNames();
private:
    CCDictionary *mBoneAniDataDic;
    std::vector<std::string> mAnimationNames;



};

