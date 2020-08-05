#ifndef WINDOW_H
#define WINDOW_H



#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <list>
#include "conf_load.h"


typedef GLFWwindow  WindowBase;

#define WINDOW_MAX_SIZE 20

using namespace std;

class WindowManager;




void KeyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseFunc(GLFWwindow* window, int button, int action, int mods);


struct WindowSetting
{

    const char* m_strTitle;
    unsigned int            m_uiScreenW;
    unsigned int            m_uiScreenH;
    int                     m_iAntiaLevel;
    bool                    m_bShowCursor;
    bool                    m_bResizeable;

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


class Window
{
    int                     m_iID;
    int                     m_iLay;
    bool                    m_bCreateSuccess;
    bool                    m_bClosed;
    int                     m_iParentID;
    WindowBase* m_window;
    WindowSetting           m_wsSetting;
    std::list<size_t>       m_listChildren;


private:
    void SetupHintsBeforeSetting(WindowSetting& setting)
    {
        if (setting.m_bResizeable == true) glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        if (setting.m_iAntiaLevel > 0) glfwWindowHint(GLFW_SAMPLES, setting.m_iAntiaLevel);
    }
    void SetupHintsAfterSetting(Window* window, WindowSetting& setting)
    {
        if (setting.m_bShowCursor == false) glfwSetInputMode(window->m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        if (setting.m_iAntiaLevel > 0) glEnable(GL_MULTISAMPLE);
    }

public:
    Window(const char* title, unsigned int width, unsigned int height, int layer = -1)
    {
        this->m_iID = -1;
        this->m_iParentID = -1;
        this->m_iLay = layer;
        this->m_bClosed = false;

        this->m_wsSetting.m_strTitle = title;
        this->m_wsSetting.m_uiScreenW = width;
        this->m_wsSetting.m_uiScreenH = height;
        this->m_window = glfwCreateWindow(width, height, title, NULL, NULL);
        this->m_bCreateSuccess = (this->m_window != NULL) ? true : false;
        this->m_listChildren.clear();
    }

    Window(WindowSetting setting, int layer = -1)
    {
        this->m_iID = -1;
        this->m_iParentID = -1;
        this->m_iLay = layer;
        this->m_bClosed = false;

        this->m_wsSetting = setting;
        this->m_window = glfwCreateWindow(setting.m_uiScreenW, setting.m_uiScreenH, setting.m_strTitle, NULL, NULL);;
        this->m_bCreateSuccess = (this->m_window != NULL) ? true : false;
        this->m_listChildren.clear();
    }

    void SetViewPort(int left, int top, int width, int height)
    {
        glViewport(left, top, width, height);
    }

    void SetPosition(int iLeft, int iRight)
    {
        glfwSetWindowPos(this->m_window, iLeft, iRight);
    }

    void RemoveChildrenID(int childrenID)
    {
        m_listChildren.remove(childrenID);
    }
    void RemoveAllChidrenID()
    {
        m_listChildren.clear();
    }

    bool AddChildren(Window* window)
    {
        if (window->m_iID == this->m_iID || this == window
            || this->m_window == window->m_window || window->m_iParentID >= 0) return true;

        if (m_listChildren.empty() || std::find(m_listChildren.begin(), m_listChildren.end(), window->m_iID) != m_listChildren.end())
        {
            window->m_iLay = this->m_iLay + 1;
            window->m_iParentID = this->m_iID;
            m_listChildren.push_back(window->m_iID);
            return true;
        }
        return false;
    }

    void Copy(Window* window)
    {
        this->m_iID = -1;
        this->m_iLay = window->m_iID;
        this->m_bClosed = true;
        this->m_listChildren.clear();
    }

    void RemoveData()
    {
        this->m_iID = -1;
        this->m_iLay = -1;
        this->m_bClosed = true;
        this->m_window = NULL;
        this->m_listChildren.clear();
    }

    void SetSetting(WindowSetting setting)
    {
        this->m_wsSetting = setting;
    }

    WindowSetting* GetSetting()
    {
        return &this->m_wsSetting;
    }

    int GetParentID()
    {
        return this->m_iParentID;
    }
    int GetID()
    {
        return this->m_iID;
    }
    void SetID(int id)
    {
        this->m_iID = id;
    }

    int IsClosed()
    {
        return  glfwWindowShouldClose(m_window);
    }
    int Close()
    {
        glfwSetWindowShouldClose(m_window, true);
    }
    void Distroy() 
    {
        glfwDestroyWindow(m_window);
    }

    void SwapBuffers()
    {
        glfwSwapBuffers(m_window);
    }
    friend class XWindowManager;
};



class XWindowManager
{
    std::vector<Window* >   m_vWindows;
    Window* m_pWinContextCurrent;
    Window* m_pWinActive;
    std::list<int>          m_ltRetainsID;
    unsigned int            m_uiMaxWindow;

private:
    XWindowManager()
    {
        cout << "[Init XWindwoManager]" << endl;
        initWindowManager();
    }
    void ResetProperty()
    {
        m_vWindows.clear();
        m_ltRetainsID.clear();
        m_pWinContextCurrent = NULL;
        m_pWinActive = NULL;
        m_uiMaxWindow = 0;
    }
    void initWindowManager()
    {
        if (!m_vWindows.empty())
        {
            destroyWindowManager();
        }
        else this->ResetProperty();
        m_vWindows.resize(WINDOW_MAX_SIZE);
    }
    void destroyWindowManager()
    {
        auto itWindow = m_vWindows.begin();
        while (itWindow != m_vWindows.end() && *itWindow != NULL)
        {
            glfwSetWindowShouldClose((*itWindow)->m_window, true);
            glfwDestroyWindow((*itWindow)->m_window);
            delete (*itWindow);
            (*itWindow) = NULL;
        }
        this->ResetProperty();
    }

private:

