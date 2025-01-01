cmake_minimum_required(VERSION 3.10)

project(SplitGui)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE TESTS test/*/main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

if(NOT DEFINED WM_MANUAL)

    if(WIN32)
        set(VK_WINDOW_MANAGER VK_USE_PLATFORM_WIN32_KHR)
    elseif(APPLE)
        message(FATAL_ERROR "macOS is not supported")
    elseif(UNIX)

        if("$ENV{XDG_SESSION_TYPE}" STREQUAL "x11")

            set(VK_WINDOW_MANAGER VK_USE_PLATFORM_XLIB_KHR)

        elseif("$ENV{XDG_SESSION_TYPE}" STREQUAL "wayland")

            set(VK_WINDOW_MANAGER VK_USE_PLATFORM_WAYLAND_KHR)

        else()
            message(FATAL_ERROR "Unsupported Linux windowing system detected")
        endif()
    else()
        message(FATAL_ERROR "Unsupported platform detected")
    endif()

endif()

if(NOT DEFINED VK_WINDOW_MANAGER)
    message(FATAL_ERROR "no window manager defined")
endif()

message("-- window manager: ${VK_WINDOW_MANAGER}")

add_library(splitgui SHARED src/splitgui.cpp)

target_link_libraries(splitgui PRIVATE ${CMAKE_BINARY_DIR}/libglfw3.a ${CMAKE_CURRENT_LIST_DIR}/include/volk/build/libvolk.a)

target_compile_definitions(splitgui PRIVATE BUILD_SPLITGUI ${VK_WINDOW_MANAGER})

foreach(main_file ${TESTS})

    get_filename_component(main_dir ${main_file} DIRECTORY)
    get_filename_component(main_name ${main_dir} NAME_WE)

    add_executable(${main_name} ${main_file})
    
    target_compile_definitions(${main_name} PRIVATE ${VK_WINDOW_MANAGER})

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

