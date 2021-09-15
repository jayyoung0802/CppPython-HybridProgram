#include <Python.h>
#include <iostream>
#include <string>
#include <thread>

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

int CallSomeFunction(char const *a)
{
    // int res = 0;
    // int nHold = PyGILState_Check(); //检测当前线程是否拥有GIL,python 3+可以用
    // PyGILState_STATE gstate;
    // if (!nHold)
    // {
    //     gstate = PyGILState_Ensure(); //如果没有GIL，则申请获取GIL
    // }

    // std::cout << " PyGILState_Check : " << PyGILState_Check() << std::endl;

    // Py_BEGIN_ALLOW_THREADS;
    // Py_BLOCK_THREADS;

    // PyRun_SimpleString("import sys");
    // PyRun_SimpleString("sys.path.append('./')");

    // PyObject *pModule, *pFunc;
    // PyObject *pArgs, *pValue;
    // //这里是要调用的文件名
    // pModule = PyImport_ImportModule("thread_task");
    // if (!pModule) // 加载模块失败
    // {
    //     std::cout << "[ERROR] Python get module failed." << std::endl;
    //     return 1;
    // }
    // /* build args */
    // pArgs = PyTuple_New(1);
    // PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(a));
    // //这里是要调用的函数名
    // pFunc = PyObject_GetAttrString(pModule, "py_timer");
    // if (!pFunc || !PyCallable_Check(pFunc)) // 验证是否加载成功
    // {
    //     std::cout << "[ERROR] Can't find funftion " << std::endl;
    //     return 2;
    // }
    // pValue = PyObject_CallObject(pFunc, pArgs);
    // if (pValue) // 验证是否调用成功
    // {
    //     res = PyLong_AsLong(pValue);
    //     std::cout << "result:" << res << std::endl;
    // }

    // Py_DECREF(pArgs);
    // Py_DECREF(pFunc);
    // Py_DECREF(pModule);

    // Py_UNBLOCK_THREADS;
    // Py_END_ALLOW_THREADS;
    // std::cout << " PyGILState_Check : " << PyGILState_Check() << std::endl;
    // if (!nHold)
    // {
    //     PyGILState_Release(gstate); //释放当前线程的GIL
    // }
    // std::cout << " PyGILState_Check : " << PyGILState_Check() << std::endl;
    // return res;

    int argc = 5;
    const char *argv[] = {"", "multiply", "multiply", "3", "2"};
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    pName = PyUnicode_DecodeFSDefault(argv[1]);
    /* Error checking of pName left out */

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL)
    {
        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc))
        {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i)
            {
                pValue = PyLong_FromLong(atoi(argv[i + 3]));
                if (!pValue)
                {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL)
            {
                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else
            {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return 1;
            }
        }
        else
        {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else
    {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }

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
    Py_Initialize();
    PyEval_InitThreads();

    printf("%d", PyEval_ThreadsInitialized());
    printf("a%d\n", PyGILState_Check());

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

    Py_UNBLOCK_THREADS;
    Py_END_ALLOW_THREADS;
    Py_FinalizeEx();

    return 0;
}