#include <Python.h>
#include <iostream>
#include <string>
#include "Timer.h"

int call_python_function(char const *a)
{
    int res = 0;
    int nHold = PyGILState_Check(); //检测当前线程是否拥有GIL,python 3+可以用
    PyGILState_STATE gstate;
    if (!nHold)
    {
        gstate = PyGILState_Ensure(); //如果没有GIL，则申请获取GIL
    }

    Py_BEGIN_ALLOW_THREADS;
    Py_BLOCK_THREADS;

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    //这里是要调用的文件名
    pModule = PyImport_ImportModule("thread_task");
    if (!pModule) // 加载模块失败
    {
        std::cout << "[ERROR] Python get module failed." << std::endl;
        return 1;
    }
    /* build args */
    pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(a));
    //这里是要调用的函数名
    pFunc = PyObject_GetAttrString(pModule, "py_timer");
    if (!pFunc || !PyCallable_Check(pFunc)) // 验证是否加载成功
    {
        std::cout << "[ERROR] Can't find funftion " << std::endl;
        return 2;
    }
    pValue = PyObject_CallObject(pFunc, pArgs);
    if (pValue) // 验证是否调用成功
    {
        res = PyLong_AsLong(pValue);
        std::cout << "result:" << res << std::endl;
    }

    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    Py_UNBLOCK_THREADS;
    Py_END_ALLOW_THREADS;

    if (!nHold)
    {
        PyGILState_Release(gstate); //释放当前线程的GIL
    }

    return res;
}
int main_function()
{
    //使用python之前，要调用Py_Initialize();这个函数进行初始化
    Py_Initialize();
    PyEval_InitThreads();
    return call_python_function("hello world");
    //调用Py_Finalize，这个根Py_Initialize相对应的,必须在python调用完以后执行,否则找不到python module
    Py_Finalize();
}

int main()
{
    Timer timer;
    timer.start();
    int res = 0;
    main_function();
    timer.stop();
    std::cout << "Seconds: " << timer.elapsedSeconds() << std::endl;
    return 0;
}
