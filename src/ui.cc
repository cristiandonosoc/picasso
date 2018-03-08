#include "ui.h"
#include "shaders/shader_registry.h"

namespace picasso {

void ImGuiExample(const ImVec4& clear_color, bool show_demo_window,
                  bool show_another_window) {
  // 1. Show a simple window.
  // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
  {
      static float f = 0.0f;
      static int counter = 0;
      ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
      ImGui::Checkbox("Another Window", &show_another_window);

      if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
          counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }

  // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
  if (show_another_window)
  {
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
          show_another_window = false;
      ImGui::End();
  }

  // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
  if (show_demo_window)
  {
      ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
      ImGui::ShowDemoWindow(&show_demo_window);
  }
}

void RunUi(UiData *) {
  ImGuiIO& io = ImGui::GetIO();

  ImGui::SetNextWindowSize({500, io.DisplaySize.y}, ImGuiCond_Once);
  ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Once);
  /* ImGui::SetNextWindowSize({0, io.DisplaySize.y}, ImGuiCond_Always); */
  ImGui::Begin("Shaders", nullptr);

  ImGui::BeginChild("Left Pane", {150, 0}, true);
  auto&& programs = shaders::ShaderRegistry::GetShaders();
  static int selected_shader = -1;
  int i = 0;
  for (auto&& it = programs.begin();
       it != programs.end();
       it++, i++) {
    auto&& shader = *it;
    char label[128];
    sprintf(label, "Shader: %s", shader->GetName().c_str());
    if (ImGui::Selectable(label, selected_shader == i)) {
      selected_shader = i;
    }
  }

  ImGui::Text("WINDOW Y: %f", ImGui::GetWindowHeight());
  ImGui::Text("CONTENT Y: %f", ImGui::GetContentRegionMax().y);



  ImGui::EndChild();
  ImGui::SameLine();

  static char text[1024*16] =
      "/*\n"
      " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
      " the hexadecimal encoding of one offending instruction,\n"
      " more formally, the invalid operand with locked CMPXCHG8B\n"
      " instruction bug, is a design flaw in the majority of\n"
      " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
      " processors (all in the P5 microarchitecture).\n"
      "*/\n\n"
      "label:\n"
      "\tlock cmpxchg8b eax\n";


  float text_height = (ImGui::GetContentRegionAvail().y - 2 * ImGui::GetFontSize()) / 2;
  /* ImGui::BeginGroup(); */
    /* ImGui::BeginChild("Vertex Shader", {-1, -window_height / 2}); */
    ImGui::BeginChild("Vertex Shader", {-1, -1});
      ImGui::Text("Vertex Shader");
      ImGui::Separator();
      ImGui::InputTextMultiline("##vs", text, sizeof(text), {-1, text_height}, 
                                ImGuiInputTextFlags_AllowTabInput);
    /* ImGui::EndChild(); */
    /* ImGui::BeginChild("Fragment Shader", {-1, -window_height / 2}); */
      ImGui::Text("Fragment Shader");
      ImGui::Separator();
      ImGui::InputTextMultiline("##fs", text, sizeof(text), {-1, -1}, 
                                ImGuiInputTextFlags_AllowTabInput);
    ImGui::EndChild();
  /* ImGui::EndGroup(); */

  ImGui::End();

}

}   // namespace picasso
