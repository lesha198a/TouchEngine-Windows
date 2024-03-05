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
        src/d3dx12.h
        src/DocumentWindow.cpp
        src/DocumentWindow.h
        src/Drawable.cpp
        src/Drawable.h
        src/DX11Device.cpp
        src/DX11Device.h
        src/DX11Image.cpp
        src/DX11Image.h
        src/DX11Renderer.cpp
        src/DX11Renderer.h
        src/DX11Texture.cpp
        src/DX11Texture.h
        src/DX11VertexShader.cpp
        src/DX11VertexShader.h
        src/DX12Image.cpp
        src/DX12Image.h
        src/DX12Renderer.cpp
        src/DX12Renderer.h
        src/dx12shaders.hlsl
        src/DX12Texture.cpp
        src/DX12Texture.h
        src/DX12Utility.h
        src/DXGIUtility.cpp
        src/DXGIUtility.h
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
        src/Strings.cpp
        src/Strings.h
        src/targetver.h

        src/TouchEngineExample.rc
)

target_include_directories(TouchEngine_Windows INTERFACE ${WinSDK})

target_link_libraries(TouchEngine_Windows PRIVATE
        TDP
        opengl32.lib
        d3d11.lib
        d3d12.lib
        Pathcch.lib
        dxgi.lib
        d3dcompiler.lib
        dxguid.lib
)

add_library(TDP SHARED IMPORTED GLOBAL)

set_property(TARGET TDP PROPERTY IMPORTED_IMPLIB
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/TouchEngine.lib
)

set_property(TARGET TDP PROPERTY IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/bin/TouchEngine.dll
)

target_include_directories(TDP INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

get_target_property(TDP_DLL_PATH TDP IMPORTED_LOCATION)

add_custom_command(TARGET TouchEngine_Windows POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TDP_DLL_PATH}
        $<TARGET_FILE_DIR:TouchEngine_Windows>)

add_custom_command(TARGET TouchEngine_Windows POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/bin/dx12shaders.hlsl
        $<TARGET_FILE_DIR:TouchEngine_Windows>)

find_program(FXC fxc.exe HINTS "C:/Program Files (x86)/Windows Kits/10/bin/x86")

if (NOT FXC)
    message(FATAL_ERROR "fxc.exe not found")
endif ()

function(compile_shader SHADER_FILE SHADER_TYPE SHADER_MODEL OUTPUT_FILE TARGET)
    add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${FXC} /T ${SHADER_TYPE}_${SHADER_MODEL} /E main /Fo ${OUTPUT_FILE} ${SHADER_FILE}
            MAIN_DEPENDENCY ${SHADER_FILE}
            COMMENT "Compiling ${SHADER_FILE}"
    )
    target_sources(${TARGET} PRIVATE ${OUTPUT_FILE})
endfunction()

compile_shader("${CMAKE_SOURCE_DIR}/src/TestPixelShader.hlsl" "ps" "5_0"
        "${CMAKE_BINARY_DIR}/TestPixelShader.cso" TouchEngine_Windows)
compile_shader("${CMAKE_SOURCE_DIR}/src/TestVertexShader.hlsl" "vs" "5_0"
        "${CMAKE_BINARY_DIR}/TestVertexShader.cso" TouchEngine_Windows)
