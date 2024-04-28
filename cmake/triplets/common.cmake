# pass through environment variables to the toolchain
set(VCPKG_ENV_PASSTHROUGH "LLVM_DIR;LLVMInstallDir")
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/../toolchains/clang.cmake")