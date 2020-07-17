# CMakeList.txt : CMake project for OpenGLCubePipeline, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10.2)
set(CMAKE_CXX_STANDARD 17)

include_directories(../ThirdParty)
add_library(glm STATIC ../ThirdParty/glm)
set_target_properties(glm PROPERTIES LINKER_LANGUAGE CXX)
add_library(app-glue STATIC ${APP_GLUE_DIR}/android_native_app_glue.c)
#add_library(jsoncpp STATIC ../ThirdParty/jsoncpp)
#set_target_properties(jsoncpp PROPERTIES LINKER_LANGUAGE CXX)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )


# Add source to this project's executable.
add_library (MiniRenderOpenGLPipeline "OpenGLCubePipeline.cpp" "OpenGLCubePipeline.h" "src/OpenGLPipelineService.cpp" "include/OpenGLPipelineService.h" "include/FileLoadingService.h" "include/Vertex.h" "include/CameraBufferObject.h" "include/ShaderProgram.h" "ThirdParty/jsoncpp/jsoncpp.cpp" "ThirdParty/jsoncpp/json/json.h" "ThirdParty/jsoncpp/json/json-forwards.h")
target_include_directories(MiniRenderOpenGLPipeline PRIVATE ${STB_INCLUDES})

target_link_libraries(MiniRenderOpenGLPipeline  app-glue android libEGL libGLESv3 glm ${log-lib})
