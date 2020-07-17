#include "../include/OpenGLPipelineService.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include "../../ThirdParty/stb/stb_image.h"

void OpenGLPipelineService::initVoxelData(android_app* app) {
    Json::Value root;
    auto path = "Resources/VoxelModels/Test.json";
    auto json = FileLoadingService::readFile(app, path);

    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    builder["collectComments"] = true;
    Json::String errs;

    if (!reader->parse(json.data(), json.data() + json.size(), &root, &errs)) {
        throw std::runtime_error(errs);
    }

    Json::Value array = root["vectorValues"];

    //BUG: if you uncomment any of these, you'll see it fall over
    for (size_t i = 0; i < array.size(); i++) {
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat(), array[static_cast<int>(i)][1].asFloat(), array[static_cast<int>(i)][2].asFloat())));

        //BUG: if this line is uncommented, it seems to align correctly? I'm guessing I screwed something up elsewhere.
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat() + 20, array[static_cast<int>(i)][1].asFloat(), array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat() - 20, array[static_cast<int>(i)][1].asFloat(), array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat(), array[static_cast<int>(i)][1].asFloat() - 20, array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat(), array[static_cast<int>(i)][1].asFloat() + 20, array[static_cast<int>(i)][2].asFloat())));

        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat() + 40, array[static_cast<int>(i)][1].asFloat(), array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat() - 40, array[static_cast<int>(i)][1].asFloat(), array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat(), array[static_cast<int>(i)][1].asFloat() - 40, array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat(), array[static_cast<int>(i)][1].asFloat() + 40, array[static_cast<int>(i)][2].asFloat())));

        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat() + 60, array[static_cast<int>(i)][1].asFloat(), array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat() - 60, array[static_cast<int>(i)][1].asFloat(), array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat(), array[static_cast<int>(i)][1].asFloat() - 60, array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat(), array[static_cast<int>(i)][1].asFloat() + 60, array[static_cast<int>(i)][2].asFloat())));

        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat() + 80, array[static_cast<int>(i)][1].asFloat(), array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat() - 80, array[static_cast<int>(i)][1].asFloat(), array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat(), array[static_cast<int>(i)][1].asFloat() - 80, array[static_cast<int>(i)][2].asFloat())));
        _transformData.emplace_back(glm::translate(glm::identity<glm::mat4>(), glm::vec3(array[static_cast<int>(i)][0].asFloat(), array[static_cast<int>(i)][1].asFloat() + 80, array[static_cast<int>(i)][2].asFloat())));
    }

    _voxelInstanceCount = _transformData.size();

    std::cout << "Detected " << _voxelInstanceCount << " voxels. This will result in " << _voxelInstanceCount * _vertices.size() << " unique vertices, " << _voxelInstanceCount * _indices.size() << " indices, and " << _voxelInstanceCount * (_indices.size() / 3) << " faces in the world." << std::endl;
}

#if defined(_WIN64) || defined(_WIN32)
void OpenGLPipelineService::checkForOptimus(const char* library) {
    _optimus = LoadLibrary(reinterpret_cast<LPCSTR>(library));
}
#endif

void OpenGLPipelineService::bindCameraUboForProgram(GLuint shaderProgramId) {
    GLuint uboIndex = glGetUniformBlockIndex(shaderProgramId, "CameraBufferObject");
    glUniformBlockBinding(shaderProgramId, uboIndex, 0);
}

void OpenGLPipelineService::bindTransformDataUboForProgram(GLuint shaderProgramId) {
    GLuint uboIndex = glGetUniformBlockIndex(shaderProgramId, "TransformBufferObject");
    glUniformBlockBinding(shaderProgramId, uboIndex, 1);
}

void OpenGLPipelineService::bindLightPosDataUboForProgram(GLuint shaderProgramId) {
    GLuint uboIndex = glGetUniformBlockIndex(shaderProgramId, "lightPosData");
    glUniformBlockBinding(shaderProgramId, uboIndex, 2);
}

GLuint OpenGLPipelineService::getCameraUboHandle() {
    if (!_cameraUboGenerated) {
        glGenBuffers(1, &_cameraUboHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, _cameraUboHandle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, _cameraUboHandle, 0, sizeof(glm::mat4) * 2);
        _cameraUboGenerated = true;
    }

    return _cameraUboHandle;
}

GLuint OpenGLPipelineService::getLightPosDataUboHandle() {
    if (!_lightPosUboGenerated) {
        glGenBuffers(1, &_lightPosUboHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, _lightPosUboHandle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, _lightPosUboHandle, 0, sizeof(glm::vec3));
        _lightPosUboGenerated = true;
    }

    return _lightPosUboHandle;
}

GLuint OpenGLPipelineService::getTransformBufferDataUboHandle() {
    if (!_transformDataUboGenerated) {
        glGenBuffers(1, &_transformDataUboHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, _transformDataUboHandle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 1024, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, _transformDataUboHandle, 0, sizeof(glm::mat4) * 1024);
        _transformDataUboGenerated = true;
    }

    return _transformDataUboHandle;
}

