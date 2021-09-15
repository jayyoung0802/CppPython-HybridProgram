import threading as T

def py_task(msg):
    s = 0
    for i in range(0, 10**7):
        s = s + i
    return s

def py_timer(msg):
    threads = []
    for i in range(6):
        t = T.Thread(target=py_task, args=(msg,))
        t.start()
        threads.append(t)

    for t in threads:
        t.join()

    return 100
