#include "Application/Application.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <chrono>

#include "Shader.h"

#include "Model/Model.h"

extern std::string vertexShaderRender;
extern std::string fragmentShaderRender;

void renderObject(Shader& mainShader, glm::mat4& projection, glm::mat4& view, Model& zombieModel, Model& turretModel, Model& tankModel);

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

    // Shader mainShader(vertexShaderRender, fragmentShaderRender);

    // mainShader.use();

    // Model zombieModel("../../../resources/zombie_char_7_4.glb", glm::vec3(0.0f, 0.0f, 0.0f));
    // Model turretModel("../../../resources/turret.glb", glm::vec3(0.0f, 0.0f, 0.0f));
    // Model tankModel("../../../resources/zombie_char_7_4.glb", glm::vec3(0.0f, 0.0f, 0.0f));

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

void renderObject(Shader& mainShader, glm::mat4& projection, glm::mat4& view, Model& zombieModel, Model& turretModel, Model& tankModel) {
    mainShader.use();

    mainShader.setMat4("projection", projection);
    mainShader.setMat4("view", view);
    
    //mainShader.setVec3("camPos", camera.Position);

    // light properties
    mainShader.setVec3("light.ambient", 1.0f, 0.2f, 0.2f);
    mainShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    mainShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    // mainShader.setFloat("material.shininess", 32.0f);

    glm::mat4 modelZombie = glm::mat4(1.0f);
    modelZombie = glm::translate(modelZombie, glm::vec3(0.0f, -2.0f, 10.0f));

    mainShader.setMat4("model", modelZombie);
    zombieModel.Draw(mainShader);

    glm::mat4 modelTurret = glm::mat4(1.0f);
    modelTurret = glm::translate(modelTurret, glm::vec3(-1.5f, -2.0f, 10.0f));

    mainShader.setMat4("model", modelTurret);
    turretModel.Draw(mainShader);

    glm::mat4 modelTank = glm::mat4(1.0f);
    modelTank = glm::translate(modelTank, glm::vec3(-3.5f, -0.5f, 10.0f));

    mainShader.setMat4("model", modelTank);
    tankModel.Draw(mainShader);
}
