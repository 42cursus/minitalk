cmake_minimum_required(VERSION 3.8.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-narrowing")

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_C_STANDARD 90)

cmake_policy(SET CMP0048 NEW) # support PROJECT_VERSION
cmake_policy(SET CMP0075 NEW) # honor ``CMAKE_REQUIRED_LIBRARIES``

project(minitalk VERSION 0.1.0 LANGUAGES C CXX ASM)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_REQUIRED_LIBRARIES "-lbsd")

include_directories(include)

set(FTLIB_name FTLIB)
set(FTLIB_filename libft.a)
set(FTLIB_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib/libft)
set(FTLIB_build_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib/libft/build)

file(GLOB FTLIB_HEADERS "${FTLIB_dir}/*.h")
file(GLOB FTLIB_SOURCES "${FTLIB_dir}/**/ft_*.c")

add_custom_target(${FTLIB_name}_target ALL
        COMMAND make -C ${FTLIB_dir}
        BYPRODUCTS ${FTLIB_build_dir}/${FTLIB_filename}
        SOURCES "${FTLIB_HEADERS};${FTLIB_SOURCES}"
        COMMENT "Building 42lib")

# What are the differences between IMPORTED target and INTERFACE libraries?
# https://stackoverflow.com/questions/36648375/
add_library(${FTLIB_name} STATIC IMPORTED)
add_dependencies(${FTLIB_name} ${FTLIB_name}_target)
set_target_properties(${FTLIB_name} PROPERTIES
        IMPORTED_LOCATION ${FTLIB_build_dir}/${FTLIB_filename} # Make sure to use absolute path here
        INTERFACE_COMPILE_DEFINITIONS "OPTIMISTIC"
        INTERFACE_SOURCES "${FTLIB_HEADERS};${FTLIB_SOURCES}"
        INTERFACE_INCLUDE_DIRECTORIES "${FTLIB_dir};${CMAKE_CURRENT_SOURCE_DIR}/include")

set(TARGET_NAME client)

file(GLOB TARGET_HEADERS "src/${TARGET_NAME}/*.h")
file(GLOB TARGET_SOURCES "src/${TARGET_NAME}/*.c")

MESSAGE(STATUS "${TARGET_NAME}:")
MESSAGE(STATUS "${TARGET_HEADERS};${TARGET_SOURCES}")

set(TARGET_IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/.out/bin/${TARGET_NAME}")

add_custom_target(${TARGET_NAME}_target
        SOURCES "${TARGET_HEADERS};${TARGET_SOURCES}"
        COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR} ${TARGET_NAME}
        COMMAND cp ${TARGET_IMPORTED_LOCATION} ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Building ${TARGET_NAME}")

add_executable(${TARGET_NAME}_t IMPORTED)
add_dependencies(${TARGET_NAME}_t ${TARGET_NAME}_target)
set_target_properties(${TARGET_NAME}_target PROPERTIES
        LINKER_LANGUAGE C
        IMPORTED_LOCATION ${TARGET_IMPORTED_LOCATION} # Make sure to use absolute path here
        INTERFACE_COMPILE_DEFINITIONS "OPTIMISTIC"
        INTERFACE_SOURCES "${TARGET_HEADERS};${TARGET_SOURCES}"
        INTERFACE_LINK_LIBRARIES "${FTLIB_name}"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(${TARGET_NAME} /dev/null)
target_link_libraries(${TARGET_NAME} ${FTLIB_name})

set_target_properties(${TARGET_NAME}
        PROPERTIES
        LINKER_LANGUAGE C
        COMPILE_OPTIONS "-c"
        LINK_OPTIONS "-c"
)
add_dependencies(${TARGET_NAME} ${TARGET_NAME}_t)

set(TARGET_NAME server)
file(GLOB TARGET_HEADERS "src/${TARGET_NAME}/*.h")
file(GLOB TARGET_SOURCES "src/${TARGET_NAME}/*.c")

MESSAGE(STATUS "${TARGET_NAME}:")
MESSAGE(STATUS "${TARGET_HEADERS};${TARGET_SOURCES}")

set(TARGET_IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/.out/bin/${TARGET_NAME}")

add_custom_target(${TARGET_NAME}_target
    SOURCES "${TARGET_HEADERS};${TARGET_SOURCES}"
    COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR} ${TARGET_NAME}
    COMMAND cp ${TARGET_IMPORTED_LOCATION} ${CMAKE_CURRENT_BINARY_DIR}/
    COMMENT "Building ${TARGET_NAME}")

add_executable(${TARGET_NAME}_t IMPORTED)
add_dependencies(${TARGET_NAME}_t ${TARGET_NAME}_target)
set_target_properties(${TARGET_NAME}_t PROPERTIES
        LINKER_LANGUAGE C
        IMPORTED_LOCATION ${TARGET_IMPORTED_LOCATION} # Make sure to use absolute path here
        INTERFACE_COMPILE_DEFINITIONS "OPTIMISTIC"
        INTERFACE_SOURCES "${TARGET_HEADERS};${TARGET_SOURCES}"
        INTERFACE_LINK_LIBRARIES "${FTLIB_name}"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(${TARGET_NAME} /dev/null)
target_link_libraries(${TARGET_NAME} ${FTLIB_name})

set_target_properties(${TARGET_NAME}
        PROPERTIES
            LINKER_LANGUAGE C
            COMPILE_OPTIONS "-c"
            LINK_OPTIONS "-c"
)
add_dependencies(${TARGET_NAME} ${TARGET_NAME}_t)

add_custom_target(fclean
        COMMAND make -C ${FTLIB_dir} fclean)

add_custom_target(re
        COMMAND make -C ${FTLIB_dir} re)

