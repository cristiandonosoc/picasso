#include <cstdio>
#include <GL/gl3w.h>
#include <SDL.h>
#include <SDL_syswm.h>

#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>

#include "models/attrib_pointer.h"
#include "assets/mesh.h"
#include "assets/shaders/shader_registry.h"
#include "assets/materials/material_registry.h"
#include "assets/mesh_registry.h"

#include "assets/texture_registry.h"

#include "ui/ui.h"
#include "utils/file.h"
#include "utils/gl.h"
#include "logging/log.h"

#include "utils/printable_enum.h"
#include "utils/paths.h"

#include "mappers/shader_material_mapper.h"

#ifdef _WIN32
#include "Windows.h"
#endif

#include "utils/scope_trigger.h"

#include "camera.h"

#include "debug/debug.h"


using ::picasso::assets::Mesh;
using ::picasso::assets::TextureRegistry;


using ::picasso::assets::shaders::Shader;
using ::picasso::assets::materials::Material;
using ::picasso::assets::Mesh;

using ::picasso::assets::shaders::ShaderRegistry;
using ::picasso::assets::materials::MaterialRegistry;
using ::picasso::assets::MeshRegistry;

using ::picasso::models::AttributeKind;
using ::picasso::models::AttributePointer;

using ::picasso::utils::paths::GetExecutableDir;

using ::picasso::mappers::ShaderMaterialMapper;

using ::picasso::utils::ScopeTrigger;

using namespace ::picasso::utils;

using ::picasso::Camera;

namespace picasso {
// global camera
Camera GLOBAL_CAMERA;

}   // namespace picasso


#include <external/stb_image.h>

BEGIN_IGNORE_WARNINGS();
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
END_IGNORE_WARNINGS();

namespace {

SDL_Window *SetupSDL() {
  // Setup window
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  SDL_Window *window = SDL_CreateWindow("Picasso", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        1280, 720,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  return window;
}


}   // namespace

int main(int, char **) {
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    LOGERR_FATAL("SDL_Init Error: %s", SDL_GetError());
    return 1;
  }

  // Setup window
  SDL_Window *window = SetupSDL();
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1); // Enable vsync
  gl3wInit();

  // Setup ImGUI binding
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  // Mainly sets up the HDC and SDL Keyboard/Mouse stuf
  ImGui_ImplSdlGL3_Init(window);

  // We maximize the window
  //
#ifdef _WIN32
  // TODO(Cristian): For some reason, we need this for stdout logging in windows
  //                 Move this to the platform layer
  fflush(stderr);

  // Maximize the window
  ShowWindow((HWND)io.ImeWindowHandle, SW_MAXIMIZE);
