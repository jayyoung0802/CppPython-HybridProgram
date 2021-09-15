import test_pybind
import time
import threading as T
import time

func = test_pybind.func
sum_range = test_pybind.sum_range


def cpp_task():
    sum_range()
    #func()
    #py_task()
    # func()


def py_task():
    s = 0
    for i in range(0, 10**7):
        s = s + i
    return s


def py_ser(n):
    start_time = time.time()
    for i in range(n):
        py_task()
    end_time = time.time()
    print(f"Time elapsed = {end_time-start_time:.5f}s")


def py_timer(n):
    start_time = time.time()
    threads = []
    for i in range(n):
        t = T.Thread(target=py_task, args=())
        threads.append(t)

    for t in threads:
        t.start()

    for t in threads:
        t.join()

    end_time = time.time()
    print(f"Time elapsed = {end_time-start_time:.5f}s")


def cpp_timer(n):
    start_time = time.time()
    threads = []
    for i in range(n):
        t = T.Thread(target=cpp_task, args=())
        threads.append(t)

    for t in threads:
        t.start()

    for t in threads:
        t.join()

    end_time = time.time()
    print(f"Time elapsed = {end_time-start_time:.5f}s")


py_timer(1)
py_timer(2)
py_timer(3)
py_timer(4)
py_timer(5)
py_timer(6)
print('----------------')
py_ser(1)
py_ser(2)
py_ser(3)
py_ser(4)
py_ser(5)
py_ser(6)
print('----------------')
cpp_timer(1)
cpp_timer(2)
cpp_timer(3)
cpp_timer(4)
cpp_timer(5)
cpp_timer(6)