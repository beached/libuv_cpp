cmake_minimum_required( VERSION 2.8.12 )

project( libuv_cpp_prj )

find_package( Boost 1.60.0 COMPONENTS system unit_test_framework REQUIRED )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")

find_package( Libuv REQUIRED )

enable_testing( )
add_definitions( -DBOOST_ALL_NO_LIB )

#find_package( Threads REQUIRED )
set( CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested.")

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_compile_options( -D_WIN32_WINNT=0x0601 )
else( )
	if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" )
		add_compile_options(-march=native -pthread -pedantic -Weverything -Wno-c++98-compat -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options(-march=native -Wall -Wno-noexcept-type -Wno-deprecated-declarations -Wduplicated-cond -Wlogical-op -Wnull-dereference -Wold-style-cast -Wshadow)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	endif( )
endif( )

if( EXISTS "${CMAKE_SOURCE_DIR}/dependent_projects/build" )
	add_custom_target( dependency_stub )
	include_directories( SYSTEM "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/include" )
	link_directories( "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/lib" )
else( )
	set( SHOULD_BUILD_DEPS ON )
	include( "${CMAKE_SOURCE_DIR}/dependent_projects/dependencies.txt" )

	add_library( dependency_stub STATIC "${CMAKE_SOURCE_DIR}/dependent_projects/stub.cpp" )
	add_dependencies( dependency_stub header_libraries_prj date_prj )
endif( )

set( HEADER_FOLDER "include" )
set( SOURCE_FOLDER "src" )
set( TEST_FOLDER "tests" )

include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
include_directories( SYSTEM ${LIBUV_INCLUDE_DIR} )
include_directories( ${HEADER_FOLDER} )

include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )

link_directories( ${Boost_LIBRARY_DIRS} )

set( HEADER_FILES
	${HEADER_FOLDER}/daw/libuv_cpp.h
)

set( SOURCE_FILES
	${SOURCE_FOLDER}/libuv_cpp.cpp
)

add_definitions( -DBOOST_TEST_DYN_LINK -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK )

add_custom_target( check COMMAND ${CMAKE_CTEST_COMMAND} )

add_library( uv_cpp ${HEADER_FILES} ${SOURCE_FILES} )
target_link_libraries( uv_cpp ${LIBUV_LIBRARIES} )
add_dependencies( uv_cpp dependency_stub )

add_executable( libuv_cpp_test_bin EXCLUDE_FROM_ALL ${TEST_FOLDER}/libuv_cpp_test.cpp )
target_link_libraries( libuv_cpp_test_bin uv_cpp ${Boost_LIBRARIES} ${LIBUV_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
add_test( libuv_cpp_test libuv_cpp_test_bin )
add_dependencies( check libuv_cpp_test_bin )

#install( TARGETS function_stream_lib task_scheduler_lib DESTINATION lib )
install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw/fs )

