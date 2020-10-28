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

# Support out-of-source builds.
include_directories(${CMAKE_BINARY_DIR})

#find packages

find_path(RELIC_INCLUDE_DIR relic/relic.h)
find_library(RELIC_LIBRARY NAMES relic)
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args(RELIC DEFAULT_MSG RELIC_INCLUDE_DIR RELIC_LIBRARY)
if(RELIC_FOUND)
	set(RELIC_LIBRARIES ${RELIC_LIBRARY})
	set(RELIC_INCLUDE_DIRS ${RELIC_INCLUDE_DIR})
endif()

if (GMP_INCLUDE_DIR AND GMP_LIBRARIES)
		set(GMP_FIND_QUIETLY TRUE)
endif (GMP_INCLUDE_DIR AND GMP_LIBRARIES)
mark_as_advanced(GMP_INCLUDE_DIR GMP_LIBRARIES)

find_path(GMP_INCLUDE_DIR NAMES gmp.h )
find_library(GMP_LIBRARIES NAMES gmp libgmp )
find_library(GMPXX_LIBRARIES NAMES gmpxx libgmpxx)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system date_time thread chrono)

include_directories(${CMAKE_SOURCE_DIR} ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${GMP_INCLUDE_DIR} ${RELIC_INCLUDE_DIR})

add_executable(test_s test_s.cpp)

message(STATUS "LIBS:" ${OPENSSL_LIBRARIES} " | " ${Boost_LIBRARIES} " | " ${GMP_LIBRARIES} " | " ${GMPXX_LIBRARIES} " | " ${RELIC_LIBRARY})
                                             
target_link_libraries(test_s ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${GMP_LIBRARIES} ${GMPXX_LIBRARIES} ${RELIC_LIBRARY} libOTe cryptoTools KyberOT)

