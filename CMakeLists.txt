cmake_minimum_required(VERSION 3.10.2)
project(TheStdandardLib
    VERSION 1.0
    LANGUAGES C
)

option(TARGET_X86_64
    "Default target device is set to x86_64"
    "ON"
)

set(CMAKE_ANDROID_ARCH_ABI "unsupported_cpu_architecture")
if(TARGET_X86_64)
    set(CMAKE_ANDROID_ARCH_ABI "x86_64")
    set(ANDROID_ABI "x86_64") # FIXME: Don't know how keep compatable with Android NDK
    set(CMAKE_ASM_NASM_COMPILE_OPTIONS "${CMAKE_ASM_NASM_COMPILE_OPTIONS} -f elf64 -g")
endif()
if(CMAKE_ANDROID_ARCH_ABI STREQUAL "unsupported_cpu_architecture")
    message(WARNING "Currently, only X86_64 is fully supported. (FIXME: Other none-X86 target devices are going to be supported in the future)")
endif()

include(CheckLanguage)
check_language(ASM_NASM)
if(CMAKE_ASM_NASM_COMPILER)
    enable_language(ASM_NASM)
endif()
if (NOT CMAKE_ASM_NASM_COMPILER_LOADED)
    message(WARNING "CMAKE_ASM_NASM_COMPILER is not found...\n"
        "X86_64 related features will not be available!\n"
        "Suggest: sudo apt-get install yasm")
endif()

message(STATUS "CMAKE_ASM_NASM_COMPILER = " ${CMAKE_ASM_NASM_COMPILER})
message(STATUS "CMAKE_ASM_NASM_COMPILER_LOADED = " ${CMAKE_ASM_NASM_COMPILER_LOADED})
message(STATUS "CMAKE_ASM_NASM_COMPILE_OPTIONS = " ${CMAKE_ASM_NASM_COMPILE_OPTIONS})

set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS S)
message(STATUS "CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS = " ${CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS})
set(X86_64_ASM_NASM_SOURCES
    tsl/coro/x86_64_coro_swap.S
)
if(CMAKE_ANDROID_ARCH_ABI STREQUAL "x86_64")
    set(ASM_NASM_SOURCES ${X86_64_ASM_NASM_SOURCES})
elseif(CMAKE_ANDROID_ARCH_ABI STREQUAL "aarch64")
    message(FATAL_ERROR "${CMAKE_ANDROID_ARCH_ABI} will be supported later!")
elseif(CMAKE_ANDROID_ARCH_ABI STREQUAL "armv7l")
    message(FATAL_ERROR "${CMAKE_ANDROID_ARCH_ABI} will be supported later!")
elseif(CMAKE_ANDROID_ARCH_ABI STREQUAL "armv6l")
    message(FATAL_ERROR "Sorry, armv6l(RespberryPi Model-2) will not be supported...")
else()
    message(FATAL_ERROR "Unable to recognize target type: ${CMAKE_ANDROID_ARCH_ABI}")
endif()

set(C_SOURCES
    tsl/time.c
    tsl/diag.c
    tsl/worker_thread.c
    tsl/version_dump/version_dump.c
    tsl/printf_helpers.c
    tsl/assert.c
    tsl/hexdump.c
    tsl/threading.c
    tsl/cpumask.c
    tsl/rbtree.c
    tsl/parsers.c
    tsl/coro/coroutine.c
    tsl/coro/x86_64_coro_init.c
    tsl/frame_alloc.c
    tsl/errors.c
    tsl/safe_string.c
    tsl/version.c
    tsl/panic.c
    tsl/safe_alloc.c
    tsl/alloc/allocator.c
)

set(H_FILES
    tsl/work_queue.h
    tsl/bits.h
    tsl/rbtree.h
    tsl/alloc.h
    tsl/ticket_lock.h
    tsl/time_priv.h
    tsl/result.h
    tsl/refcnt.h
    tsl/threading.h
    tsl/coro/x86_64.h
    tsl/coro/detail.h
    tsl/printf_helpers.h
    tsl/version.h
    tsl/worker_thread.h
    tsl/cpumask.h
    tsl/frame_alloc.h
    tsl/basic.h
    tsl/types.h
    tsl/diag.h
    tsl/coroutine.h
    tsl/parsers.h
    tsl/notifier.h
    tsl/atomic_arm.h
    tsl/panic.h
    tsl/sections.h
    tsl/assert.h
    tsl/cal.h
    tsl/alloc/alloc_priv.h
    tsl/time.h
    tsl/hexdump.h
    tsl/test/helpers.h
    tsl/speed.h
    tsl/safe_alloc.h
    tsl/errors.h
    tsl/list.h
    tsl/safe_string.h
)

