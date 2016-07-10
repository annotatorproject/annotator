
# Check if examples are enabled
if(NOT OPTION_BUILD_PLUGINS)
    return()
endif()

# plugins

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MAIN ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/plugins")

add_subdirectory(camshift)
add_subdirectory(correlationtracker)
add_subdirectory(haarcascade)
add_subdirectory(kalmanfilter)
add_subdirectory(templatematcher)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_MAIN})
