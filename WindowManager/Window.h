#ifndef WINDOW_H
#define WINDOW_H



#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <iostream>
#include <vector>
#include <list>

#ifdef DEBUG
#       define ASSERT assert
#else 
#       define ASSERT 
#endif // DEBUG

#define ASSERT assert


using namespace std;


struct WindowSetting
{
    const char* strTitle;
    unsigned int uiScreenW;
    unsigned int uiScreenH;
    int iAntialiasing;

    WindowSetting()
    {
        strTitle = "OpenGL";
        strTitle = "OpenGL";
        uiScreenW = 800;
        uiScreenH = 600;
        iAntialiasing = -1;
    }
};
//

// This is comment 
class Window
{

private:
    int m_id;
    GLFWwindow* m_window;
    WindowSetting m_wsSetting;
    std::list<size_t> m_listChildren;

public:
    Window(GLFWwindow* win, int id, const char* title, unsigned int width, unsigned int height)
    {
        this->m_window = win;
        this->m_id = id;
        this->m_wsSetting.strTitle = title;
        this->m_wsSetting.uiScreenW = width;
        this->m_wsSetting.uiScreenH = height;
        this->m_listChildren.clear();
    }

    Window(GLFWwindow* win, int id, WindowSetting setting)
    {
        this->m_window = win;
        this->m_wsSetting = setting;
        this->m_id = id;
        this->m_listChildren.clear();
    }

    bool AddChildren(Window* window)
    {
        auto isIDExist = std::find(m_listChildren.begin(), m_listChildren.end(), window->m_id);
        if (isIDExist != m_listChildren.end())
        {
            m_listChildren.push_back(window->m_id);
            return true;
        }
        return false;
    }

    friend class XWindowManager;
};

class XWindowManager
{

private:
    std::vector<Window*> m_vWindows;
    Window* m_wCurrent;
    std::list<int> m_ltRetainsID;

private:
    XWindowManager()
    {
        cout << "[Init XWindwoManager]" << endl;
        initWindowManager();
    }

