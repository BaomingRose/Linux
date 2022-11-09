include=$(wildcard ./include/*.h)
src=$(wildcard ./src/*.cpp)		#定义一个普通变量，获取当前目录以.c结尾的文件
obj=$(patsubst %.cpp, %.o, $(src))	# 将src的内存进行字符串替换，把.c换成.o

shareLib=./out/libcalc.so
targetPath=../UseDynamicSo
.PHONY: $(shareLib)

$(shareLib): $(obj)
	g++ $^ -shared -o $@
	rm $(obj) -f
	cp $@ $(targetPath)/lib
	cp $(include) $(targetPath)/include
	sudo rm /lib64/$(notdir $@) -f
	sudo ln $@ /lib64/$(notdir $@)	# 将so copy到项目路径，但是执行时链接不到，这是一种copy到根目录的方法，保证执行文件可以链接到
$(obj): $(src)
	g++ -c $^ -o $@ -fPIC

