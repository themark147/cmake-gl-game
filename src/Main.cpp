#include "Application/Application.h"

int mainLa()
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

    while (!glfwWindowShouldClose(application.getWindow()))
    {
        application.Run();
    }

    application.Shutdown();

    return 0;
}
