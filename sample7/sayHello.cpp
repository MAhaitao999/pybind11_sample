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
