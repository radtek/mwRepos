#!/bin/bash
#make_* .sh
#author : Shijh
#create by 2017/08/14

#跳转至当前Shell路径
cd $(dirname $0)

#输入参数判断*************************************************
if [ $# -ne 3 ]
then
  echo "error : Param count != 3"
  exit 1
fi
if [ ! -f "makes_dir/make_$1.sh" ]
then
  echo "error : make_$1.sh not found!"
  exit 1
fi

#加载其它shell脚本********************************************
#根目录目录
ROOT_PATH=../..
source makes_dir/make_$1.sh

#编译配置项**************************************************
#gcc 版本
CC="g++"
#包含的头文件
INCLUDE_BASE="$INCLUDE -I$ROOT_PATH/mwinclude/ -I$ROOT_PATH/tzwinclude/ -I$ROOT_PATH/mwinclude/utility -I/usr/include/mysql"
#需要依赖的头文件路径
INCLUDE_DEPEND=$INCLUDE_BASE
#依赖库文件
LIBS="$LIBFILE"
#编译参数
CFLAGS=
CFLAGS_TYPE=
CFLAGS_DLL="-fPIC -shared"
#debug模式配置
DEBUG_CONFIG="-w -m64 -ggdb -D_DEBUG -g -Wall"
#release模式配置
RELEASE_CONFIG="-w -D_RELEASE"

#生成EXE的配置
CFLAGS_EXE=""

CFLAGS_COMM="-std=c++11 $INCLUDE_BASE $LIBS -D__MYSQL__ -DZIP_STD -Dlinux -DLINUX -DIOCP_CCS -D_X64_ -lstdc++ -Wl,-rpath,-lrt,-ldl,-lz,-rpath,./"
#输出的基础目录
OUTPUT_BASE_PATH="$ROOT_PATH/output/linux/$3"
#过程文件目录
TMP_PATH="$ROOT_PATH/output/linux/tmp/$1"
#生成文件目录及名称
TARGET_PATH="$OUTPUT_BASE_PATH/$TARGET_NAME"
#过程文件后缀名
OBJECT_FILE_NAME=

#编译线使用的程数
COMPILE_THREAD=8

function CreateDirectory()
{
  if [ ! -d "$OUTPUT_BASE_PATH" ]; then
     mkdir -p "$ROOT_PATH/output/linux"
     mkdir -p "$ROOT_PATH/output/linux/tmp"
     cp -r "./basefile_dir" "$OUTPUT_BASE_PATH"
  fi
  mkdir -p "$TMP_PATH"
}

#函数定义**************************************************
#获取源文件
function GetSourceFiles()
{
  #查找所有cpp文件名
  CPP_FILES=$(find $DIRS -name *.cpp)
echo "CPP_FILES=$CPP_FILES"
  for element in $IGNORE_FILES
  do
    CPP_FILES=$(echo $CPP_FILES | sed "s/${element//\//\\/}//g")
  done
}

#生成makefile文件
function OutMakefile()
{
  #查找所有cpp文件名
  GetSourceFiles;
  #获取所有obj文件名
  for element in $CPP_FILES
  do
    OBJ_FILES+="$TMP_PATH/$(basename $element).$OBJECT_FILE_NAME "
  done
  #写Makefile文件
  MAKE_FILE="$TMP_PATH/make"
  echo "#Makefile" > $MAKE_FILE
  echo " " >> $MAKE_FILE
  echo "all: $TARGET_PATH" >> $MAKE_FILE
  echo "$TARGET_PATH: $OBJ_FILES" >> $MAKE_FILE
  echo "	@echo create $TARGET_PATH ">> $MAKE_FILE
  echo "	@$CC -o $TARGET_PATH $OBJ_FILES -L$OUTPUT_BASE_PATH/bin $CFLAGS_TYPE $CFLAGS_COMM $DEBUG_CONFIG" >> $MAKE_FILE

  for element in $CPP_FILES
  do
  {
    DEPEND_FILE=$TMP_PATH/$(basename $element).d
    if [ ! -f "$DEPEND_FILE" ]; then
#写依赖文件
       DEPEND_STRING=$($CC -MM -std=c++11 $INCLUDE_DEPEND $element)
       echo "$TMP_PATH/$(basename $element).o: ${DEPEND_STRING##*:} " > $DEPEND_FILE
       echo "	@echo compiling $element ..." >> $DEPEND_FILE
       echo "	@$CC -o $TMP_PATH/$(basename $element).o -c -L$OUTPUT_BASE_PATH/bin $CFLAGS_TYPE $CFLAGS_COMM $RELEASE_CONFIG $element " >> $DEPEND_FILE
       
       echo "$TMP_PATH/$(basename $element).od: ${DEPEND_STRING##*:} " >> $DEPEND_FILE
       echo "	@echo compiling $element ..." >> $DEPEND_FILE
       echo "	@$CC -o $TMP_PATH/$(basename $element).od -c -L$OUTPUT_BASE_PATH/bin $CFLAGS_TYPE $CFLAGS_COMM $DEBUG_CONFIG $element " >> $DEPEND_FILE
    fi
#在Makefile中包含依赖文件
    echo "sinclude $DEPEND_FILE" >> $MAKE_FILE
  } &
  done

  wait
}

#脚本运行**************************************************
echo Making $1 by  $2 ---------

#生成类型判断
if [ "$TARGET_TYPE" == "EXE" ]; then
  CFLAGS_TYPE=$CFLAGS_EXE
elif [ "$TARGET_TYPE" == "DLL" ]; then
  CFLAGS_TYPE=$CFLAGS_DLL
else
  echo "TARGET_TYPE error!"
  exit 1
fi

#生成并执行makefile
if [ "$2" == "debug" ]; then
   CFLAGS=$DEBUG_CONFIG
   OBJECT_FILE_NAME="od"
   CreateDirectory;
   export LD_LIBRARY_PATH="$OUTPUT_BASE_PATH/bin"
   export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_DEFAULT"
   OutMakefile;
   if [ ! -f "$TMP_PATH/lastfile" ]; then
      rm -f $OUTPUT_BASE_PATH/$TARGET_NAME
      echo "$3.debug" > "$TMP_PATH/lastfile"
   else
      LAST_TYPE=$(cat $TMP_PATH/lastfile)
      if [ "$LAST_TYPE" != "$3.debug" ]; then
          rm -f $OUTPUT_BASE_PATH/$TARGET_NAME
          echo "$3.debug" > "$TMP_PATH/lastfile"
      fi
   fi
   make all -j $COMPILE_THREAD -f $MAKE_FILE && EndMake;

elif [ "$2" == "release" ]; then
   CFLAGS=$RELEASE_CONFIG
   OBJECT_FILE_NAME="o"
   CreateDirectory;
   export LD_LIBRARY_PATH="$OUTPUT_BASE_PATH/bin"
   export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_DEFAULT"
   OutMakefile;
   
   if [ ! -f "$TMP_PATH/lastfile" ]; then
      rm -f $OUTPUT_BASE_PATH/$TARGET_NAME
      echo "$3.release" > "$TMP_PATH/lastfile"
   else
      LAST_TYPE=$(cat $TMP_PATH/lastfile)
      if [ "$LAST_TYPE" != "$3.release" ]; then
          rm -f $OUTPUT_BASE_PATH/$TARGET_NAME
          echo "$3.release" > "$TMP_PATH/lastfile"
      fi
   fi

   make all -f $MAKE_FILE && EndMake;

elif [ "$2" == "clean" ]; then
   rm -f $TMP_PATH/* $OUTPUT_BASE_PATH/$TARGET_NAME
   
elif [ "$2" == "sourcelist" ]; then
   GetSourceFiles;
   for element in $CPP_FILES
   do
      echo $element;
   done
elif [ "$2" == "test" ]; then
   echo test
else
  echo "error : param2 not support!"
  exit 1
fi
#保存make返回值
RET=$?
#返回make的返回值
exit $RET