ShaderProgram OpenGLPipelineService::loadShaders(const std::string& vertexFileName, const std::string& fragmentFileName) {

    // Create the shaders
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    std::string shadersDirPath = "Resources/Shaders/";

    // Read the Vertex Shader code from the file
    std::string vertexShaderCode;

    std::ifstream VertexShaderStream(shadersDirPath + vertexFileName, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        vertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else {
        throw std::runtime_error("Unable to compile vertex shader! Aborting...");
    }

    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(shadersDirPath + fragmentFileName, std::ios::in);
    if (fragmentShaderStream.is_open()) {
        std::stringstream stringStream;
        stringStream << fragmentShaderStream.rdbuf();
        fragmentShaderCode = stringStream.str();
        fragmentShaderStream.close();
    }
    else {
        throw std::runtime_error("Unable to compile fragment shader! Aborting...");
    }

    GLint Result = GL_FALSE;
    int infoLogLength;

    // Compile Vertex Shader
    char const* vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderId, 1, &vertexSourcePointer, nullptr);
    glCompileShader(vertexShaderId);

    // Check Vertex Shader
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> vertexShaderErrorMessage(static_cast<size_t>(infoLogLength) + 1);
        glGetShaderInfoLog(vertexShaderId, infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
        std::cout << std::string(&vertexShaderErrorMessage[0]) << std::endl;
        throw std::runtime_error("Unable to continue! Please fix the compile time error in the specified shader.");
    }

    // Compile Fragment Shader
    const char* FragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderId, 1, &FragmentSourcePointer, nullptr);
    glCompileShader(fragmentShaderId);

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> fragmentShaderErrorMessage(static_cast<size_t>(infoLogLength) + 1);
        glGetShaderInfoLog(fragmentShaderId, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
        std::cout << std::string(&fragmentShaderErrorMessage[0]) << std::endl;
        throw std::runtime_error("Unable to continue! Please fix the compile time error in the specified shader.");
    }

    // Link the program
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);

    // Check the program
    glGetProgramiv(programId, GL_LINK_STATUS, &Result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(static_cast<size_t>(infoLogLength) + 1);
        glGetProgramInfoLog(programId, infoLogLength, nullptr, &ProgramErrorMessage[0]);
        std::cout << std::string(&ProgramErrorMessage[0]) << std::endl;
        throw std::runtime_error("Unable to continue! Please fix the specified error in the shader program.");
    }

    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    ShaderProgram returnProg;
    returnProg.shaderProgramId = programId;
    returnProg.finalViewMatrixBufferUboId = getCameraUboHandle();
    returnProg.lightPosDataUboId = getLightPosDataUboHandle();
    //returnProg.transformBufferDataUboId = getTransformBufferDataUboHandle();
    bindCameraUboForProgram(programId);
    bindTransformDataUboForProgram(programId);
    bindLightPosDataUboForProgram(programId);

    return returnProg;
}

void OpenGLPipelineService::loadTextures() {
    int texWidth, texHeight, texChannels;


    // Read the file:

    AAsset* file = AAssetManager_open(_app->activity->assetManager,
                                      "Resources/Textures/texture.png", AASSET_MODE_BUFFER);
    size_t fileLength = AAsset_getLength(file);
    stbi_uc* fileContent = new unsigned char[fileLength];

    AAsset_read(file, fileContent, fileLength);
    AAsset_close(file);

    uint32_t imgWidth, imgHeight, n;
    unsigned char* pixels = stbi_load_from_memory(
            fileContent, fileLength, reinterpret_cast<int*>(&texWidth),
            reinterpret_cast<int*>(&texHeight), reinterpret_cast<int*>(&n), 4);
    assert(n == 4);

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }
    
    glGenTextures(1, &_textureHandle);
    glBindTexture(GL_TEXTURE_2D, _textureHandle);

    int mode = GL_RGBA;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, texWidth, texHeight, 0, mode, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(pixels));
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);
}

void OpenGLPipelineService::setUpCubeInstancing() {
    glGenVertexArrays(1, &_cubeVAO);
    glGenBuffers(1, &_vboDataHandle);
    glGenBuffers(1, &_indicesHandle);
    glGenBuffers(1, &_instancesHandle);
    bufferVertexData();
}

void OpenGLPipelineService::bufferVertexData() {
    glBindBuffer(GL_ARRAY_BUFFER, _vboDataHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * _indices.size(), _indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, _instancesHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * _voxelInstanceCount, _transformData.data(), GL_STATIC_DRAW);
}

