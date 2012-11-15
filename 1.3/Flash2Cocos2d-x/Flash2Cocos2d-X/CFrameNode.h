#pragma once

#include "CBaseNode.h"

//�ؼ�֡����
class CCFrameNode : public CCBaseNode
{
public:
	static CCFrameNode *create();

public:
	CCFrameNode();
	~CCFrameNode(void);

	void		copy(CCBaseNode* _fv);

	std::string toString();

protected:
	void reset();

public:
	float	offR;
	int		frame;
	std::string img;
};