    void SetFunctionActive(Window* window)
    {
        glfwSetKeyCallback(window->m_window, KeyboardFunc);
        glfwSetMouseButtonCallback(window->m_window, MouseFunc);
    }
public:

    ~XWindowManager()
    {
        cout << "[Destroy XWindwoManager]" << endl;
        destroyWindowManager();
    }

    //============================================================
    // Set connext draw OpenGL
    //============================================================
    Window* SetContextCurrent(int iWindowID)
    {
        ASSERT(iWindowID >= 0);
        if (iWindowID < 0) return NULL;
        if (iWindowID < 0 || !m_vWindows[iWindowID] ||
            !m_vWindows[iWindowID]->m_window) return NULL;

        m_pWinContextCurrent = m_vWindows[iWindowID];
        glfwMakeContextCurrent(m_pWinContextCurrent->m_window);
        return m_pWinContextCurrent;
    }

    Window* SetContextCurrent(Window* ptrActiveWindow)
    {
        ASSERT(ptrActiveWindow);
        if (!ptrActiveWindow) return NULL;
        auto itWindow = m_vWindows.begin();
        while (itWindow != m_vWindows.end())
        {
            if ((*itWindow) == ptrActiveWindow)
            {
                m_pWinContextCurrent = (*itWindow);
                if (m_pWinContextCurrent != NULL)
                {
                    glfwMakeContextCurrent(m_pWinContextCurrent->m_window);
                }
                return (*itWindow);
            }
            ++itWindow;
        }
        return NULL;
    }

    void SetActiveWindow(Window* ptrActiveWindow)
    {
        Window* window = this->SetContextCurrent(ptrActiveWindow);
        ASSERT(window);
        if (window != NULL)
        {
            m_pWinActive = window;
            SetFunctionActive(window);
        }
    }

