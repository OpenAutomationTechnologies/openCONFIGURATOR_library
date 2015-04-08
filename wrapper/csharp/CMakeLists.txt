################################################################################
#
# Project: openCONFIGURATOR core library C# wrapper
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: CMake file for the openCONFIGURATOR core C# wrapper
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
PROJECT ( OPEN_CONFIGURATOR_CORE_WRAPPER_CSHARP )

###############################################################################
# Ensure SWIG installation
###############################################################################
FIND_PACKAGE(SWIG 3.0.0 REQUIRED)
IF(NOT SWIG_FOUND)
	MESSAGE(SEND_ERROR "SWIG not found. Add its directory to your Path-Variable or set SWIG_EXECUTABLE to point to swig.exe.")
	MESSAGE(FATAL_ERROR "Your Path-Variable: $ENV{Path}")
ENDIF()
INCLUDE(${SWIG_USE_FILE}) # Provides CMake support for SWIG
UNSET(CMAKE_SWIG_FLAGS) #wipe out global cmake SWIG flags

###############################################################################
# Set wrapper include directories
###############################################################################
INCLUDE_DIRECTORIES(
		${PROJECT_SOURCE_DIR}/../../library/Include/
		${PROJECT_SOURCE_DIR}/../../library/src/
		SYSTEM ${Boost_INCLUDE_DIRS}
		)

IF(WIN32)
    LINK_DIRECTORIES(${Boost_LIBRARIES})
ENDIF()
###############################################################################
# Set wrapper output directories
###############################################################################
SET(CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/csharp/src")

###############################################################################
# Swig wrapper build options
###############################################################################
SET_SOURCE_FILES_PROPERTIES(openconfigurator_core_wrapper_net.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(openconfigurator_core_wrapper_net csharp openconfigurator_core_wrapper_net.i)

###############################################################################
# Remove lib prefix under Linux
###############################################################################
IF(UNIX)
	SET_TARGET_PROPERTIES(openconfigurator_core_wrapper_net PROPERTIES PREFIX "")
ENDIF()

###############################################################################
# Create C# wrapper library
###############################################################################
SWIG_LINK_LIBRARIES(openconfigurator_core_wrapper_net openconfigurator_core_lib ${Boost_LIBRARIES})

###############################################################################
# Install C# wrapper library to bin directory
###############################################################################
INSTALL(
	TARGETS openconfigurator_core_wrapper_net
	ARCHIVE DESTINATION ${INSTALL_DIR}
	LIBRARY DESTINATION ${INSTALL_DIR}
)