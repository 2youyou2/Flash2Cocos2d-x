#pragma once

#include <string>
#include "cocos2d.h"

using namespace cocos2d;

/*�¼�֡����*/
class CCEventFrame : public CCObject {
public:
    static CCEventFrame *create(const char* _name, int _frame);
public:
    /**
    * @param _name ֡�¼���
    * @param _frame ���¼������ڵڼ�֡
    */
    CCEventFrame(const char* _name, int _frame);
    ~CCEventFrame(void);

    virtual bool init();

public:
    /*֡�¼���*/
    CC_SYNTHESIZE(const char*, mName, Name);

    /*���¼������ڵڼ�֡*/
    CC_SYNTHESIZE_PASS_BY_REF(int, mFrame, Frame);

};

