#include "CMathHelper.h"

#include "cocos2d.h"
using namespace cocos2d;

long CCMathHelper::millisecondNow(){ 
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL); 
	return (now.tv_sec * 1000 + now.tv_usec / 1000); 
} 