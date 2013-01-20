#include "SPProcessBase.h"
#include "SPUtilMath.h"

namespace SPII
{
namespace SPArmature
{

ProcessBase::ProcessBase(void)
    :m_fAnimationScale(1)
    ,m_bIsComplete(true)
    ,m_bIsPause(true)
    ,m_fCurrentFrame(0)
	,m_fCurrentPrecent(0.0f)
    ,m_iToIndex(0)
    ,m_iDurationTween(1)
    ,m_iDuration(1)
    ,m_iLoop(LIST_LOOP_START)
    ,m_iTweenEasing(0)
{
    /*
     *  set m_fAnimationInternal defualt value to CCDirector::sharedDirector()
     *  ->getAnimationInterval(), in line with game update speed
     */
    m_fAnimationInternal = CCDirector::sharedDirector()->getAnimationInterval();
}


ProcessBase::~ProcessBase(void)
{
}


void ProcessBase::pause()
{
    m_bIsPause = true;
}


void ProcessBase::resume()
{
    m_bIsPause = false;
}

void ProcessBase::stop()
{
    m_bIsComplete = true;
    m_fCurrentFrame = 0;
}

void ProcessBase::play(void * _animation, int _durationTo, int _durationTween,  int _loop, int _tweenEasing)
{
    m_bIsComplete = false;
    m_bIsPause = false;
    m_fCurrentFrame = 0;
    
    /*
     *  Set m_iTotalFrames to durationTo, it is used for change tween between two animation. 
     *  When changing end, m_iTotalFrames will be setted to _durationTween
     */
    m_iTotalFrames = _durationTo;
    m_iTweenEasing = _tweenEasing;
}
  
void ProcessBase::update(float dt)
{
    
    if (m_bIsComplete || m_bIsPause)
    {
        return;
    }
    
	if (m_iTotalFrames <= 0) {
		m_fCurrentFrame = m_iTotalFrames = 1;
	}

    /*
     *  update m_fCurrentFrame, every update add the frame passed. 
     *  dt/m_fAnimationInternal determine it is not a frame animation. If frame speed changed, it will not make our 
     *  animation speed slower or quicker.
     */
    m_fCurrentFrame += m_fAnimationScale * (dt/m_fAnimationInternal);
    
    //CCLOG("%f, %f, %f, %f", dt, m_fCurrentFrame, m_fAnimationScale, m_fAnimationInternal);
    

	m_fCurrentPrecent = m_fCurrentFrame / m_iTotalFrames;

	/*
	 *	if m_fCurrentFrame is bigger or equal than m_iTotalFrames, then reduce it util m_fCurrentFrame is 
     *  smaller than m_iTotalFrames
	 */
    SP_TAKE_FLOAT(m_fCurrentFrame, m_iTotalFrames);
    
	updateHandler();
	
    
}

void ProcessBase::updateHandler()
{
}


}
}