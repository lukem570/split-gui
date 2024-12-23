cmake_minimum_required(VERSION 3.10)

project(SplitGui)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE TESTS test/*/main.cpp)

add_library(mylibrary SHARED src/splitgui.cpp)

target_compile_definitions(mylibrary PRIVATE BUILD_MYLIBRARY)

foreach(main_file ${TESTS})

    get_filename_component(main_dir ${main_file} DIRECTORY)
    get_filename_component(main_name ${main_dir} NAME_WE)

    
    add_executable(${main_name} ${main_file})

    target_link_libraries(${main_name} mylibrary)

    set_target_properties(mylibrary PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
    )

    set_target_properties(${main_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
    )

    add_custom_command(TARGET ${main_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:mylibrary>
            $<TARGET_FILE_DIR:${main_name}> 
    )

endforeach()

