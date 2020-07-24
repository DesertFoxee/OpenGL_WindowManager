#ifndef WINDOW_H
#define WINDOW_H



#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <iostream>
#include <vector>
#include <list>
#include "conf_load.h"



#define MAX_WINDOW 20


using namespace std;


void KeyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
};


void MouseFunc(GLFWwindow* window, int button, int action , int mods) {

    
}




struct WindowSetting
{
    const char* m_strTitle;
    unsigned int    m_uiScreenW;
    unsigned int    m_uiScreenH;
    int             m_iAntiaLevel;
    bool            m_bShowCursor;
    bool            m_bResizeable;

    WindowSetting()
    {
        m_strTitle = "OpenGL";
        m_uiScreenW = 800;
        m_uiScreenH = 600;
        m_iAntiaLevel = -1;
        m_bShowCursor = true;
        m_bResizeable = false;
    }
    WindowSetting(const char* title, unsigned int width, unsigned int height, int antiaLevel = -1,
        bool showCursor = true, bool resizeAble = true)
    {
        m_strTitle = title;
        m_uiScreenW = width;
        m_uiScreenH = height;
        m_iAntiaLevel = antiaLevel;
        m_bShowCursor = showCursor;
        m_bResizeable = resizeAble;
    }
};
//

// This is comment 
class Window
{

private:
    int                     m_id;
    bool                    m_bClosed;
    GLFWwindow*             m_window;
    WindowSetting           m_wsSetting;
    std::list<size_t>       m_listChildren;
    

public:
    Window(GLFWwindow* win, int id, const char* title, unsigned int width, unsigned int height)
    {
        this->m_window = win;
        this->m_id = id;
        this->m_bClosed = false;
        this->m_wsSetting.m_strTitle = title;
        this->m_wsSetting.m_uiScreenW = width;
        this->m_wsSetting.m_uiScreenH = height;
        this->m_listChildren.clear();
    }

    Window(GLFWwindow* win, int id, WindowSetting setting)
    {
        this->m_window = win;
        this->m_wsSetting = setting;
        this->m_id = id;
        this->m_bClosed = false;
        this->m_listChildren.clear();
    }

    void SetViewPort(int left, int top, int width, int height)
    {
        glViewport(left, top, width, height);
    }

    void SetPosition(int iLeft, int iRight) {
        glfwSetWindowPos(this->m_window, iLeft, iRight);
    }

    bool AddChildren(Window* window)
    {
        if (window->m_id == this->m_id || this == window
            || this->m_window == window->m_window) return true;

        auto isIDExist = std::find(m_listChildren.begin(), m_listChildren.end(), window->m_id);
        if (isIDExist != m_listChildren.end())
        {

            m_listChildren.push_back(window->m_id);
            return true;
        }
        return false;
    }

    void SetSetting(WindowSetting setting) {
        this->m_wsSetting = setting;
    }

    WindowSetting* GetSetting() {
        return &this->m_wsSetting;
    }

    friend class XWindowManager;
};



class XWindowManager
{

private:
    std::vector<Window> m_vWindows;
    Window* m_wCurrent;
    std::list<int> m_ltRetainsID;
    unsigned int m_uiMaxWindow;

private:
    XWindowManager()
    {
        cout << "[Init XWindwoManager]" << endl;
        initWindowManager();
    }

