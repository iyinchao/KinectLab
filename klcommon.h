#ifndef KLCOMMON_H
#define KLCOMMON_H

template<class Interface>
inline void safeRelease(Interface *& pInterfaceToRelease)
{
    if (pInterfaceToRelease != NULL)
    {
        pInterfaceToRelease->Release();
        pInterfaceToRelease = NULL;
    }
}

template<class Interface>
inline void safeDelete(Interface *& pInterfaceToDelete)
{
    if (pInterfaceToDelete != NULL)
    {
        delete pInterfaceToDelete;
        pInterfaceToDelete = NULL;
    }
}

template<typename T>
class KLComPtr
{
private:

    T* ptr = nullptr;

public:

    ~KLComPtr()
    {
        if ( ptr != nullptr ){
            ptr->Release();
            ptr = nullptr;
        }
    }

    T** operator & ()
    {
        return &ptr;
    }

    T* operator -> ()
    {
        return ptr;
    }

    operator T* ()
    {
        return ptr;
    }
};

#endif // KLCOMMON_H
