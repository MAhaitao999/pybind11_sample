#include <pybind11/pybind11.h>

int add(int i, int j) {

	return i + j;
}

PYBIND11_MODULE(py2cpp, m) {

	m.doc() = "pybind11 example";
	m.def("add", &add, "add two number");
}

//在python中使用 模块名.函数名 来访问
//例如本例子为  py2cpp.add(1,2)
