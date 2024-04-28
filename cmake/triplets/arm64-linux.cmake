set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_CMAKE_SYSTEM_NAME Linux)

set(CLANG_TARGET_TRIPLE "aarch64-linux-gnu")

include("${CMAKE_CURRENT_LIST_DIR}/common.cmake")
