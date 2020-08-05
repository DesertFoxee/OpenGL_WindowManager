//[!] Include file glew.h => glfw3.h
#include "Window.h"

using namespace std;

void fun()
{
    ConfLoad::LoadGLFWLibrary();
    WindowManager m_WinManger;
    m_WinManger.CreateWindow("ROOT window", 200, 200);
    ConfLoad::LoadGLEWLibrary();
    m_WinManger.UpdateDraw();

    ConfLoad::UnLoadLibrary();
}

int main(void)
{
    fun();
    getchar();
    exit(EXIT_SUCCESS);
}
