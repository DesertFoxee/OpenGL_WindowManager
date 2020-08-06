#ifndef WINDOW_H
#define WINDOW_H



#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include "conf_load.h"
#include "func_base.h"


typedef GLFWwindow  WindowBase;

#define WINDOW_MAX_SIZE 20
#define Deleteptr(ptr) delete(ptr); ptr = NULL;
#define deleteaptr

using namespace std;

class WindowManager;





void KeyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseFunc(GLFWwindow* window, int button, int action, int mods);


void window_focus_callback(GLFWwindow* window, int focused);
void cursor_enter_callback(GLFWwindow* window, int entered);



struct WindowSetting
{

    char* m_strTitle;
    unsigned int            m_uiScreenW;
    unsigned int            m_uiScreenH;
    int                     m_iAntiaLevel;
    bool                    m_bShowCursor;
    bool                    m_bResizeable;

    WindowSetting()
    {
        m_strTitle = _strdup("OpenGL");
        m_uiScreenW = 200;
        m_uiScreenH = 200;
        m_iAntiaLevel = -1;
        m_bShowCursor = true;
        m_bResizeable = false;
    }
    WindowSetting(char* title, unsigned int width, unsigned int height, int antiaLevel = -1,
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
    bool                    m_bHide;
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
        this->m_bHide = false;

        this->m_wsSetting.m_strTitle = _strdup(title);
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
        this->m_bHide = false;

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
    void RemoveAllChidren()
    {
        m_listChildren.clear();
    }

    bool AddChildren(Window* window)
    {
        ASSERT(window);
        if (!window || window->m_iID == this->m_iID || this == window
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
    void Hide()
    {
        this->m_bHide = true;
        glfwHideWindow(m_window);
    }
    void Show()
    {
        this->m_bHide = false;
        glfwShowWindow(m_window);
    }

    void SetOpacity(float opacity)
    {
        glfwSetWindowOpacity(m_window, opacity);
    }

    int IsClosed()
    {
        return  glfwWindowShouldClose(m_window);
    }
    void Close()
    {
        glfwSetWindowShouldClose(m_window, true);
        glfwDestroyWindow(m_window);
        this->RemoveData();
    }
    void Focus()
    {
        glfwFocusWindow(m_window);
    }
    void Draw()
    {
        glfwSwapBuffers(m_window);
    }
    friend class XWindowManager;
};



class XWindowManager
{
    std::vector<Window* >   m_vWindows;
    Window* m_pWinContextCurrent;
    Window* m_pWinRoot;
    Window* m_pWinActive;
    Window* m_pNewWinCreated;

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
        m_pNewWinCreated = NULL;
        m_uiMaxWindow = 0;
    }
    void initWindowManager()
    {
        if (!m_vWindows.empty())
        {
            DestroyWindowManager();
        }
        else this->ResetProperty();
        m_vWindows.resize(WINDOW_MAX_SIZE);
    }
    void DestroyWindowManager()
    {
        if (!m_vWindows.empty())
        {
            auto itWindow = m_vWindows.begin();
            while (itWindow != m_vWindows.end() && *itWindow != NULL)
            {
                (*itWindow)->Close();
                SafeDelete(&*itWindow);
            }
        }
        this->ResetProperty();
    }

private:

    void SetFunctionActive(Window* window)
    {
        if (window)
        {
            glfwSetKeyCallback(window->m_window, KeyboardFunc);
            glfwSetMouseButtonCallback(window->m_window, MouseFunc);
            glfwSetWindowFocusCallback(window->m_window, window_focus_callback);
            glfwSetCursorEnterCallback(window->m_window, cursor_enter_callback);
        }
    }
public:

    ~XWindowManager()
    {
        cout << "[Destroy XWindwoManager]" << endl;
        DestroyWindowManager();
    }

    //============================================================
    // Set connext draw OpenGL
    //============================================================
    Window* SetContextCurrent(int iWindowID)
    {
        ASSERT((iWindowID >= 0 && iWindowID < WINDOW_MAX_SIZE));
        if (iWindowID < 0 || !m_vWindows[iWindowID] ||
            !m_vWindows[iWindowID]->m_window) return NULL;

        m_pWinContextCurrent = m_vWindows[iWindowID];
        glfwMakeContextCurrent(m_pWinContextCurrent->m_window);
        return m_pWinContextCurrent;
    }

    void SetActiveWindow(Window* ptrActiveWindow)
    {
        ASSERT(ptrActiveWindow);
        int iWindowID = -1;
        if (ptrActiveWindow) iWindowID = ptrActiveWindow->GetID();
        Window* window = this->SetContextCurrent(iWindowID);
        if (window)
        {
            window->Focus();
        }
        ASSERT(window);
        m_pWinActive = window;
        SetFunctionActive(window);
    }

    void SetActiveWindow(int iWindowID)
    {
        ASSERT((iWindowID >= 0 && iWindowID < WINDOW_MAX_SIZE));
        Window* window = this->SetContextCurrent(iWindowID);
        if (window)
        {
            window->Focus();
        }
        ASSERT(window);
        m_pWinActive = window;
        SetFunctionActive(window);
    }

    //============================================================
    // Create new window and add this to windows manager 
    //============================================================
    Window* CreateWindow(WindowSetting setting, int iLay = -1)
    {
        Window* ptrNewWindow = new Window(setting);
        ASSERT(ptrNewWindow != NULL);
        if (ptrNewWindow == NULL || !ptrNewWindow->m_bCreateSuccess)
        {
            if (ptrNewWindow != NULL) SafeDelete(&ptrNewWindow);
            return NULL;
        }
        ptrNewWindow->m_iLay = iLay;
        bool bAddSucc = AddWindow(ptrNewWindow);
        if (bAddSucc == false)
        {
            SafeDelete(&ptrNewWindow);
        }
        return ptrNewWindow;
    }

    //============================================================
    // Create new window and add this to windows manager 
    //============================================================
    Window* CreateWindow(const char* title, unsigned int width, unsigned int height, int iLay = -1)
    {
        Window* ptrNewWindow = new Window(title, width, height);
        ASSERT(ptrNewWindow != NULL);
        if (ptrNewWindow == NULL || !ptrNewWindow->m_bCreateSuccess)
        {
            if (ptrNewWindow != NULL) delete ptrNewWindow;
            return NULL;
        }
        ptrNewWindow->m_iLay = iLay;
        bool bAddSucc = this->AddWindow(ptrNewWindow);
        if (bAddSucc == false)
        {
            SafeDelete(&ptrNewWindow);
        }
        m_pNewWinCreated = ptrNewWindow;
        return ptrNewWindow;
    }

    //============================================================
    // Add new window to windows manager using setting
    //============================================================
    bool AddWindow(Window* window)
    {
        ASSERT(window);
        if (window == NULL) return false;
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
        ASSERT((iWindowID > 0 && iWindowID >= WINDOW_MAX_SIZE));
        if (iWindowID < 0 || iWindowID >= WINDOW_MAX_SIZE) return;
        for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end(); ++itWindow)
        {
            if (!(*itWindow)) continue;
            if ((*itWindow)->m_iID == iWindowID)
            {
                RemoveAllChildren(*itWindow);
                GetParentWindow(*itWindow)->RemoveChildrenID((*itWindow)->m_iID);
                m_ltRetainsID.push_back((*itWindow)->m_iID);
                (*itWindow)->Close();
                SafeDelete(&*itWindow);
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
                Window* ptrParentWin = GetParentWindow(ptrWindow);
                if (ptrParentWin) ptrParentWin->RemoveChildrenID((*itWindow)->m_iID);
                m_ltRetainsID.push_back((*itWindow)->m_iID);
                (*itWindow)->Close();
                SafeDelete(&*itWindow);
            }
        }
    }

    //============================================================
    // Remove the entire window as a child of the window 
    //============================================================
    void RemoveAllChildren(Window* ptrWindow)
    {
        ASSERT(ptrWindow);
        if (!ptrWindow) return;

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
                    Window* ptrParentWin = GetParentWindow(ptrWindow);
                    if (ptrParentWin) ptrParentWin->RemoveChildrenID((*itWindow)->m_iID);
                    m_ltRetainsID.push_back((*itWindow)->m_iID);
                    (*itWindow)->Close();
                    SafeDelete(&*itWindow);
                }
            }
            itChildID++;
        }
    }

    void SortVisible()
    {
        ASSERT(m_pWinRoot);
        if (!m_pWinRoot) return;
        queue<Window*> queSortWin;
        queue<float > shadown;
        this->HideAll();
        queSortWin.push(m_pWinRoot);
        while (!queSortWin.empty() && queSortWin.size() < m_uiMaxWindow)
        {
            Window* nextWin = queSortWin.front();
            for (auto idChild : nextWin->m_listChildren)
            {
                Window* childWin = GetWindow(idChild);
                if (childWin)
                {
                    shadown.push(0);
                    queSortWin.push(childWin);
                }
            }
            nextWin->Focus();
            nextWin->Show();
            queSortWin.pop();
            
        }
    }

    Window* GetWindow(int iWindowID)
    {
        ASSERT((iWindowID >= 0 && iWindowID < WINDOW_MAX_SIZE));
        if (iWindowID < 0 && iWindowID >= WINDOW_MAX_SIZE) return NULL;

        return m_vWindows[iWindowID];
    }

    void HideAll()
    {
        for (auto window : m_vWindows)
        {
            if (window)
            {
                window->Hide();
            }
        }
    }
    void ShowAll()
    {
        for (auto window : m_vWindows)
        {
            if (window)
            {
                window->Show();
            }
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
    void SetIsRootWindow(Window* ptr)
    {
        m_pWinRoot = ptr;
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
            if (i != NULL)
            {
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

    static bool             m_bCreateNewWin;
    static bool             m_bSortWin;
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
                    m_wsNewSetting.m_strTitle = NumberToPChar(m_pWinActive->GetID() + 1);
                    Window* ptrNewWin = CreateWindow(m_wsNewSetting);
                    if (ptrNewWin)
                    {
                        m_pWinActive->AddChildren(ptrNewWin);
                        SetActiveWindow(ptrNewWin);
                        glfwWaitEvents();
                    }
                    this->m_bCreateNewWin = false;
                }
                if (m_bSortWin)
                {
                    SortVisible();
                    m_pWinActive->Focus();
                    this->m_bSortWin = false;
                }
                m_pWinActive->Draw();
                glfwPollEvents();
            }
            Window* wParentWindow = this->GetParentWindow(m_pWinActive);
            RemoveWindow(m_pWinActive);
            if (wParentWindow)
            {
                SetActiveWindow(wParentWindow->GetID());
                m_pWinActive->Show();
            }
            else
            {
                m_bCloseAllWindow = true;
            }
        }
    }
};


#endif // !WINDOW_H
