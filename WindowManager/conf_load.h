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




using namespace std;


class conf_load
{
public :

    static bool bLoadGLFW;
    static bool bLoadGLEW;

	static void loadLibrary(){
        int iInitGLFW = glfwInit(); int iInitGLEW = glewInit();
        ASSERT(iInitGLFW != GLFW_FALSE);
        ASSERT(iInitGLEW == GLEW_OK);
        if (iInitGLFW == GLFW_FALSE){
            cout << "Error [conf_load.cpp]: Load glfwInit() failed !" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        else{
            bLoadGLFW = true;
        }
        if (iInitGLEW != GLEW_OK){
            ASSERT(0);
            cout << "Error [conf_load.cpp]: Load glewInit() failed !" << endl;
        }
        else  bLoadGLEW = true;
       
	}
    static void releaseLibrary(){
        glfwTerminate();
    }

};



#endif // !CONF_LOAD


