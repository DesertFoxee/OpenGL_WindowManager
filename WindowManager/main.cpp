//[!] Include file glew.h => glfw3.h
#include "Window.h"

using namespace std;

void fun()
{
    ConfLoad::LoadGLFWLibrary();
    WindowManager m_WinManger;
    Window *win = m_WinManger.CreateWindow("ROOT window", 200, 200 , 0);
    m_WinManger.SetActiveWindow(win->GetID());
    m_WinManger.SetIsRootWindow(win);
    ConfLoad::LoadGLEWLibrary();
    m_WinManger.UpdateDraw();

    ConfLoad::UnLoadLibrary();
}

int main(void)
{
    fun();
    //getchar();
    exit(EXIT_SUCCESS);
}
