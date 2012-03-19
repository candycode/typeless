#Author: Ugo Varetto

cmake_minimum_required(VERSION 2.8)

project(any-test)

#::::: LIBRARIES :::::#
#BOOST
set( BOOST_INCLUDE_DIR "/usr/include" CACHE PATH "Boost include directory" )
set( BOOST_LIB_DIR "/usr/lib" CACHE PATH "Boost lib directory" )
set( BOOST_LIBRARIES "boost_thread" CACHE FILEPATH "Boost libraries" )

set( BOOST_ATOMIC_INCLUDE_DIR "../utility/boost.atomic" )

set( ANY_INCLUDE_DIR "../include" CACHE PATH "any include directory" )

include_directories( ${BOOST_INCLUDE_DIR} ${BOOST_ATOMIC_INCLUDE_DIR} ${ANY_INCLUDE_DIR} ) 
link_directories( ${BOOST_LIB_DIR} )

#::::: BUILD TARGETS :::::# 
add_executable( any-test any-test.cpp )
add_executable( anyoperators-test anyoperators-test.cpp )
add_executable( anystorage-test anystorage-test.cpp )
target_link_libraries( anystorage-test ${BOOST_LIBRARIES} )
