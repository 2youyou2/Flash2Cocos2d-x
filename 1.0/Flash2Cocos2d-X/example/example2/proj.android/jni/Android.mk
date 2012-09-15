LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := helloworld/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
				   ../../Classes/CKnightArmature.cpp \
				   ../../../../Flash2Cocos2d-X/CAnimation.cpp \
				   ../../../../Flash2Cocos2d-X/CArmature.cpp \
				   ../../../../Flash2Cocos2d-X/CArmatureAniData.cpp \
				   ../../../../Flash2Cocos2d-X/CBaseNode.cpp \
				   ../../../../Flash2Cocos2d-X/CBone.cpp \
				   ../../../../Flash2Cocos2d-X/CBoneAniData.cpp \
				   ../../../../Flash2Cocos2d-X/CConnectionData.cpp \
				   ../../../../Flash2Cocos2d-X/CEventFrame.cpp \
				   ../../../../Flash2Cocos2d-X/CFrameNode.cpp \
				   ../../../../Flash2Cocos2d-X/CFrameNodeList.cpp \
				   ../../../../Flash2Cocos2d-X/CMathHelper.cpp \
				   ../../../../Flash2Cocos2d-X/CProcessBase.cpp \
				   ../../../../Flash2Cocos2d-X/CTween.cpp \
				   ../../../../Flash2Cocos2d-X/CTweenNode.cpp \
				   ../../../../tinyxml/tinystr.cpp \
				   ../../../../tinyxml/tinyxml.cpp \
				   ../../../../tinyxml/tinyxmlerror.cpp \
				   ../../../../tinyxml/tinyxmlparser.cpp


                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
										$(LOCAL_PATH)/../../../../../     \
										$(LOCAL_PATH)/../../../../../cocos2dx \
										$(LOCAL_PATH)/../../../../../cocos2dx/include \
										$(LOCAL_PATH)/../../../../../CocosDenshion/include \
										$(LOCAL_PATH)/../../../../../cocos2dx/platform \
										$(LOCAL_PATH)/../../../../Flash2Cocos2d-X \
										$(LOCAL_PATH)/../../../../tinyxml


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,chipmunk)
