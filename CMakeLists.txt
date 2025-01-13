cmake_minimum_required(VERSION 3.10)

project(SplitGui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

file(GLOB_RECURSE TESTS test/*/main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SHADERS
    "${CMAKE_SOURCE_DIR}/src/shaders/fragment.frag"
    "${CMAKE_SOURCE_DIR}/src/shaders/vertex.vert"
    "${CMAKE_SOURCE_DIR}/src/shaders/geometry.geom"
)

foreach(SHADER_SOURCE ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME_WE)
    set(SHADER_OUTPUT "${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME}.spv")

    add_custom_command(
        OUTPUT ${SHADER_OUTPUT}
        COMMAND glslc ${SHADER_SOURCE} -o ${SHADER_OUTPUT}
        DEPENDS ${SHADER_SOURCE}
        COMMENT "Compiling GLSL shader ${SHADER_SOURCE} to SPIR-V"
    )

    list(APPEND SPIR_V_FILES ${SHADER_OUTPUT})
endforeach()

add_custom_target(
    shaders ALL
    DEPENDS ${SPIR_V_FILES}
)

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

add_library(splitgui SHARED ${CMAKE_SOURCE_DIR}/src/splitgui.cpp)

target_link_libraries(splitgui PRIVATE 
    ${CMAKE_BINARY_DIR}/libglfw3.a 
    ${CMAKE_SOURCE_DIR}/include/volk/build/libvolk.a 
    ${CMAKE_SOURCE_DIR}/include/msdfgen/build/libmsdfgen-core.a
    ${CMAKE_SOURCE_DIR}/include/src/freetype/build/libfreetype.a
)

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

