#pragma once

#include "CBaseNode.h"

//¹Ø¼üÖ¡Êý¾Ý
class CCFrameNode : public CCBaseNode
{
public:
	static CCFrameNode *create(float _x = 0, float _y = 0, float _rotation = 0);

public:
	CCFrameNode();
	~CCFrameNode(void);

	void		copy(CCBaseNode* _fv);

	std::string toString();

protected:
	void reset();

public:
	float	scaleX;
	float	scaleY;
	float	alpha;
	float	offR;
	int		frame;
};

