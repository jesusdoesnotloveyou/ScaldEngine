cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(PROJECT_NAME ScaldEngine)

project(${PROJECT_NAME}
            VERSION 0.0.1
            LANGUAGES CXX C 
)

# common
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# output directory for library artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# cpp options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDES
    ScaldEngine/Data/ModelData.h

    ScaldEngine/Delegates/Delegates.h
    ScaldEngine/Delegates/catch.hpp
    
    ScaldEngine/Games/Katamari/KatamariMovementComponent.h
    ScaldEngine/Games/Katamari/KatamariPlayerPawn.h
    
    ScaldEngine/Graphics/Camera/Camera.h
    ScaldEngine/Graphics/Camera/ThirdPersonCamera.h

    ScaldEngine/Graphics/Particles/FireParticleSystem.h
    ScaldEngine/Graphics/Particles/ParticleHelper.h
    ScaldEngine/Graphics/Particles/ParticleSystem.h

    ScaldEngine/Graphics/Renderer/DeferredRenderer.h
    ScaldEngine/Graphics/Renderer/ForwardRenderer.h
    ScaldEngine/Graphics/Renderer/Renderer.h

    ScaldEngine/Graphics/Shadows/CascadeShadowMap.h
    ScaldEngine/Graphics/Shadows/ShadowMap.h

    ScaldEngine/Graphics/Scene/Scene.h
    ScaldEngine/Graphics/Scene/LightSceneProxy.h
    ScaldEngine/Graphics/Scene/PrimitiveSceneProxy.h

    ScaldEngine/Graphics/DXHelper.h
    ScaldEngine/Graphics/Graphics.h
    ScaldEngine/Graphics/Mesh.h
    ScaldEngine/Graphics/Model.h
    ScaldEngine/Graphics/LightHelper.h
    ScaldEngine/Graphics/ScaldCoreDefines.h
    ScaldEngine/Graphics/ScaldCoreTypes.h
    ScaldEngine/Graphics/Shaders.h
    ScaldEngine/Graphics/VertexBuffer.h
    ScaldEngine/Graphics/IndexBuffer.h
    ScaldEngine/Graphics/ConstantBuffer.h
    ScaldEngine/Graphics/StructuredBuffer.h
    
    ScaldEngine/GameFramework/Components/Collision/CollisionComponent.h
    ScaldEngine/GameFramework/Components/Movement/MovementComponent.h
    ScaldEngine/GameFramework/Components/Input/InputComponent.h
    ScaldEngine/GameFramework/Components/Transform.h

    ScaldEngine/GameFramework/Components/ScaldActorComponent.h
    ScaldEngine/GameFramework/Components/SceneComponent.h
    ScaldEngine/GameFramework/Components/LightComponentBase.h
    ScaldEngine/GameFramework/Components/PrimitiveComponent.h
    ScaldEngine/GameFramework/Components/MeshComponent.h
    
    ScaldEngine/GameFramework/ScaldObject.h
    ScaldEngine/GameFramework/World.h
    ScaldEngine/GameFramework/Objects/Actor.h
    ScaldEngine/GameFramework/Objects/Pawn.h
    
    ScaldEngine/Utils/Shapes.h

    ScaldEngine/ScaldCore/Engine/Engine.h
    ScaldEngine/ScaldCore/Engine/RenderWindow.h
    ScaldEngine/ScaldCore/Engine/ScaldTimer.h
    ScaldEngine/ScaldCore/Input/InputDevice.h
    ScaldEngine/ScaldCore/Input/Keys.h

    ScaldEngine/ScaldException.h
)
    
