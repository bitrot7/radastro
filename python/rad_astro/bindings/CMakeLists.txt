# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT rad_astro_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT rad_astro_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################
list(APPEND rad_astro_python_files
    avg_python.cc
    filewriter_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(rad_astro
   ../../..
   gr::rad_astro
   "${rad_astro_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET rad_astro_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rad_astro_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/rad_astro/
)

install(TARGETS rad_astro_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/rad_astro COMPONENT pythonapi)
