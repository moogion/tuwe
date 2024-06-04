set(HEADER_LIST "${Tuwe_SOURCE_DIR}/include/main.hpp")

add_executable(TuweApp ${HEADER_LIST}
    main.cpp
    tuwe/client/client.cpp
    tuwe/client/views.cpp)

find_package(ftxui CONFIG REQUIRED)
target_link_libraries(TuweApp PRIVATE ftxui::dom ftxui::screen ftxui::component)

target_compile_features(TuweApp PRIVATE cxx_std_17)
target_include_directories(TuweApp PUBLIC ../include)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
