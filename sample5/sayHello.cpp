#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <string>

class ContainerTest {
	public:
		ContainerTest(){}
		void Set(std::vector<int> v) {
		    mv = v;
		}

		void Print() {
		    for (size_t i = 0; i < mv.size(); i++) {
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

