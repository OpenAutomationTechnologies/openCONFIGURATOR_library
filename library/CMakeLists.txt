################################################################################
#
# Project: openCONFIGURATOR core library
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: CMake file for the openCONFIGURATOR core library
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

PROJECT (OPEN_CONFIGURATOR_CORE_LIB)

###############################################################################
# Set general sources and header of core library
###############################################################################
FILE (GLOB_RECURSE LIB_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
FILE (GLOB_RECURSE LIB_HEADERS "${PROJECT_SOURCE_DIR}/Include/*.h")

###############################################################################
# Set library include directories
###############################################################################
IF(WIN32)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Include/)
	INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
ELSE(WIN32)
	# This will cause the Boost-Headers to be included via GNU GCC's "-isystem"
	# option and hence will treat the Boost-Headers as system headers
	# (and suppress warnings for them).
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Include/)
	INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
ENDIF(WIN32)

###############################################################################
# If DLL option is chosen library type is shared
###############################################################################
SET(LIBRARY_TYPE SHARED)

###############################################################################
# Create the core library
###############################################################################
ADD_LIBRARY(openconfigurator_core_lib ${LIBRARY_TYPE} ${LIB_SOURCES} ${LIB_HEADERS})

###############################################################################
# Remove lib prefix under Linux
###############################################################################
#IF(UNIX)
#	SET_TARGET_PROPERTIES(openconfigurator_core_lib PROPERTIES PREFIX "")
#ENDIF()

###############################################################################
# Link the core library
###############################################################################
TARGET_LINK_LIBRARIES(openconfigurator_core_lib ${ADDITIONAL_DEPENDENCIES} ${Boost_LIBRARIES})

###############################################################################
# Install core library and dependencies to bin directory
###############################################################################
IF(WIN32)
	# Install Boost libraries
	FILE (GLOB BOOST_DEBUG_LIBS "${Boost_LIBRARY_DIRS}/*mt-gd*.dll")
	FILE (GLOB BOOST_RELEASE_LIBS "${Boost_LIBRARY_DIRS}/*mt-1*.dll")
	INSTALL(FILES ${BOOST_DEBUG_LIBS} DESTINATION ${INSTALL_DIR} CONFIGURATIONS Debug)
	INSTALL(FILES ${BOOST_RELEASE_LIBS} DESTINATION ${INSTALL_DIR} CONFIGURATIONS Release)
ENDIF()

INSTALL(
	FILES "${CMAKE_SOURCE_DIR}/resources/boost_log_settings.ini" DESTINATION ${INSTALL_DIR}
)

INSTALL(
	TARGETS openconfigurator_core_lib
	RUNTIME DESTINATION ${INSTALL_DIR}
	LIBRARY DESTINATION ${INSTALL_DIR}
)
