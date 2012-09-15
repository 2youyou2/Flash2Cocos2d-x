#pragma once

#include "CBaseNode.h"
#include "CTweenNode.h"

using namespace cocos2d;

#define  ANGLE_TO_RADIAN  (M_PI / 180)


class CCBone : public CCObject
{
public:
	/**
	* ����Boneʵ����Bone ��Ϊ���õ�ʵ�����ö���ع���������
	* @param _isRadian ������ת�Ƕ��Ƿ���û����ƣ����� Starling ʹ�õ��ǻ�����
	* @return ���� Bone ʵ��
	*/
	static CCBone* create(bool _isRadian = false);

	static void	removePoolObject();

protected:

	static CCArray	*sObjectPool;

	static int		sPoolIndex;

	static void		recycle(CCBone* _bone);
	

public:

	/**
	* ���캯��
	* @param _isRadian ������ת�Ƕ��Ƿ���û����ƣ����� Starling ʹ�õ��ǻ�����
	*/
	CCBone();
	~CCBone(void);

	virtual bool init(bool _isRadian);

	
	/* ɾ�������� */
	void	remove();

	/* ����ӹ��� */
	void	addBoneChild(CCBone* _child);
	/* �����ڸ������еİ�λ�ã�����ӵ�� parent ʱ������ */
	void	setLockPosition(float _x, float _y, float _r = 0);
	/* ����λ�� */
	void	update( float dt );
	/* ��ȡ�� Armatur.display �е�ȫ�� x ���� */
	float	 getGlobalX();
	/* ��ȡ�� Armatur.display �е�ȫ�� y ���� */
	float	getGlobalY();
	/* ��ȡ�� Armatur.display �е�ȫ�� rotation */
	float	getGlobalR();


protected:

	/* ���� display */
	void updateDisplay();

	void reset();

public:

	/* �������� Armature ͨ������������ Bone��Animation ͨ������������ Bone �� Tween �Ĺ��� */	
	CC_SYNTHESIZE_PASS_BY_REF(std::string, mName, Name);

	/* �����󶨵���ʾ���󣬲����Ǳ���ģ����Բ�����ʾ���� */
	CC_SYNTHESIZE_RETAIN(CCNode	*, mDisplay, Display);

	/* �����ܿ��ƵĹؼ��� */
	CC_SYNTHESIZE_RETAIN(CCBaseNode	*, mNode, Node);

	/* �� Tween ���ƵĹؼ��㣬������ Tween ������ node �ĺ�ֵ���� */
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

