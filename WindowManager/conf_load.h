#ifndef CONF_LOAD
#define CONF_LOAD

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define DEBUG

//=========================
// Define
//=========================

#ifdef DEBUG
#       define ASSERT(f) _ASSERT(f)
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

    static void LoadGLFWLibrary()
    {
        int iInitGLFW = glfwInit();
        ASSERT(iInitGLFW != GLFW_FALSE);
        if (iInitGLFW == GLFW_FALSE)
        {
            m_bGLFWStatus = false;
            cout << "Error [conf_load.cpp]: Load glfwInit() failed !" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
    }

    static void LoadGLEWLibrary()
    {
        int iInitGLEW = glewInit();
        ASSERT(iInitGLEW == GLEW_OK);
        if (iInitGLEW != GLEW_OK)
        {
            m_bGLEWStatus = false;
            cout << "Error [conf_load.cpp]: Load glewInit() failed !" << endl;
        }
    }

    static void UnLoadLibrary()
    {
        glfwTerminate();
    }

};

#endif // !CONF_LOAD
