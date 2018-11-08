#!/bin/bash
#make_* .sh
#author : Shijh
#create by 2017/08/14

#该工程路径,该变量只在这个文件中使用,方面设置一些工程子目录下的路径
PROJECT_PATH=

#需要编译的文件所在的文件夹
DIRS="$ROOT_PATH/utility/common \
      $ROOT_PATH/utility/json"

#生成文件的类型
TARGET_TYPE="DLL"

#生成路径与文件名
TARGET_NAME="bundles/libexport_cpp_utility64.so"

#依赖库设置
LIBFILE="-ldl -lrt -liconv"

#需要包含的头文件路径,需加 -I
INCLUDE=""

#需要忽略的编译源文件
IGNORE_FILES=""

#编译完成时执行的函数
function EndMake()
{
  return 0; 
}
