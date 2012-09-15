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

/*�Ǽܣ���������Ĺ�������*/
class CCArmature : public CCNode,
    public CCBoneDisplayFactory,
    public CCArmatureDisplayFactory

{

public:
    /**
    * �Ǽ����ɷ���
    * @param _name �Ǽ���
    * @param _animationName ������
    * @param _plistPath 
	* @param _imagePath 
	* @param _batchNode 
	* @param _isRadian �Ƿ���û�����
    */
    static CCArmature *create(const char* _name, const char* _animationName, CCSpriteBatchNode *_batchNode, int _index = 0, bool _isRadian = false, const char* _eachImageSuffix = ".png");

    

public:
    CCArmature();
    ~CCArmature(void);

    virtual bool init(const char* _name, const char* _animationName, CCSpriteBatchNode *_batchNode, int _index = 0, bool _isRadian = false, const char* _eachImageSuffix = ".png");

    /**
    * ���� Tween��Bone��Animation
    */
    void	update(float dt);
    /**
    * ɾ��������
    */
    void	remove();
 
    /**
    * �󶨹���
    * @param _name ������
    * @param _parentName �󶨵���������
    * @param _display ��������ʾ����
    * @param _index ��ʾ����󶨵���ȣ�������滻ԭ����ʾ������ʹ��ԭ��ʾ�������ȡ�-1:����ӵ���ߴ���-2:��ִ��addChild����
    * @return �������ɵ� Bone ʵ��
    */
    CCBone	*addBone(const char* _name, const char* _parentName = NULL, CCNode* _display=NULL, int _index = -1);
    /**
    * ��ȡ�Ǽ��еĹ���
    * @param _name ��������
    */
    CCBone	*getBone(const char* _name);
    /**
    * ɾ������
    * @param _name ��������
    */
    void	removeBone(const char* _name);

	void	resetBonesParent();

private:
    /**
    * @param _fullName ��ͼȫ��
    * @return ���� CCSprite ʵ��
    */
    CCSprite	*getTextureDisplay(const char* _fullName);
    /**
    * �������ɽӿ�
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

