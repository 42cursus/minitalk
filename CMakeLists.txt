# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    CMakeLists.txt                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/03 16:35:56 by abelov            #+#    #+#              #
#    Updated: 2024/06/03 16:35:56 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

cmake_minimum_required(VERSION 3.8.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-narrowing")

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_C_STANDARD 90)

cmake_policy(SET CMP0048 NEW) # support PROJECT_VERSION
cmake_policy(SET CMP0075 NEW) # honor ``CMAKE_REQUIRED_LIBRARIES``

project(minitalk VERSION 0.1.0 LANGUAGES C CXX ASM)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(include)

set(PARENT_PROJECT_NAME "${PROJECT_NAME}")

set(FT_LIB_NAME ft)
set(FT_LIB_FILENAME libft.a)
set(FT_LIB_DIR "${PROJECT_SOURCE_DIR}/lib/ft")

file(GLOB_RECURSE FT_LIB_HEADERS "${FT_LIB_DIR}/*.h")
file(GLOB_RECURSE FT_LIB_SOURCES "${FT_LIB_DIR}/*.c")

add_custom_target(${FT_LIB_NAME}_target ALL
        COMMAND make -C ${FT_LIB_DIR}
        BYPRODUCTS "${FT_LIB_DIR}/${FT_LIB_FILENAME}"
        SOURCES "${FT_LIB_HEADERS};${FT_LIB_SOURCES}"
        COMMENT "Building FT_LIB")

add_library(${FT_LIB_NAME} STATIC IMPORTED)
add_dependencies(${FT_LIB_NAME} "${FT_LIB_NAME}_target")
set_target_properties(${FT_LIB_NAME} PROPERTIES
        IMPORTED_LOCATION ${FT_LIB_DIR}/${FT_LIB_FILENAME} # Make sure to use absolute path here
        INTERFACE_COMPILE_DEFINITIONS ""
        INTERFACE_LINK_OPTIONS ""
        INTERFACE_SOURCES "${FT_LIB_HEADERS}"
        INTERFACE_INCLUDE_DIRECTORIES "${FT_LIB_DIR};${FT_LIB_DIR}/include")

#===========================================================================

set(TARGET_NAME client)

file(GLOB TARGET_HEADERS "src/${TARGET_NAME}/*.h")
file(GLOB TARGET_SOURCES "src/${TARGET_NAME}/*.c")

MESSAGE(STATUS "${TARGET_NAME}:")
MESSAGE(STATUS "${TARGET_HEADERS};${TARGET_SOURCES}")

set(TARGET_IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/build/bin/${TARGET_NAME}")

add_custom_target(${TARGET_NAME}_target
        SOURCES "${TARGET_HEADERS};${TARGET_SOURCES}"
        COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR} ${TARGET_NAME}
        COMMAND cp ${TARGET_IMPORTED_LOCATION} ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Building ${TARGET_NAME}")

add_executable(${TARGET_NAME}_t IMPORTED "${TARGET_HEADERS};${TARGET_SOURCES}")
add_dependencies(${TARGET_NAME}_t ${TARGET_NAME}_target)
set_target_properties(${TARGET_NAME}_target PROPERTIES
        LINKER_LANGUAGE C
        IMPORTED_LOCATION ${TARGET_IMPORTED_LOCATION} # Make sure to use absolute path here
        INTERFACE_COMPILE_DEFINITIONS "OPTIMISTIC"
        INTERFACE_SOURCES "${TARGET_HEADERS};${TARGET_SOURCES}"
        INTERFACE_LINK_LIBRARIES "${FTLIB_name}"
        INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/include")

add_executable(${TARGET_NAME} /dev/null)
target_link_libraries(${TARGET_NAME} ${FTLIB_name})
target_include_directories(${TARGET_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
set_target_properties(${TARGET_NAME}
        PROPERTIES
        LINKER_LANGUAGE C
        COMPILE_OPTIONS "-c"
        LINK_OPTIONS "-c"
)
add_dependencies(${TARGET_NAME} ${TARGET_NAME}_t)

#===========================================================================

set(TARGET_NAME server)
file(GLOB TARGET_HEADERS "src/${TARGET_NAME}/*.h")
file(GLOB TARGET_SOURCES "src/${TARGET_NAME}/*.c")

MESSAGE(STATUS "${TARGET_NAME}:")
MESSAGE(STATUS "${TARGET_HEADERS};${TARGET_SOURCES}")

set(TARGET_IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/build/bin/${TARGET_NAME}")

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
        COMMAND make -C ${PROJECT_SOURCE_DIR} fclean)

add_custom_target(re
        COMMAND make -C ${PROJECT_SOURCE_DIR} re)

