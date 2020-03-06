#a:a.c b.c
#		gcc $^ -o $@
#
#
#
#

#a:a.c b.c
#	gcc $^ -o $@

src=$(wildcard ./*.c)	#定义一个普通变量 获取当前目录下以.c结尾的文件
obj=$(patsubst %.c, %.o, $(src))	#将src的内容进行字符串替换，把.c替换成.o

bin=./a
.PHONY: $(bin) 
$(bin):$(obj)
	gcc $^ -o $@
%.o:%.c
	gcc -c $< -o $@

clean:
	rm -rf $(obj) $(bin)


#这个代码可以拿到任何目录下使用
