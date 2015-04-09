################################################################################
#
# Project: openCONFIGURATOR core test
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: CMAKE file for the openCONFIGURATOR core library CPPUNIT tests
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
PROJECT ( OPEN_CONFIGURATOR_CORE_TEST )

###############################################################################
# Add CPPUNIT libraries
###############################################################################
Include(${PROJECT_SOURCE_DIR}/FindCppUnit.cmake)

### Find Boost Libraries BEGIN ###
SET(Boost_DEBUG TRUE)
SET(Boost_USE_STATIC_RUNTIME OFF)
SET(Boost_ADDITIONAL_VERSIONS "1.54.0" "1.55.0")
FIND_PACKAGE(Boost COMPONENTS log date_time filesystem system thread REQUIRED)
# Necessary for Auto-Linking under VS, ignored under Unix
ADD_DEFINITIONS(/DBOOST_ALL_DYN_LINK)
### Find Boost Libraries END ###

###############################################################################
# Set general sources and header of openCONF library
###############################################################################
FILE ( GLOB_RECURSE LIB_TEST_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" )
FILE ( GLOB_RECURSE LIB_TEST_HEADERS "${PROJECT_SOURCE_DIR}/Include/*.h" )

###############################################################################
# Set library include directories
###############################################################################
INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/library/Include
	${PROJECT_SOURCE_DIR}/Include
)
INCLUDE_DIRECTORIES(SYSTEM
	${CPPUNIT_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
)

###############################################################################
# Link library directories
###############################################################################
LINK_DIRECTORIES(
	${Boost_LIBRARY_DIRS}
)

###############################################################################
# Create openCONFIGURATOR core test executable
###############################################################################
ADD_EXECUTABLE(openconfigurator_core_test ${LIB_TEST_HEADERS} ${LIB_TEST_SOURCES})

###############################################################################
# Link openCONFIGURATOR core test executable to libraries
###############################################################################
TARGET_LINK_LIBRARIES(openconfigurator_core_test debug ${CPPUNIT_DEBUG_LIBRARY} openconfigurator_core_lib)
TARGET_LINK_LIBRARIES(openconfigurator_core_test optimized ${CPPUNIT_LIBRARY} openconfigurator_core_lib)

###############################################################################
# Copy unit test dependency and execute the tests
###############################################################################
ADD_CUSTOM_COMMAND(TARGET openconfigurator_core_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_BINARY_DIR}/../library/$<CONFIGURATION>/"
        "${PROJECT_BINARY_DIR}/$<CONFIGURATION>")

ADD_CUSTOM_COMMAND(TARGET openconfigurator_core_test POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${Boost_LIBRARY_DIRS}"
        "${PROJECT_BINARY_DIR}/$<CONFIGURATION>")


ADD_CUSTOM_COMMAND(TARGET openconfigurator_core_test POST_BUILD
                   COMMAND "$(TargetDir)$(TargetFileName)" > test_output.txt
                   COMMENT "Running openCONFIGURATOR core unit tests")