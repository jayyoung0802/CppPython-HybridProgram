#include <pybind11/pybind11.h>
#include <iostream>
#include <unistd.h>
namespace py = pybind11;

long long sum_range()
{
    long long i;
    long long s = 0LL;
    long long high = 1e7;

    for (i = 0LL; i < high; i++)
        s += (long long)i;
    return s;
}

void func()
{
    while (true)
    {
        /* code */
    }

    return;
}

PYBIND11_MODULE(test_pybind, m)
{
    m.doc() = "Export the sum_range function as sum_range";

    m.def("func", &func, py::call_guard<py::gil_scoped_release>());
    m.def("sum_range", &sum_range, py::call_guard<py::gil_scoped_release>());
}
