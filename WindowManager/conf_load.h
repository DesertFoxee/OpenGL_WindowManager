#ifndef CONF_LOAD
#define CONF_LOAD

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

//=========================
// Define
//=========================

#ifdef DEBUG
#       define ASSERT assert
#else 
#       define ASSERT 
#endif // DEBUG

//=========================
// Define
//=========================


using namespace std;


class ConfLoad
{

public:
    static bool m_bGLFWStatus;
    static bool m_bGLEWStatus;

    static void LoadLibrary()
    {
        int iInitGLFW = glfwInit(); int iInitGLEW = glewInit();
        m_bGLFWStatus = true;
        m_bGLEWStatus = true;

        ASSERT(iInitGLFW != GLFW_FALSE);

        if (iInitGLFW == GLFW_FALSE)
        {
            m_bGLFWStatus = false;
            cout << "Error [Window.cpp]: Load glfwInit() failed !" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        ASSERT(iInitGLEW == GLEW_OK);
        if (iInitGLEW != GLEW_OK)
        {
            m_bGLEWStatus = false;
            cout << "Error [Window.cpp]: Load glewInit() failed !" << endl;
        }

    }

    static void UnLoadLibrary()
    {
        glfwTerminate();
    }

};

#endif // !CONF_LOAD
