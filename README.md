# c-python
c++ 和python混合编程实例  
各子目录中的run是c++文件编译命令  
c++多线程情况很多api需要python3+版本才支持
## c++调用python相关链接
[C++与Python的混合编程-C++调用Python](https://www.jianshu.com/p/ce26bfc7474f)  
[C++ & Python 混合编程（1）-- C++使用Python](https://blog.csdn.net/zizi7/article/details/79098097)  
[C/C++与python互相调用](https://blog.csdn.net/u012234115/article/details/50210835)


## 实验
```
    main   thread    task   
1   cpp    cpp       cpp     
2   cpp    cpp       py
3   cpp    py        cpp
4   cpp    py        py
5   py     cpp       cpp
6   py     cpp       py
7   py     py        cpp
8   py     py        py


#其中 1，8属于原生，task不考虑cpp，综上则排除 1，3，5，7，8，剩余2，4, 6
#2,4,7 均以调通
#执行6个线程 0～1e7 求和运算  
#(方案1,原生cpp)0.024s  (方案4) 2.583s  (方案2)2.642s  (方案7)0.003s  (方案8，python原生) 2.939s


#原生cpp与方案7可释放锁，所以速度快
#方案4和方案2暂时未找到释放锁的方法，但均比原生快
#方案7比方案1快 有点不合常理,检查线程输出结果正确
#方案7可以释放gil锁，采用多核

```
