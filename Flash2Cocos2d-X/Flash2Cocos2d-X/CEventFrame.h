#pragma once

#include <string>
#include "cocos2d.h"

using namespace cocos2d;

/*事件帧数据*/
class CCEventFrame : public CCObject {
public:
    static CCEventFrame *create(const char* _name, int _frame);
public:
    /**
    * @param _name 帧事件名
    * @param _frame 该事件发生在第几帧
    */
    CCEventFrame(const char* _name, int _frame);
    ~CCEventFrame(void);

    virtual bool init();

public:
    /*帧事件名*/
    CC_SYNTHESIZE(const char*, mName, Name);

    /*该事件发生在第几帧*/
    CC_SYNTHESIZE_PASS_BY_REF(int, mFrame, Frame);

};

