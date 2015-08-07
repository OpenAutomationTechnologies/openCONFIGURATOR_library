################################################################################
#
# Project: openCONFIGURATOR core library Java wrapper
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: CMake file for the openCONFIGURATOR core library Java wrapper
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
PROJECT ( OPEN_CONFIGURATOR_CORE_WRAPPER_JAVA )

# The name of the SWIG interface file
SET(SWIG_INTERFACE_FILE openconfigurator_core_wrapper_java.i)

OPTION (OPEN_CONFIGURATOR_CORE_JAVA_WRAPPER_DEBUG_INFO "Include debug information into openCONFIGURATOR core java wrapper jar." OFF)

# Compiler flags for MSVC
IF(MSVC)
  # __stdcall calling convention (/Gz) necessary for SWIG
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /nologo /Ot /GL /GS /W4 /Gd")
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Oi /Oy /GF")
ENDIF()

###############################################################################
# Ensure SWIG installation
###############################################################################
FIND_PACKAGE(SWIG 3.0.3 REQUIRED)
IF(NOT SWIG_FOUND)
	MESSAGE(SEND_ERROR "SWIG not found. Add its directory to your Path-Variable or set SWIG_EXECUTABLE to point to swig.exe.")
	MESSAGE(FATAL_ERROR "Your Path-Variable: $ENV{Path}")
ENDIF()
INCLUDE(${SWIG_USE_FILE}) # Provides CMake support for SWIG
UNSET(CMAKE_SWIG_FLAGS) #wipe out global cmake SWIG flags

###############################################################################
# Ensure Java installation (needed for javac and .jar generation)
###############################################################################
FIND_PACKAGE(Java 1.7.0.75 COMPONENTS Development REQUIRED)
IF(NOT Java_Development_FOUND )
	MESSAGE(SEND_ERROR "Java not found. Add its directory to your Path-Variable or set JAVA_HOME to point to the installation.")
	MESSAGE(FATAL_ERROR "Your Path-Variable: $ENV{Path}")
ENDIF()
INCLUDE(UseJava) # Provides CMake support for .jar generation

###############################################################################
# Ensure JNI installation
###############################################################################
INCLUDE(FindJNI) # Find JNI headers and libs
INCLUDE_DIRECTORIES(${JNI_INCLUDE_DIRS})

###############################################################################
# Set include directories in addition to openCONFIGURATOR_INCLUDE_DIR
###############################################################################
# Needed for the wrapper compiler to find Libraries
INCLUDE_DIRECTORIES(
		${PROJECT_SOURCE_DIR}/../../library/Include/
		${PROJECT_SOURCE_DIR}/../../library/src/
		SYSTEM ${Boost_INCLUDE_DIRS}
		)

IF(WIN32)
    LINK_DIRECTORIES(${JAVA_LIBRARIES} ${Boost_LIBRARIES})
ENDIF()

# Add "-outdir <dir>" to SWIG command
SET(CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/Java/src")
# Add "-c++" option to SWIG command
SET_SOURCE_FILES_PROPERTIES(${SWIG_INTERFACE_FILE} PROPERTIES CPLUSPLUS ON)
SET_PROPERTY(SOURCE ${SWIG_INTERFACE_FILE} PROPERTY SWIG_FLAGS "-package" "org.epsg.openconfigurator.lib.wrapper")
SWIG_ADD_MODULE(openconfigurator_core_wrapper_java java ${SWIG_INTERFACE_FILE})
#SET_TARGET_PROPERTIES(openconfigurator_core_wrapper_java PROPERTIES PREFIX "")

# Create openPOWERLINK wrapper library
SWIG_LINK_LIBRARIES(openconfigurator_core_wrapper_java openconfigurator_core_lib ${JAVA_LIBRARIES} ${Boost_LIBRARIES})

###############################################################################
# Compile SWIG-Generated Java-Sources using javac and create a *.jar
###############################################################################
SET(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.7" "-target" "1.7")
IF (OPEN_CONFIGURATOR_CORE_JAVA_WRAPPER_DEBUG_INFO)
	SET(CMAKE_JAVA_COMPILE_FLAGS "${CMAKE_JAVA_COMPILE_FLAGS}" "-g")
ENDIF()

FILE(GLOB JAVA_SOURCE_FILES ${CMAKE_CURRENT_BINARY_DIR}/Java/src/*.java)
ADD_JAR(org.epsg.openconfigurator.lib.wrapper.v1.0
  SOURCES ${JAVA_SOURCE_FILES}
  OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Java/bin"
)
ADD_DEPENDENCIES(org.epsg.openconfigurator.lib.wrapper.v1.0 openconfigurator_core_wrapper_java)

###############################################################################
# Install
###############################################################################
INSTALL(
TARGETS openconfigurator_core_wrapper_java
LIBRARY DESTINATION ${INSTALL_DIR}
)

INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/Java/bin/org.epsg.openconfigurator.lib.wrapper.v1.0.jar" DESTINATION ${INSTALL_DIR})