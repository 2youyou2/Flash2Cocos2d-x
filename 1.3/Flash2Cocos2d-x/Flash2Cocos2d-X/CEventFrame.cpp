#include "CEventFrame.h"

CCEventFrame *CCEventFrame::create(const char* _name, int _frame) {
    CCEventFrame *pEventFrame = new CCEventFrame(_name, _frame);
    if (pEventFrame && pEventFrame->init()) {
        pEventFrame->autorelease();
        return pEventFrame;
    }
    CC_SAFE_DELETE(pEventFrame);
    return NULL;
}


CCEventFrame::CCEventFrame(const char* _name, int _frame) {
    mName = _name;
    mFrame = _frame;
}


CCEventFrame::~CCEventFrame(void) {
}

bool CCEventFrame::init() {
    bool bRet = false;
    do {
        bRet = true;
    } while (0);

    return bRet;
}
