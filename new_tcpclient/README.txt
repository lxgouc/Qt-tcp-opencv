在main中创建了两个线程，第一个线程建立tcp连接并获取摄像头数据完成一定转换后发送给服务器端。第二个线程待开发。

注意 在linux中 PATH变量保存的是可执行文件的查找路径，没有被PATH包含的路径，其中的可执行文件是不能被查找到的，仅仅是在这个变量所包含路径中的可执行文件才会被执行

#在PATH中找到可执行文件程序的路径。
export PATH =$PATH:$HOME/bin

#gcc找到头文件的路径
C_INCLUDE_PATH=/usr/include/libxml2:/MyLib
export C_INCLUDE_PATH

#g++找到头文件的路径
CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/include/libxml2:/MyLib
export CPLUS_INCLUDE_PATH

#找到动态链接库的路径
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/MyLib
export LD_LIBRARY_PATH

#找到静态库的路径
LIBRARY_PATH=$LIBRARY_PATH:/MyLib
export LIBRARY_PATH


有一点要明白，编译时候指定头文件和库所在路径原因是我们的gcc和g++默认下只会载入c/c++语言使用的标准库，而我们的程序可能有外部的库，所以要指定，这是在编译时候。动态链接库只是在程序运行时候才调用的，为什么编译时候也要指定，原因是可执行文件中要有动态链接库的信息，包括函数起始地址，偏移量等等，运行时候会调用动态链接库，通常我们在安装某个库时候这些库会自动加入到系统默认的库目录中，所以在运行时候可执行文件会自动去系统默认库目录下寻找库，如果没有加入到系统默认库目录中就要单独指定了，常用方法就是将其所在的路径加入到环境变量中，这样可执行文件也会通过自定义的环境变量找到库。

linux 系统把 /lib 和 /usr/lib 两个目录作为默认的库搜索路径，所以使用这两个目录中的库时不需要进行设置搜索路径即可直接使用。对于处于默认库搜索路径之外的库，需要将库的位置添加到 库的搜索路径之中
