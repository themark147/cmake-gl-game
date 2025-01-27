#include "Application/Application.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLGame::Application application = GLGame::Application::get();
    if (int returnCode = application.Init() != 0) {
        return returnCode;
    }

    // Setup Dear ImGui context
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    // ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // ImGui::StyleColorsLight();

    // ImGui_ImplGlfw_InitForOpenGL(window, true);
    // ImGui_ImplOpenGL3_Init("#version 130");

    // glm::vec3 light(15.0f, 30.0f, 5.0f);

    while (!glfwWindowShouldClose(application.getWindow()))
    {
        application.Run();

        // At the end as "overlay"
        //if (controller.isKeyPressed(KeyDefinition::KEY_T)) {
           // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            //ImGui_ImplOpenGL3_NewFrame();
            //ImGui_ImplGlfw_NewFrame();
            //ImGui::NewFrame();
            
            //ImGui::Begin("Light position");
            // ImGui::SliderFloat("float", &XLight, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            //ImGui::SliderFloat3("floatt", &light.x, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            //ImGui::End();

            // Rendering
            //ImGui::Render();
            //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       // }
        //else {
        //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
       // }
    }

    application.Shutdown();

    return 0;
}
