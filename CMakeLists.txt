cmake_minimum_required(VERSION 3.4)

#set(LLVM_ROOT "${CMAKE_SOURCE_DIR}/llvm_build/" CACHE PATH "Root of LLVM build.")

execute_process(
  COMMAND llvm-config-7 --src-root
  RESULT_VARIABLE HAD_ERROR
  OUTPUT_VARIABLE LLVM_ROOT
)

if (NOT HAD_ERROR)
  string(REGEX REPLACE "[ \t]*[\r\n]+[ \t]*" "" LLVM_ROOT ${LLVM_ROOT})
else()
  message(FATAL_ERROR "llvm-config-7 failed -- ${HAD_ERROR}")
endif()

  # A bit of a sanity  check:
if (NOT EXISTS ${LLVM_ROOT}/include/llvm)
  message(FATAL_ERROR "LLVM_ROOT (${LLVM_ROOT}) is invalid")
endif()

list(APPEND CMAKE_MODULE_PATH "${LLVM_ROOT}/lib/cmake/llvm")
list(APPEND CMAKE_PREFIX_PATH "${LLVM_ROOT}/lib/cmake/llvm")

find_package(LLVM REQUIRED CONFIG)

include(HandleLLVMOptions) # load  additional  config
include(AddLLVM) # used to add  our  own  modules

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_subdirectory(dumbo)  # Use your pass name here.
