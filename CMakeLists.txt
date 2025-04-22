cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)

set(PROJECT_NAME atmega-common-code)

project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX ASM)

set(MCU atmega64m1)
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

set(PROG_TYPE atmelice_dw) # TODO: Not needed if programming externally

add_compile_options(
    -mmcu=${MCU} # MCU
    -std=gnu99 # C99 standard
    -Os # optimize
    -Wall # enable warnings
    -Wno-main
    -Wundef
    -pedantic
    -Wfatal-errors
)

file(GLOB SUPPORT_PACKAGE_C_FILES "./atmega64m1-support-package/lib/src/*.c")

file(GLOB ATMEGA_COMMON_ASM_FILES "./psndl-atmega-common-code/include/*.S")
file(GLOB ATMEGA_COMMON_C_FILES "./psndl-atmega-common-code/src/*.c")

# This is the only source file used from the embedded-dsa library
file(GLOB EMBEDDED_DSA_C_FILES "./embedded-dsa/circular_buffer.c")

add_executable(${PROJECT_NAME} ${SUPPORT_PACKAGE_C_FILES} ${EMBEDDED_DSA_C_FILES} ${ATMEGA_COMMON_C_FILES} ${ATMEGA_COMMON_ASM_FILES})

# All the Microchip Studio stuff that was deemed necessary to build
target_include_directories(${PROJECT_NAME} PRIVATE "./psndl-atmega-common-code/config")
target_include_directories(${PROJECT_NAME} PRIVATE "./psndl-atmega-common-code/include")
target_include_directories(${PROJECT_NAME} PRIVATE "./psndl-atmega-common-code/utils")
target_include_directories(${PROJECT_NAME} PRIVATE "./psndl-atmega-common-code/src")
target_include_directories(${PROJECT_NAME} PRIVATE "./psndl-atmega-common-code/utils/assembler")

# TODO: Make and then link this instead of doing it like this
target_include_directories(${PROJECT_NAME} PRIVATE "./atmega64m1-support-package/lib/inc")
target_include_directories(${PROJECT_NAME} PRIVATE "./atmega64m1-support-package/lib/src")

# Include the embedded-dsa library
target_include_directories(${PROJECT_NAME} PRIVATE "./embedded-dsa")

# Include and link the AVR libraries 
target_include_directories(${PROJECT_NAME} PRIVATE "/usr/lib/avr/include")
target_include_directories(${PROJECT_NAME} PRIVATE "/usr/lib/avr/include/avr")
target_include_directories(${PROJECT_NAME} PRIVATE "/usr/lib/avr/x86_64-linux-gnu/include")

target_link_directories(${PROJECT_NAME} PRIVATE "/usr/lib/avr/lib")
target_link_directories(${PROJECT_NAME} PRIVATE "/usr/lib/avr/x86_64-linux-gnu/lib")

# Rename the output to .elf as we will create multiple files
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

# Strip binary for upload
add_custom_target(strip ALL avr-strip ${PROJECT_NAME}.elf DEPENDS ${PROJECT_NAME})

# Transform binary into hex file, we ignore the eeprom segments in the step
add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex DEPENDS strip)

# Transform binary into hex file, this is the eeprom part (empty if you don't
# use eeprom static variables)
add_custom_target(eeprom avr-objcopy -j .eeprom  --set-section-flags=.eeprom="alloc,load"  --change-section-lma .eeprom=0 -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.eep DEPENDS strip)

# Upload the firmware with avrdude
# add_custom_target(upload avrdude  -c ${PROG_TYPE} -p ${MCU} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)

# Upload the eeprom with avrdude
# add_custom_target(upload_eeprom avrdude -c ${PROG_TYPE} -p ${MCU}  -U eeprom:w:${PROJECT_NAME}.eep DEPENDS eeprom)

# Make sure output files are added to make clean
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst")