    void initWindowManager()
    {

        if (!m_vWindows.empty())
        {
            destroyWindowManager();
        }
        m_vWindows.clear();
        m_wCurrent = NULL;
        m_ltRetainsID.clear();
        m_uiMaxWindow = 0;
    }
    void destroyWindowManager()
    {
        auto itWindow = m_vWindows.begin();
        while (itWindow != m_vWindows.end())
        {
            glfwSetWindowShouldClose(itWindow->m_window, true);
            glfwDestroyWindow(itWindow->m_window);
        }
        glfwTerminate();
        m_vWindows.clear();
        m_wCurrent = NULL;
        m_ltRetainsID.clear();
        m_uiMaxWindow = 0;
    }
private:
    void SetupHintsBeforeSetting(WindowSetting& setting)
    {
        if (setting.m_bResizeable == true) glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        if (setting.m_iAntiaLevel > 0) glfwWindowHint(GLFW_SAMPLES, setting.m_iAntiaLevel);

    }
    void SetupHintsAfterSetting(Window* window, WindowSetting& setting) {
        if (setting.m_bShowCursor == false) glfwSetInputMode(window->m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        if (setting.m_iAntiaLevel > 0) glEnable(GL_MULTISAMPLE);
    }

    void SetFunctionActive(Window* window)
    {
        glfwSetKeyCallback(window->m_window, KeyboardFunc);
        glfwSetMouseButtonCallback(window->m_window , MouseFunc);
    }
public:
    ~XWindowManager()
    {
        cout << "[Destroy XWindwoManager]" << endl;
        destroyWindowManager();
    }
    Window* SetContextCurrent(int iWindowID)
    {

        auto itWindow = m_vWindows.begin();
        while (itWindow != m_vWindows.end())
        {
            if (itWindow->m_id == iWindowID)
            {
                m_wCurrent = &(*itWindow);
                if (m_wCurrent != NULL)
                {
                    glfwMakeContextCurrent(m_wCurrent->m_window);
                }
                return &*itWindow;
            }
        }
        return NULL;
    }
    Window* SetContextCurrent(Window* ptrActiveWindow)
    {
        auto itWindow = m_vWindows.begin();
        while (itWindow != m_vWindows.end())
        {
            if (&(*itWindow) == ptrActiveWindow)
            {
                m_wCurrent = &(*itWindow);
                if (m_wCurrent != NULL)
                {
                    glfwMakeContextCurrent(m_wCurrent->m_window);
                }
                return &*itWindow;
            }
        }
        return NULL;
    }

    void SetActiveWindow(int iWindowID) {
        Window* window = this->SetContextCurrent(iWindowID);
        if (window != NULL)
        {
            SetFunctionActive(window);
        }
    }

    void AddWindow(WindowSetting setting)
    {

        SetupHintsBeforeSetting(setting);
        GLFWwindow* glfwWin = glfwCreateWindow(setting.m_uiScreenW, setting.m_uiScreenH, setting.m_strTitle, NULL, NULL);
        ASSERT(glfwWin != NULL);
        if (glfwWin != NULL && m_uiMaxWindow < MAX_WINDOW)
        {

            Window window(glfwWin, m_vWindows.size(), setting);
            SetupHintsAfterSetting(&window, setting);
            if (!m_ltRetainsID.empty())
            {
                window.m_id = m_ltRetainsID.front();
                m_ltRetainsID.pop_front();
                m_vWindows.push_back(window);
            }
            else
            {
                window.m_id = m_uiMaxWindow;
                m_vWindows.push_back(window);
                m_uiMaxWindow++;
            }
        }
        else
        {
            // Output Log 
            cout << "Error [Window.cpp]: Create glfwCreateWindow() failed! " << endl;
        }
    }

    void AddWindow(const char* title, unsigned int width, unsigned int height)
    {

        WindowSetting setting(title, width, height);
        SetupHintsBeforeSetting(setting);
        GLFWwindow* glfwWin = glfwCreateWindow(width, height, title, NULL, NULL);
        ASSERT(glfwWin != NULL);
        if (glfwWin != NULL && m_uiMaxWindow < MAX_WINDOW)
        {


            Window window(glfwWin, m_vWindows.size(), setting);
            SetupHintsAfterSetting(&window, setting);

            if (!m_ltRetainsID.empty())
            {
                window.m_id = m_ltRetainsID.front();
                m_ltRetainsID.pop_front();
                m_vWindows.push_back(window);
            }
            else
            {
                window.m_id = m_uiMaxWindow;
                m_vWindows.push_back(window);
                m_uiMaxWindow++;
            }
        }
        else
        {
            //Output log 
            cout << "Error [Window.cpp]: Create glfwCreateWindow() failed! " << endl;
        }

    }

    bool RemoveAllChildren(Window* ptrWindow)
    {
        auto itChildID = ptrWindow->m_listChildren.begin();
        while (itChildID != ptrWindow->m_listChildren.end())
        {
            for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end(); )
            {
                Window* winChild = &(*itWindow);
                if (winChild->m_id == *itChildID && winChild->m_id != ptrWindow->m_id)
                {
                    if (winChild == m_wCurrent) m_wCurrent = NULL;
                    RemoveAllChildren(winChild);
                    itWindow = m_vWindows.erase(itWindow);
                }
                else ++itWindow;
            }
            itChildID++;
        }
    }


    void RemoveWindow(int iWindowID)
    {
        for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end(); )
        {
            Window* ptrWin = &(*itWindow);
            if (ptrWin->m_id = iWindowID) {
                RemoveAllChildren(ptrWin);
            }
            else ++itWindow;
        }
    }

    void RemoveWindow(Window* ptrWindow)
    {
        for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end();)
        {
            Window* ptrWin = &(*itWindow);
            if (ptrWin == ptrWindow)
            {
                RemoveAllChildren(ptrWin);
            }
            else  itWindow++;
        }
    }


    void UpdateDraw() {
        
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
