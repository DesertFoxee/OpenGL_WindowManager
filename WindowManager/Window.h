#ifndef WINDOW_H
#define WINDOW_H



#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <iostream>
#include <vector>
#include <list>

#include "conf_load.h"


using namespace std;


struct WindowSetting
{
	const char* m_strTitle;
	unsigned int m_uiScreenW;
	unsigned int m_uiScreenH;
	int m_iAntialiasing;

	WindowSetting()
	{
		m_strTitle = "OpenGL";
		m_uiScreenW = 800;
		m_uiScreenH = 600;
		m_iAntialiasing = -1;
	}
};


class Window
{

private:
	int m_id;
	int m_iMultiSamp;

	GLFWwindow* m_window;
	WindowSetting m_tSetting;
	std::list<size_t> m_listChildren;

public:
	Window()
	{
		this->m_window = NULL;
		this->m_id = -100;
		this->m_iMultiSamp = -1;
		this->m_tSetting.m_strTitle = "";
		this->m_tSetting.m_uiScreenW = 100;
		this->m_tSetting.m_uiScreenH = 100;
		this->m_listChildren.clear();
	}
	Window(GLFWwindow* win, int id, const char* title, unsigned int width, unsigned int height, int iMulti = -1)
	{
		this->m_window = win;
		this->m_id = id;
		this->m_iMultiSamp = iMulti;
		this->m_tSetting.m_strTitle = title;
		this->m_tSetting.m_uiScreenW = width;
		this->m_tSetting.m_uiScreenH = height;
		this->m_listChildren.clear();
	}

	Window(GLFWwindow* win, int id, WindowSetting setting)
	{
		this->m_window = win;
		this->m_id = id;
		this->m_iMultiSamp = (setting.m_iAntialiasing > 0) ? setting.m_iAntialiasing : -1;
		this->m_tSetting = setting;
		this->m_listChildren.clear();
	}
	void Close()
	{
		glfwSetWindowShouldClose(m_window, true);
		glfwDestroyWindow(m_window);
	}
	bool AddChildren(Window* window)
	{
		if (window->m_id == this->m_id) return true;
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
	int m_iMaxWindow;
	std::vector<Window*> m_vWindows;
	Window* m_wCurrent;
	std::list<int> m_ltRetainsID;

private:
	XWindowManager()
	{
		cout << "[Init XWindwoManager]" << endl;
		initWindowManager();
	}
	void resetProperty()
	{
		m_iMaxWindow = 0;
		m_vWindows.clear();
		m_wCurrent = NULL;
		m_ltRetainsID.clear();
	}

	void initWindowManager()
	{
		if (!m_vWindows.empty())
		{
			destroyWindowManager();
		}
		this->resetProperty();
	}
	void destroyWindowManager()
	{
		for (Window* win : m_vWindows)
		{
			glfwSetWindowShouldClose(win->m_window, true);
			glfwDestroyWindow(win->m_window);
			delete win;
		}
		resetProperty();
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

	//============================================================
	// Add new window to windows manager using setting
	//============================================================
	Window* AddWindow(WindowSetting setting)
	{
		Window* newWin = NULL;
		GLFWwindow* glfwWin = glfwCreateWindow(setting.m_uiScreenW, setting.m_uiScreenH, setting.m_strTitle, NULL, NULL);
		ASSERT(glfwWin != NULL);
		if (glfwWin != NULL)
		{
			if (m_ltRetainsID.empty())
			{
				newWin = new Window(glfwWin, m_iMaxWindow, setting);
				m_iMaxWindow++;
			}
			else
			{
				newWin = new Window(glfwWin, m_ltRetainsID.front(), setting);
				m_ltRetainsID.pop_front();
			}
			if (newWin != NULL)
			{
				m_vWindows.push_back(newWin);
			}
		}
		else
		{
			// Output Log 
			cout << "Error [Window.cpp]: Create glfwCreateWindow() failed! " << endl;
		}
		return newWin;
	}
	//============================================================
	// Add new window to windows manager using Basic paramater
	//============================================================
	Window* AddWindow(const char* title, unsigned int width, unsigned int height)
	{
		Window* newWin = NULL;
		GLFWwindow* glfwWin = glfwCreateWindow(width, height, title, NULL, NULL);
		ASSERT(glfwWin != NULL);
		if (glfwWin != NULL)
		{
			WindowSetting setting;
			setting.m_strTitle = title;
			setting.m_uiScreenW = width;
			setting.m_uiScreenH = height;

			if (m_ltRetainsID.empty())
			{
				newWin = new Window(glfwWin, m_iMaxWindow, setting);
				m_iMaxWindow++;
			}
			else
			{
				newWin = new Window(glfwWin, m_ltRetainsID.front(), setting);
				m_ltRetainsID.pop_front();
			}
			if (newWin != NULL)
			{
				m_vWindows.push_back(newWin);
			}
		}
		else
		{
			//Output log 
			cout << "Error [Window.cpp]: Create glfwCreateWindow() failed! " << endl;
		}
		return newWin;
	}

	//============================================================
	// Remove the specified window using ID
	//============================================================
	void RemoveWindow(int iWindowID)
	{
		for (auto itWin = m_vWindows.begin(); itWin != m_vWindows.end(); )
		{
			Window* winRemove = *itWin;
			if (winRemove->m_id == iWindowID)
			{
				if (winRemove == m_wCurrent) m_wCurrent = NULL;
				if (winRemove != NULL && winRemove->m_window != NULL)
				{
					this->RemoveAllChildren((*itWin));
					winRemove->Close();
					m_ltRetainsID.push_back((*itWin)->m_id);
					m_vWindows.erase(itWin);
				}
				if (winRemove != NULL)
				{
					delete winRemove;
				}
				break;
			}
			else  itWin++;
		}
	}

	//============================================================
	// Remove the specified window using pointer window
	//============================================================
	void RemoveWindow(Window* ptrWindow)
	{
		for (auto itWin = m_vWindows.begin(); itWin != m_vWindows.end(); )
		{
			if (*itWin == ptrWindow)
			{
				if (ptrWindow == m_wCurrent) m_wCurrent = NULL;
				if (ptrWindow != NULL && ptrWindow->m_window != NULL)
				{
					this->RemoveAllChildren((*itWin));
					ptrWindow->Close();
					m_ltRetainsID.push_back((*itWin)->m_id);
					m_vWindows.erase(itWin);
				}
				if (*itWin != NULL)
				{
					delete (*itWin);
				}
				break;
			}
			else  itWin++;
		}
	}
	//============================================================
	// Remove the entire window as a child of the window 
	//============================================================
	bool RemoveAllChildren(Window* ptrWindow)
	{
		auto itChildren = ptrWindow->m_listChildren.begin();
		while (itChildren != ptrWindow->m_listChildren.end())
		{
			auto itWindow = m_vWindows.begin();
			while (itWindow != m_vWindows.end())
			{

				if ((*itWindow)->m_id == *itChildren)
				{
					RemoveAllChildren(*itWindow);
					itWindow = m_vWindows.erase(itWindow);
				}
				else ++itWindow;
			}
			++itChildren;
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
