#pragma once
#include "cocos2d.h"
using namespace cocos2d;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 

#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define CCXLOG(...)       do {} while (0)

#elif COCOS2D_DEBUG == 1
#define CCXLOG(format, ...)      CCLog(format, ##__VA_ARGS__)
#endif

#else

// cocos2d debug
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define CCXLOG(...)       do {} while (0)

#elif COCOS2D_DEBUG == 1
#define CCXLOG(format, ...)      CCLog(format, ##__VA_ARGS__)

#elif COCOS2D_DEBUG > 1
#define CCXLOG(format, ...)      CCLog(format, ##__VA_ARGS__)
#endif // COCOS2D_DEBUG


#endif 