    void initWindowManager()
    {
        int iInitR = glfwInit();
        ASSERT(iInitR != GLFW_FALSE);
        if (iInitR == GLFW_FALSE)
        {
            cout << "Error [Window.cpp]: Load glfwInit() failed !" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        if (glewInit() != GLEW_OK)
        {
            cout << "Error [Window.cpp]: Load glewInit() failed !" << endl;
        }
        if (!m_vWindows.empty())
        {
            destroyWindowManager();
        }
        m_vWindows.clear();
        m_wCurrent = NULL;
    }
    void destroyWindowManager()
    {
        for (Window* win : m_vWindows)
        {
            glfwSetWindowShouldClose(win->m_window, true);
            glfwDestroyWindow(win->m_window);
            delete win;
        }
        glfwTerminate();
        m_vWindows.clear();
        m_wCurrent = NULL;
        m_ltRetainsID.clear();
    }

public:
    ~XWindowManager()
    {
        cout << "[Destroy XWindwoManager]" << endl;
        destroyWindowManager();
    }
    void setActive(int iIDWindow)
    {
        for (Window* window : m_vWindows)
        {
            if (window->m_id = iIDWindow)
            {
                m_wCurrent = window;
                glfwMakeContextCurrent(window->m_window);
                break;
            }
        }
    }
    void setActive(Window* ptrActiveWindow)
    {
        for (Window* window : m_vWindows)
        {
            if (window == ptrActiveWindow)
            {
                m_wCurrent = window;
                glfwMakeContextCurrent(window->m_window);
                break;
            }
        }
    }

    void createWindow(WindowSetting setting)
    {
        GLFWwindow* glfwWin = glfwCreateWindow(setting.uiScreenW, setting.uiScreenH, setting.strTitle, NULL, NULL);

        if (glfwWin != NULL)
        {
            Window* win = new Window(glfwWin, m_vWindows.size(), setting);

            m_vWindows.push_back(win);
        }
        else
        {
            // Output Log 
            cout << "Error [Window.cpp]: Create glfwCreateWindow() failed! " << endl;
        }
    }

    void createWindow(const char* title, unsigned int width, unsigned int height, WindowSetting setting)
    {
        GLFWwindow* glfwWin = glfwCreateWindow(width, height, title, NULL, NULL);

        if (glfwWin != NULL)
        {

            setting.strTitle = title;
            setting.uiScreenW = width;
            setting.uiScreenH = height;

            Window* win = new Window(glfwWin, m_vWindows.size(), setting);

            m_vWindows.push_back(win);
        }
        else
        {
            //Output log 
            cout << "Error [Window.cpp]: Create glfwCreateWindow() failed! " << endl;
        }

    }
    void RemoveWindow(int iWindowID)
    {
        for (auto itWin = m_vWindows.begin(); itWin != m_vWindows.end(); )
        {
            if ((*itWin)->m_id == iWindowID)
            {
                if (*itWin == m_wCurrent) m_wCurrent = NULL;

                if ((*itWin) != NULL && (*itWin)->m_window != NULL)
                {
                    glfwSetWindowShouldClose((*itWin)->m_window, true);
                    glfwDestroyWindow((*itWin)->m_window);
                    m_ltRetainsID.push_back((*itWin)->m_id);
                    m_vWindows.erase(itWin);
                }
                if (*itWin != NULL)
                {
                    delete (*itWin);
                }
            }
            else  itWin++;
        }
    }

    void RemoveWindow(Window* ptrWindow)
    {
        for (auto itWin = m_vWindows.begin(); itWin != m_vWindows.end(); )
        {
            if (*itWin == ptrWindow)
            {
                if (*itWin == m_wCurrent) m_wCurrent = NULL;
                if ((*itWin) != NULL && (*itWin)->m_window != NULL)
                {
                    glfwSetWindowShouldClose((*itWin)->m_window, true);
                    glfwDestroyWindow((*itWin)->m_window);
                    m_ltRetainsID.push_back((*itWin)->m_id);
                    m_vWindows.erase(itWin);
                }
                if (*itWin != NULL)
                {
                    delete (*itWin);
                }
            }
            else  itWin++;
        }
    }
    bool RemoveAllChildren(Window* ptrWindow)
    {
        auto itChildren = ptrWindow->m_listChildren.begin();
        while (itChildren != ptrWindow->m_listChildren.end())
        {
            for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end(); )
            {
                if ((*itWindow)->m_id == *itChildren)
                {
                    itWindow = m_vWindows.erase(itWindow);
                }
                else ++itWindow;
            }
            m_vWindows.erase(m_vWindows.begin());

            itChildren++;
        }
        // Remove all window in array window manager
        for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end(); )
        {
            if (*itWindow == ptrWindow)
            {
                itWindow = m_vWindows.erase(itWindow);
            }
            else ++itWindow;
        }

    }
    void removeWindow(int iIDWindow)
    {
        for (Window* window : m_vWindows)
        {
            if (window->m_id == iIDWindow)
            {
                if (window == m_wCurrent)
                {
                    m_wCurrent = NULL;
                }
                if (window != NULL && window->m_window != NULL)
                {
                    glfwSetWindowShouldClose(window->m_window, true);
                    glfwDestroyWindow(window->m_window);
                }
                delete window;
            }
        }
    }
    friend class WindowManager;
};


class WindowManager
{
private:
    static XWindowManager* m_winManager;


public:
    WindowManager()
    {
        cout << "[Init WindowManager]" << endl;
        if (m_winManager == NULL)
        {
            m_winManager = new XWindowManager();
        }
    }

    XWindowManager* getWindows()
    {
        return m_winManager;
    }

    ~WindowManager()
    {
        cout << "[Destroy WindowManager]" << endl;
        delete m_winManager;
    }

};


#endif // !WINDOW_H
