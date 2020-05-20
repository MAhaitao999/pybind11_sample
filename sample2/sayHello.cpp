#include <pybind11/pybind11.h>
#include <iostream>
#include <string>

class Hello {
	public:
		Hello(){}
		void say(const std::string s) {
		    std::cout << s << std::endl;
		}
};

class Box {
    public:
		double x1, y1, x2, y2;
		Box(double x1, double x2, double y1, double y2);
		void print() {
		    std::cout << "x1 is: " << x1 << " " << "x2 is: " << x2 << std::endl
			          << "y1 is: " << y2 << " " << "y2 is: " << y2 << std::endl;
		}
};

Box::Box(double x1, double x2, double y1, double y2) {
    std::cout << "construct successful!!" << std::endl;
}

PYBIND11_MODULE(py2cpp, m) {
    m.doc() = "pybind11 example";

	pybind11::class_<Hello>(m, "Hello")
		.def(pybind11::init())
		.def("say", &Hello::say);

	pybind11::class_<Box>(m, "Box")
		.def(pybind11::init<double, double, double, double>())
		.def("print", &Box::print);
}

// python 调用方式
// 1. 先通过构造器来构建实例, 方法为 模块名.构造器名
// 2. 调用对应的方法, 模块名.方法名
// 例如本例子需要如下调用
// c=py2cpp.Hello()
// c.say()
