all:
	g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` sayHello.cpp -o py2cpp`python3.6-config --extension-suffix`
