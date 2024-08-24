# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/allen/esp/v5.3/esp-idf/components/bootloader/subproject"
  "C:/Users/allen/Desktop/repos/duckyPad2/firmware/tusb_hid/build/bootloader"
  "C:/Users/allen/Desktop/repos/duckyPad2/firmware/tusb_hid/build/bootloader-prefix"
  "C:/Users/allen/Desktop/repos/duckyPad2/firmware/tusb_hid/build/bootloader-prefix/tmp"
  "C:/Users/allen/Desktop/repos/duckyPad2/firmware/tusb_hid/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/allen/Desktop/repos/duckyPad2/firmware/tusb_hid/build/bootloader-prefix/src"
  "C:/Users/allen/Desktop/repos/duckyPad2/firmware/tusb_hid/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/allen/Desktop/repos/duckyPad2/firmware/tusb_hid/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/allen/Desktop/repos/duckyPad2/firmware/tusb_hid/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
