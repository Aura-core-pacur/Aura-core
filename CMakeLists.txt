cmake_minimum_required(VERSION 3.16)

project(AuraCore LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)

if(WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS")
    set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /DWIN32")
endif()

find_package(JNI REQUIRED)
find_package(PkgConfig REQUIRED)

find_program(KOTLIN_COMPILER kotlinc)
if(KOTLIN_COMPILER)
    message(STATUS "Kotlin compiler found: ${KOTLIN_COMPILER}")
    file(GLOB KOTLIN_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/kotlin/**/*.kt")
    if(KOTLIN_SOURCES)
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/aura_kotlin.jar
            COMMAND ${KOTLIN_COMPILER} -d ${CMAKE_CURRENT_BINARY_DIR}/aura_kotlin.jar ${KOTLIN_SOURCES}
            DEPENDS ${KOTLIN_SOURCES}
            COMMENT "Compiling Kotlin bridge for Aura Core"
            VERBATIM
        )
        add_custom_target(aura_kotlin_bridge DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/aura_kotlin.jar)
    endif()
endif()

if(APPLE)
    enable_language(Swift)
    file(GLOB SWIFT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/swift/*.swift")
    if(SWIFT_SOURCES)
        add_library(aura_swift_bridge STATIC ${SWIFT_SOURCES})
        target_include_directories(aura_swift_bridge PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
        target_compile_options(aura_swift_bridge PRIVATE -Xfrontend -enable-experimental-cxx-interop)
    endif()
endif()

pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavcodec
    libavformat
    libavutil
    libswscale
)

set(RUST_LIB ${CMAKE_CURRENT_SOURCE_DIR}/src/core/target/release/libaura_rust.a)
set(GO_LIB ${CMAKE_CURRENT_SOURCE_DIR}/src/net/libaura_net.a)

add_custom_command(
    OUTPUT ${RUST_LIB}
    COMMAND bash -lc "export PATH=$ENV{HOME}/.cargo/bin:$PATH && cd ${CMAKE_CURRENT_SOURCE_DIR}/src/core && cargo build --release"
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Cargo.toml
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/src/lib.rs
    COMMENT "Building Rust staticlib for Aura Core"
    VERBATIM
)

add_custom_command(
    OUTPUT ${GO_LIB}
    COMMAND bash -lc "cd ${CMAKE_CURRENT_SOURCE_DIR}/src/net && go build -buildmode=c-archive -o libaura_net.a"
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/net/net_engine.go
    COMMENT "Building Go c-archive for Aura Core"
    VERBATIM
)

add_custom_target(aura_rust_static DEPENDS ${RUST_LIB})
add_custom_target(aura_net_static DEPENDS ${GO_LIB})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys
    ${CMAKE_CURRENT_SOURCE_DIR}/src/media
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gfx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${JNI_INCLUDE_DIRS}
)

add_library(aura_core_native SHARED
    src/sys/aura_core_api.cpp
    src/sys/jni_bindings.cpp
    src/sys/os_paths.cpp
    src/media/media_decoder.cpp
    src/gfx/display_list.cpp
    src/ui/event_router.cpp
    src/ui/xml_parser.cpp
)

add_dependencies(aura_core_native aura_rust_static aura_net_static)

target_link_libraries(aura_core_native
    PRIVATE
        ${RUST_LIB}
        ${GO_LIB}
        PkgConfig::FFMPEG
)

target_include_directories(aura_core_native
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sys
        ${CMAKE_CURRENT_SOURCE_DIR}/src/media
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gfx
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
        ${JNI_INCLUDE_DIRS}
)

set_target_properties(aura_core_native PROPERTIES
    OUTPUT_NAME "aura_core_native"
    PREFIX "lib"
    RUNTIME_OUTPUT_NAME "aura_core_native"
)

if(WIN32)
    set(WIN32_RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/sys/win/aura_core.rc")
    if(EXISTS "${WIN32_RESOURCE_FILE}")
        target_sources(aura_core_native PRIVATE "${WIN32_RESOURCE_FILE}")
    endif()
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/ico/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/ico)

message(STATUS "JNI include dirs: ${JNI_INCLUDE_DIRS}")
message(STATUS "FFmpeg include dirs: ${FFMPEG_INCLUDE_DIRS}")
message(STATUS "Aura Core native library configured.")
