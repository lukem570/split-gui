#ifndef MYHEADER_HPP
#define MYHEADER_HPP

#ifdef BUILD_MYLIBRARY
    #define MYLIBRARY_EXPORT __declspec(dllexport)  // For Windows DLLs
#else
    #define MYLIBRARY_EXPORT __declspec(dllimport)  // For using DLLs
#endif

class MYLIBRARY_EXPORT MyClass {
public:
    MyClass();
    void greet();
};

#endif // MYHEADER_HPP