set(SOURCES
    ScaldEngine/Delegates/Delegates.cpp
    
    ScaldEngine/Games/Katamari/KatamariMovementComponent.cpp
    ScaldEngine/Games/Katamari/KatamariPlayerPawn.cpp
    
    ScaldEngine/Graphics/Camera/Camera.cpp
    ScaldEngine/Graphics/Camera/ThirdPersonCamera.cpp
    
    ScaldEngine/Graphics/Particles/FireParticleSystem.cpp
    ScaldEngine/Graphics/Particles/ParticleSystem.cpp
    
    ScaldEngine/Graphics/Renderer/DeferredRenderer.cpp
    ScaldEngine/Graphics/Renderer/ForwardRenderer.cpp
    ScaldEngine/Graphics/Renderer/Renderer.cpp
    
    ScaldEngine/Graphics/Shadows/CascadeShadowMap.cpp
    ScaldEngine/Graphics/Shadows/ShadowMap.cpp

    ScaldEngine/Graphics/Scene/Scene.cpp
    ScaldEngine/Graphics/Scene/LightSceneProxy.cpp
    ScaldEngine/Graphics/Scene/PrimitiveSceneProxy.cpp

    ScaldEngine/Graphics/Graphics.cpp
    ScaldEngine/Graphics/Mesh.cpp
    ScaldEngine/Graphics/Model.cpp
    ScaldEngine/Graphics/Shaders.cpp

    ScaldEngine/GameFramework/Components/Collision/CollisionComponent.cpp
    ScaldEngine/GameFramework/Components/Movement/MovementComponent.cpp
    ScaldEngine/GameFramework/Components/Input/InputComponent.cpp
    ScaldEngine/GameFramework/Components/Transform.cpp
    
    ScaldEngine/GameFramework/Components/ScaldActorComponent.cpp
    ScaldEngine/GameFramework/Components/SceneComponent.cpp
    ScaldEngine/GameFramework/Components/LightComponentBase.cpp
    ScaldEngine/GameFramework/Components/PrimitiveComponent.cpp
    ScaldEngine/GameFramework/Components/MeshComponent.cpp
    
    ScaldEngine/GameFramework/World.cpp
    ScaldEngine/GameFramework/Objects/Actor.cpp
    ScaldEngine/GameFramework/Objects/Pawn.cpp

    
    ScaldEngine/Utils/Shapes.cpp

    ScaldEngine/ScaldCore/Engine/Engine.cpp
    ScaldEngine/ScaldCore/Engine/RenderWindow.cpp
    ScaldEngine/ScaldCore/Engine/ScaldTimer.cpp
    ScaldEngine/ScaldCore/Input/InputDevice.cpp
    
    ScaldEngine/Main.cpp
    ScaldEngine/ScaldException.cpp
)

# 3rd party
include(FetchContent)

FetchContent_Declare(
	Assimp
    	GIT_REPOSITORY https://github.com/assimp/assimp.git
    	GIT_TAG v6.0.5
    	GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Assimp)

if(${WIN32})
	FetchContent_Declare(
    		DirectxTex
    		GIT_REPOSITORY https://github.com/microsoft/DirectXTex.git
    		GIT_TAG oct2025
    		GIT_SHALLOW TRUE
	)
    FetchContent_MakeAvailable(DirectxTex)

    FetchContent_Declare(
        DirectXTK
        GIT_REPOSITORY https://github.com/microsoft/DirectXTK.git
        GIT_TAG may2026
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(DirectXTK)

else()
    message(WARNING "DirectXTex and DirectXTK are WIN32 only library, it would not be loaded")
endif()

if(${WIN32})
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${INCLUDES})
else()
    message(WARNING "Project is WIN32 only for now")
endif()

# pch
target_precompile_headers(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/ScaldEngine/stdafx.h
)

# includes
target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/ScaldEngine
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE assimp::assimp DirectXTex DirectXTK
    PRIVATE dxgi.lib d3d11.lib d3dcompiler.lib
)

# compile options for target
# target_compile_options(${PROJECT_NAME} PRIVATE <>)

# compile definitions for target
# if (MSVC)
# 	target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE) # CHAR WCHAR windows stuff
# endif()

########### other actions
set(ASSET_PATHS Data/Shaders Data/Models Data/Textures)
foreach(PATH IN LISTS ASSET_PATHS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/ScaldEngine/${PATH}"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PATH}"
)
endforeach()

# =============================================================
# debug info
if(${WIN32})
	message("Running on Windows")
elseif(${LINUX})
	message("Running on Linux")
endif()

if(${MSVC})
	message("MSVC version: ${MSVC_VERSION}")
	message("MSVC toolset version: ${MSVC_TOOLSET_VERSION}")
endif()

message("Using ${CMAKE_CXX_COMPILER_ID} compiler")
message("Compiler flags: ${CMAKE_CXX_FLAGS}")
message("Compiler debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
message("Compiler release flags: ${CMAKE_CXX_FLAGS_RELEASE}")

message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

message("Configuration types: ${CMAKE_CONFIGURATION_TYPES}")