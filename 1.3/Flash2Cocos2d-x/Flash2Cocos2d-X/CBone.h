#pragma once

#include "CBaseNode.h"
#include "CTweenNode.h"

using namespace cocos2d;

#define  ANGLE_TO_RADIAN  (M_PI / 180)


class CCBone : public CCObject
{
public:
	/**
	* 创建Bone实例，Bone 做为常用的实例，用对象池管理创建回收
	* @param _isRadian 骨骼旋转角度是否采用弧度制，比如 Starling 使用的是弧度制
	* @return 返回 Bone 实例
	*/
	static CCBone* create(bool _isRadian = false);

	static void	removePoolObject();

protected:

	static CCArray	*sObjectPool;

	static int		sPoolIndex;

	static void		recycle(CCBone* _bone);
	

public:

	/**
	* 构造函数
	* @param _isRadian 骨骼旋转角度是否采用弧度制，比如 Starling 使用的是弧度制
	*/
	CCBone();
	~CCBone(void);

	virtual bool init(bool _isRadian);

	
	/* 删除，回收 */
	void	remove();

	/* 添加子骨骼 */
	void	addBoneChild(CCBone* _child);
	/* 调整在父骨骼中的绑定位置，仅在拥有 parent 时起作用 */
	void	setLockPosition(float _x, float _y, float _r = 0);
	/* 更新位置 */
	void	update( float dt );
	/* 获取在 Armatur.display 中的全局 x 坐标 */
	float	 getGlobalX();
	/* 获取在 Armatur.display 中的全局 y 坐标 */
	float	getGlobalY();
	/* 获取在 Armatur.display 中的全局 rotation */
	float	getGlobalR();


protected:

	/* 更新 display */
	void updateDisplay();

	void reset();

public:

	/* 骨骼名称 Armature 通过骨骼名索引 Bone。Animation 通过骨骼名建立 Bone 与 Tween 的关联 */	
	CC_SYNTHESIZE_PASS_BY_REF(std::string, mName, Name);

	/* 骨骼绑定的显示对象，并不是必须的，可以不绑定显示对象 */
	CC_SYNTHESIZE_RETAIN(CCNode	*, mDisplay, Display);

	/* 骨骼受控制的关键点 */
	CC_SYNTHESIZE_RETAIN(CCBaseNode	*, mNode, Node);

	/* 受 Tween 控制的关键点，骨骼由 Tween 和自身 node 的合值控制 */
	CC_SYNTHESIZE_RETAIN(CCTweenNode *, mTweenNode, TweenNode);

	CC_SYNTHESIZE(CCBone *, mParent, Parent);
	CC_SYNTHESIZE_PASS_BY_REF(std::string, mParentName, ParentName);

protected:
	
	bool	mIsRadian;
	float	mTransformX;
	float	mTransformY;
	float	mLockX;
	float	mLockY;
	float	mLockR;
	float	mParentX;
	float	mParentY;
	float	mParentR;
};

