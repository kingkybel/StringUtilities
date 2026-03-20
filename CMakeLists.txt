cmake_minimum_required(VERSION 3.26)
project(StringUtilities
        VERSION 1.0.0
        DESCRIPTION "A header only library for string utilities"
        HOMEPAGE_URL "https://github.com/kingkybel/StringUtilities")

include(GNUInstallDirs)
include(CTest)

set(DKYB_CMAKE_COMMON_LOCAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-common" CACHE PATH "Path to local dkyb cmake-common checkout")
set(DKYB_CMAKE_COMMON_GIT_REPOSITORY "https://github.com/kingkybel/CMakeCommon.git" CACHE STRING "dkyb cmake-common repository")
set(DKYB_CMAKE_COMMON_GIT_TAG "v0.1.0" CACHE STRING "Pinned dkyb cmake-common tag")

set(_dkyb_common_module "${DKYB_CMAKE_COMMON_LOCAL_PATH}/cmake/DkybBuildSettings.cmake")
if(EXISTS "${_dkyb_common_module}")
        include("${_dkyb_common_module}")
else()
        include(FetchContent)
        FetchContent_Declare(
                dkyb_cmake_common
                GIT_REPOSITORY "${DKYB_CMAKE_COMMON_GIT_REPOSITORY}"
                GIT_TAG "${DKYB_CMAKE_COMMON_GIT_TAG}"
        )
        FetchContent_MakeAvailable(dkyb_cmake_common)
        include("${dkyb_cmake_common_SOURCE_DIR}/cmake/DkybBuildSettings.cmake")
endif()

dkyb_apply_common_settings()

set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(
        BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/include
        system
)
link_directories(
        /usr/local/lib
        system
)

add_library(DirectedGraphs INTERFACE)
# Adding the install interface generator expression makes sure that the include
# files are installed to the proper location (provided by GNUInstallDirs)
target_include_directories(
        DirectedGraphs
        INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(DirectedGraphs INTERFACE cxx_std_23)

if(BUILD_TESTING)
        add_subdirectory(test)
endif()

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=* -p ${CMAKE_SOURCE_DIR}/build")

file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

# Install the header files directly under 'include/dkyb'
install(FILES ${HEADER_FILES}
        DESTINATION include/dkyb)
