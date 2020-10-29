cmake_minimum_required (VERSION 3.12)

project(test_s)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")

	############################################
	#          If top level cmake              #
	############################################
	if(${CMAKE_VERSION} VERSION_LESS "3.12.0") 
		message("Please consider updating CMake to 3.12+")
	endif()

	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)

	############################################
	#           Flag and #defines              #
	############################################
	add_definitions(-DSOLUTION_DIR='${CMAKE_SOURCE_DIR}')
	set(COMMON_FLAGS "-Wall -march=native -Wfatal-errors -fPIC")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
	
	# -Wno-ignored-attributes -Wno-parentheses
	# -maes -msse2 -msse3 -msse4.1 -mpclmul  
	# -std=c++14
	# -fPIC -no-pie 

	# Select flags.
	SET(CMAKE_CXX_FLAGS_RELEASE "-O3  -DNDEBUG -fPIC") 
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO " -O2 -g -ggdb -rdynamic -fPIC")
	SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g -ggdb -rdynamic -fPIC")
	

	############################################
	#           Build mode checks              #
	############################################
	
	# Set a default build type for single-configuration
	# CMake generators if no build type is set.
	if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	   SET(CMAKE_BUILD_TYPE Release)
	endif()

	if(    NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release"
       AND NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" 
       AND NOT "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" )
		  
        message(FATAL_ERROR ": Unknown build type - \${CMAKE_BUILD_TYPE}=${CMAKE_BUILD_TYPE}.  Please use one of Debug, Release, or RelWithDebInfo. e.g. call\n\tcmake . -DCMAKE_BUILD_TYPE=Release\n" )        
	endif()
endif()



add_executable(test_s test_s.cpp)

## libOTe
###########################################################################


find_path(libOTe_INCLUDE_DIR NAMES libOTe/config.h )
find_library(libOTe_LIB NAMES libOTe)

find_path(cryptoTools_INCLUDE_DIR NAMES cryptoTools/Common/config.h )
find_library(cryptoTools_LIB NAMES cryptoTools)
	

if (NOT EXISTS ${libOTe_LIB})
	message(FATAL_ERROR "Failed to find libOTe: ${libOTe_LIB}")
endif ()
if (NOT EXISTS ${libOTe_INCLUDE_DIR})
	message(FATAL_ERROR "Failed to find libOTe include: ${libOTe_INCLUDE_DIR}")
endif ()

message(STATUS "libOTe_LIB:  ${libOTe_LIB} ${cryptoTools_LIB}")
message(STATUS "libOTe_inc: ${libOTe_INCLUDE_DIR} ${cryptoTools_INCLUDE_DIR}\n")

target_link_libraries(test_s ${libOTe_LIB} ${cryptoTools_LIB})

## Relic
###########################################################################

  find_package(Relic REQUIRED)

  if (NOT Relic_FOUND)
    message(FATAL_ERROR "Failed to find Relic")
  endif (NOT Relic_FOUND)

  message(STATUS "Relic_LIB:  ${RELIC_LIBRARIES} ${RLC_LIBRARY}")
  message(STATUS "Relic_inc:  ${RELIC_INCLUDE_DIR} ${RLC_INCLUDE_DIR}\n")

  target_include_directories(test_s PUBLIC ${RELIC_INCLUDE_DIR} ${RLC_INCLUDE_DIR}) 
  target_link_libraries(test_s ${RELIC_LIBRARIES} ${RLC_LIBRARY})



## Boost
###########################################################################

set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/linux/boost/")

if(EXISTS ${BOOST_ROOT})
	set(Boost_NO_SYSTEM_PATHS ON)
endif()

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     ON)

macro(findBoost)
	find_package(Boost 1.69.0 COMPONENTS system thread)
endmacro()

findBoost()
# then look at system dirs
if(NOT Boost_FOUND)
	set(Boost_NO_SYSTEM_PATHS  OFF)
	findBoost()
endif()

if(NOT Boost_FOUND)
	message(FATAL_ERROR "Failed to find boost 1.69.0 or newer. Looked at system dirs and: " ${BOOST_ROOT})
endif()

target_include_directories(test_s PUBLIC ${Boost_INCLUDE_DIR}) 
target_link_libraries(test_s ${Boost_LIBRARIES})

message(STATUS "Boost_LIB: ${Boost_LIBRARIES}" )
message(STATUS "Boost_INC: ${Boost_INCLUDE_DIR}\n\n" )

## GMP
###########################################################################

if (GMP_INCLUDE_DIR AND GMP_LIBRARIES)#
		set(GMP_FIND_QUIETLY TRUE)
endif (GMP_INCLUDE_DIR AND GMP_LIBRARIES)
mark_as_advanced(GMP_INCLUDE_DIR GMP_LIBRARIES)

find_path(GMP_INCLUDE_DIR NAMES gmp.h )
find_library(GMP_LIBRARIES NAMES gmp libgmp )
find_library(GMPXX_LIBRARIES NAMES gmpxx libgmpxx)
#find_package(GMP REQUIRED)

message(STATUS "GMP_LIB: ${GMPXX_LIBRARIES} ${GMP_LIBRARIES}" )
message(STATUS "GMP_INC: ${GMP_INCLUDE_DIR}\n\n" )

target_link_libraries(test_s  gmp gmpxx)

## OpenSSL
###########################################################################
find_package(OpenSSL REQUIRED)
target_link_libraries(test_s ${OPENSSL_LIBRARIES} )