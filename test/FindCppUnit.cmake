################################################################################
#
# Project: openCONFIGURATOR core test
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: CMake file to find CppUnit framework
#
# License:
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#   1. Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#   2. Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#
#   3. Neither the name of the copyright holders nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without prior written permission. For written
#      permission, please contact office@br-automation.com.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#   COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.
#
#   Severability Clause:
#
#       If a provision of this License is or becomes illegal, invalid or
#       unenforceable in any jurisdiction, that shall not affect:
#       1. the validity or enforceability in that jurisdiction of any other
#          provision of this License; or
#       2. the validity or enforceability in other jurisdictions of that or
#          any other provision of this License.
#
################################################################################

# - try to find cppunit library
#
# Cache Variables: (probably not for direct use in your scripts)
#  CPPUNIT_INCLUDE_DIR
#  CPPUNIT_LIBRARY
#
# Non-cache variables you might use in your CMakeLists.txt:
#  CPPUNIT_FOUND
#  CPPUNIT_INCLUDE_DIRS
#  CPPUNIT_LIBRARIES
#
# Requires these CMake modules:
#  SelectLibraryConfigurations (included with CMake >= 2.8.0)
#  FindPackageHandleStandardArgs (known included with CMake >=2.6.2)
#
# Original Author:
# 2009-2011 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryand.net>
# http://academic.cleardefinition.com
# Iowa State University HCI Graduate Program/VRAC
#
# Copyright Iowa State University 2009-2011.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

set(CPPUNIT_ROOT_DIR
	"${CPPUNIT_ROOT_DIR}"
	CACHE
	PATH
	"Directory to search")

find_library(CPPUNIT_LIBRARY_RELEASE
	NAMES
	cppunit
	HINTS
	"${CPPUNIT_ROOT_DIR}/lib")

find_library(CPPUNIT_LIBRARY_DEBUG
	NAMES
	cppunitd
	HINTS
	"${CPPUNIT_ROOT_DIR}/lib")

include(SelectLibraryConfigurations)
select_library_configurations(CPPUNIT)

# Might want to look close to the library first for the includes.
get_filename_component(_libdir "${CPPUNIT_LIBRARY_RELEASE}" PATH)

find_path(CPPUNIT_INCLUDE_DIR
	NAMES
	cppunit/TestCase.h
	HINTS
	"${_libdir}/.."
	PATHS
	"${CPPUNIT_ROOT_DIR}"
	PATH_SUFFIXES
	include/)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(cppunit
	DEFAULT_MSG
	CPPUNIT_LIBRARY
	CPPUNIT_INCLUDE_DIR)

if(CPPUNIT_FOUND)
	set(CPPUNIT_LIBRARIES ${CPPUNIT_LIBRARY} ${CMAKE_DL_LIBS})
	set(CPPUNIT_INCLUDE_DIRS "${CPPUNIT_INCLUDE_DIR}")
	mark_as_advanced(CPPUNIT_ROOT_DIR)
endif()

mark_as_advanced(CPPUNIT_INCLUDE_DIR
	CPPUNIT_LIBRARY_RELEASE
	CPPUNIT_LIBRARY_DEBUG)