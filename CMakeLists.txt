cmake_minimum_required(VERSION 3.27)
project(xllmu VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

# Find LLDB framework
execute_process(COMMAND xcode-select -p
    OUTPUT_VARIABLE CLI_TOOLCHAIN_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE XCODE_SELECT_RESULT
)
if (NOT XCODE_SELECT_RESULT EQUAL 0)
    message(FATAL_ERROR "xcode-select failed with ${XCODE_SELECT_RESULT}")
endif()
set(LLDB_FRAMEWORK_PATH ${CLI_TOOLCHAIN_PATH}/Library/PrivateFrameworks/LLDB.framework)

add_executable(xllmu src/main.cc)

target_include_directories(xllmu PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(xllmu PRIVATE ${LLDB_FRAMEWORK_PATH}/LLDB)
