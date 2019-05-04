# Copyright 2019 Saleem Abdulrasool <compnerd@compnerd.org>

cmake_minimum_required(VERSION 3.4)
project(eds LANGUAGES C)

set(CMAKE_C_STANDARD 99)

option(WITH_EXAMPLES "build example program" YES)

include(CheckFunctionExists)
include(CheckLibraryExists)
include(GNUInstallDirs)

check_function_exists(sqrt HAVE_SQRT)
if(NOT HAVE_SQRT)
  unset(HAVE_SQRT CACHE)
  check_library_exists(m sqrt "" HAVE_SQRT)
  if(NOT HAVE_SQRT)
    message(SEND_ERROR "unable to find `sqrt`")
  endif()
  set(LIBM_LIBS m)
endif()

if(WITH_EXAMPLES)
  add_executable(parse-edid
    src/examples/parse-edid/parse-edid.c)
  if(MSVC)
    target_compile_options(parse-edid PRIVATE
      /FI${CMAKE_SOURCE_DIR}/src/eds/macros.h)
  else()
    target_compile_options(parse-edid PRIVATE
      -include;${CMAKE_SOURCE_DIR}/src/eds/macros.h)
  endif()
  target_include_directories(parse-edid PRIVATE
    src
    src/eds)
endif()

install(FILES
          src/eds/cea861.h
          src/eds/edid.h
          src/eds/hdmi.h
        DESTINATION
          ${CMAKE_INSTALL_FULL_INCLUDE_DIR}/eds)

