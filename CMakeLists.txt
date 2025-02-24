cmake_minimum_required(VERSION 3.10)

project(SplitGui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -g")

file(GLOB_RECURSE TESTS test/*/main.cpp)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/shaders)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
endif()

# ---- fetch dependencies ----

function(copy_files, src, dst)
    if(NOT EXISTS ${src})
        file(MAKE_DIRECTORY ${src})
    endif()

    file(COPY ${src}/* DESTINATION ${dst} FOLLOW_SYMLINKS)
endfunction()

set(DEPENDENCIES
    "https://github.com/freetype/freetype.git"
    "https://github.com/glfw/glfw.git"
    "https://github.com/KhronosGroup/Vulkan-Hpp.git"
    "https://github.com/KhronosGroup/Vulkan-Headers.git"
    "https://github.com/lukem570/glfwpp.git"
    "https://github.com/zeux/volk.git"
    "https://github.com/Chlumsky/msdfgen.git"
)

set(DEPENDENCIES_DO_BUILD
    "TRUE"                          # freetype
    "TRUE"                          # glfw
    "FALSE"                         # vulkan hpp
    "FALSE"                         # vulkan headers
    "FALSE"                         # glfwpp
    "TRUE"                          # volk
    "TRUE"                          # msdfgen
)

set(DEPENDENCIES_INCLUDE
    "include"                       # freetype
    "include"                       # glfw
    ""                              # vulkan hpp
    "include"                       # vulkan headers
    "include"                       # glfwpp
    "../"                           # volk
    "../"                           # msdfgen
)

set(INCLUDE_PATHS ${CMAKE_SOURCE_DIR}/include)

set(index 0)

foreach(git_repo_link ${DEPENDENCIES})
    string(REGEX MATCH "([^/]+)\\.git" repo_name_match ${git_repo_link})
    string(REGEX REPLACE ".git" "" repo_name ${repo_name_match})

    set(repo_path ${CMAKE_BINARY_DIR}/${repo_name})

    list(GET DEPENDENCIES_DO_BUILD ${index} DO_BUILD)
    list(GET DEPENDENCIES_INCLUDE ${index} REPO_INCLUDE_PATH)

    set(INCLUDE_PATHS ${INCLUDE_PATHS} ${repo_path}/${REPO_INCLUDE_PATH})

    if(EXISTS ${repo_path})
        math(EXPR index "${index} + 1")
        continue()
    endif()


    file(MAKE_DIRECTORY ${repo_path})

    execute_process(
        COMMAND git clone ${git_repo_link} ${repo_path} --recursive
        RESULT_VARIABLE CLONE_RESULT
        ERROR_VARIABLE CLONE_ERROR
        OUTPUT_QUIET
    )
    
    if(NOT CLONE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to clone repository: ${repo_name}. Error: ${CLONE_ERROR}")
    else()
        message(STATUS "Successfully cloned: ${repo_name}")
    endif()

    if(${DO_BUILD} STREQUAL "FALSE")
        math(EXPR index "${index} + 1")
        continue()
    endif()

    # I am aware that this is cursed but I can't get it to work any other way
    execute_process(
        COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} -B build 
        -DCMAKE_CXX_FLAGS=-fPIC 
        -DCMAKE_C_FLAGS=-fPIC 
        -DFT_DISABLE_ZLIB=TRUE 
        -DFT_DISABLE_BZIP2=TRUE 
        -DFT_DISABLE_PNG=TRUE 
        -DFT_DISABLE_HARFBUZZ=TRUE 
        -DFT_DISABLE_BROTLI=TRUE 
        -DMSDFGEN_CORE_ONLY=ON
        WORKING_DIRECTORY ${repo_path}
        RESULT_VARIABLE CONFIGURE_RESULT
        ERROR_VARIABLE CONFIGURE_ERROR
        OUTPUT_QUIET
    )

    if(NOT CONFIGURE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to configure repository: ${repo_name}. Error: ${CONFIGURE_ERROR}")
    else()
        message(STATUS "Successfully configured: ${repo_name}")
    endif()
    
    string(REGEX REPLACE ".*/" "" make_program ${CMAKE_MAKE_PROGRAM})

    execute_process(
        COMMAND ${make_program}
        WORKING_DIRECTORY ${repo_path}/build
        RESULT_VARIABLE BUILD_RESULT
        ERROR_VARIABLE BUILD_ERROR
        OUTPUT_QUIET
    )

    if(NOT BUILD_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to build repository: ${repo_name}. Error: ${BUILD_ERROR}")
    else()
        message(STATUS "Successfully built: ${repo_name}")
    endif()

    math(EXPR index "${index} + 1")

endforeach()

# ---- compile shaders ----

set(SHADERS
    "${CMAKE_SOURCE_DIR}/src/shaders/fragment.frag"
    "${CMAKE_SOURCE_DIR}/src/shaders/vertex.vert"
)

foreach(SHADER_SOURCE ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME_WE)
    set(SHADER_OUTPUT "${CMAKE_BINARY_DIR}/tests/shaders/${SHADER_NAME}.spv")

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

# ---- provide window manager to vulkan ----

if(NOT DEFINED WM_MANUAL)
    if(WIN32)
        set(VK_WINDOW_MANAGER VK_USE_PLATFORM_WIN32_KHR)
        set(WINDOW_LIB kernel32)
    elseif(APPLE)
        message(FATAL_ERROR "macOS is not supported")
    elseif(UNIX)

        if("$ENV{XDG_SESSION_TYPE}" STREQUAL "x11")

            set(VK_WINDOW_MANAGER VK_USE_PLATFORM_XLIB_KHR)
            set(WINDOW_LIB X11)

        elseif("$ENV{XDG_SESSION_TYPE}" STREQUAL "wayland")

            set(VK_WINDOW_MANAGER VK_USE_PLATFORM_WAYLAND_KHR)
            set(WINDOW_LIB wayland-client wayland-egl wayland-cursor)

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

message(STATUS "window manager: ${VK_WINDOW_MANAGER}")

# ---- compile splitgui ----

include_directories(${INCLUDE_PATHS})

link_directories(
    ${CMAKE_BINARY_DIR}/glfw/build/src
    ${CMAKE_BINARY_DIR}/volk/build
    ${CMAKE_BINARY_DIR}/msdfgen/build
    ${CMAKE_BINARY_DIR}/freetype/build
)

add_library(splitgui SHARED ${CMAKE_SOURCE_DIR}/src/splitgui.cpp)

target_link_libraries(splitgui
    glfw3
    volk
    msdfgen-core
    freetype
    ${WINDOW_LIB}
)

target_compile_definitions(splitgui PRIVATE BUILD_SPLITGUI ${SPLITGUI_DEV} ${VK_WINDOW_MANAGER})

# ---- compile tests ----

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