    void SetActiveWindow(int iWindowID)
    {
        Window* window = this->SetContextCurrent(iWindowID);
        ASSERT(window);
        if (window != NULL)
        {
            m_pWinActive = window;
            SetFunctionActive(window);
        }
    }
    //============================================================
    // Create new window and add this to windows manager 
    //============================================================
    Window* CreateWindow(WindowSetting setting)
    {
        Window* ptrNewWindow = new Window(setting);
        ASSERT(ptrNewWindow != NULL);
        if (ptrNewWindow == NULL || !ptrNewWindow->m_bCreateSuccess)
        {
            if (ptrNewWindow != NULL) delete ptrNewWindow;
            return NULL;
        }
        bool bAddSucc = this->AddWindow(ptrNewWindow);
        if (bAddSucc == false)
        {
            delete ptrNewWindow;
            ptrNewWindow = NULL;
        }
        return ptrNewWindow;
    }

    Window* CreateWindow(const char* title, unsigned int width, unsigned int height)
    {
        Window* ptrNewWindow = new Window(title, width, height);
        ASSERT(ptrNewWindow != NULL);
        if (ptrNewWindow == NULL || !ptrNewWindow->m_bCreateSuccess)
        {
            if (ptrNewWindow != NULL) delete ptrNewWindow;
            return NULL;
        }
        bool bAddSucc = this->AddWindow(ptrNewWindow);
        if (bAddSucc == false)
        {
            delete ptrNewWindow;
            ptrNewWindow = NULL;
        }
        return ptrNewWindow;
    }

    //============================================================
    // Add new window to windows manager using setting
    //============================================================
    bool AddWindow(Window* window)
    {
        if (window == NULL) return false;
        if (m_uiMaxWindow == 0)
        {
            window->m_iID = 0;
            window->m_iLay = 1;
            m_vWindows[0] = window;
            m_uiMaxWindow++;
            SetActiveWindow(window);
            return true;
        }

        if (m_uiMaxWindow < WINDOW_MAX_SIZE)
        {
            int idRentain = m_ltRetainsID.empty() ? -1 : m_ltRetainsID.front();
            if (idRentain != -1 && !m_vWindows[idRentain])
            {
                window->m_iID = idRentain;
                m_ltRetainsID.pop_front();
                m_vWindows[idRentain] = window;
            }
            else
            {
                window->m_iID = m_uiMaxWindow;
                m_vWindows[m_uiMaxWindow] = window;
                m_uiMaxWindow++;
            }
            return true;
        }
        return false;
    }

    //============================================================
    // Remove the specified window using ID
    //============================================================
    void RemoveWindow(int iWindowID)
    {
        ASSERT(iWindowID > 0);
        if (iWindowID < 0) return;
        for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end(); ++itWindow)
        {
            if (!(*itWindow)) continue;
            if ((*itWindow)->m_iID == iWindowID)
            {
                RemoveAllChildren(*itWindow);
                m_ltRetainsID.push_back((*itWindow)->m_iID);
                showRetain();
                glfwSetWindowShouldClose((*itWindow)->m_window, true);
                glfwDestroyWindow((*itWindow)->m_window);
                delete (*itWindow);
                *itWindow = NULL;
                showManager();
            }
        }
    }

    //============================================================
    // Remove the specified window using pointer window
    //============================================================
    void RemoveWindow(Window* ptrWindow)
    {
        ASSERT(ptrWindow);
        if (!ptrWindow) return;
        for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end(); ++itWindow)
        {
            if (!(*itWindow)) continue;
            if ((*itWindow) == ptrWindow)
            {
                RemoveAllChildren(*itWindow);
                this->GetParentWindow(ptrWindow)->RemoveChildrenID((*itWindow)->m_iID);
                m_ltRetainsID.push_back((*itWindow)->m_iID);
                showRetain();
                glfwSetWindowShouldClose((*itWindow)->m_window, true);
                glfwDestroyWindow((*itWindow)->m_window);
                delete (*itWindow);
                *itWindow = NULL;
                showManager();
            }
        }
    }

    void DistroyWindow()
    {

    }

