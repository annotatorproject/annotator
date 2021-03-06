include(ExternalProject)

if( NOT annotatorlib_FOUND AND OPTION_BUILD_ANNOTATORLIB)

ExternalProject_Add(
    annotatorlib
    GIT_REPOSITORY https://github.com/annotatorproject/annotatorlib.git
    GIT_TAG master
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/annotatorlib
    INSTALL_COMMAND ""
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DOPTION_BUILD_TESTS=Off
)

ExternalProject_Get_Property(annotatorlib source_dir binary_dir)

add_library(annotatorlib::annotatorlib IMPORTED STATIC GLOBAL)
add_dependencies(annotatorlib::annotatorlib annotatorlib)

set_target_properties(annotatorlib::annotatorlib PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libannotatorlib.so"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

include_directories("${source_dir}/source/annotatorlib/include"
		"${binary_dir}/source/annotatorlib/include")

endif( NOT annotatorlib_FOUND AND OPTION_BUILD_ANNOTATORLIB)

if( OPTION_BUILD_TESTS )

ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(gtest source_dir binary_dir)

add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

endif( OPTION_BUILD_TESTS )

## CrashCpp

if( OPTION_INCLUDE_CRASHCPP )

ExternalProject_Add(
    crashcpp
    GIT_REPOSITORY https://github.com/falsecam/crashcpp.git
    GIT_TAG master
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/crashcpp
    INSTALL_COMMAND ""
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DOPTION_BUILD_TESTS=Off source
)

ExternalProject_Get_Property(crashcpp source_dir binary_dir)

file(COPY "${binary_dir}/libcrashcpp.so" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(libcrashcpp IMPORTED STATIC GLOBAL)
add_dependencies(libcrashcpp crashcpp)


set_target_properties(libcrashcpp PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libcrashcpp.so"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

include_directories("${source_dir}/source/crashcpp/include"
		"${binary_dir}/source/crashcpp/include")

add_definitions(-DOPTION_INCLUDE_CRASHCPP)
set(crashcpp_LIB libcrashcpp)

endif(OPTION_INCLUDE_CRASHCPP)
