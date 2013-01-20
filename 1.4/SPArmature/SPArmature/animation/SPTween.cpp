#include "SPTween.h"
#include "SPBone.h"
#include "SPArmature.h"
#include "SPUtilMath.h"

namespace SPII{
namespace SPArmature{


Tween* Tween::create(Bone *_bone)
{
    Tween *pTween = new Tween();
    if (pTween && pTween->init(_bone))
    {
        pTween->autorelease();
        return pTween;
    }
    CC_SAFE_DELETE(pTween);
    return NULL;

}



Tween::Tween()
    :m_pFrom(NULL)
    ,m_pNode(NULL)
    ,m_pBetween(NULL)
    ,m_pBone(NULL)
	,m_pCurrentKeyFrame(NULL)
	,m_pNextKeyFrame(NULL)
	,m_pMovementBoneData(NULL)
	,m_fFrameTweenEasing(0)
{

}


Tween::~Tween(void)
{
    CC_SAFE_DELETE( m_pFrom );
    CC_SAFE_DELETE( m_pNode );
    CC_SAFE_DELETE( m_pBetween );
}


bool Tween::init(Bone *_bone)
{
    bool bRet = false;
    do
    {

        m_pFrom = FrameData::create();
        m_pFrom->retain();
        
		m_pNode = FrameData::create();
        m_pNode->retain();
		m_pNode->m_iZOrder = _bone->getBoneData()->m_iZOrder;

        m_pBetween = FrameData::create();
        m_pBetween->retain();
        
        m_pBone = _bone;

        bRet = true;
    }
    while (0);

    return bRet;
}


void Tween::play(MovementBoneData *_movementBoneData, int _durationTo, int _durationTween,  int _loop, int _tweenEasing)
{
    m_pCurrentKeyFrame = NULL;
    m_pNextKeyFrame = NULL;
    m_bIsTweenKeyFrame = false;
    
    ProcessBase::play(NULL, _durationTo, _durationTween, _loop, _tweenEasing);
   
    m_pMovementBoneData = _movementBoneData ;

    m_iTotalDuration = 0;
    m_iBetweenDuration = 0;
    m_iToIndex = 0;
    //m_pNode->m_fSkewY = int(m_pNode->m_fSkewY) % 360;

    if (m_pMovementBoneData->getFrames()->count() == 1)
    {
        m_iLoop = SINGLE;
        m_pNextKeyFrame = m_pMovementBoneData->getFrame(0);
        setBetween(m_pNode, m_pNextKeyFrame);
        m_bIsTweenKeyFrame = true;
        m_fFrameTweenEasing = 1;
    }
    else if (m_pMovementBoneData->getFrames()->count() > 1)
    {
        if (_loop)
        {
            m_iLoop = LIST_LOOP_START;
            m_iDuration = m_pMovementBoneData->getDuration();
        }
        else
        {
            m_iLoop = LIST_START;
            m_iDuration = m_pMovementBoneData->getDuration() - 1;
        }

        m_iDurationTween = _durationTween * m_pMovementBoneData->getScale();

        if (_loop && m_pMovementBoneData->getDelay() != 0)
        {
            int i = 0;
            setBetween(m_pNode, tweenNodeTo(updateFrameData(1 - m_pMovementBoneData->getDelay()), m_pBetween));
            
			i =1 ;
        }
        else
        {
            m_pNextKeyFrame = m_pMovementBoneData->getFrame(0);
            setBetween(m_pNode, m_pNextKeyFrame);
            m_bIsTweenKeyFrame = true;
        }
    }
}


void Tween::updateHandler()
{
	
    if (m_fCurrentPrecent >= 1)
    {
        switch(m_iLoop)
        {
        case SINGLE:
            m_pCurrentKeyFrame = m_pNextKeyFrame;
            m_fCurrentPrecent = 1;
            m_bIsComplete = true;
            break;
        case LIST_START:
            m_iLoop = LIST;
            if (m_iDurationTween <= 0)
            {
                m_fCurrentPrecent = 1;
            }
            else
            {
                m_fCurrentPrecent = (m_fCurrentPrecent - 1) * m_iTotalFrames / m_iDurationTween;
            }
            if (m_fCurrentPrecent >= 1)
            {
                m_fCurrentPrecent = 1;
                m_bIsComplete = true;
                break;
            }
            else
            {
                m_iTotalFrames = m_iDurationTween;
                m_iTotalDuration = 0;
                break;
            }
        case LIST:
            m_fCurrentPrecent = 1;
            m_bIsComplete = true;
            break;
        case LIST_LOOP_START:
            m_iLoop = 0;
            
			m_iTotalFrames = m_iDurationTween > 0 ? m_iDurationTween : 1;
                
			if (m_pMovementBoneData->getDelay() != 0)
            {
                //
                m_fCurrentFrame = (1 - m_pMovementBoneData->getDelay()) * (float)m_iTotalFrames;
                m_fCurrentPrecent = m_fCurrentFrame / m_iTotalFrames;
                
                
            }else
            {
				m_fCurrentPrecent = 0;
                m_fCurrentFrame = 0;
			}
                
            m_iTotalDuration = 0;
            m_iBetweenDuration = 0;
            m_iToIndex = 0;
            
            break;
        default:
            m_iLoop += int(m_fCurrentPrecent);
            SP_TAKE_FLOAT(m_fCurrentPrecent, 1);
            SP_TAKE_FLOAT(m_fCurrentFrame, m_iTotalFrames);
 
            m_iTotalDuration = 0;
            m_iBetweenDuration = 0;
            m_iToIndex = 0;
            break;
        }
    }
    else if (m_iLoop < LIST)
    {
        m_fCurrentPrecent = sin(m_fCurrentPrecent * SP_HALF_PI);
    }

    if (m_iLoop >= LIST)
    {
        m_fCurrentPrecent = updateFrameData(m_fCurrentPrecent, true);
    }
	
    if(m_fFrameTweenEasing != TWEEN_EASING_MAX)
	{
		tweenNodeTo(m_fCurrentPrecent);
	}
    else if(m_pCurrentKeyFrame)
    {
        tweenNodeTo(0);
    }
	
    if(m_pCurrentKeyFrame)
    {
        
        int _displayIndex = m_pCurrentKeyFrame->m_iDisplayIndex;

		if ( m_pCurrentKeyFrame->m_iDisplayIndex == -1 || m_pNextKeyFrame->m_iDisplayIndex == -1 )
		{
			_displayIndex = -1;
		}

        
		m_pBone->changeDisplayByIndex(_displayIndex);
        
        m_pBone->setZOrder(m_pCurrentKeyFrame->m_iZOrder);
        
//		if (m_pBone->getArmature())
//		{
//			m_pBone->getArmature()->setBonesIndexChanged(true);
//		}

//         Armature *_childAramture = m_pBone->childArmature;
//         m_pBone->changeDisplay(_displayIndex);
//         if(_childAramture)
//         {
//             _childAramture.origin.z = m_pCurrentKeyFrame->z;
//             if(m_pCurrentKeyFrame->m_strMovement)
//             {
//                 _childAramture.animation.play(m_pCurrentKeyFrame->m_strMovement);
//             }
//         }
        
        Armature *_childAramture = m_pBone->getChildArmature();
        
        if(_childAramture)
        {
            if(m_pCurrentKeyFrame->m_strMovement.compare("") != 0)
            {
                _childAramture->getAnimation()->play(m_pCurrentKeyFrame->m_strMovement.c_str());
            }
        }

        if(m_pCurrentKeyFrame->m_strEvent.length() != 0)
        {
            //m_pBone->dispatchEventWith(Event.BONE_EVENT_FRAME, m_pCurrentKeyFrame->event);
        }
        if(m_pCurrentKeyFrame->m_strSound.length() != 0)
        {
            //soundManager.dispatchEventWith(Event.SOUND_FRAME, m_pCurrentKeyFrame->sound);
        }
        m_pCurrentKeyFrame = NULL;
    }

//    if(m_bIsTweenKeyFrame)
//    {
//        //to
//        /*if(m_pNextKeyFrame.displayIndex < 0){
//        	//m_pBone->changeDisplay(m_pNextKeyFrame.displayIndex);
//        	if(m_pBone->armature){
//        		//m_pBone->armature.bonesIndexChanged = true;
//        	}
//        }*/
//        m_bIsTweenKeyFrame = false;
//    }
}


void Tween::setBetween(FrameData *_from, FrameData *_to)
{
    if(_from->m_iDisplayIndex == -1 && _to->m_iDisplayIndex == -1)
    {
        return;
    }
    else if (_from->m_iDisplayIndex == -1)
    {
        m_pBetween->subtract(_to, _to);
    }
    else if(_to->m_iDisplayIndex == -1)
    {
        m_pBetween->subtract(_from, _from);
    }
    else
    {
        m_pFrom->copy(_from);
        m_pBetween->subtract(_from, _to);
    }


}

FrameData *Tween::tweenNodeTo(float _percent, FrameData *_node)
{
    if( _node == NULL )
    {
        _node = m_pNode;
    }


    _node->m_fX = m_pFrom->m_fX + _percent * m_pBetween->m_fX;
    _node->m_fY = m_pFrom->m_fY + _percent * m_pBetween->m_fY;
    _node->m_fScaleX = m_pFrom->m_fScaleX + _percent * m_pBetween->m_fScaleX;
    _node->m_fScaleY = m_pFrom->m_fScaleY + _percent * m_pBetween->m_fScaleY;
    _node->m_fSkewX = m_pFrom->m_fSkewX + _percent * m_pBetween->m_fSkewX;
    _node->m_fSkewY = m_pFrom->m_fSkewY + _percent * m_pBetween->m_fSkewY;
    
    _node->m_iAlpha = m_pFrom->m_iAlpha + _percent * m_pBetween->m_iAlpha;
    _node->m_iRed = m_pFrom->m_iRed + _percent * m_pBetween->m_iRed;
    _node->m_iGreen = m_pFrom->m_iGreen + _percent * m_pBetween->m_iGreen;
    _node->m_iBlue = m_pFrom->m_iBlue + _percent * m_pBetween->m_iBlue;

    return _node;
}

float Tween::updateFrameData(float _currentPrecent, bool _activeFrame)
{

    float _played = (float)m_iDuration * _currentPrecent;
    
    
    int _fromIndex;
    FrameData *_from;
    FrameData *_to;
    bool _isListEnd;

    // if play to current frame's front or back, then find current frame again
    if (_played >= m_iTotalDuration || _played < m_iTotalDuration - m_iBetweenDuration)
    {
        
        /*
         *  get frame length, if m_iToIndex >= _length, then set m_iToIndex to 0, start anew.
         *  m_iToIndex is next index will play
         */
        int _length = m_pMovementBoneData->getLength();
        do
        {
            m_iBetweenDuration = m_pMovementBoneData->getFrame(m_iToIndex)->m_iDuration;
            m_iTotalDuration += m_iBetweenDuration;
            _fromIndex = m_iToIndex;
            if (++m_iToIndex >= _length)
            {
                m_iToIndex = 0;
            }
        }
        while (_played >= m_iTotalDuration);
        
        _isListEnd = m_iLoop == LIST && m_iToIndex == 0;
        
        if(_isListEnd)
        {
            _to = _from = m_pMovementBoneData->getFrame(_fromIndex);
        }
        else
        {
            _from = m_pMovementBoneData->getFrame(_fromIndex);
            _to = m_pMovementBoneData->getFrame(m_iToIndex);
        }
        
        m_fFrameTweenEasing = _from->m_fTweenEasing;
        
        if (_activeFrame)
        {
            m_pCurrentKeyFrame = m_pNextKeyFrame;

            if(!_isListEnd)
            {
                m_pNextKeyFrame = _to;
                m_bIsTweenKeyFrame = true;
            }
        }
        
        setBetween(_from, _to);
        
    }
    _currentPrecent = 1 - (m_iTotalDuration - _played) / (float)m_iBetweenDuration;

    
	/*
     *  if frame tween easing equal to TWEEN_EASING_MAX, then it will not do tween.
     *  -1  :   fade out
     *  0   :   line
     *  1   :   fade in
     *  2   :   fade in and out
     */
 	float _tweenEasing = 0.0f;
 	if ( m_fFrameTweenEasing != TWEEN_EASING_MAX)
     {
         _tweenEasing = (m_iTweenEasing == TWEEN_EASING_MAX) ? m_fFrameTweenEasing : m_iTweenEasing;
         if (_tweenEasing)
         {
             _currentPrecent = getEaseValue(_currentPrecent, _tweenEasing);
         }
     }
    return _currentPrecent;
}

float Tween::getEaseValue(float _percent, float _easing)
{
    if (_easing > 1)
    {
        _percent = 0.5 * (1 - cos(_percent * SP_PI ));
        _easing -= 1;
    }
    else if (_easing > 0)
    {
        _percent = sin(_percent * SP_HALF_PI);
    }
    else
    {
        _percent = 1 - cos(_percent * SP_HALF_PI);
        _percent = -_easing;
    }
    return _percent * _easing + (1 - _easing);
}


}
}