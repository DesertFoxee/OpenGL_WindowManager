//[!] Include file glew.h => glfw3.h
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include <vector>


using namespace std;

class Window1
{
public:
    int                     m_id;
    bool                    m_bClosed;
    GLFWwindow              * m_window;
    std::list<size_t>       m_listChildren;
    int                     m_iLay;

    Window1()
    {
        m_iLay = -1;
    }
};

std::vector<Window1> m_vWindows;

bool RemoveAllChildren(Window1* ptrWindow)
{
    auto itChildID = ptrWindow->m_listChildren.begin();
    while (itChildID != ptrWindow->m_listChildren.end())
    {
        for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end(); )
        {
            Window1* winChild = &(*itWindow);
            if (winChild->m_iLay >= ptrWindow->m_iLay && winChild->m_id == *itChildID
                && winChild->m_id != ptrWindow->m_id)
            {
                RemoveAllChildren(winChild);
                cout << "Remove : " << itWindow->m_id << endl;
                itWindow = m_vWindows.erase(itWindow);
            }
            else ++itWindow;
        }
        itChildID++;
    }
    return true;
}


//============================================================
   // Remove the specified window using ID
   //============================================================
void RemoveWindow(Window1* ptrWindow)
{
    for (auto itWindow = m_vWindows.begin(); itWindow != m_vWindows.end();)
    {
        Window1* ptrWin = &(*itWindow);
        if (ptrWin == NULL)
        {
            itWindow = m_vWindows.erase(itWindow);
            continue;
        }
        else if (ptrWin == ptrWindow)
        {
            RemoveAllChildren(ptrWin);
            itWindow = m_vWindows.erase(itWindow);
        }
        else  ++itWindow;
    }
}


void show()
{
    cout << "==== Show : " << endl;
    for (auto i : m_vWindows)
    {
        cout << " -> " << i.m_id << "( ";
        for (auto ii : i.m_listChildren)
        {
            cout << ii << " , ";
        }
        cout << ")";

    }
    cout << endl;
}

using namespace std;

int main(void)
{

    const int max_e = 9;
    Window1 w[max_e];
    for (int i = 0; i < max_e; i++)
    {
        w[i].m_id = i;
    }
    w[1].m_iLay = 1;
    w[1].m_listChildren.push_back(2);
    w[1].m_listChildren.push_back(3);


    w[2].m_iLay = 2;
    w[3].m_iLay = 2;
    w[2].m_listChildren.push_back(1);
    w[2].m_listChildren.push_back(3);

    for (int i = 0; i < max_e; i++)
    {
        m_vWindows.push_back(w[i]);
    }
    show();
    cout << "====================" << endl;
    RemoveWindow(&m_vWindows.at(1));
    cout << "====================" << endl;
    show();
    getchar();
    exit(EXIT_SUCCESS);
}
