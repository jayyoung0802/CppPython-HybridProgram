def multiply(a,b):
    print("Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c

def hello2():
    print('hello')


def py_task():
    s = 0
    for i in range(0, 10**7):
        s = s + i
    return s

    
def tset_use_pd():
    import pandas as pd
    print(pd.DataFrame({'a':[1,2,3],'b':[4,5,6]}))

def test_raise_error():
    raise ValueError('test raise valueerror')

def test_use_mulit_params(use_int, use_str: str, use_byte: bytes, use_list: list, use_tuple: tuple, use_dict: dict, use_complex):
    print('use_int', use_int)
    print('use_str', use_str)
    print('use_byte', use_byte)
    print('use_list', use_list)
    print('use_tuple', use_tuple)
    print('use_dict', use_dict)
    print('use_complex', use_complex)
    return (use_int, use_str, use_byte, use_list, use_tuple, use_dict, use_complex)