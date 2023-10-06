set(VCPKG_BOOTSTRAP OFF)
set(VCPKG_SCRIPT_RELATIVE_PATH "scripts/buildsystems/vcpkg.cmake")

# Try to find vcpkg from submodule
set(VCPKG_SUBMODULE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../vcpkg")
set(VCPKG_SUBMODULE_TOOLCHAIN_FILE "${VCPKG_SUBMODULE_ROOT}/${VCPKG_SCRIPT_RELATIVE_PATH}")
if(NOT DEFINED VCPKG_TOOLCHAIN_FILE AND EXISTS ${VCPKG_SUBMODULE_TOOLCHAIN_FILE})
    set(VCPKG_BOOTSTRAP ON)
    set(VCPKG_TOOLCHAIN_FILE ${VCPKG_SUBMODULE_TOOLCHAIN_FILE})
    get_filename_component(VCPKG_TOOLCHAIN_FILE ${VCPKG_TOOLCHAIN_FILE} ABSOLUTE)
endif()

# Bootstrap vcpkg
if(${VCPKG_BOOTSTRAP})
    if(WIN32 AND NOT EXISTS ${VCPKG_SUBMODULE_ROOT}/vcpkg.exe)
        message(STATUS "Running boostrap-vcpkg.bat...")
        execute_process(COMMAND cmd /c ${VCPKG_SUBMODULE_ROOT}/bootstrap-vcpkg.bat -disableMetrics)
    elseif(UNIX AND NOT EXISTS ${VCPKG_SUBMODULE_ROOT}/vcpkg)
        message(STATUS "Running boostrap-vcpkg.sh...")
        execute_process(COMMAND ${VCPKG_SUBMODULE_ROOT}/bootstrap-vcpkg.sh -disableMetrics)
    endif()
endif()

# Make sure vcpkg was found
if(DEFINED VCPKG_TOOLCHAIN_FILE)
    message(STATUS "Using vcpkg from path: ${VCPKG_TOOLCHAIN_FILE}")
else()
    message(FATAL_ERROR "Something went wrong trying to find vcpkg.")
endif()

# Chainload toolchain file
get_filename_component(CMAKE_TOOLCHAIN_FILE_NAME_COMPONENT "${CMAKE_TOOLCHAIN_FILE}" NAME)
if(NOT CMAKE_TOOLCHAIN_FILE_NAME_COMPONENT STREQUAL "vcpkg.cmake")
    set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${CMAKE_TOOLCHAIN_FILE}")
endif()

if (NOT DEFINED VCPKG_MANIFEST_DIR)
    set(VCPKG_MANIFEST_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
endif()

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_TOOLCHAIN_FILE}")