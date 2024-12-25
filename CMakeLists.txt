cmake_minimum_required(VERSION 3.10)

project(SplitGui)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE TESTS test/*/main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(splitgui SHARED src/splitgui.cpp)

target_compile_definitions(splitgui PRIVATE BUILD_SPLITGUI)

foreach(main_file ${TESTS})

    get_filename_component(main_dir ${main_file} DIRECTORY)
    get_filename_component(main_name ${main_dir} NAME_WE)

    
    add_executable(${main_name} ${main_file})

    target_link_libraries(${main_name} splitgui)

    set_target_properties(splitgui PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )

    set_target_properties(${main_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )

    add_custom_command(TARGET ${main_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:splitgui>
            $<TARGET_FILE_DIR:${main_name}> 
    )

endforeach()

