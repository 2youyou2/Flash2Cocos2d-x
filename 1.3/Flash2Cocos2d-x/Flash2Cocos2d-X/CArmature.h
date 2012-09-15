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
	std::string boneImage;
    float x;
    float y;
    float z;
    float pivotX;
    float pivotY;
    float width;
    float height;
	float skewX;
	float skewY;
	float rotation;
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
    static CCArmature *create(const char* _name, const char* _animationName, CCSpriteBatchNode *_batchNode, int _index = 0, bool _isRadian = false, const char* _eachImageSuffix = ".png");

    

public:
    CCArmature();
    ~CCArmature(void);

    virtual bool init(const char* _name, const char* _animationName, CCSpriteBatchNode *_batchNode, int _index = 0, bool _isRadian = false, const char* _eachImageSuffix = ".png");

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

	void	resetBonesParent();

private:
    /**
    * @param _fullName 贴图全称
    * @return 返回 CCSprite 实例
    */
    CCSprite	*getTextureDisplay(const char* _fullName);
    /**
    * 骨骼生成接口
    */
    CCSprite	*createBoneDisplay(const char* _boneName);

    CCSprite	*createArmatureDisplay(const char* _armatureName);

	void		createBones(CCArray *_armatureDatas);

public:
	CC_SYNTHESIZE(CCNode *,	mDisplay,	Display);
	CC_SYNTHESIZE(CCArmatureAnimation *, mAnimation, Animation);

	CC_SYNTHESIZE(CCBoneDisplayFactory	*, mBoneDisplayFactory,	 BoneDisplayFactory);
	CC_SYNTHESIZE(CCArmatureDisplayFactory	*, mArmaureDisplayFactory, ArmaureDisplayFactory);

	CC_SYNTHESIZE_RETAIN( CCSpriteBatchNode	*, mDisPlayBatchNode, DisPlayBatchNode);

	CC_SYNTHESIZE_READONLY(std::string,	mName, Name);
protected:
    
    
    bool							mIsRadian;
    // <const char*, CCBone*>
    CCDictionary					*mBoneDic;
	// <CCBone*>
    CCArray							*mBoneList;

	const char*						mEachImageSuffix;

	
};

