cmake_minimum_required(VERSION 3.24)
project(TouchEngine_Windows)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DUNICODE -D_UNICODE -DGLEW_STATIC)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:wWinMainCRTStartup")

include_directories(src)
include_directories(src/GL)

add_executable(TouchEngine_Windows
        src/GL/glew.h
        src/GL/wglew.h
        src/DirectXDevice.cpp
        src/DirectXDevice.h
        src/DirectXImage.cpp
        src/DirectXImage.h
        src/DirectXRenderer.cpp
        src/DirectXRenderer.h
        src/DirectXTexture.cpp
        src/DirectXTexture.h
        src/DocumentWindow.cpp
        src/DocumentWindow.h
        src/Drawable.cpp
        src/Drawable.h
        src/FileReader.cpp
        src/FileReader.h
        src/glew.c
        src/OpenGLImage.cpp
        src/OpenGLImage.h
        src/OpenGLProgram.cpp
        src/OpenGLProgram.h
        src/OpenGLRenderer.cpp
        src/OpenGLRenderer.h
        src/OpenGLTexture.cpp
        src/OpenGLTexture.h
        src/Renderer.cpp
        src/Renderer.h
        src/resource.h
        src/stdafx.cpp
        src/stdafx.h
        src/targetver.h
        src/VertexShader.cpp
        src/VertexShader.h

        src/TouchEngineExample.rc)

target_include_directories(TouchEngine_Windows INTERFACE ${WinSDK})

target_link_libraries(TouchEngine_Windows PRIVATE TDP opengl32.lib d3d11.lib Pathcch.lib)

add_library(IPM SHARED IMPORTED GLOBAL)
add_library(TDP SHARED IMPORTED GLOBAL)
add_library(TPC SHARED IMPORTED GLOBAL)

set_property(TARGET TDP PROPERTY IMPORTED_IMPLIB
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/TouchEngine.lib)
set_property(TARGET TDP PROPERTY IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/TouchEngine.dll)

set_property(TARGET IPM PROPERTY IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libIPM.dll)
set_property(TARGET TPC PROPERTY IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libTPC.dll)

target_include_directories(TDP INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_program(FXC fxc.exe HINTS "C:/Program Files (x86)/Windows Kits/10/bin/x86")

if(NOT FXC)
    message(FATAL_ERROR "fxc.exe not found")
endif()

function(compile_shader SHADER_FILE SHADER_TYPE SHADER_MODEL OUTPUT_FILE TARGET)
    add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${FXC} /T ${SHADER_TYPE}_${SHADER_MODEL} /E main /Fo ${OUTPUT_FILE} ${SHADER_FILE}
            MAIN_DEPENDENCY ${SHADER_FILE}
            COMMENT "Compiling ${SHADER_FILE}"
    )
    target_sources(${TARGET} PRIVATE ${OUTPUT_FILE})
endfunction()

compile_shader("${CMAKE_SOURCE_DIR}/src/TestPixelShader.hlsl" "ps" "5_0" "${CMAKE_BINARY_DIR}/TestPixelShader.cso" TouchEngine_Windows)
compile_shader("${CMAKE_SOURCE_DIR}/src/TestVertexShader.hlsl" "vs" "5_0" "${CMAKE_BINARY_DIR}/TestVertexShader.cso" TouchEngine_Windows)
