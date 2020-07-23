
#include <iostream>
#include <algorithm>


#define MAX_ELEMENT_ARRAY_DF 200

template < typename T>
class DArray {

private:
    T* m_tArray = nullptr;
    size_t m_sLength;

public:

    DArray() {
        m_tArray = new T[MAX_ELEMENT_ARRAY_DF];
        m_sLength = MAX_ELEMENT_ARRAY_DF;
    }
    DArray(T initValue) {
        m_tArray = new T[MAX_ELEMENT_ARRAY_DF](initValue);
        m_sLength = MAX_ELEMENT_ARRAY_DF;
    }
    ~DArray() {
        delete[] m_tArray;
        m_tArray = nullptr;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Allocate new memory for DArray
    // - First  : Allocate new memory temp * T [minLength]
    // - Secord : Copy memory the array current to new memory
    // - Third  : Delete the old Memory
    ///////////////////////////////////////////////////////////////////////////
    void ReSize(const size_t& _size) {
        if (m_tArray == nullptr) {
            m_tArray = new  T[_size];
            m_sLength = _size;
        }
        else {
            if (_size == 0) {
                delete[] m_tArray;
                m_tArray = nullptr;  m_sLength = 0;
                return;
            }
            else {
                T* temp = new T[_size];
                size_t minSize = std::min(_size, m_sLength);
                std::copy(m_tArray, m_tArray + minSize, temp);
                delete[] m_tArray;
                m_tArray = temp;
                m_sLength = _size;
            }
        }
    }

    void RemoveAt(size_t index) {
        if (index < m_sLength) {
            
        }
    };

};