#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <string>

struct ST {
    std::string str;
	uint64_t i;
};

PYBIND11_MODULE(py2cpp, m) {
    m.doc() = "pybind11 example",
	pybind11::class_<ST>(m, "ST")
	    .def(pybind11::init())
		.def_readwrite("str", &ST::str)
		.def_readwrite("i", &ST::i);
}

// 本例子需要如下调用
// c = py2cpp.ST()
// c.str = "hello"
// c.i = 123
// print(c.str)
// print(c.i)