#endif

  LOG_INFO("OpenGL Vendor: %s", glGetString(GL_VENDOR));
  LOG_INFO("OpenGL Renderer: %s", glGetString(GL_RENDERER));
  LOG_INFO("OpenGL Version: %s", glGetString(GL_VERSION));
  LOG_INFO("OpenGL Shading Language Version: %s",
              glGetString(GL_SHADING_LANGUAGE_VERSION));
  LOG_INFO("OpenGL Extension: %s", glGetString(GL_EXTENSIONS));

  // Load a shader
	std::string simple_vs = ReadWholeFile(GetExecutableDir() +
                                        "/shaders/simple.vert");
	std::string simple_fs = ReadWholeFile(GetExecutableDir() +
                                        "/shaders/simple.frag");
  std::string color_fs = ReadWholeFile(GetExecutableDir() +
                                       "/shaders/color.frag");

  auto simple_shader_key = ShaderRegistry::Create("test_shader",
      simple_vs, simple_fs).ConsumeOrDie();
  auto color_shader_key= ShaderRegistry::Create("color_shader",
      simple_vs, color_fs).ConsumeOrDie();

  // We load a texture
  auto texture_key = TextureRegistry::Create("test_texture",
      GetExecutableDir() + "textures/container.jpg").ConsumeOrDie();
  auto texture = TextureRegistry::Get(texture_key).lock();
  LOG_DEBUG("TEXTURE READ. POINTER: %p, WIDTH: %d, HEIGHT: %d",  texture->GetData(), texture->GetWidth(), texture->GetHeight());

  auto res = TextureRegistry::Create("happy",
      GetExecutableDir() + "textures/awesomeface.png");
  if (!res.Ok()) {
    LOGERR_FATAL("Could not load texture: %s", res.GetErrorMsg().c_str());
    return 1;
  }
  auto tex2 = TextureRegistry::Get(res.ConsumeOrDie()).lock();

  LOG_SEPARATOR;
  LOG_INFO("Creating material");
  std::string mat_name = "mat0";
  auto material_res = MaterialRegistry::Create(mat_name);
  if (!material_res.Ok()) {
    LOGERR_FATAL("Could not create material \"%s\"", mat_name.c_str());
    return 1;
  }

  Material* material = material_res.ConsumeOrDie();

  auto mapping_res = ShaderMaterialMapper::AddMapping(simple_shader_key, mat_name);
  LOG_NON_OK_STATUS(mapping_res);

  material->SetValue<uint32_t>("tex0", texture->GetId());
  material->SetValue<uint32_t>("tex1", tex2->GetId());


  // We create some sample points
  // Actual points to be renderer
  // There are 6 floats from each element. So stride is 24
  float vertices[] = {
     // positions         // colors
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f,   0.5f, 0.7f, 0.2f,  0.0f, 1.0f   // top left
  };
  // Indices that OpenGL will use to generate the primitives
  // This enables us to send much less data
  unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };

  auto mesh_res = MeshRegistry::Create("mesh0");
  assert(mesh_res.Ok());
  Mesh *mesh = mesh_res.ConsumeOrDie();

  mesh->SetVertexBuffer(sizeof(vertices), vertices);
  mesh->SetIndexBuffer(sizeof(indices), indices);

  int stride = 8 * sizeof(float);
  mesh->AddAttributePointer({AttributeKind::VERTEX, 3, GL_FLOAT, false, stride, 0});
  mesh->AddAttributePointer({AttributeKind::COLOR, 3, GL_FLOAT, false, stride, 3 * sizeof(float)});
  mesh->AddAttributePointer({AttributeKind::UV, 2, GL_FLOAT, false, stride, 6 * sizeof(float)});

  mesh->SetupBuffers();

  // Setup style
  ImGui::StyleColorsDark();

  picasso::ui::UiData ui_data;
  ui_data.clear_color = { 0.137f, 0.152f, 0.637f, 1.00f };


  picasso::GLOBAL_CAMERA.transform.SetTranslation({0, 0, -3});
  picasso::GLOBAL_CAMERA.ReloadViewMatrix();


  // Main loop
  bool done = false;
  while (!done)
  {
      // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
      // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
      // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
      // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
          ImGui_ImplSdlGL3_ProcessEvent(&event);
          if (event.type == SDL_QUIT)
              done = true;
      }

			if (io.KeysDown[io.KeyMap[ImGuiKey_Escape]]) {
				done = true;
			}

      ImGui_ImplSdlGL3_NewFrame(window);


      /* picasso::ImGuiExample(clear_color, show_demo_window, show_another_window); */
      picasso::ui::RunUi(&ui_data);
      /* ImGui::ShowMetricsWindow(nullptr); */
      static bool show_demo = true;
      ImGui::ShowDemoWindow(&show_demo);


      // Rendering
      // Clear the window
      glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
      glClearColor(ui_data.clear_color.x,
                   ui_data.clear_color.y,
                   ui_data.clear_color.z,
                   ui_data.clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);

      ::picasso::debug::Debug::FrameInit();
      ::picasso::debug::Debug::AddLine({0, 0, 0}, {1, 0, 0}, {1, 0, 0});
      ::picasso::debug::Debug::AddLine({0, 0, 0}, {0, 1, 0}, {0, 1, 0});
      ::picasso::debug::Debug::AddLine({0, 0, 0}, {0, 0, 1}, {0, 0, 1});
      ::picasso::debug::Debug::FrameRender();

      /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
      /* mesh->Render(::picasso::GLOBAL_CAMERA, *material); */

      ImGui::Render();
      ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
      SDL_GL_SwapWindow(window);
  }

  // Cleanup
  ImGui_ImplSdlGL3_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