void OpenGLPipelineService::initOpenGLES(android_app* app) {
    _app = app;
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLDisplay display;
    EGLConfig config;
    EGLint numConfigs;
    EGLint format;
    EGLSurface surface;
    EGLContext context;
    EGLint width;
    EGLint height;
    GLfloat ratio;

    //LOG_INFO("Initializing context");

    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
        throw std::runtime_error("eglGetDisplay() returned error: " + eglGetError());
    }
    if (!eglInitialize(display, 0, 0)) {
        throw std::runtime_error("eglInitialize() returned error: " + eglGetError());
    }

    if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
        destroyEGLContext();
        throw std::runtime_error("eglChooseConfig() returned error: " + eglGetError());

    }

    if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
        destroyEGLContext();
        throw std::runtime_error("eglGetConfigAttrib() returned error: " + eglGetError());
    }

    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

    if (!(surface = eglCreateWindowSurface(display, config, app->window, 0))) {
        destroyEGLContext();
        throw std::runtime_error("eglCreateWindowSurface() returned error: " + eglGetError());

    }

    if (!(context = eglCreateContext(display, config, 0, 0))) {
        destroyEGLContext();
        throw std::runtime_error("eglCreateContext() returned error: " + eglGetError());

    }

    if (!eglMakeCurrent(display, surface, surface, context)) {
        destroyEGLContext();
        throw std::runtime_error("eglMakeCurrent() returned error: " + eglGetError());

    }

    if (!eglQuerySurface(display, surface, EGL_WIDTH, &width) ||
        !eglQuerySurface(display, surface, EGL_HEIGHT, &height)) {
        destroyEGLContext();
        throw std::runtime_error("eglQuerySurface() returned error: " + eglGetError());

    }

    _display = display;
    _surface = surface;
    _context = context;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadTextures();
    setUpCubeInstancing();
    _phongProgram = loadShaders("Shader.vert", "Shader.frag");
    _initialised = true;
}

void OpenGLPipelineService::destroyEGLContext() {
    //LOG_INFO("Destroying context");

    eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(_display, _context);
    eglDestroySurface(_display, _surface);
    eglTerminate(_display);

    _display = EGL_NO_DISPLAY;
    _surface = EGL_NO_SURFACE;
    _context = EGL_NO_CONTEXT;

    return;
}

void OpenGLPipelineService::handleCameraBufferObject() {
    static auto startTime = std::chrono::high_resolution_clock::now();
    static auto previousTime = std::chrono::high_resolution_clock::now();
    static std::chrono::duration<float, std::chrono::seconds::period> delta;
    auto currentTime = std::chrono::high_resolution_clock::now();
    delta = currentTime - previousTime;
    float time = delta.count();


    glBindBuffer(GL_UNIFORM_BUFFER, _phongProgram.finalViewMatrixBufferUboId);
    //should be 12, 12, 20
    _ubo.view = glm::rotate(_ubo.view, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //glm::lookAt(glm::vec3(40.0f, 40.0f, 56.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    _ubo.proj = glm::perspective(glm::radians(90.0f), (float)_width / (float)_height, 0.1f, 65565.0f);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraBufferObject), &_ubo, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    previousTime = std::chrono::high_resolution_clock::now();
}

void OpenGLPipelineService::handleLightPosData() {
    glBindBuffer(GL_UNIFORM_BUFFER, _phongProgram.lightPosDataUboId);
    glm::vec3 lightPos(4.0f, 4.0f, 4.0f);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), &lightPos, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLPipelineService::handleTransformDataBufferObject() {
    //glBindBuffer(GL_UNIFORM_BUFFER, _phongProgram.transformBufferDataUboId);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * _transformData.size(), _transformData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLPipelineService::handleTexture() {
    glBindTexture(GL_TEXTURE_2D, _textureHandle);
}

void OpenGLPipelineService::handleVAODraw() {
    glBindVertexArray(_cubeVAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vboDataHandle);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, pos)
        );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _vboDataHandle);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color)
        );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, _vboDataHandle);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, texCoord)
        );

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, _vboDataHandle);
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, normal)
        );
    glBindBuffer(GL_ARRAY_BUFFER, _instancesHandle);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
        4,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::mat4),
        (void*)0
        );
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(
        5,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::mat4),
        (void*)sizeof(glm::vec4)
        );

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(
        6,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::mat4),
        (void*)(sizeof(glm::vec4) * 2)
        );
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(
        7,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::mat4),
        (void*)(sizeof(glm::vec4) * 3)
        );

    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesHandle);
    glDrawElementsInstanced(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_SHORT, 0, _transformData.size()); //infinite recursion followed by segfault occurs here. Stacktrace is useless.
    
    glDisableVertexAttribArray(7);
    glDisableVertexAttribArray(6);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void OpenGLPipelineService::drawFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    glUseProgram(_phongProgram.shaderProgramId);

    handleCameraBufferObject();
    //handleTransformDataBufferObject();
    handleLightPosData();
    bufferVertexData();
    handleTexture();
    handleVAODraw();

    if (!eglSwapBuffers(_display, _surface)) {
        cleanup();
        throw std::runtime_error("eglSwapBuffers() returned error: " + eglGetError());
    }
}

void OpenGLPipelineService::mainLoop() {
/*    while (!glfwWindowShouldClose(_window.get())) {
        glfwPollEvents();
        drawFrame();
    }*/
}

void OpenGLPipelineService::cleanup() {
    glDeleteVertexArrays(1, &_cubeVAO);
    destroyEGLContext();
    _initialised = false;
}

OpenGLPipelineService::OpenGLPipelineService() noexcept {
}

void OpenGLPipelineService::launch() {
/*    initOpenGLES(_app);
    mainLoop();
    cleanup();*/
}
