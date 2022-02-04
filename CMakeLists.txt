cmake_minimum_required(VERSION 3.15.3)
# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)
# Setup project
project(Arduino_Core_Rudiron)
SET(CMAKE_BUILD_TYPE Debug)

set(cpu_name MDR32F9Qx)

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

#Sources
set(core_path, ${CMAKE_SOURCE_DIR}/core)
file(GLOB_RECURSE core_sources
        ${core_path}/*.c
        ${core_path}/*.h
        ${core_path}/*.cpp
        ${core_path}/*.cpp
        ${core_path}/avr/*.c
        ${core_path}/avr/*.h
        ${core_path}/avr/*.cpp
        ${core_path}/avr/*.cpp
        )

set(libraries_path, ${CMAKE_SOURCE_DIR}/libraries)
file(GLOB_RECURSE libraries_sources
        ${libraries_path}/*.c
        ${libraries_path}/*.h
        ${libraries_path}/*.cpp
        ${libraries_path}/*.cpp
        )

set(sketch_path, ${CMAKE_SOURCE_DIR}/sketch)
file(GLOB_RECURSE sketch_sources
        ${sketch_path}/*.c
        ${sketch_path}/*.h
        ${sketch_path}/*.cpp
        ${sketch_path}/*.cpp
        )

set(cmsis_path, ${CMAKE_SOURCE_DIR}/system/Drivers/CMSIS/Device/MILANDR/${cpu_name})

set(cmsis_include_path, ${cmsis_path}/Include)
file(GLOB_RECURSE cmsis_include_sources
        ${cmsis_include_path}/*.c
        ${cmsis_include_path}/*.h
        ${cmsis_include_path}/*.cpp
        ${cmsis_include_path}/*.cpp
        )

set(cmsis_source_path, ${cmsis_path}/Source)
file(GLOB_RECURSE cmsis_source_sources
        ${cmsis_source_path}/Templates/*.c
        ${cmsis_source_path}/Templates/*.h
        ${cmsis_source_path}/Templates/*.cpp
        ${cmsis_source_path}/Templates/*.cpp
        )

set(cmsis_startup_path, ${cmsis_source_path}/Templates/gcc)
file(GLOB_RECURSE cmsis_startup_sources
        ${cmsis_startup_path}/gcc/*.S
        )

set(std_periph_driver_path, ${CMAKE_SOURCE_DIR}/system/Drivers/${cpu_name}_StdPeriph_Driver)
file(GLOB_RECURSE std_periph_driver_sources
        ${std_periph_driver_path}/inc/*.c
        ${std_periph_driver_path}/inc/*.h
        ${std_periph_driver_path}/inc/*.cpp
        ${std_periph_driver_path}/inc/*.hpp
        ${std_periph_driver_path}/inc/USB_Library/*.c
        ${std_periph_driver_path}/inc/USB_Library/*.h
        ${std_periph_driver_path}/inc/USB_Library/*.cpp
        ${std_periph_driver_path}/inc/USB_Library/*.hpp
        ${std_periph_driver_path}/src/*.c
        ${std_periph_driver_path}/src/*.h
        ${std_periph_driver_path}/src/*.cpp
        ${std_periph_driver_path}/src/*.hpp
        ${std_periph_driver_path}/src/USB_Library/*.c
        ${std_periph_driver_path}/src/USB_Library/*.h
        ${std_periph_driver_path}/src/USB_Library/*.cpp
        ${std_periph_driver_path}/src/USB_Library/*.hpp
        )

set(SRC_LIST
        core_sources
        libraries_sources
        sketch_sources
        cmsis_include_sources
        cmsis_source_sources
        cmsis_startup_sources
        std_periph_driver_sources
        )

#Executable
set(EXECUTABLE ${PROJECT_NAME}.elf)
#Linker file
set(LINKER_FILE ${cmsis_devicesupport_startup_gcc_path}/${cpu_name}.ld)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(${EXECUTABLE}
        ${SRC_LIST}
        )

#Includes
target_include_directories(${EXECUTABLE} PRIVATE

        )

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE

        )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
        "-mcpu=cortex-${core}"
        "-mthumb"
        "-ffunction-sections"
        "-fdata-sections"
        "-pipe"
        "-DUSE_${cpu_name}"
        "-D__STARTUP_CLEAR_BSS"
        "-D_start=main"
        )


# Linker options
target_link_options(${EXECUTABLE} PRIVATE
        "-T${cmsis_devicesupport_startup_gcc_path}/${cpu_name}.ld"
        "-mcpu=cortex-${core}"
        "-mthumb"
        "-Wl,--gc-sections"
        "-specs=nosys.specs"
        "-specs=nano.specs"
        "-ffreestanding"
        "-nostartfiles"
        )


# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${EXECUTABLE} ${PROJECT_NAME}.s19
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)


# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE})