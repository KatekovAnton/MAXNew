LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_CFLAGS += -std=c++11

LOCAL_SRC_FILES := main.cpp \
AppDelegate.h \
AppDelegate.cpp \
../../MAX/Game/Engine/MAXCamera.cpp \
../../MAX/Game/Engine/MAXEngine.cpp \
../../MAX/Game/Engine/Animation/MAXAnimationBase.cpp \
../../MAX/Game/Engine/Animation/MAXAnimationEffect.cpp \
../../MAX/Game/Engine/Animation/MAXAnimationManager.cpp \
../../MAX/Game/Engine/Animation/MAXAnimationObject.cpp \
../../MAX/Game/Engine/Animation/MAXAnimationObjectUnit.cpp \
../../MAX/Game/Engine/Animation/MAXAnimationRepeat.cpp \
../../MAX/Game/Engine/Animation/MAXAnimationSequence.cpp \
../../MAX/Game/Engine/Animation/MAXAnimationWait.cpp \
../../MAX/Game/Engine/MAXContetnLoader/MAXContentLoader.cpp \
../../MAX/Game/Engine/MAXContetnLoader/MAXContentObjects/MAXClanConfig.cpp \
../../MAX/Game/Engine/MAXContetnLoader/MAXContentObjects/MAXConfigManager.cpp \
../../MAX/Game/Engine/MAXContetnLoader/MAXContentObjects/MAXContentMap.cpp \
../../MAX/Game/Engine/MAXContetnLoader/MAXContentObjects/MAXObjectConfig.cpp \
../../MAX/Game/Engine/ObjectComponents/Material/Material.cpp \
../../MAX/Game/Engine/ObjectComponents/Material/MAXMapMaterial.cpp \
../../MAX/Game/Engine/ObjectComponents/Material/MAXREsourceMapMaterial.cpp \
../../MAX/Game/Engine/ObjectComponents/Material/MAXUnitMaterial.cpp \
../../MAX/Game/Engine/ObjectComponents/Material/TextureMaterial.cpp \
../../MAX/Game/Engine/ObjectComponents/Meshes/EngineMesh.cpp \
../../MAX/Game/Engine/ObjectComponents/Meshes/EngineTiledDynamicMesh.cpp \
../../MAX/Game/Engine/ObjectComponents/Textures/Texture.cpp \
../../MAX/Game/Engine/Objects/LevelObject.cpp \
../../MAX/Game/Engine/Objects/MapRenderObject.cpp \
../../MAX/Game/Engine/Objects/MAXEffectObject.cpp \
../../MAX/Game/Engine/Objects/MAXMapObject.cpp \
../../MAX/Game/Engine/Objects/MAXObject.cpp \
../../MAX/Game/Engine/Objects/MAXUnitObject.cpp \
../../MAX/Game/Engine/Objects/MAXUnitRenderObject.cpp \
../../MAX/Game/Engine/Objects/PivotObject.cpp \
../../MAX/Game/Engine/Objects/RenderObject.cpp \
../../MAX/Game/Engine/Objects/UnAnimRenderObject.cpp \
../../MAX/Game/Engine/Renderer/Framebuffer.cpp \
../../MAX/Game/Engine/Renderer/MAXDrawPrimitives.cpp \
../../MAX/Game/Engine/Renderer/MAXGrid.cpp \
../../MAX/Game/Engine/Renderer/MAXResourceMapRenderer.cpp \
../../MAX/Game/Engine/Renderer/MAXSolidTileRenderer.cpp \
../../MAX/Game/Engine/Renderer/MAXStatusRenderer.cpp \
../../MAX/Game/Engine/Renderer/MAXUnitSelection.cpp \
../../MAX/Game/Engine/Renderer/RenderSystem.cpp \
../../MAX/Game/Engine/Renderer/Shader/Shader.cpp \
../../MAX/Game/Engine/Scene/SceneSystem.cpp \
../../MAX/Game/Engine/Scene/SceneGraph/MAXSceneGraph.cpp \
../../MAX/Game/Engine/Scene/SceneGraph/MAXSceneGraphNode.cpp \
../../MAX/Game/Game/MAXGame.cpp \
../../MAX/Game/Game/MAXGameController.cpp \
../../MAX/Game/Game/GameMatch/GameMap.cpp \
../../MAX/Game/Game/GameMatch/GameMapResources.cpp \
../../MAX/Game/Game/GameMatch/GameMatch.cpp \
../../MAX/Game/Game/GameMatch/GameMatchPlayer.cpp \
../../MAX/Game/Game/GameMatch/GameSettings.cpp \
../../MAX/Game/Game/GameMatch/MatchMapAgregator.cpp \
../../MAX/Game/Game/GameMatch/Pathfinder/Pathfinder.cpp \
../../MAX/Game/Game/GameMatch/Pathfinder/PFWave.cpp \
../../MAX/Game/Game/GameMatch/Pathfinder/PFWaveCell.cpp \
../../MAX/Game/Game/GameMatch/PlayerInformation/GameFog.cpp \
../../MAX/Game/Game/GameMatch/PlayerInformation/PlayerBase.cpp \
../../MAX/Game/Game/GameMatch/PlayerInformation/PlayerResearchManager.cpp \
../../MAX/Game/Game/GameMatch/PlayerInformation/PlayerResourceMap.cpp \
../../MAX/Game/Game/GameMatch/PlayerInformation/PlayerUpgradeManager.cpp \
../../MAX/Game/Game/Interface/GameInteface.cpp \
../../MAX/Game/Game/Interface/UnitParameterNode/GIParameterRow.cpp \
../../MAX/Game/Game/Interface/UnitParameterNode/GIUnitParametersNode.cpp \
../../MAX/Game/Game/Objects/GameObject.cpp \
../../MAX/Game/Game/Objects/GameUnit.cpp \
../../MAX/Game/Game/Objects/GameUnitCurrentState.cpp \
../../MAX/Game/Game/Objects/GameUnitParameters.cpp \
../../MAX/Game/Game/Objects/SelectedGameObjectDelegate.cpp \
../../MAX/Game/Game/Objects/Effects/GameEffect.cpp \
../../MAX/Game/Vendors/CocosHelper/CocosHelper.cpp \
../../MAX/Sys/Display.cpp \
../../MAX/Sys/DisplayPinchDelegate.cpp \
../../MAX/Sys/FileManger.cpp \
../../MAX/Utils/BinaryReader.cpp \
../../MAX/Utils/IdGenerator.cpp \
../../MAX/Utils/StringUtils.cpp \
../../MAX/Utils/Network/Request.cpp \
../../MAX/Utils/Network/RequestManager.cpp \
../../MAX/Utils/Network/Response.cpp \
../../MAX/Utils/Threads/Thread.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../MAX/Game/Engine/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/Animation
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/MAXContetnLoader
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/MAXContetnLoader/MAXContentObjects
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/ObjectComponents/Material
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/ObjectComponents/Meshes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/ObjectComponents/Textures
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/Objects
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/Renderer
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/Renderer/Shader
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/Scene
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Engine/Scene/SceneGraph
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Game
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Game/GameMatch
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Game/GameMatch/Pathfinder
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Game/GameMatch/PlayerInformation
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Game/Interface
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Game/Interface/UnitParameterNode
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Game/Objects
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Game/Objects/Effects
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Vendors/aharren-LibComponentLogging-Core-17e2940
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Vendors/CocosHelper
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Game/Vendors/SQLite
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Sys
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Utils
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Utils/MathKit
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Utils/Network
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Utils/Network/curl
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../MAX/Utils/Threads

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
