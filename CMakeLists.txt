cmake_minimum_required(VERSION 3.7)

project(TSL VERSION 0.5.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -gdwarf-4 -rdynamic")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Installation path information
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

# Add our default set of gcc warnings
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wundef -Wstrict-prototypes -Wmissing-prototypes -Wno-trigraphs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format-security")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-delete-null-pointer-checks -Wuninitialized -Wmissing-include-dirs -Wshadow")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wframe-larger-than=2047")

# Our header install base is a bit convoluted to avoid pollution
set(TSL_HEADERS_INSTALL_BASE "include/tsl")

add_definitions(-D_ATS_IN_TREE -D_GNU_SOURCE)
add_definitions(-DSYS_CACHE_LINE_LENGTH=64)
add_definitions(-DCONFIG_DIRECTORY_DEFAULT="/etc/tsl")

# Make sure we set the right build flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

set(TSL_BASE_DIR "${PROJECT_SOURCE_DIR}")

# Grab the revision from git
execute_process(COMMAND git describe --abbrev=16 --dirty --always --tags
    OUTPUT_VARIABLE GIT_REV
    ERROR_QUIET)

if ("${GIT_REV}" STREQUAL "")
    set(GIT_REV "NotInGit")
else()
    string(STRIP "${GIT_REV}" GIT_REV)
endif()

add_definitions(-D_VC_VERSION="${GIT_REV}")

# Grab the CPU model we're building on (no cross-compiling support)
execute_process(COMMAND uname -m
    OUTPUT_VARIABLE CPU_ARCH
    ERROR_QUIET)

if("${CPU_ARCH}" STREQUAL "armv7l")
    add_definitions(-D_USE_ARM_NEON)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=cortex-a7 -mfpu=crypto-neon-fp-armv8 -mfloat-abi=hard")
elseif("${CPU_ARCH}" STREQUAL "aarch64")
    add_definitions(-D_USE_ARM_NEON)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=cortex-a53")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -mtune=native")
endif()

# We use pkg-config to find our required libraries
find_package(PkgConfig REQUIRED)

# Get libck
pkg_check_modules(CK REQUIRED ck)

# Get JANSSON details
pkg_check_modules(JANSSON REQUIRED jansson)

add_subdirectory(tsl)
target_include_directories(tsl INTERFACE "${PROJECT_SOURCE_DIR}")
# Set the include directory to the base directory
target_include_directories(tsl PUBLIC
    "${TSL_BASE_DIR}"
    "${JANSSON_INCLUDE_DIRS}"
    "${CK_INCLUDE_DIRS}")

add_subdirectory(app)
add_subdirectory(test)
add_subdirectory(config)

# Create the pkg-config file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tsl.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/tsl.pc
    @ONLY)

# Install the pkg-config file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tsl.pc
    DESTINATION ${INSTALL_PKGCONFIG_DIR})

# Generate dpkg automatically
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_NAME "libtsl-dev")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "Phil Vachon <phil@security-embedded.com>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "TSL - The Standard Library")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libjansson-dev, libck-dev")
include(CPack)

