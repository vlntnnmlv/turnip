#include "engine.hpp"
#include <bx/math.h>

Turnip::Turnip() : m_window(nullptr), m_running(false) {}
static int w = 0;
static int h = 0;

bool Turnip::Init(const char *title, int width, int height) {
    if (!SDL_Init(0)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    w = width;
    h = height;

    m_window = SDL_CreateWindow(title, width, height,
                                SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!m_window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        return false;
    }
    bgfx::renderFrame();

    SDL_ShowWindow(m_window);

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    SDL_PropertiesID windowProperties = SDL_GetWindowProperties(m_window);
    void *nativeWindowHandle =
        SDL_GetPointerProperty(windowProperties, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    if (!nativeWindowHandle) {
        throw std::runtime_error("Couldn't get native window handle!");
    }
    init.platformData.nwh = nativeWindowHandle;
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;

    init.platformData.ndt = nullptr;
    init.platformData.context = nullptr;
    init.platformData.backBuffer = nullptr;
    init.platformData.backBufferDS = nullptr;

    // On Apple's macOS, you must set the NSHighResolutionCapable Info.plist property to YES,
    // otherwise you will not receive a High-DPI OpenGL canvas.

    if (!bgfx::init(init)) {
        throw std::runtime_error("bgfx::init failed!");
    }

    m_running = true;
    return true;
}

struct PosColorVertex {
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cubeVertices[] = {
    {-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},
    {-1.0f, -1.0f, 1.0f, 0xff00ff00},  {1.0f, -1.0f, 1.0f, 0xff00ffff},
    {-1.0f, 1.0f, -1.0f, 0xffff0000},  {1.0f, 1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
};

static const uint16_t cubeTriList[] = {
    0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6,
    1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
};

static bgfx::ShaderHandle loadShader(const char *FILENAME) {
    const char *shaderPath = "???";

    switch (bgfx::getRendererType()) {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        shaderPath = "shaders/dx11/";
        break;
    case bgfx::RendererType::Gnm:
        shaderPath = "shaders/pssl/";
        break;
    case bgfx::RendererType::Metal:
        shaderPath = "../vendor/bgfx.cmake/cmake/examples/runtime/shaders/metal/";
        break;
    case bgfx::RendererType::OpenGL:
        shaderPath = "shaders/glsl/";
        break;
    case bgfx::RendererType::OpenGLES:
        shaderPath = "shaders/essl/";
        break;
    case bgfx::RendererType::Vulkan:
        shaderPath = "vendor/bgfx.cmake/cmake/examples/runtime/shaders/spirv/";
        break;
    }

    size_t shaderLen = strlen(shaderPath);
    size_t fileLen = strlen(FILENAME);
    char *filePath = (char *)malloc(shaderLen + fileLen);
    memcpy(filePath, shaderPath, shaderLen);
    memcpy(&filePath[shaderLen], FILENAME, fileLen);

    FILE *file = fopen(shaderPath, "rb");
    if (!file) {
        std::cout << shaderPath << "\n";
        throw std::runtime_error("Couldn't load shader file at:");
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}

void Turnip::Run() {
    unsigned int counter = 0;

    bgfx::VertexLayout pcvDecl;
    pcvDecl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    bgfx::VertexBufferHandle vbh =
        bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvDecl);
    bgfx::IndexBufferHandle ibh =
        bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

    bgfx::ShaderHandle vsh = loadShader("vs_cubes.bin");
    bgfx::ShaderHandle fsh = loadShader("fs_cubes.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    while (m_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            bgfx::renderFrame();

            if (event.type == SDL_EVENT_QUIT) {
                m_running = false;
            }
        }

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff);
        bgfx::setViewRect(0, 0, 0, uint16_t(w), uint16_t(h));

        //
        const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
        const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, float(w) / float(h), 0.1f, 100.0f,
                    bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);

        float mtx[16];
        bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
        bgfx::setTransform(mtx);

        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        bgfx::submit(0, program);
        bgfx::frame();
        counter++;
        //

        bgfx::touch(0);
        bgfx::frame();
    }

    bgfx::destroy(ibh);
    bgfx::destroy(vbh);
}

void Turnip::Shutdown() {
    // bgfx::shutdown();
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit();
}