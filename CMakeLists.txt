cmake_minimum_required(VERSION 3.7)
project(ldd)

set(CMAKE_C_STANDARD 99)
set(KERNEL_DIR /lib/modules/4.8.0-36-generic/build)
set(KERNEL_HEADER /usr/src/linux-headers-4.8.0-36-generic/include/)

function(compile_module obj)
    set(TARGET_NAME ${obj})
    add_custom_target(${TARGET_NAME} ALL cp -f ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND echo "compiling module ${obj}.ko..."
            )
    list(LENGTH ARGN argn_len)
    set(i 0)
    set(depend_objlist "")
    while( i LESS ${argn_len})
        list(GET ARGN ${i} argn_value)
        set(depend_objlist "${depend_objlist} ${argn_value}.o")
        math(EXPR i "${i} + 1")
    endwhile()

    add_custom_command(TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND echo "obj-m := ${obj}.o" > ${CMAKE_CURRENT_BINARY_DIR}/Makefile
            COMMAND echo "${obj}-objs:=${depend_objlist}" >>${CMAKE_CURRENT_BINARY_DIR}/Makefile
            COMMAND make -C ${KERNEL_DIR} M=${CMAKE_CURRENT_BINARY_DIR} modules
            )
endfunction()

include_directories(${KERNEL_HEADER})
add_subdirectory(hello)
add_subdirectory(chardev)
add_subdirectory(mytun)

set(SOURCE_FILES main.c)
add_executable(ldd ${SOURCE_FILES})