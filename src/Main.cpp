#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <reactphysics3d/reactphysics3d.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <vector>
#include <chrono>

#include "Shader.h"

#include "Scene/Scene.h"
#include "Player/Player.h"

#include "Model/Mesh.h"
#include "Model/Model.h"

#include "Application/Application.h"

#include "Input/KeyController.h"

#include "Debug/VertexArrayObject.h"
#include "Debug/VertexBufferObject.h"

using namespace KeyInput;

extern std::string vertexShaderDebug;
extern std::string fragmentShaderDebug;

extern std::string vertexShaderRender;
extern std::string fragmentShaderRender;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void createBox(PhysicsCommon& common, PhysicsWorld* world);
void initDebug();
void renderObject(Shader& mainShader, glm::mat4& projection, glm::mat4& view, Model& zombieModel, Model& turretModel, Model& tankModel);
void drawDebug(DebugRenderer& debugRenderer, uint vertexPositionLoc, uint vertexColorLoc);

int widthScreen = 1920;
int heightScreen = 1080;

float lastX = widthScreen / 2.0f;
float lastY = heightScreen / 2.0f;
bool firstMouse = true;

using chrono_clock = std::chrono::high_resolution_clock;

std::chrono::time_point<chrono_clock> mStartTime;
std::chrono::time_point<std::chrono::high_resolution_clock> mLastUpdateTime;

/// Used to fix the time step and avoid strange time effects
std::chrono::duration<double> mAccumulator;
std::chrono::duration<double> deltaTime;

/// Vertex Buffer Object for the debug info lines vertices data
openglframework::VertexBufferObject mDebugVBOLinesVertices(GL_ARRAY_BUFFER);

/// Vertex Array Object for the lines vertex data
openglframework::VertexArrayObject mDebugLinesVAO;

/// Vertex Buffer Object for the debug info trinangles vertices data
openglframework::VertexBufferObject mDebugVBOTrianglesVertices(GL_ARRAY_BUFFER);

/// Vertex Array Object for the triangles vertex data
openglframework::VertexArrayObject mDebugTrianglesVAO;


// Application - init openGL & other stuff

// Scene - predefined objects or creating new one
//  - render method - simply interate over each obj.

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
    GLFWwindow* window = application.getWindow();

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader debugShader(vertexShaderDebug, fragmentShaderDebug);

    GLGame::Scene scene = GLGame::Scene(debugShader);
    GLGame::Player player = scene.getPlayer();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    stbi_set_flip_vertically_on_load(true); // Because of textures
    glEnable(GL_DEPTH_TEST);

    initDebug();

    glfwSwapInterval(0); // vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Shader mainShader(vertexShaderRender, fragmentShaderRender);

    mainShader.use();
   
    PhysicsCommon physicsCommon;

    // Create a physics world
    PhysicsWorld* world = scene.getWorld();

    DebugRenderer& debugRenderer = world->getDebugRenderer();

    // Select the contact points and contact normals to be displayed
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);

    // TODO part of Physics.cpp -> tick() -> step()
    // step
    std::chrono::duration<double> timeStep = std::chrono::duration<double>(1.0f / 100.0f);

    mStartTime = std::chrono::high_resolution_clock::now();
    mLastUpdateTime = mStartTime;
    mAccumulator = std::chrono::duration<double>::zero();

    // Model zombieModel("../../../resources/zombie_char_7_4.glb", glm::vec3(0.0f, 0.0f, 0.0f));
    // Model turretModel("../../../resources/turret.glb", glm::vec3(0.0f, 0.0f, 0.0f));
    // Model tankModel("../../../resources/zombie_char_7_4.glb", glm::vec3(0.0f, 0.0f, 0.0f));

    glm::vec3 light(15.0f, 30.0f, 5.0f);
    KeyInput::KeyController& controller = KeyInput::KeyController::get();

    // Application::run()
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        /*if (keys[KeyDefinition::KEY_F].state) {
            createBox(common, world);
        }*/
        controller.processKeys(window);
        
        if (controller.isKeyPressed(KeyInput::KeyDefinition::KEY_TAB)) {
            world->setIsDebugRenderingEnabled(true);
        }

        glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = currentTime - mLastUpdateTime;

        // Update the current display time
        mLastUpdateTime = currentTime;
        mAccumulator += deltaTime;

        
        while (mAccumulator >= timeStep) {
            mainShader.setVec3("light.position", light.x, light.y, light.z); // ImGui
            world->update(timeStep.count());
            player.processInput();

            mAccumulator -= timeStep;
        }        

        scene.render();

        // ----- Triangles ---- //
        const uint nbTriangles = debugRenderer.getNbTriangles();

        if (nbTriangles > 0)
        {
            // Vertices
            mDebugVBOTrianglesVertices.bind();
            GLsizei sizeVertices = static_cast<GLsizei>(nbTriangles * sizeof(rp3d::DebugRenderer::DebugTriangle));
            mDebugVBOTrianglesVertices.copyDataIntoVBO(sizeVertices, debugRenderer.getTrianglesArray(), GL_STREAM_DRAW);
            mDebugVBOTrianglesVertices.unbind();
        }

        int vertexPositionLoc = debugShader.getAttribLocation("aPos");
        // int vertexColorLoc = debugShader.getAttribLocation("vertexColor");

        // Triangles
        if (nbTriangles > 0) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            drawDebug(debugRenderer, vertexPositionLoc, 2);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // renderObject(mainShader, projection, view, zombieModel, turretModel, tankModel);

        // At the end as "overlay"
        if (controller.isKeyPressed(KeyDefinition::KEY_T)) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            ImGui::Begin("Hello, world!");
            // ImGui::SliderFloat("float", &XLight, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat3("floatt", &light.x, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    widthScreen = width;
    heightScreen = height;

    glViewport(0, 0, width, height);
}

void createBox(PhysicsCommon& common, PhysicsWorld* world)
{
    //glm::vec3 spawnPosition = (camera.Front * glm::vec3(15)) + camera.Position;
    //Object* object = new Object(spawnPosition);

    //boxes.push_back(object);
    //object->create(common, world, BodyType::DYNAMIC, Vector3(1.5, 1.5, 1.5));
    // object->getRigidBody()->applyLocalForceAtLocalPosition(Vector3(1000, 1000, 1000) * Vector3(camera.Front.x, camera.Front.y, camera.Front.z), Vector3(0.15, 0.7, 1.5));
}

void initDebug()
{
    mDebugVBOLinesVertices.create();

    // Create the VAO for both VBOs
    mDebugLinesVAO.create();
    mDebugLinesVAO.bind();

    // Bind the VBO of vertices
    mDebugVBOLinesVertices.bind();

    // Unbind the VAO
    mDebugLinesVAO.unbind();

    mDebugVBOLinesVertices.unbind();

    // ----- Triangles ----- //

    // Create the VBO for the vertices data
    mDebugVBOTrianglesVertices.create();

    // Create the VAO for both VBOs
    mDebugTrianglesVAO.create();
    mDebugTrianglesVAO.bind();

    // Bind the VBO of vertices
    mDebugVBOTrianglesVertices.bind();

    // Unbind the VAO
    mDebugTrianglesVAO.unbind();

    mDebugVBOTrianglesVertices.unbind();
}

void drawDebug(DebugRenderer& debugRenderer, uint vertexPositionLoc, uint vertexColorLoc)
{
    // Bind the VAO
    mDebugTrianglesVAO.bind();

    mDebugVBOTrianglesVertices.bind();

    glVertexAttribPointer(vertexPositionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (char*)nullptr);
    glEnableVertexAttribArray(vertexPositionLoc);

   // glVertexAttribIPointer(vertexColorLoc, 3, GL_UNSIGNED_INT, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (void*)sizeof(rp3d::Vector3));
   // glEnableVertexAttribArray(vertexColorLoc);

    // Draw the triangles geometry
    glDrawArrays(GL_TRIANGLES, 0, debugRenderer.getNbTriangles() * 3);

    glDisableVertexAttribArray(vertexPositionLoc);
    //glDisableVertexAttribArray(vertexColorLoc);

    mDebugVBOTrianglesVertices.unbind();

    // Unbind the VAO
    mDebugTrianglesVAO.unbind();
}