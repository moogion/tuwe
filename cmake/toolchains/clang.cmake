cmake_minimum_required(VERSION 3.9)

# check if the user has set the LLVM_DIR environment variable
if(DEFINED ENV{LLVM_DIR})
    list(APPEND CMAKE_PROGRAM_PATH "$ENV{LLVM_DIR}")
endif()

# check if the user has set the LLVMInstallDir environment variable
if(DEFINED ENV{LLVMInstallDir})
    list(APPEND CMAKE_PROGRAM_PATH "$ENV{LLVMInstallDir}")
endif()

# find clang and clang++ executables
find_program(CLANG_EXECUTABLE "clang" PATHS "${CMAKE_PROGRAM_PATH}" ENV PATH PATH_SUFFIXES "bin")
find_program(CLANGXX_EXECUTABLE "clang++" PATHS "${CMAKE_PROGRAM_PATH}" ENV PATH PATH_SUFFIXES "bin")

# set C and ASM compilers
if(NOT CLANG_EXECUTABLE)
    set(CMAKE_C_COMPILER "clang" CACHE STRING "C Compiler")
    set(CMAKE_ASM_COMPILER "clang" CACHE STRING "ASM Compiler")
else()
    set(CMAKE_C_COMPILER "${CLANG_EXECUTABLE}" CACHE FILEPATH "C Compiler")
    set(CMAKE_ASM_COMPILER "${CLANG_EXECUTABLE}" CACHE FILEPATH "ASM Compiler")
endif()

# set C++ compiler
if(NOT CLANGXX_EXECUTABLE)
    set(CMAKE_CXX_COMPILER "clang++" CACHE STRING "C++ Compiler")
else()
    set(CMAKE_CXX_COMPILER "${CLANGXX_EXECUTABLE}" CACHE FILEPATH "C++ Compiler")
endif()

if(NOT DEFINED CMAKE_SYSTEM_NAME)
    set(CMAKE_SYSTEM_NAME "${CMAKE_HOST_SYSTEM_NAME}" CACHE STRING "")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "MinGW")
    set(CMAKE_SYSTEM_NAME Windows CACHE STRING "" FORCE)
    add_definitions(-municode)
endif()
string(TOLOWER "${CMAKE_SYSTEM_NAME}" CMAKE_SYSTEM_NAME)

if(NOT DEFINED CMAKE_SYSTEM_PROCESSOR)
    set(CMAKE_SYSTEM_PROCESSOR "${VCPKG_TARGET_ARCHITECTURE}" CACHE STRING "")
endif()

if(VCPKG_TARGET_ARCHITECTURE STREQUAL "x86")
    set(CMAKE_SYSTEM_PROCESSOR i686 CACHE STRING "" FORCE)
elseif(VCPKG_TARGET_ARCHITECTURE STREQUAL "x64")
    set(CMAKE_SYSTEM_PROCESSOR x86_64 CACHE STRING "" FORCE)
elseif(VCPKG_TARGET_ARCHITECTURE STREQUAL "arm")
    set(CMAKE_SYSTEM_PROCESSOR armv7 CACHE STRING "" FORCE)
elseif(VCPKG_TARGET_ARCHITECTURE STREQUAL "arm64")
    set(CMAKE_SYSTEM_PROCESSOR aarch64 CACHE STRING "" FORCE)
endif()

# set clang target triple for cross-compilation
if(NOT DEFINED CLANG_TARGET_TRIPLE)
    set(CLANG_TARGET_TRIPLE "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_SYSTEM_NAME}-gnu")
endif()

# set the target triple for the compilers
if(DEFINED CLANG_TARGET_TRIPLE)
    foreach(LANG C;ASM;CXX)
        set("CMAKE_${LANG}_COMPILER_TARGET" "${CLANG_TARGET_TRIPLE}")
    endforeach()
endif()

# Don't run the linker on compiler check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# For libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)