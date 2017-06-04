# Install script for directory: C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/src/libmetal/lib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/atomic.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/atomic.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/compiler.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/compiler.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/config.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/config.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/device.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/device.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/io.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/io.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/list.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/list.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/log.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/log.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/mutex.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/mutex.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/condition.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/condition.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/shmem.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/shmem.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/sys.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/sys.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/utilities.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/utilities.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/version.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/version.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/cpu.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/cpu.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/spinlock.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/spinlock.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/irq.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/irq.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/alloc.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/alloc.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/cache.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/cache.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/dma.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/dma.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/time.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/time.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/sleep.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/sleep.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/compiler/gcc/atomic.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/compiler/gcc/atomic.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/compiler/gcc/compiler.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/compiler/gcc/compiler.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/processor/arm/atomic.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/processor/arm/atomic.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/processor/arm/cpu.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/processor/arm/cpu.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/system/generic/mutex.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/system/generic/mutex.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/system/generic/sys.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/system/generic/sys.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/system/generic/condition.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/system/generic/condition.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/system/generic/alloc.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/system/generic/alloc.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/system/generic/irq.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/system/generic/irq.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE RENAME "metal/system/generic/zynq7/sys.h" FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/metal/system/generic/zynq7/sys.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/libmetal.a")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/compiler/cmake_install.cmake")
  include("C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/processor/cmake_install.cmake")
  include("C:/ver201604template/ver201604template.sdk/cpu1_amp/ps7_cortexa9_1/libsrc/libmetal_v1_1/build_libmetal/lib/system/cmake_install.cmake")

endif()

