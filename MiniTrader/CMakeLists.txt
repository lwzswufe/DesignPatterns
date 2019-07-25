cmake_minimum_required(VERSION 2.6)
# 指令定义工程名称
PROJECT(MiniTrader)

# SET指令的语法是  SET(VAR [VALUE] [CACHE TYPE DOCSTRING [FORCE]])
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(platform x64)

# 为多个源文件定义别名
set(SOURCE_FILES local_quote_convert.cpp My_Quote_Api_Test.cpp My_Quote_Api.cpp
    Quote_api.cpp Quote_generator.cpp Quote_spi.cpp read_local_quote.cpp)

# 为头个源文件定义别名
set(HEAD_FILES base_data_struct.h local_quote_struct.h local_quote_convert.h
    My_Quote_Api.h quote_api_struct.h Quote_api.h Quote_generator.h Quote_spi.h 
    read_local_quote.h)

# 定义了这个工程会生成一个文件名为main 的可执行文件，相关的源文件是SRC_LIST中定义的源文件列表
ADD_EXECUTABLE(MiniTrader ${HEAD_FILES} ${SOURCE_FILES})

SET_TARGET_PROPERTIES(MiniTrader PROPERTIES OUTPUT_NAME "MiniTrader")
GET_TARGET_PROPERTY (OUTPUT_VALUE MiniTrader OUTPUT_NAME)

# 这个指令用于向终端输出用户定义的信息，包含了三种类型:
# SEND_ERROR，产生错误，生成过程被跳过。
# SATUS，输出前缀为—的信息。
# FATAL_ERROR，立即终止所有cmake过程。
MESSAGE (STATUS "This is the MiniTrader OUTPUT_NAME: " ${OUTPUT_VALUE})