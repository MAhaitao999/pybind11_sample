### pybind11安装

```
git clone https://github.com/pybind/pybind11
cd pybind11 && mkdir build && cd build && cmake ..  
sudo make install
```

由于pybind11依赖于pytest, 所以在安装前需要先把pytest给安装上.

```
pip3 install python3-dev pytest
```

不过在cmake时, 有可能会提示缺失Catch, 不过没有关系, cmake的提示上会把
解决方式带上的, 只要执行下`cmake -DDOWNLOAD_CATCH=1`就行了.

### 编译命令

```
g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` 要编译的源代码 -o 模块名`python3-config --extension-suffix` -I /path/to/python3
```

### sample简介

pybind11封装c++代码 例子

pybind11封装c++代码用法相当简单, 所有的封装代码都需要在`PYBIND11_MODULE` 函数里面, 具体定义如下:

```
PYBIND11_MODULE(模块名, 模块实例对象) {
    m.doc() = "pybind11 example"; // 可选, 说明这个模块是做什么的
    // 封装的具体操作. 这些操作包括普通的函数的封装, 类的访问等下面用不同例子来说明问题
}
```

- sample1: 普通函数的调用

pybind11的模块实例对象提供了def()函数, 用来封装普通的函数, 具体的用法为:

```
def ("给python调用方法名", &实际操作的函数, "函数功能说明"). // 其中函数功能说明为可选
```

下面给出一个简单的例子:

```
#include <pybind11/pybind11.h>

int add( int i, int j ){
    return i+j;
}

PYBIND11_MODULE( py2cpp, m ){
    m.doc() = "pybind11 example";
    m.def("add", &add, "add two number");
}

// 在python中使用**模块名.函数名**来访问
// 例如本例子为: py2cpp.add(1,2)
```

- sample2: 调用类的成员函数--调用命名空间外的类

pybind11提供了访问成员函数的能力. 和访问普通的函数不同, 访问成员函数之前先要生成一个对象实例, 因此, 对任意的一个类而言, 都需要有两步, 第一步是包装一个实例构造方法, 另外一个是成员函数的范围方式.

```
pybind11::class_<命名空间::类名>(m, "在python中构造这个类的方法名")
    .def(pybind11<>::init()) // 构造器, 对应的是c++类的构造函数, 如果没有这个构造函数, 或者参数对不是会调用失败
    .def("python中函数名", &命名空间::类名::函数名);
```

```
#include <pybind11/pybind11.h>
class Hello
{
public:
    Hello(){}
    void say( const std::string s ) {
        std::cout << s << std::endl;
    }
};

PYBIND11_MODULE( py2cpp, m ) {
    m.doc() = "pybind11 example";

    pybind11::class_<Hello>(m, "Hello" )
        .def(pybind11::init())
        .def( "say", &Hello::say );
}
// python 调用方式
// 1. 先通过构造器来构建实例，方法为 模块名.构造器名
// 2. 调用对应的方法， 模块名.方法名
// 例如本例子需要如下调用
// c=py2cpp.Hello()
// c.say()
```

- sample3: 调用类的成员函数--调用命名空间中的对象

```
#include <pybind11/pybind11.h>

namespace NS {
    class World {
        Public:
            World(){}
            void say (const std::string s) {
                std::cout << s << std::endl;
            }
    };
}

PYBIND11_MODULE(py2cpp, m) {
    m.doc() = "pybind11 example";
    pybind11::class_<NS::World>(m, "World")
        .def(pybind11::init())
        .def("say", &NS::World::say);
}

// 本例子需要如下调用:
// c=py2cpp.World()
// c.say()
```

- sample4: 带有参数的C++函数的访问--带参数的构造函数

上面两个例子所访问的函数都是不带参数或者只有基本数据类型的参数的, 那如果带有参数要怎样处理? 如果使用stl的容器又要怎样处理.

```
#include <iostream>
#include <pybind11/pybind11.h>

class Test {
    public:
        Test(int i, int j):mI(i), mJ(j){}

        void Print() {
            std::cout << "i= " << mI << "j= " << mJ << std::endl;
        }

    private:
        int mI;
        int mJ;
};

PYBIND11_MODULE(py2cpp, m) {
    m.doc() = "pybind11 example";
    pybind11::class_<Test>(m, "Test")
        .def(pybind11::init<int, int>()) // 构造器模版参数列表中需要按照构造函数的参数类型填入才能调对应参数
        .def("print", &Test::Print);
}

// 本例子需要如下调用:
// c=py2cpp.Test(1,2)
// c.print()
```
- sample5: 使用stl容器的作为参数的函数

pybind11提供了stl容器的封装类, 当需要处理stl容器时, 只要额外包括头文件`<filepybind11/stl.h>`即可. pybin11提供的自动转换包括`std::vector<>/std::list<>/std::array<>`转换成Python list; `std::set<>/std::unordered_set<>`转换成 python set; `andstd::map<>/std::unordered_map<>`则转换成dict几种. 至于`std::pair<>`和`std::tuple<>`的转换也在`<pybind11/pybind11.h>`头文件中提供了. 下面提供一个简单的例子:

```
#include <iostream>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class ContainerTest {
    public:
        ContainerTest(){}
        void Set(std::vector<int>) {
            mv = v;
        }

        void Print() {
            for (int size_t i = 0; i < mv.size(); i++) {
                std::cout << mv[i] << " ";
            }
            std::cout << std::endl;
        }

    private:
        std::vector<int> mv;
};

PYBIND11_MODULE(py2cpp, m) {
    m.doc() = "pybind11 example";
    pybind11::class_<ContainerTest>(m, "CT")
        .def(pybind11::init())
        .def("set", &ContainerTest::Set)
        .def("print", &ContainerTest::Print);
}

// 本例子需要如下调用
// c=py2cpp.CT()
// c.set( [1,2,3] )
// c.print()
```

- sample6: 访问struct/class的公有非成员变量

对于公有非成员变量的访问, pybind11提供了`def_readwrite()`方法来支持. 具体定义如下:

```
def_readwrite("在python中访问的变量名", &要访问的变量);
```

下面给出一个例子:

```
#include <pybind11/pybind11.h>

struct ST{
    std::string str;
    uint64_t i;
};

PYBIND11_MODULE( py2cpp, m ){
    m.doc() = "pybind11 example";
    pybind11::class_<ST>(m, "ST")
        .def( pybind11::init() )
        .def_readwrite("str", &ST::str )
        .def_readwrite("i", &ST::i );
}

// 本例子需要如下调用
// c = py2cpp.ST()
// c.str="hello"
// c.i = 123
// print(c.str)
// print(c.i)
```

- sample7: 枚举类型

```
#include <iostream>
#include <pybind11/pybind11.h>

enum Sentiment {
    Angry = 0,
	Happy,
	Confused
};

void mood(Sentiment s) {
    std::cout << "test!!!" << std::endl;
}

namespace py = pybind11;

PYBIND11_MODULE(py2cpp, m) {
    m.doc() = "pybind11 example";

	py::enum_<Sentiment>(m, "Sentiment")
		.value("Angry", Angry)
		.value("Happy", Happy)
        .value("Confused", Confused)
        .export_values();

	m.def("mood", &mood, "Demonstrate using an enum");
}
```
