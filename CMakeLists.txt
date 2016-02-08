################################################################################
#
# Project: openCONFIGURATOR
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: Main CMAKE file of openCONFIGURATOR library
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
# Ensure working support of ADD_JAR command
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.12)

INCLUDE(CMakeDependentOption)

PROJECT (OPEN_CONFIGURATOR)
###############################################################################
# Compiler flags for MS Visual Studio
###############################################################################
IF(MSVC)
	SET(CMAKE_CXX_FLAGS "/wd\"4512\" /wd\"4251\" /Zi /nologo /W4 /WX- /Od /Oy- /Ob0 /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS /Gm- /EHsc /GS /fp:precise /Zc:wchar_t /Zc:forScope /Gd /analyze- /errorReport:queue /GR")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /DDEBUG")
	STRING(FIND "${CMAKE_GENERATOR}" "Win64" FOUND)
	IF(${FOUND} EQUAL -1)
		SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin/win32/x86/\${BUILD_TYPE}")
		SET(INSTALL_DIR_VS "${CMAKE_SOURCE_DIR}/bin/win32/x86/$(Configuration)")
	ELSE()
		SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin/win32/x86_64/\${BUILD_TYPE}")
		SET(INSTALL_DIR_VS "${CMAKE_SOURCE_DIR}/bin/win32/x86_64/$(Configuration)")
	ENDIF()
ELSEIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Weffc++ -std=c++11")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
	IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
		SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin/linux/x86_64")
	ELSE()
		SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin/linux/x86")
	ENDIF()
	IF(MINGW)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -U__STRICT_ANSI__ -D__NO_INLINE__ -Wl,--kill-at")
	ENDIF(MINGW)
ENDIF(MSVC)
###############################################################################
# Find BOOST libraries
###############################################################################
SET(Boost_DEBUG TRUE)
SET(Boost_USE_STATIC_RUNTIME OFF)
SET(Boost_ADDITIONAL_VERSIONS "1.54.0" "1.59.0" "1.60.0")
FIND_PACKAGE(Boost COMPONENTS log log_setup chrono date_time filesystem system thread REQUIRED)
# Necessary for Auto-Linking under VS, ignored under Unix
ADD_DEFINITIONS(/DBOOST_ALL_DYN_LINK)
IF(NOT Boost_FOUND)
	MESSAGE(SEND_ERROR "Boost libraries not found. Please add BOOST_ROOT environment variable to system.")
# Boost version above 1.55.0 require boost atomic to be linked
ELSEIF(Boost_FOUND AND Boost_MINOR_VERSION GREATER 55)
	FIND_PACKAGE(Boost COMPONENTS atomic REQUIRED)
ENDIF()

###############################################################################
# Set options
###############################################################################
OPTION (OPEN_CONFIGURATOR_CORE_NET_WRAPPER "Build openCONFIGURATOR core C# wrapper library." OFF)
OPTION (OPEN_CONFIGURATOR_CORE_JAVA_WRAPPER "Build openCONFIGURATOR core Java wrapper library." OFF)
OPTION (OPEN_CONFIGURATOR_CORE_UNIT_TESTS "Build openCONFIGURATOR core unit tests." OFF)
IF(MSVC)
CMAKE_DEPENDENT_OPTION(OPEN_CONFIGURATOR_CORE_NET_APP "Build openCONFIGURATOR C# demo executable." OFF
			"OPEN_CONFIGURATOR_CORE_NET_WRAPPER" ON)
ENDIF()
OPTION (OPEN_CONFIGURATOR_CORE_DOCUMENTATION "Build openCONFIGURATOR core source code documentation." OFF)

###############################################################################
# Add a target to generate API documentation with Doxygen
###############################################################################
IF(OPEN_CONFIGURATOR_CORE_DOCUMENTATION)
	FIND_PACKAGE(Doxygen REQUIRED)
	IF(DOXYGEN_FOUND)
		CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/resources/doc/openconfigurator_core.doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		ADD_CUSTOM_TARGET(
			DOCUMENTATION ALL ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
	ELSE(DOXYGEN_FOUND)
		MESSAGE(SEND_ERROR "Doxygen installation not found. Please install doxygen and add binary to system path.")
	ENDIF(DOXYGEN_FOUND)
ENDIF()

###############################################################################
# Add a target to generate the openCONFIGURATOR library
###############################################################################
ADD_SUBDIRECTORY(library)

###############################################################################
# Add a target to generate .NET wrapper
###############################################################################
IF(OPEN_CONFIGURATOR_CORE_NET_WRAPPER)
	ADD_SUBDIRECTORY(wrapper/csharp)
ENDIF()

###############################################################################
# Add a target to generate Java wrapper
###############################################################################
IF(OPEN_CONFIGURATOR_CORE_JAVA_WRAPPER)
	ADD_SUBDIRECTORY(wrapper/java)
ENDIF()

###############################################################################
# Add a target to generate CPPUNIT tests
###############################################################################
IF(OPEN_CONFIGURATOR_CORE_UNIT_TESTS)
	ADD_SUBDIRECTORY(test)
ENDIF()

IF(WIN32)
	IF(MSVC)
		IF(OPEN_CONFIGURATOR_CORE_NET_WRAPPER AND OPEN_CONFIGURATOR_CORE_NET_APP)
			CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/demo/csharp/openconfigurator_core_net_app.csproj ${CMAKE_SOURCE_DIR}/demo/csharp/openconfigurator_core_net_app_cmake.csproj)
			INCLUDE_EXTERNAL_MSPROJECT(openconfigurator_core_net_demo ${CMAKE_SOURCE_DIR}/demo/csharp/openconfigurator_core_net_app_cmake.csproj
				TYPE 84C4FA53-4EC7-493F-BC56-B4D51E7504D0
				PLATFORM "Any CPU"
				openconfigurator_core_wrapper_net)
		ENDIF()
	ENDIF(MSVC)
ENDIF(WIN32)

