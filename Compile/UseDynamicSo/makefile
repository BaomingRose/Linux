src=$(wildcard ./src/*.cpp)

exePath=./out/main
libPath=./lib
linkLib=$(notdir $(wildcard ./lib/*.so))	#待完成，未实现
.PHONY: $(exePath)

$(exePath): $(src)
	#g++ $^ -o $@ -L $(libPath) -lcalc -D USE_DYNAMICLIB
	g++ $^ -o $@ -ldl -D USE_DLOPEN # -D动态添加宏
