#include "Window.h"


int WindowManager::m_bCreateNewWin = 0;
WindowSetting WindowManager::m_wsNewSetting;

void KeyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void MouseFunc(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        WindowManager::m_bCreateNewWin = true;
    }
}
