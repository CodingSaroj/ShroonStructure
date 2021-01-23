cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

project(ShroonStructure)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Target OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Target CPU: ${CMAKE_SYSTEM_PROCESSOR}")

set(ShroonIncludeDir "${CMAKE_CURRENT_LIST_DIR}/include")

file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/src/*.hpp")

file(MAKE_DIRECTORY "${ShroonIncludeDir}/Shroon/Structure/")
file(COPY ${HEADER_FILES} DESTINATION "${ShroonIncludeDir}/Shroon/Structure/")

add_library(ShroonStructure INTERFACE)

target_include_directories(ShroonStructure INTERFACE ${ShroonIncludeDir})

install(
    TARGETS ShroonStructure EXPORT ShroonStructureTargets
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/ShroonStructureConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

export(
    EXPORT ShroonStructureTargets
    NAMESPACE ShroonStructure::
    FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/ShroonStructureTargets.cmake"
)

if (${SHRN_STRUCTURE_BUILD_EXAMPLE})
    message(STATUS "Build Example: ON")

    add_executable(Example "${CMAKE_CURRENT_LIST_DIR}/example/main.cpp")

    set_target_properties(Example PROPERTIES
        VERSION 1.0.0
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/example/"
    )

    target_include_directories(Example PUBLIC "${ShroonIncludeDir}")
else()
    message(STATUS "Build Example: OFF")
endif()

if (${SHRN_STRUCTURE_BUILD_DOCS})
    message(STATUS "Build docs: ON")

    add_custom_target(
        docs ALL
        COMMAND "doxygen"
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMENT "Generating API documentation with Doxygen"
    )
else()
    message(STATUS "Build docs: OFF")
endif()

unset(ShroonIncludeDir)
