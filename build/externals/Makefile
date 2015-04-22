# build externals dependencies
all: jsoncpp yaml-cpp

jsoncpp:
	cd ../../externals/jsoncpp/ && cmake . && make
	
yaml-cpp:
	cd ../../externals/yaml-cpp/ && rm -rf build/ && mkdir -p build && cd build && cmake .. && make
	