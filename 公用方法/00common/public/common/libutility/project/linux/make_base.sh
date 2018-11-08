#!/bin/bash
#make_* .sh
#author : Shijh
#create by 2017/08/14

#生成工程文件名设置
BASE_FILE=$(basename $0 .sh)
echo Start Make $BASE_FILE

#设置主运行程序名称
export EXE_NAME=mw_$BASE_FILE

#编译操作
for element in $BASE_COMPILE
do
	./make.sh $element $1 $BASE_FILE
     [ $? != 0 ] && exit 0
done

for element in $USER_COMPILE
do
     ./make.sh $element $1 $BASE_FILE
     [ $? != 0 ] && exit 0
done

#生成发布包
bash make_pack.sh $2

echo Make all over