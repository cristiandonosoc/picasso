#include "shader.h"
#include "ui.h"
#include "utils/file.h"
#include "utils/gl.h"
#include "utils/log.h"

#include <cstdio>
#include <GL/gl3w.h>
#include <SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>

void SetupSDL() {
  // Setup window
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
}

using namespace picasso::utils;

int main(int, char **) {


// TODO(Cristian): For some reason, we need this for stdout logging in windows
//                 Move this to the platform layer
#ifdef WIN32

  fflush(stderr);
#endif


  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    logerr::Error("SDL_Init Error: %s", SDL_GetError());
    return 1;
  }

  // Setup window
  SetupSDL();
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  SDL_Window *window = SDL_CreateWindow("Picasso", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        1280, 720,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1); // Enable vsync

  gl3wInit();


  // Setup ImGUI binding
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // Mainly sets up the HDC and SDL Keyboard/Mouse stuf
  ImGui_ImplSdlGL3_Init(window);

  logout::Info("GL_VERSION: %sn", (char*)glGetString(GL_VERSION));

  // Load a shader
	std::string vs = picasso::utils::ReadWholeFile("../shaders/simple.vert");
	std::string fs = picasso::utils::ReadWholeFile("../shaders/simple.frag");
  auto shader_res = picasso::ShaderProgram::Create(vs, fs);
  picasso::ShaderProgram shader;
  if (shader_res.Valid()) {
    shader = shader_res.ConsumeOrDie();
    logout::Info("Successful shader");
  } else {
    logout::Info("Error getting shader: %s\n", shader_res.ErrorMsg().c_str());
  }

  logerr::Info("Printing shader attributes:");
  const auto& attribs = shader.GetAttributes();
  for (const auto& attrib : attribs) {
    std::string type_name;
    auto type_name_res = picasso::utils::GL_TYPES_TO_STRING.Get(attrib.GetType());
    if (type_name_res.Valid()) {
      type_name = type_name_res.ConsumeOrDie();
    }
    logout::IndentInfo(2, "NAME: %s, TYPE: %s, SIZE: %zu, LOCATION: %d",
                       attrib.GetName().c_str(),
                       type_name.c_str(),
                       attrib.GetSize(),
                       attrib.GetLocation());
  }

  logerr::Info("Printing shader uniforms. Length: %zu", shader.GetUniforms().size());
  for (auto&& it = shader.UniformBegin();
       it != shader.UniformEnd();
       it++) {
    const auto& uniform = *it;
    logout::IndentInfo(2, "NAME: %s, TYPE: %s, SIZE: %zu, LOCATION: %d",
                       uniform.GetName().c_str(),
                       uniform.GetTypeName().c_str(),
                       uniform.GetSize(),
                       uniform.GetLocation());
  }

  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  // Establishing data to the GPU

  // 1. We create a buffers. In OpenGL, you create buffers that get binded to
  //    certain types of buffers. The in received is the "name" of the buffer
  //    that we are generating. They are unbinded (have no type) at first.
  unsigned int vbo;
  glGenBuffers(1, &vbo);

  // 2. Bind it to a particular type. In this case to GL_ARRAY_BUFFER.
  //    This will make vbo into a VERTEX BUFFER OBJECT (hence the name).
  //    When a buffer is binded some things happen:
  //    - Any previous binded data of another type is discarded
  //    - If the buffer wasn't binded to this type, a new array of that
  //      type is created
  //    - This buffer is the active one for this type.
  //      This means that any future calls that affect GL_ARRAY_BUFFER will
  //      now affect the one binded to the vbo, until another binding occurs.
  glBindBuffer(GL_ARRAY_BUFFER, vbo);   // Make

  // 3. Send data to the active buffer (vbo in this case).
  //    This call will scale the buffer to the size specified here.
  //    Any previous data associated with the buffer will be discarded.
  glBufferData(GL_ARRAY_BUFFER,       // Buffer being sent data
               sizeof(vertices),      // New size of the buffer,
               vertices,              // Pointer to the array to send
               GL_STATIC_DRAW);       // Type of memory where to store it
                                      // Depending on the usage, some are
                                      // better than others

  // OpenGL has buffer with vertices in it, but has no idea how to interpret them
  // (which ones are colors, which ones positions, uv, etc.).
  // For that we need to tell which attribute holds which information where.
  // That configuration can (and should) be stored in an object that can then
  // be binded to the current buffer so that OpenGL knows how to interpret it.
  // These objects are called VERTEX ATTRIBUTE OBJECTS

  // 4. Create the VAO
  unsigned int vao;
  glGenVertexArrays(1, &vao);

  // 5. Bind the VAO as active. Any vertex attrib calls will now be stored in
  //    this VAO. A VAO could be thought as a "replay" of the attrib/buffer
  //    binding made.
  glBindVertexArray(vao);

  // 6. Associate the buffer with this VAO. That way, when we bing the VAO
  //    we are also
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // 7. Set the vertex attribute points. This calls describe exactly how
  //    OpenGL should interpret the data in the buffer.
  //    The buffer can hold many interleaved data, that is accessed in the
  //    shader through the attributes.
  //    This means that we need to set an attrib pointer foreach attribute
  // Position is number 2
  // TODO(Cristian): Have a way to query for attributes by name
  int pos_attrib_location = 0;
  glVertexAttribPointer(pos_attrib_location,      // Attribute location
                        3,                        // size (?)
                        GL_FLOAT,                 // Type of the attribute
                        GL_FALSE,                 // Whether values should be normalized
                        0,                        // stride (space between instances of the attrib)
                        0);                       // Offset to the first component

  // 8. We need to enable the attrib.
  //    If disabled, apparently they are not accesible from the shader (?)
  glEnableVertexAttribArray(pos_attrib_location);


  // Setup style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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



      picasso::ImGuiExample(clear_color, show_demo_window, show_another_window);

      // Rendering
      // Clear the window
      glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);

      // Use our program
      glUseProgram(shader.GetProgramHandle());
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 3);

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
