add_executable(TuweTest test.cpp)

target_compile_features(TuweTest PRIVATE cxx_std_17)

add_test(NAME MainTest COMMAND $<TARGET_FILE:TuweTest>)