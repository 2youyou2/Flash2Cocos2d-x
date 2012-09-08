#pragma once

#include "CAnimation.h"

typedef struct _CCSkeletonFactory *PCCSkeletonFactory;

typedef struct _CCArmatureData : public CCObject {
    static  _CCArmatureData *create() {
        _CCArmatureData *pArmatureData = new _CCArmatureData();
        if (pArmatureData) {
            pArmatureData->autorelease();
            return pArmatureData;
        }
        CC_SAFE_DELETE(pArmatureData);
        return NULL;
    }

    std::string name;
    std::string parent;
    float x;
    float y;
    float z;
    float registrationPointX;
    float registrationPointY;
    float width;
    float height;
} CCArmatureData, *PCCArmatureData;

class CCBoneDisplayFactory {
public:
    virtual CCSprite* createBoneDisplay(const char* _boneName) = 0;
};

class CCArmatureDisplayFactory {
public:
    virtual CCSprite* createArmatureDisplay(const char* _armatureName) = 0;
};

/*骨架，管理自身的骨骼数据*/
class CCArmature : public CCNode,
    public CCBoneDisplayFactory,
    public CCArmatureDisplayFactory

{

public:
    /**
    * 骨架生成方法
    * @param _name 骨架名
    * @param _animationName 动画名
    * @param _plistPath 
	* @param _imagePath 
	* @param _batchNode 
	* @param _isRadian 是否采用弧度制
    */
    static CCArmature *create(const char* _name, const char* _animationName, const char* _plistPath, const char* _imagePath, CCSpriteBatchNode *_batchNode, int _index = 0, bool _isRadian = false, const char* _eachImageSuffix = ".png");

    

public:
    /**
    * 构造函数
    * @param _display 包含所有骨骼显示对象的显示容器
    * @param _isRadian 骨骼旋转角度是否采用弧度制，比如 Starling 使用的是弧度制
    */
    CCArmature();
    ~CCArmature(void);

    virtual bool init(const char* _name, const char* _animationName, const char* _plistPath, const char* _imagePath,  CCSpriteBatchNode *_batchNode, int _index = 0, bool _isRadian = false, const char* _eachImageSuffix = ".png");

    /**
    * 更新 Tween、Bone、Animation
    */
    void	update(float dt);
    /**
    * 删除，回收
    */
    void	remove();
 
    /**
    * 绑定骨骼
    * @param _name 骨骼名
    * @param _parentName 绑定到父骨骼名
    * @param _display 骨骼的显示对象
    * @param _index 显示对象绑定到深度，如果是替换原有显示对象，则使用原显示对象的深度。-1:则添加到最高处，-2:则不执行addChild操作
    * @return 返回生成的 Bone 实例
    */
    CCBone	*addBone(const char* _name, const char* _parentName = NULL, CCNode* _display=NULL, int _index = -1);
    /**
    * 获取骨架中的骨骼
    * @param _name 骨骼名称
    */
    CCBone	*getBone(const char* _name);
    /**
    * 删除骨骼
    * @param _name 骨骼名称
    */
    void	removeBone(const char* _name);

private:
    /**
    * @param _cache CCSpriteFrameCache
    * @param _fullName 贴图全称
    * @return 返回 CCSprite 实例
    */
    CCSprite* getTextureDisplay(CCSpriteFrameCache* _cache, const char* _fullName);
    /**
    * 骨骼生成接口
    */
    CCSprite* createBoneDisplay(const char* _boneName);

    CCSprite* createArmatureDisplay(const char* _armatureName);

public:
	CC_SYNTHESIZE(CCNode *,					mDisplay,					Display);
	CC_SYNTHESIZE(CCARmatureAnimation *,	mAnimation,					Animation);

	CC_SYNTHESIZE(CCBoneDisplayFactory		*, mBoneDisplayFactory,		BoneDisplayFactory);
	CC_SYNTHESIZE(CCArmatureDisplayFactory	*, mArmaureDisplayFactory,	ArmaureDisplayFactory);

	CC_SYNTHESIZE_RETAIN( CCSpriteBatchNode	*, mDisPlayBatchNode,		DisPlayBatchNode);

	CC_SYNTHESIZE_READONLY(std::string, mName, Name);
protected:
    
    
    bool							mIsRadian;
    // <const char*, CCBone*>
    CCDictionary					*mBoneDic;
	// <CCBone*>
    CCArray							*mBoneList;

	const char*						mEachImageSuffix;

	CCSpriteFrameCache				*cache;

	
};