    //============================================================
    // Remove the entire window as a child of the window 
    //============================================================
    bool RemoveAllChildren(Window* ptrWindow)
    {
        ASSERT(ptrWindow);
        if (!ptrWindow) return true;

        auto itChildID = ptrWindow->m_listChildren.begin();
        while (itChildID != ptrWindow->m_listChildren.end())
        {
            for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end(); ++itWindow)
            {
                if (!(*itWindow)) continue;

                if ((*itWindow)->m_iID == *itChildID &&
                    (*itWindow)->m_iLay >= ptrWindow->m_iLay &&
                    (*itWindow)->m_iID != (*itWindow)->m_iID &&
                    (*itWindow)->m_iLay >= 0)
                {
                    RemoveAllChildren((*itWindow));
                    this->GetParentWindow(ptrWindow)->RemoveChildrenID((*itWindow)->m_iID);
                    m_ltRetainsID.push_back((*itWindow)->m_iID);
                    
                    glfwSetWindowShouldClose((*itWindow)->m_window, true);
                    glfwDestroyWindow((*itWindow)->m_window);
                    (*itWindow)->RemoveData();
                    delete (*itWindow);
                    (*itWindow) = NULL;
                }
            }
            itChildID++;
        }
    }

    Window* GetParentWindow(Window* window)
    {
        ASSERT(window);
        if (!window || window->GetParentID() >= WINDOW_MAX_SIZE || window->GetParentID() < 0
            || !m_vWindows[window->m_iParentID]) return NULL;

        return m_vWindows[window->m_iParentID];
    }
    Window* GetParentWindow(int windowID)
    {
        ASSERT(windowID < WINDOW_MAX_SIZE);
        if (m_vWindows[windowID] != NULL)
        {
            return m_vWindows[windowID];
        }
        return NULL;
    }

    Window* GetWindowActive()
    {
        return m_pWinActive;
    }
    Window* GetContextCurrent()
    {
        return m_pWinContextCurrent;
    }

    void showManager()
    {
        cout << "Show >" << endl;
        for (auto i : m_vWindows)
        {
            if (i != NULL) {
                cout << i->m_iID;
                cout << "( ";
                for (auto const ii : i->m_listChildren)
                {
                    std::cout << ii << ", ";
                }
                cout << " )";
                cout << "-> ";
            }
        }
        cout << endl;
    }

    void showRetain()
    {
        cout << "Retain >" << endl;
        cout << "[ ";
        for (auto i : m_ltRetainsID)
        {
            cout << i << " ,";
        }
        cout << "] ";
        cout << endl;
    }

    void UpdateDraw()
    {

    }
    friend class WindowManager;
};


class WindowManager : public XWindowManager
{
private:
    int                     m_bCloseAllWindow;                        // 1: Cửa sổ đóng hết  0:Vẫn còn cửa sổ hiện hành
public:

    static int              m_bCreateNewWin;
    static WindowSetting    m_wsNewSetting;
public:
    WindowManager() : XWindowManager()
    {
        this->m_bCloseAllWindow = false;
        cout << "[Init WindowManager]" << endl;
    }
    ~WindowManager()
    {
        cout << "[Destroy WindowManager]" << endl;
    }

    void UpdateDraw()
    {
        while (m_bCloseAllWindow == false)
        {
            while (!m_pWinActive->IsClosed())
            {
                glClear(GL_COLOR_BUFFER_BIT);

                if (m_bCreateNewWin == true)
                {
                    Window* ptrNewWin = CreateWindow(m_wsNewSetting);
                    m_pWinActive->AddChildren(ptrNewWin);
                    showManager();
                    SetActiveWindow(ptrNewWin);
                    this->m_bCreateNewWin = false;
                }
                m_pWinActive->SwapBuffers();
                glfwPollEvents();
            }
            Window* wParentWindow = this->GetParentWindow(m_pWinActive);
            RemoveWindow(m_pWinActive);
            if (wParentWindow != NULL)
            {
                SetActiveWindow(wParentWindow->GetID());
            }
            else
            {
                m_bCloseAllWindow = true;
            }
        }
    }
};



#endif // !WINDOW_H
