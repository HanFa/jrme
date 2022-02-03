OS?=LINUX
#编译或者清除时是否连同处理插件
#build or clean plugins while make
PLUGIN?=n

#需要清除的临时文件
#The tempory files which need to be clear
TEMP_FILES=src/*.o lib/*.o

ifeq ($(OS),LINUX)
	TOOL_CHAIN?=gcc
else 
	TOOL_CHAIN?=x86_64-w64-mingw32-gcc
endif
					
ifeq ($(OS),LINUX)							#根据不同的平台，选择不同的链接库 
	CFLAGS:= -I./src -I./include -I./lib -I./plugin/timeparser/include -L./lib -lc -lstdc++ -ldl -static
else
	CFLAGS:= -I./src -I./include -I./lib -L./lib -lstdc++ -ldl  -static -DWINDOWS
endif

#传参决定是否需要调试，如果DEBUG=exclusive，则调试的时候会删除release版本
#debug or not. if DEBUG=exclusive, release version will be delate after build
ifdef DEBUG
	CFLAGS+=-g
endif	

#寻找所有.cpp 和.c文件的路径
#find all path of .cpp and .c
SRC_CPP:=$(wildcard  ./src/*.cpp ./lib/*.cpp)
SRC_C:=$(wildcard  ./src/*.c ./lib/*.c)
#指定中间文件存放位置
#choose a directory to save .o
BUILD_DIR:=$(shell pwd)/build/
#指定jrme的输出
#choose a directory to build out jrme
OUT_DIR:=$(shell pwd)/bin/
#指定可执行文件位置与名字
ifeq ($(OS),LINUX)
	OUT_FILE?=jrme.elf
else
	OUT_FILE?=jrme.exe
endif
OUT_TARGET:=$(OUT_DIR)$(OUT_FILE)
#调试版本的文件名
#the file name of debug version
DOUT_FILE?=jrme_debug.elf
DOUT_TARGET:=$(OUT_DIR)$(DOUT_FILE)

#所有的.cpp和.c文件的文件名
#the file name of all .cpp and .c
CPP_FILE:=$(notdir $(SRC_CPP))
C_FILE:=$(notdir $(SRC_C))
#把.cpp和c 换成.o
#replace .cpp or .c with .o
OBJ_SRC_CPP:=$(patsubst %.cpp, %.o, $(SRC_CPP))
OBJ_SRC_C:=$(patsubst %.c, %.o, $(SRC_C))


ifeq ($(PLUGIN),y)
	MODULES+=plugin
endif

#------------------------------remove files-------------------------------#
CLEAN_TARGET:= $(OUT_DIR)* $(TEMP_FILES)
ifeq ($(PLUGIN),y)
	CLEAN_TARGET+=$(JRME_PLUGIN)*
endif

#----------------------------------build----------------------------------#
#如果定义互斥(DEBUG=exclusive)，则调试的时候会删除发行版
#if DEBUG=exclusive, release version will be delate after build
ifeq ($(DEBUG),exclusive)
#编译调试版本
#bild debug version
all:build_prepare $(MODULES) $(OBJ_SRC_CPP) $(OBJ_SRC_C)
	$(TOOL_CHAIN) $(OBJ) -o $(DOUT_TARGET) $(CFLAGS)
	rm -f $(OUT_TARGET)

else
#编译release版本
#build
all:$(MODULES) $(OBJ_SRC_CPP) $(OBJ_SRC_C)
	$(TOOL_CHAIN) $< -o $(OUT_TARGET) $(CFLAGS)
endif

#将.cpp和.c编译为.o
#compile .cpp and .c to .o
$(OBJ_SRC_CPP) : %.o:%.cpp
	$(TOOL_CHAIN) $< -o $@ -c  $(CFLAGS)

$(OBJ_SRC_C) : %.o:%.c
	$(TOOL_CHAIN) $< -o $@ -c  $(CFLAGS)

#编译测试用例
#compile test code
.PHONY: test
test: $(MODULES)
	$(MAKE) -C ./test UPPER_BUILD_DIR=$(BUILD_DIR) TOOL_CHAIN=$(TOOL_CHAIN) OS=$(OS) OUT_DIR=$(OUT_DIR)

.PHONY: plugin
plugin:
	$(MAKE) -C ./plugin UPPER_BUILD_DIR=$(BUILD_DIR) TOOL_CHAIN=$(TOOL_CHAIN) OS=$(OS)

.PHONY: build_prepare
build_prepare:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OUT_DIR)

#清除可执行文件
#clean files
.PHONY: clean
clean:
	rm -rf $(CLEAN_TARGET)