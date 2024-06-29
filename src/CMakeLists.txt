set(HEADER_LIST "")

add_executable(TuweApp ${HEADER_LIST}
    main.cpp
    tuwe/client/client.cpp
    tuwe/client/models.cpp
    tuwe/client/weather.cpp
    tuwe/client/components.cpp
    tuwe/client/views.cpp)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(TuweApp PRIVATE fmt::fmt)

find_package(ftxui CONFIG REQUIRED)
target_link_libraries(TuweApp PRIVATE ftxui::dom ftxui::screen ftxui::component)

find_package(Boost REQUIRED COMPONENTS thread beast coroutine2 url json optional)
target_link_libraries(TuweApp PRIVATE Boost::thread Boost::beast Boost::coroutine2 Boost::url Boost::json Boost::optional)

find_package(OpenSSL CONFIG REQUIRED)
target_link_libraries(TuweApp PRIVATE OpenSSL::SSL)

target_compile_features(TuweApp PRIVATE cxx_std_17)
target_include_directories(TuweApp PUBLIC ../include)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
