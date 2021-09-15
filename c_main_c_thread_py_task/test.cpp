#include <Python.h>
#include <iostream>
#include <string>
#include <thread>
#include "Timer.h"

// 封装PyGILState_Ensure/PyGILState_Release
class PythonThreadLocker
{
    PyGILState_STATE state;

public:
    PythonThreadLocker() : state(PyGILState_Ensure())
    {
    }
    ~PythonThreadLocker()
    {
        PyGILState_Release(state);
    }
};

int CallSomeFunction(char const *msg)
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    long long res = 0;

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyUnicode_DecodeFSDefault("func");
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    if (!pModule) // 加载模块失败
    {
        std::cout << "[ERROR] Python get module failed." << std::endl;
        return 1;
    }

    pFunc = PyObject_GetAttrString(pModule, "test_task");
    if (!pFunc || !PyCallable_Check(pFunc)) // 验证是否加载成功
    {
        std::cout << "[ERROR] Can't find funftion " << std::endl;
        return 2;
    }
    /* pFunc is a new reference */

    pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(msg));

    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    if (pValue)
    {
        res = PyLong_AsLong(pValue);
        std::cout << "result:" << res << std::endl;
        Py_DECREF(pValue);
    }

    Py_XDECREF(pFunc);
    Py_DECREF(pModule);

    return 0;
}

void use_thread_a()
{
    PythonThreadLocker locker;
    int result = CallSomeFunction("hello");
}

// 创建线程
int main(int argc, char *argv[])
{

    Timer timer;
    timer.start();

    Py_Initialize();
    PyEval_InitThreads();

    printf("a%d\n", PyEval_ThreadsInitialized());

    Py_BEGIN_ALLOW_THREADS;
    printf("b%d\n", PyGILState_Check());

    std::thread threads[6];
    std::cout << "Spawning 5 threads...\n";
    for (int i = 0; i < 6; i++)
    {
        threads[i] = std::thread(use_thread_a);
    }
    std::cout << "Done spawning threads! Now wait for them to join\n";
    for (auto &t : threads)
    {
        t.join();
    }

    std::cout << "All threads joined.\n";

    printf("c%d\n", PyGILState_Check());
    Py_END_ALLOW_THREADS;
    printf("d%d\n", PyGILState_Check());

    Py_FinalizeEx();

    timer.stop();
    std::cout << "Seconds: " << timer.elapsedSeconds() << std::endl;
    return 0;
}