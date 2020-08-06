#include "Window.h"


bool WindowManager::m_bCreateNewWin = 0;
bool WindowManager::m_bSortWin = 0;
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

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        WindowManager::m_bSortWin = true;
    }
}

void window_focus_callback(GLFWwindow* window, int focused)
{
    cout << focused << endl;
}

void cursor_enter_callback(GLFWwindow* window, int entered)
{
    cout << entered << endl;
}


