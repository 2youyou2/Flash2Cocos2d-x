#include "SPAnimation.h"
#include "SPArmature.h"
#include "SPBone.h"
#include "SPArmatureDefine.h"
#include "SPUtilMath.h"
#include "SPConstValue.h"
using namespace std;

namespace SPII
{
namespace SPArmature
{
    
        
Animation *Animation::create(Armature *_armature)
{
    Animation *pARmatureAnimation = new Animation();
    if (pARmatureAnimation && pARmatureAnimation->init(_armature))
    {
        pARmatureAnimation->autorelease();
        return pARmatureAnimation;
    }
    CC_SAFE_DELETE(pARmatureAnimation);
    return NULL;
}

Animation::Animation()
:m_pAnimationData(NULL)
//,m_pMovementDataList(NULL)
,m_pCurrentFrameData(NULL)
,m_strMovementID("")
,m_pArmature(NULL)
{
    
}

Animation::~Animation(void)
{
    
}

bool Animation::init(Armature *_armature)
{
    bool bRet = false;
    do
    {
        
        m_pArmature = _armature;
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}


void Animation:: pause()
{
    ProcessBase::pause();
    
    CCDictElement *_element = NULL;
    CCDictionary *_dict = m_pArmature->getBoneDic();
    CCDICT_FOREACH(_dict, _element)
    {
        Bone *_bone = (Bone*)_element->getObject();
        if (_bone->getTween())
        {
            _bone->getTween()->pause();
        }
    }
    
}

void Animation::resume()
{
    ProcessBase::resume();
    
    CCDictElement *_element = NULL;
    CCDictionary *_dict = m_pArmature->getBoneDic();
    CCDICT_FOREACH(_dict, _element)
    {
        Bone* _bone = (Bone*)_element->getObject();
        if (_bone->getTween())
        {
            _bone->getTween()->resume();
        }
    }
    
}

void Animation::stop()
{
    ProcessBase::stop();
    
    CCDictElement *_element = NULL;
    CCDictionary *_dict = m_pArmature->getBoneDic();
    CCDICT_FOREACH(_dict, _element)
    {
        Bone *_bone = (Bone*)_element->getObject();
        if (_bone->getTween())
        {
            _bone->getTween()->stop();
        }
    }
    
}


void Animation::setData(AnimationData* _aniData)
{
    stop();
    m_pAnimationData = _aniData;
    m_arrMovNames = m_pAnimationData->getMovNames();
}

void Animation::setAnimationScale(float _animationScale )
{
    if(_animationScale == m_fAnimationScale)
    {
        return;
    }
    
    /*
     *  when calculate m_fCurrentFrame, we will do a processing of m_fCurrentFrame += m_fAnimationScale * (dt/m_fAnimationInternal);
     *  then if m_fAnimationScale is bigger, the m_fCurrentFrame grow quicker, animation 
     *  scale up.
     *  We want animation slower when m_fAnimationScale is bigger, so do 1/_animationScale.
     */
    m_fAnimationScale = 1/_animationScale;
    
    CCDictElement *_element = NULL;
    CCDictionary *_dict = m_pArmature->getBoneDic();
    CCDICT_FOREACH(_dict, _element)
    {
        Bone *_bone = (Bone*)_element->getObject();
        if (_bone->getTween())
        {
            _bone->getTween()->setAnimationScale(m_fAnimationScale);
        }
    }
}
    
std::vector<std::string> Animation::getMovmentNames()
{
    return m_arrMovNames;
}

int Animation::getMovementCount()
{
    return m_arrMovNames.size();
}

void Animation::play(const char *_animationName, int _durationTo, int _durationTween,  int _loop, int _tweenEasing)
{
    CCAssert(m_pAnimationData, "m_pAnimationData can not be null");
    
    m_pMovementData = m_pAnimationData->getMovment( _animationName );
    CCAssert(m_pMovementData, "m_pMovementData can not be null");
    
    m_strMovementID		= _animationName;
    
    _durationTo		= (_durationTo == -1) ? m_pMovementData->getDurationTo() : _durationTo;
    _durationTween	= (_durationTween == -1) ? m_pMovementData->getDurationTween() : _durationTween;
    _tweenEasing	= (_tweenEasing == TWEEN_EASING_MAX) ? m_pMovementData->getTweenEasing() : _tweenEasing;
    _loop			= (_loop < 0) ? m_pMovementData->getLoop() : _loop;
    
    
    ProcessBase::play((void*)_animationName, _durationTo, _durationTween, _loop, _tweenEasing);
    
    
    m_iDuration = m_pMovementData->getDuration();
    
    if (m_iDuration == 1)
    {
        m_iLoop = SINGLE;
    }
    else
    {
        if (_loop)
        {
            m_iLoop = LIST_LOOP_START;
        }
        else
        {
            m_iLoop = LIST_START;
            m_iDuration --;
        }
        m_iDurationTween = _durationTween;
    }
    
    MovementBoneData *_movementBoneData;
    
    CCDictElement *_element = NULL;
    CCDictionary *_dict = m_pArmature->getBoneDic();
    CCDICT_FOREACH(_dict, _element)
    {
        Bone *_bone = (Bone*)_element->getObject();
        _movementBoneData = m_pMovementData->getMovementBoneData(_bone->getName().c_str());
        if(_movementBoneData){
            _bone->getTween()->play(_movementBoneData, _durationTo, _durationTween, _loop, _tweenEasing);
            //            if(dynamic_cast<Armature*>(_bone) != NULL)
            //            {
            //                Armature *_armature = (Armature*)_bone;
            //                Animation *_animation = _armature->getAnimation();
            //                std::vector<std::string> names = _animation->getMovmentNames();
            //                _animation->play(names.at(0).c_str());
            //            }
        }else{
            _bone->changeDisplayByIndex(-1);
            if (_bone->getTween())
            {
                _bone->getTween()->stop();
            }
        }
    }
    
    
}


void Animation::playByIndex(int _animationIndex, int _durationTo, int _durationTween,  int _loop, int _tweenEasing)
{
    CC_ASSERT((_animationIndex > -1) && (_animationIndex < m_arrMovNames.size()), "");
    
    std::string _animationName = m_arrMovNames.at(_animationIndex);
    play(_animationName.c_str(), _durationTo, _durationTween, _loop, _tweenEasing);
}

void Animation::updateHandler()
{
    
    if (m_fCurrentPrecent >= 1)
    {
        switch(m_iLoop)
        {
            case LIST_START:
                
                m_iLoop = LIST;
                m_fCurrentFrame = (m_fCurrentPrecent-1) * m_iTotalFrames;
                m_fCurrentPrecent = m_fCurrentFrame / m_iDurationTween;
                
                if (m_fCurrentPrecent >= 1.0f)
                {
                }
                else
                {
                    m_iTotalFrames = m_iDurationTween;
                    
                    m_pArmature->onMovementEvent(START, m_strMovementID.c_str());
                    break;
                }
            case LIST:
            case SINGLE:
                m_fCurrentPrecent = 1;
                m_bIsComplete = true;
                
                m_pArmature->onMovementEvent(COMPLETE, m_strMovementID.c_str());
                break;
            case LIST_LOOP_START:
                m_iLoop = 0;
                SP_TAKE_FLOAT(m_fCurrentPrecent, 1);
                SP_TAKE_FLOAT(m_fCurrentFrame, m_iTotalFrames);
                m_iTotalFrames = m_iDurationTween > 0 ? m_iDurationTween : 1;

                m_pArmature->onMovementEvent(START, m_strMovementID.c_str());
                break;
            default:
                m_iLoop += int(m_fCurrentPrecent);
                SP_TAKE_FLOAT(m_fCurrentPrecent, 1);
                SP_TAKE_FLOAT(m_fCurrentFrame, m_iTotalFrames);
                m_iToIndex = 0;
                
                m_pArmature->onMovementEvent(LOOP_COMPLETE, m_strMovementID.c_str());
                break;
        }
    }
    
    if (m_iLoop >= LIST )
    {
        //updateFrameData(m_fCurrentPrecent);
    }
}

void Animation::updateFrameData(float _currentPercent)
{
    int _length = m_pMovementData->getMovFrameDataArr()->count();
    if(_length == 0){
        return;
    }
    float _played = m_iDuration * _currentPercent;
    
    if (!m_pCurrentFrameData || _played >= m_pCurrentFrameData->m_iDuration + m_pCurrentFrameData->m_iStart || _played < m_pCurrentFrameData->m_iStart) {
        while (true) {
            m_pCurrentFrameData =  m_pMovementData->getMoveFrameData(m_iToIndex);
            if (++m_iToIndex >= _length) {
                m_iToIndex = 0;
            }
            if(m_pCurrentFrameData && _played >= m_pCurrentFrameData->m_iStart && _played < m_pCurrentFrameData->m_iDuration + m_pCurrentFrameData->m_iStart){
                break;
            }
        }
        if(m_pCurrentFrameData->m_strEvent.length() != 0){
            m_pArmature->onMovementEvent(MOVEMENT_EVENT_FRAME, m_pCurrentFrameData->m_strEvent.c_str());
        }
        if(m_pCurrentFrameData->m_strSound.length() != 0){
            m_pArmature->onMovementEvent(SOUND_FRAME, m_pCurrentFrameData->m_strSound.c_str());
        }
        if(m_pCurrentFrameData->m_strMovement.length() != 0){
            play(m_pCurrentFrameData->m_strMovement.c_str());
        }
    }
}


        
}
}