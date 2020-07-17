// Copyright 2016 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <android/log.h>
#include <android_native_app_glue.h>
#include "OpenGLCubePipeline/include/OpenGLPipelineService.h"
#include <glfm.h>
#include <memory>

std::unique_ptr<OpenGLPipelineService> _service = nullptr;

static void onSurfaceCreated(GLFMDisplay *display, const int width, const int height) {
    _service->initOpenGLES(width, height);
}

static void onSurfaceDestroyed(GLFMDisplay *display) {
    // When the surface is destroyed, all existing GL resources are no longer valid
    _service->cleanup();
}

static void onFrame(GLFMDisplay *display, const double frameTime) {
    _service->drawFrame();
}



void glfmMain(GLFMDisplay *display) {
    _service = std::make_unique<OpenGLPipelineService>();
    _service->initVoxelData(glfmAndroidGetActivity()->assetManager);
    glfmSetDisplayConfig(display,
                         GLFMRenderingAPIOpenGLES3,
                         GLFMColorFormatRGBA8888,
                         GLFMDepthFormatNone,
                         GLFMStencilFormatNone,
                         GLFMMultisampleNone);
    glfmSetSurfaceCreatedFunc(display, onSurfaceCreated);
    glfmSetSurfaceResizedFunc(display, onSurfaceCreated);
    glfmSetSurfaceDestroyedFunc(display, onSurfaceDestroyed);
    glfmSetMainLoopFunc(display, onFrame);
}

