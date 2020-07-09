#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <vector>

#define BOOL int

#define True 1 
#define False 0

using namespace std;


struct WindowSetting
{
	const char* strTitle;
	unsigned int uiScreenW;
	unsigned int uiScreenH;
	int iAntialiasing;

	WindowSetting() {
		strTitle = "OpenGL";
		uiScreenW = 800;
		uiScreenH = 600;
		iAntialiasing = -1;
	}
};

class Window
{

private:
	int m_id;
	GLFWwindow* m_window;
	WindowSetting m_wsSetting;

public:
	Window(GLFWwindow* win, int id, const char* title, unsigned int width, unsigned int height) {
		this->m_window = win;
		this->m_id = id;
		this->m_wsSetting.strTitle = title;
		this->m_wsSetting.uiScreenW = width;
		this->m_wsSetting.uiScreenH = height;
	}

	Window(GLFWwindow* win, int id, WindowSetting setting) {
		this->m_window = win;
		this->m_wsSetting = setting;
		this->m_id = id;
	}

	friend class XWindowManager;
};


class XWindowManager
{

private:
	std::vector<Window*> m_vWindows;
	Window* m_wCurrent;

private:
	XWindowManager() {
		cout << "[Init XWindwoManager]" << endl;
		initWindowManager();
	}

	void initWindowManager() {
		if (glfwInit() == GLFW_FALSE) {
			cout << "Error [Window.cpp]: Load glfwInit() failed !" << endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		if (glewInit() != GLEW_OK) {
			cout << "Error [Window.cpp]: Load glewInit() failed !" << endl;
		}
		if (!m_vWindows.empty()) {
			destroyWindowManager();
		}
		m_vWindows.clear();
		m_wCurrent = NULL;
	}

	void destroyWindowManager() {
		for (Window* win : m_vWindows) {
			glfwSetWindowShouldClose(win->m_window, true);
			glfwDestroyWindow(win->m_window);
			delete win;
		}
		glfwTerminate();
		m_vWindows.clear();
		m_wCurrent = NULL;
	}

public:
	~XWindowManager() {
		cout << "[Destroy XWindwoManager]" << endl;
		destroyWindowManager();
	}
	void setActive(int iIDWindow) {
		for (Window* window : m_vWindows) {
			if (window->m_id = iIDWindow) {
				m_wCurrent = window;
				glfwMakeContextCurrent(window->m_window);
				break;
			}

		}
	}
	void setActive(Window* ptrActiveWindow) {
		for (Window* window : m_vWindows) {
			if (window == ptrActiveWindow) {
				m_wCurrent = window;
				glfwMakeContextCurrent(window->m_window);
				break;
			}
		}
	}

	void createWindow(WindowSetting setting) {
		GLFWwindow* glfwWin = glfwCreateWindow(setting.uiScreenW, setting.uiScreenH, setting.strTitle, NULL, NULL);

		if (glfwWin != NULL) {
			Window* win = new Window(glfwWin, m_vWindows.size(), setting);

			m_vWindows.push_back(win);
		}
		else {
			// Output Log 
			cout << "Error [Window.cpp]: Create glfwCreateWindow() failed! " << endl;
		}
	}

	void createWindow(const char* title, unsigned int width, unsigned int height, WindowSetting setting) {
		GLFWwindow* glfwWin = glfwCreateWindow(width, height, title, NULL, NULL);

		if (glfwWin != NULL) {

			setting.strTitle = title;
			setting.uiScreenW = width;
			setting.uiScreenH = height;

			Window* win = new Window(glfwWin, m_vWindows.size(), setting);

			m_vWindows.push_back(win);
		}
		else {
			//Output log 
			cout << "Error [Window.cpp]: Create glfwCreateWindow() failed! " << endl;
		}

	}

	void removeWindow(Window* ptrWindow) {
		for (Window* window : m_vWindows) {
			if (window == ptrWindow) {
				if (window == m_wCurrent) {
					m_wCurrent = NULL;
				}
				if (window != NULL && window->m_window != NULL) {
					glfwSetWindowShouldClose(window->m_window, true);
					glfwDestroyWindow(window->m_window);
				}
				delete window;
			}
		}
	}

	void removeWindow(int iIDWindow) {
		for (Window* window : m_vWindows) {
			if (window->m_id == iIDWindow) {
				if (window == m_wCurrent) {
					m_wCurrent = NULL;
				}
				if (window != NULL && window->m_window != NULL) {
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
	WindowManager() {
		cout << "[Init WindowManager]" << endl;
		if (m_winManager == NULL) {
			m_winManager = new XWindowManager();
		}
	}

	XWindowManager* getWindows() {
		return m_winManager;
	}

	~WindowManager() {
		cout << "[Destroy WindowManager]" << endl;
		delete m_winManager;
	}

};


#endif // !WINDOW_H
