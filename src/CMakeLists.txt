add_compile_options(-municode)

set(HEADER_LIST "${Tuwe_SOURCE_DIR}/include/main.hpp")

add_executable(TuweApp main.cpp ${HEADER_LIST})
target_compile_features(TuweApp PRIVATE cxx_std_17)

target_include_directories(TuweApp PUBLIC ../include)


source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