add_library(tsl SHARED "")
set_target_properties(tsl PROPERTIES PUBLIC_HEADER "${H_FILES}")
set_source_files_properties(${ASM_NASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)
set_source_files_properties(${C_SOURCES} PROPERTIES LANGUAGE C)
target_sources(tsl PRIVATE ${ASM_NASM_SOURCES} ${C_SOURCES})

add_library(tslapp SHARED
    app/app.c
    app/cpufeatures.c
)
set_target_properties(tslapp PROPERTIES
    PUBLIC_HEADER "app/app.h;app/cpufeatures.h"
)

add_library(tslconfig SHARED
    config/engine.c
    config/util.c
)
set_target_properties(tslconfig PROPERTIES
    PUBLIC_HEADER "config/engine.h;config/util.h"
)

target_compile_definitions(tsl PRIVATE _GNU_SOURCE)
target_compile_definitions(tsl PRIVATE SYS_CACHE_LINE_LENGTH=64)
target_compile_definitions(tsl PRIVATE _VC_VERSION="libtsl ${PROJECT_VERSION}")
target_compile_definitions(tslapp PRIVATE _GNU_SOURCE)
target_compile_definitions(tslconfig PRIVATE _GNU_SOURCE)
target_compile_definitions(tslconfig PRIVATE CONFIG_DIRECTORY_DEFAULT="/etc/tsl")


include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

################################
# Required development packages:
# 1. pkg-config
# 2. libjansson
# 3. ConcurrencyKit>=0.5.2
#
# Optional development packages:
# 1. librtlsdr
# 2. libuhd

find_package(PkgConfig REQUIRED "0.29.1")

pkg_check_modules(LIBJANSSON "jansson>=2" REQUIRED)
# message(STATUS "Found libjansson-" ${LIBJANSSON_VERSION})
target_include_directories(tsl PRIVATE ${LIBJANSSON_INCLUDE_DIRS})
find_library(LIBJANSSON_LIBRARIES
    NAMES jansson
    HINT ${LIBJANSSON_LIBDIR} ${LIBJANSSON_LIBRARY_DIRS}
)

message(STATUS "Notice: module ck is ConcurrencyKit(libck.so)...")
pkg_check_modules(CONCURRENCY_KIT "ck>=0.5.2" REQUIRED)
# message(STATUS "Found ConcurrencyKit-" ${CONCURRENCY_KIT_VERSION})
target_include_directories(tsl PRIVATE ${CONCURRENCY_KIT_INCLUDE_DIRS})
find_library(CONCURRENCY_KIT_LIBRARIES
    NAMES ck
    HINTS ${CONCURRENCY_KIT_LIBDIR} ${CONCURRENCY_KIT_LIBRARY_DIRS}
)

pkg_check_modules(LIBRTLSDR "librtlsdr")
if(LIBRTLSDR_FOUND)
    # message(STATUS "Found librtlsdr-" ${LIBRTLSDR_VERSION})
    target_include_directories(tsl PRIVATE ${LIBRTLSDR_INCLUDE_DIRS})
    find_library(LIBRTLSDR_LIBRARIES
        NAMES rtlsdr
        HINTS ${LIBRTLSDR_LIBDIR} ${LIBRTLSDR_LIBRARY_DIRS}
    )
endif()

pkg_check_modules(LIBUHD "uhd")
if(LIBUHD_FOUND)
    # message(STATUS "Found libuhd-" ${LIBUHD_VERSION})
    target_include_directories(tsl PRIVATE ${LIBUHD_INCLUDE_DIRS})
    find_library(LIBUHD_LIBRARIES
        NAMES uhd
        HINTS ${LIBUHD_LIBDIR} ${LIBUHD_LIBRARY_DIRS}
    )
endif()

# Where to install libtsl.a, libtslapp.a, libtslconfig.a, tsl.pc and *.h
include(GNUInstallDirs)
set(LIBDIR ${CMAKE_INSTALL_FULL_LIBDIR})
set(INCLUDEDIR ${CMAKE_INSTALL_FULL_INCLUDEDIR})
set(VERSION ${PROJECT_VERSION})
configure_file(tsl.pc.in tsl.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tsl.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
install(TARGETS tsl
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tsl
)
install(TARGETS tslapp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/app
)
install(TARGETS tslconfig
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/config
)
