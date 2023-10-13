# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/nash/esp/esp-idf/components/bootloader/subproject"
  "/home/nash/Documents/ttn-esp32/examples/deep_sleep_in_c/build/bootloader"
  "/home/nash/Documents/ttn-esp32/examples/deep_sleep_in_c/build/bootloader-prefix"
  "/home/nash/Documents/ttn-esp32/examples/deep_sleep_in_c/build/bootloader-prefix/tmp"
  "/home/nash/Documents/ttn-esp32/examples/deep_sleep_in_c/build/bootloader-prefix/src/bootloader-stamp"
  "/home/nash/Documents/ttn-esp32/examples/deep_sleep_in_c/build/bootloader-prefix/src"
  "/home/nash/Documents/ttn-esp32/examples/deep_sleep_in_c/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/nash/Documents/ttn-esp32/examples/deep_sleep_in_c/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/nash/Documents/ttn-esp32/examples/deep_sleep_in_c/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
