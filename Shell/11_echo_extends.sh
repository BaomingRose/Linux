#!/bin/bash

# 花括号的扩展，花括号中的三个内容，会分别做为花括号的位置打印三次
echo Front-{To,From,And}-Back

# 花括号中的1..20会从1-20每个数字作为一部分依次打印
echo Number{1..20}
# 各位数都会带0
echo Number{01..20}

# 会创建很多目录
# mkdir {2018..2020}-0{1..9}{2018..2020}-{10..12}

# 打印算术表达式结果必须两层括号，$((expression))，**表示幂运算
echo $(((5**2)*3))    # 75
echo $((5/2))         # 2

# 打印变量 $variable或${variable}
echo $PATH
echo ${USER}

# 打印命令的输出，$(command)或$`command`
echo $(ls)
echo `pwd`
file `ls /bin/ | grep zip`
