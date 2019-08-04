cmake_minimum_required(VERSION 2.6)
# 指令定义工程名称
PROJECT(MiniTrader)

# SET指令的语法是  SET(VAR [VALUE] [CACHE TYPE DOCSTRING [FORCE]])
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(platform x64)

# 为多个源文件定义别名
set(SOURCE_FILES Read_ini.cpp My_Quote_Api_Test.cpp simulate_trade.cpp 
    My_Quote_Api.cpp Quote_Spi.cpp sim_trade_convert.cpp read_local_quote.cpp，
    My_Trade_Api.cpp Trade_Spi.cpp sim_quote_convert.cpp )

# 为多个头文件定义别名
set(HEAD_FILES Read_ini.h base_data_struct.h sim_quote_struct.h sim_quote_convert.h
    simulate_trade.h read_local_quote.h sim_trade_struct.h sim_trade_convert.h 
    My_Quote_Api.h Quote_Api_Struct.h Quote_Api.h Quote_Spi.h 
    My_Trade_Api.h Trade_Api_Struct.h Trade_Api.h Trade_Spi.h )

# 为多个库文件定义别名
set(LIB_FILES pthread)

# 设置可执行文件的输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})      

# 添加库路径
# link_directories(/usr/lib/x86_64-linux-gnu/)

# 定义了这个工程会生成一个文件名为MiniTrader 的可执行文件，相关的源文件是SRC_LIST中定义的源文件列表
ADD_EXECUTABLE(MiniTrader ${HEAD_FILES} ${SOURCE_FILES})

# 链接库文件
TARGET_LINK_LIBRARIES(MiniTrader ${LIB_FILES})

SET_TARGET_PROPERTIES(MiniTrader PROPERTIES OUTPUT_NAME "MiniTrader.o")
GET_TARGET_PROPERTY (OUTPUT_VALUE MiniTrader OUTPUT_NAME)

# 这个指令用于向终端输出用户定义的信息，包含了三种类型:
# SEND_ERROR，产生错误，生成过程被跳过。
# SATUS，输出前缀为—的信息。
# FATAL_ERROR，立即终止所有cmake过程。
MESSAGE (STATUS "OUTPUT_FILE: " ${EXECUTABLE_OUTPUT_PATH} "/" ${OUTPUT_VALUE})