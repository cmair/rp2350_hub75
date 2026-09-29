# Compile tests: instantiate Hub75Driver<Config> for a matrix of configurations.
#
# This is a compile-time test, not a runtime one. Each generated translation unit
# does nothing but instantiate the driver, which is what fires the class-scope
# static_asserts in Hub75DriverBase. Nothing is called, and the object library
# links nothing - so it cannot change what the demos build into.
#
# Gated by HUB75_BUILD_COMPILE_TESTS, declared in the parent CMakeLists.txt.
# Turning it off drops the Python 3 dependency this directory introduces; the
# rest of the project needs no Python.

if(HUB75_BUILD_COMPILE_TESTS)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)

    set(CT_DIR ${CMAKE_CURRENT_BINARY_DIR}/compile_tests)

    # Clear first: a configuration removed from the generator must not linger in
    # the build directory and keep being compiled.
    file(REMOVE_RECURSE ${CT_DIR})

    execute_process(
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/gen_compile_tests.py --outdir ${CT_DIR}
        RESULT_VARIABLE CT_GEN_RESULT
    )
    if(NOT CT_GEN_RESULT EQUAL 0)
        message(FATAL_ERROR "gen_compile_tests.py failed to generate the compile tests")
    endif()

    file(GLOB CT_SOURCES ${CT_DIR}/*.cpp)
    list(SORT CT_SOURCES)
    if(NOT CT_SOURCES)
        message(FATAL_ERROR "gen_compile_tests.py produced no translation units")
    endif()

    # One TU per configuration: a failing static_assert then names that
    # configuration in the compiler's instantiation trail, and a single bad
    # configuration cannot hide the rest behind one shared unit.
    #
    # Linking hub75 is deliberate: it gives the test TUs the same include paths,
    # compile definitions and generated PIO header the demos get, so a config
    # that compiles here compiles for real. The cost is that CMake also compiles
    # the linked target's sources into this object library - three extra TUs via
    # pico_graphics, which is cheap and doubles as a compile check of them.
    add_library(hub75_compile_tests OBJECT ${CT_SOURCES})
    target_link_libraries(hub75_compile_tests PRIVATE hub75)

    list(LENGTH CT_SOURCES CT_COUNT)
    message(STATUS "hub75: ${CT_COUNT} configuration compile tests")
endif()
