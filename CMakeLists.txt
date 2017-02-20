cmake_minimum_required(VERSION 2.8)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

set(PROJECT_NAME TemplateName)
project( ${PROJECT_NAME} )

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)


set(WERROR_FLAG "-Werror")
if( not CMAKE_COMPILER_IS_GNUCXX)
  # Windows does not accept -Werror
  set(WERROR_FLAG "-W4")
endif()

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic -g ${WERROR_FLAG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4")



# Versioning
# Replace @VARIABLES@ with CMake values
if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.version.in)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/.version.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp"
    )
endif()

# Source
file(GLOB_RECURSE  
	Project_SOURCES
       ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    )
add_executable( ${PROJECT_NAME} ${Project_SOURCES} )

#add_custom_target( clean
#  COMMAND "$(CMAKE)" --build "${CMAKE_BINARY_DIR}" clean
#  COMMAND cd "${CMAKE_BINARY_DIR}/otherproject" && make clean
#)


########################### Libraries ##########################################

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
  target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} m)  # if using maths.h
  add_definitions(
    -lGL
    -lGLU
    -DGL_GLEXT_PROTOTYPES 
	)
endif()


# OpenCV
# find_package(OpenCV REQUIRED core imgproc highgui video ml)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
if (OPENCV_FOUND)
  target_link_libraries(${PROJECT_NAME} ${OPENCV_LIBS})
  target_link_libraries(${PROJECT_NAME} m)  # if using maths.h
endif()
  

# SFML
# SFML unknown to CMAKE by default -> need to copy FindSFML.cmake to /usr/shared/cmake-2.8/Modules
#find_package(SFML 1.6 REQUIRED system window graphics network audio)
find_package(SFML 2 REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
  add_definitions(
    -lsfml-graphics
    -lsfml-window
    -lsfml-system
	)
endif()


# boost::filesystem
find_package(Boost 1.34.0 REQUIRED system filesystem)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

# Create installer
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "My funky project")
set(CPACK_PACKAGE_VENDOR "Thibault Lucidarme")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}\\\\InstallIcon.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${PROJECT_NAME}.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} My Famous Project")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\github.com\\\\ThibaultLucidarme?tab=repositories")
  set(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
  set(CPACK_NSIS_MODIFY_PATH ON)
else(WIN32 AND NOT UNIX)
  set(CPACK_STRIP_FILES "bin/${PROJECT_NAME}")
  set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)
set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME} ${PROJECT_NAME})
include(CPack)
