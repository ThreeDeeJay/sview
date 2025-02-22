# This script finds libconfig++ library.
# The script requires:
#  LIBCONFIGCPP_DIR - root folder or folder with CMake configuration files
#
# Script will define the following variables on success:
#  LIBCONFIGCPP_FOUND        - package is successfully found
#  LIBCONFIGCPP_INCLUDE_DIR  - directory with headers
#  LIBCONFIGCPP_LIBRARY_DIR  - directory with libraries for linker
#  LIBCONFIGCPP_BINARY_DIR   - directory with DLLs
#  LIBCONFIGCPP_RESOURCE_DIR - directory with resource files
include(FindPackageHandleStandardArgs)

set (LIBCONFIGCPP_DIR "" CACHE PATH "Path to OpenAL library.")

# default paths
set (LIBCONFIGCPP_INCLUDE_DIR  "${LIBCONFIGCPP_DIR}/include")
set (LIBCONFIGCPP_LIBRARY_DIR  "${LIBCONFIGCPP_DIR}/lib")
set (LIBCONFIGCPP_BINARY_DIR   "${LIBCONFIGCPP_DIR}/bin")
set (LIBCONFIGCPP_RESOURCE_DIR "${LIBCONFIGCPP_DIR}/res")

#  list of toolkits
set (LIBCONFIGCPP_TKLIST "config++")
set (LIBCONFIGCPP_TKNAME "config++")

# validate location of libraries and headers
set (LIBCONFIGCPP_INCLUDE_DIR_FOUND)
set (LIBCONFIGCPP_LIBRARY_DIR_FOUND)
set (LIBCONFIGCPP_IMPLIB_SUFFIX ${CMAKE_STATIC_LIBRARY_SUFFIX})
set (LIBCONFIGCPP_SHAREDLIB_FOUND)
if (EXISTS "${LIBCONFIGCPP_INCLUDE_DIR}/libconfig.h++")
  set (LIBCONFIGCPP_INCLUDE_DIR_FOUND ON)
endif()

if (ANDROID AND EXISTS "${LIBCONFIGCPP_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/lib${LIBCONFIGCPP_TKNAME}.so")
  set (LIBCONFIGCPP_LIBRARY_DIR_FOUND ON)
  set (LIBCONFIGCPP_LIBRARY_DIR "${LIBCONFIGCPP_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}")
  set (LIBCONFIGCPP_IMPLIB_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
elseif (ANDROID AND EXISTS "${LIBCONFIGCPP_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/lib${LIBCONFIGCPP_TKNAME}.a")
  set (LIBCONFIGCPP_LIBRARY_DIR_FOUND ON)
  set (LIBCONFIGCPP_LIBRARY_DIR "${LIBCONFIGCPP_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}")
elseif (EXISTS "${LIBCONFIGCPP_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBCONFIGCPP_TKNAME}${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set (LIBCONFIGCPP_LIBRARY_DIR_FOUND ON)
elseif (NOT WIN32 AND EXISTS "${LIBCONFIGCPP_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBCONFIGCPP_TKNAME}${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set (LIBCONFIGCPP_LIBRARY_DIR_FOUND ON)
  set (LIBCONFIGCPP_IMPLIB_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()

if (WIN32)
  if (EXISTS "${LIBCONFIGCPP_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBCONFIGCPP_TKNAME}${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set (LIBCONFIGCPP_SHAREDLIB_FOUND ON)
  endif()
else()
  if (EXISTS "${LIBCONFIGCPP_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBCONFIGCPP_TKNAME}${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set (LIBCONFIGCPP_SHAREDLIB_FOUND ON)
  endif()
endif()

if (LIBCONFIGCPP_INCLUDE_DIR_FOUND AND LIBCONFIGCPP_LIBRARY_DIR_FOUND)
  set (LIBCONFIGCPP_FOUND ON)
  set (LIBCONFIGCPP_INSTALL_PREFIX ${LIBCONFIGCPP_DIR})
  set (LIBCONFIGCPP_LIBRARY "${LIBCONFIGCPP_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBCONFIGCPP_TKNAME}${LIBCONFIGCPP_IMPLIB_SUFFIX}")

  # Define toolkits so that CMake can put absolute paths to linker; the library existence is not checked here.
  add_library (libconfig++ SHARED IMPORTED)

  set_property (TARGET libconfig++ APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
  set_target_properties (libconfig++ PROPERTIES IMPORTED_IMPLIB "${LIBCONFIGCPP_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBCONFIGCPP_TKNAME}${LIBCONFIGCPP_IMPLIB_SUFFIX}")
  set_target_properties (libconfig++ PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${LIBCONFIGCPP_INCLUDE_DIR})
  if (LIBCONFIGCPP_SHAREDLIB_FOUND)
    if (WIN32)
      set_target_properties (libconfig++ PROPERTIES IMPORTED_LOCATION "${LIBCONFIGCPP_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBCONFIGCPP_TKNAME}${CMAKE_SHARED_LIBRARY_SUFFIX}")
    else()
      set_target_properties (libconfig++ PROPERTIES IMPORTED_LOCATION "${LIBCONFIGCPP_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBCONFIGCPP_TKNAME}${CMAKE_SHARED_LIBRARY_SUFFIX}")
    endif()
  endif()
elseif ("${LIBCONFIGCPP_DIR}" STREQUAL "")
  # fallback
  unset (LIBCONFIGCPP_INCLUDE_DIR)
  unset (LIBCONFIGCPP_LIBRARY)
  #find_package (libconfig++ REQUIRED)
  find_path    (LIBCONFIGCPP_INCLUDE_DIR "libconfig.h++")
  find_library (LIBCONFIGCPP_LIBRARY "config++" CMAKE_FIND_ROOT_PATH_BOTH)
  if (LIBCONFIGCPP_LIBRARY AND EXISTS "${LIBCONFIGCPP_LIBRARY}")
    set (LIBCONFIGCPP_FOUND ON)
    add_library (libconfig++ SHARED IMPORTED)
    set_property (TARGET libconfig++ APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
    set_target_properties (libconfig++ PROPERTIES IMPORTED_IMPLIB "${LIBCONFIGCPP_LIBRARY}")
    set_target_properties (libconfig++ PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LIBCONFIGCPP_INCLUDE_DIR}")
    set_target_properties (libconfig++ PROPERTIES IMPORTED_LOCATION "${LIBCONFIGCPP_LIBRARY}")
  endif()
else()
  set (LIBCONFIGCPP_FOUND OFF)
endif()
