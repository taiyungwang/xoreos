# xoreos - A reimplementation of BioWare's Aurora engine
#
# xoreos is the legal property of its developers, whose names
# can be found in the AUTHORS file distributed with this source
# distribution.
#
# xoreos is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
#
# xoreos is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with xoreos. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.1.0)

project(xoreos CXX)
set(xoreos_VERSION 0.0.6)


# -------------------------------------------------------------------------
# options!
option(Boost_USE_STATIC_LIBS "Use Boost static libraries" OFF)
option(GLEW_INTERNAL "Use internal glew libraries" OFF)
option(XOREOS_BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
option(ENABLE_VORBIS "Enable building with Vorbis support" ON)
option(ENABLE_MAD "Enable building with MP3 support" ON)
option(ENABLE_FAAD "Enable building with AAC support" ON)
option(ENABLE_XVIDCORE "Enable building with H.263 support" ON)
option(ENABLE_VPX "Enable building with VP8/VP9 support" ON)
option(ENABLE_LZMA "Enable building with LZMA support" ON)
option(ENABLE_XML "Enable building with XML support" ON)


# -------------------------------------------------------------------------
# default build type

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


# -------------------------------------------------------------------------
# load some cmake modules from cmake/ subfolder
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(GNUInstallDirs)
include(CheckIncludeFileCXX)
include(CMakeAM)
include(SetCheckCompilerFlag)


# -------------------------------------------------------------------------
# platform specific flags
if(CMAKE_HOST_APPLE)
  add_definitions(-DUNIX -DMACOSX)

elseif(CMAKE_HOST_UNIX)
  add_definitions(-DUNIX)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdata-sections -ffunction-sections -fno-show-column")

  # Wrap libraries in --start-group and --end-group to easily support static linking and symbol resolution, maybe useful on APPLE also, but I don't know
  string(REPLACE "<LINK_LIBRARIES>" "-Wl,--gc-sections -Wl,--start-group <LINK_LIBRARIES> -Wl,--end-group" CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE}")
  string(REPLACE "<LINK_LIBRARIES>" "-Wl,--start-group <LINK_LIBRARIES> -Wl,--end-group" CMAKE_CXX_CREATE_SHARED_LIBRARY "${CMAKE_CXX_CREATE_SHARED_LIBRARY}")
  string(REPLACE "<LINK_LIBRARIES>" "-Wl,--start-group <LINK_LIBRARIES> -Wl,--end-group" CMAKE_CXX_CREATE_SHARED_MODULE "${CMAKE_CXX_CREATE_SHARED_MODULE}")
elseif(CMAKE_HOST_WIN32)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

    if(CMAKE_CL_64)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D FPM_64BIT")
    endif()

    link_libraries(winmm.lib imm32.lib version.lib)
    if((${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.2) OR (${CMAKE_SYSTEM_VERSION} VERSION_GREATER 6.2))
      link_libraries(mincore.lib)
    endif()
  endif()
else()
  message(STATUS "Unknown platform, maybe not supported")
endif()

# Force BOOST_SCOPE_EXIT to behave in lambda mode
add_definitions(-DBOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS)

# C++ standard we're compiling against
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_HOST_WIN32 AND ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
  # Needed by _wfopen in src/common/platform.cpp for mingw32 (but technically not for mingw-w64)
  set(CMAKE_CXX_EXTENSIONS ON)
endif()

# Compiler warning flags, not for MSVC
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # Extra warnings valid for both C and C++
  set_check_compiler_flag_cxx("-Wall")
  set_check_compiler_flag_cxx("-Wignored-qualifiers")
  set_check_compiler_flag_cxx("-Wpointer-arith")
  set_check_compiler_flag_cxx("-Wshadow")
  set_check_compiler_flag_cxx("-Wsign-compare")
  set_check_compiler_flag_cxx("-Wtype-limits")
  set_check_compiler_flag_cxx("-Wuninitialized")
  set_check_compiler_flag_cxx("-Wunused-parameter")
  set_check_compiler_flag_cxx("-Wunused-but-set-parameter")
  set_check_compiler_flag_cxx("-Wduplicated-cond")
  set_check_compiler_flag_cxx("-Wduplicated-branches")
  set_check_compiler_flag_cxx("-Wlogical-op")
  set_check_compiler_flag_cxx("-Wshift-negative-value")
  set_check_compiler_flag_cxx("-Wshift-overflow=2")
  set_check_compiler_flag_cxx("-Wimplicit-fallthrough")
  set_check_compiler_flag_cxx("-Wvla")
  set_check_compiler_flag_cxx("-Wcast-function-type")
  set_check_compiler_flag_cxx("-Wnull-pointer-subtraction")

  # Extra warnings valid for C++
  set_check_compiler_flag_cxx("-Wnon-virtual-dtor")
  set_check_compiler_flag_cxx("-Wdeprecated-copy")
  set_check_compiler_flag_cxx("-Wdeprecated-copy-dtor")
  set_check_compiler_flag_cxx("-Wredundant-move")
  set_check_compiler_flag_cxx("-Wctad-maybe-unsupported")

  # Disable warnings about hiding virtual functions from a base class. We don't care
  set_check_compiler_flag_cxx("-Woverloaded-virtual" "-Wno-overloaded-virtual")

  # Disable warnings about the long long type. We need it
  set_check_compiler_flag_cxx("-Wlong-long" "-Wno-long-long")
  set_check_compiler_flag_cxx("-Wc++11-long-long" "-Wno-c++11-long-long")

  # clang is far too trigger-happy on this warning, so let's disable it
  set_check_compiler_flag_cxx("-Wundefined-var-template" "-Wno-undefined-var-template")

  # We do not yet mark any of our overrides. Let's disable this for now
  set_check_compiler_flag_cxx("-Winconsistent-missing-override" "-Wno-inconsistent-missing-override")

  # We're far from stable, so we still have several of those
  set_check_compiler_flag_cxx("-Wunused-private-field" "-Wno-unused-private-field")

  # Disable warnings about unused local typedefs that came with GLM
  set_check_compiler_flag_cxx("-Wunused-local-typedefs" "-Wno-unused-local-typedefs")

  # Disable warnings about unused static const variables
  set_check_compiler_flag_cxx("-Wunused-const-variable" "-Wno-unused-const-variable")

  # Disable warnings about >= 0 comparisons on enums
  set_check_compiler_flag_cxx("-Wtautological-unsigned-enum-zero-compare" "-Wno-tautological-unsigned-enum-zero-compare")
endif()

# platform specific warning settings
if(CMAKE_HOST_WIN32)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
      string(REGEX REPLACE "/W[0-4]" "/W3" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    endif()

    set(WARNINGS_DISABLE
      # ignore
      4250 # 'class1' : inherits 'class2::member' via dominance
      #4101 # 'identifier' : unreferenced local variable
      4100 # 'identifier' : unreferenced formal parameter
      4127 # conditional expression is constant (template parameter already evaluated)
      4189 # 'identifier' : local variable is initialized but not referenced
      4245 # 'conversion' : conversion from 'type1' to 'type2', signed/unsigned mismatch
      4435 # 'class1' : Object layout under /vd2 will change due to virtual base 'class2'
      4510 # 'class' : default constructor could not be generated
      4512 # 'class' : assignment operator could not be generated
      4610 # object 'class' can never be instantiated - user-defined constructor required
      #4702 # unreachable code
      4706 # assignment within conditional expression
      4710 # 'function' : function not inlined
      4714 # function 'function' marked as __forceinline not inlined

      # investigate later
      4305 # 'identifier' : truncation from 'type1' to 'type2'
      4244 # 'argument' : conversion from 'type1' to 'type2', possible loss of data
      4267 # 'var' : conversion from 'size_t' to 'type', possible loss of data
      4996 # 'function': was declared deprecated

      # investigate now
      #4005 # 'identifier' : macro redefinition
      #4800 # 'type' : forcing value to bool 'true' or 'false' (performance warning)
      #4146 # unary minus operator applied to unsigned type, result still unsigned
      )

    foreach(d ${WARNINGS_DISABLE})
      set(WARNINGS "${WARNINGS} /wd${d}")
    endforeach(d)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS}")
  endif()
endif()


# header detection, could get rid of it, but it is used by lua as well
include(CheckIncludeFileCXX)
function(check_has_header FILE_NAME PP_NAME)
  check_include_file_cxx(${FILE_NAME} ${PP_NAME})
  if(${PP_NAME})
    add_definitions(-D${PP_NAME}=1)
  endif()
endfunction()

check_has_header("stdint.h"    HAVE_STDINT_H)
check_has_header("inttypes.h"  HAVE_INTTYPES_H)
check_has_header("sys/types.h" HAVE_SYS_TYPES_H)

# type size checks
include(CheckTypeSize)
check_type_size("char"      SIZEOF_CHAR      LANGUAGE CXX)
check_type_size("short"     SIZEOF_SHORT     LANGUAGE CXX)
check_type_size("int"       SIZEOF_INT       LANGUAGE CXX)
check_type_size("long"      SIZEOF_LONG      LANGUAGE CXX)
check_type_size("long long" SIZEOF_LONG_LONG LANGUAGE CXX)
check_type_size("void *"    SIZEOF_VOID_P    LANGUAGE CXX)
check_type_size("intptr_t"  SIZEOF_INTPTR_T  LANGUAGE CXX)
check_type_size("uintptr_t" SIZEOF_UINTPTR_T LANGUAGE CXX)

if(HAVE_SIZEOF_CHAR AND SIZEOF_CHAR)
  add_definitions(-DSIZEOF_CHAR=${SIZEOF_CHAR})
endif()
if(HAVE_SIZEOF_SHORT AND SIZEOF_SHORT)
  add_definitions(-DSIZEOF_SHORT=${SIZEOF_SHORT})
endif()
if(HAVE_SIZEOF_INT AND SIZEOF_INT)
  add_definitions(-DSIZEOF_INT=${SIZEOF_INT})
endif()
if(HAVE_SIZEOF_LONG AND SIZEOF_LONG)
  add_definitions(-DSIZEOF_LONG=${SIZEOF_LONG})
endif()
if(HAVE_SIZEOF_LONG_LONG AND SIZEOF_LONG_LONG)
  add_definitions(-DSIZEOF_LONG_LONG=${SIZEOF_LONG_LONG})
endif()
if(HAVE_SIZEOF_VOID_P AND SIZEOF_VOID_P)
  add_definitions(-DSIZEOF_VOID_P=${SIZEOF_VOID_P})
endif()

if(HAVE_SIZEOF_INTPTR_T AND SIZEOF_INTPTR_T)
  add_definitions(-DHAVE_INTPTR_T=1)
endif()
if(HAVE_SIZEOF_UINTPTR_T AND SIZEOF_UINTPTR_T)
  add_definitions(-DHAVE_UINTPTR_T=1)
endif()

# function detection
include(CheckCXXSymbolExists)
function(check_has_function FUNCTION_NAME FILE_NAME PP_NAME)
  check_cxx_symbol_exists(${FUNCTION_NAME} "${FILE_NAME}" ${PP_NAME})
  if(${PP_NAME})
    add_definitions(-D${PP_NAME}=1)
  endif()
endfunction()

check_has_function(strtof   "cstdlib" HAVE_STRTOF)
check_has_function(strtoll  "cstdlib" HAVE_STRTOLL)
check_has_function(strtoull "cstdlib" HAVE_STRTOULL)


# endianess detection, could be replaced by including Boost.Config
include(TestBigEndian)
# CMake's endian check needs C and won't work with just CXX...
enable_language(C)
test_big_endian(XOREOS_BIG_ENDIAN)
if(XOREOS_BIG_ENDIAN)
  add_definitions(-DXOREOS_BIG_ENDIAN=1)
else()
  add_definitions(-DXOREOS_LITTLE_ENDIAN=1)
endif()


# -------------------------------------------------------------------------
# subfolders where built binaries and libraries will be located, relative to the build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


# -------------------------------------------------------------------------
# find the required libraries
set(XOREOS_LIBRARIES "")

find_package(Iconv REQUIRED)
include_directories(${Iconv_INCLUDE_DIRS})
list(APPEND XOREOS_LIBRARIES ${Iconv_LIBRARIES})

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
list(APPEND XOREOS_LIBRARIES ${ZLIB_LIBRARIES})

find_package(Boost COMPONENTS system filesystem date_time locale REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND XOREOS_LIBRARIES ${Boost_LIBRARIES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
list(APPEND XOREOS_LIBRARIES ${SDL2_LIBRARY})

find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})
list(APPEND XOREOS_LIBRARIES ${OPENAL_LIBRARY})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
link_directories(${FREETYPE_LIBRARY_DIRS})
list(APPEND XOREOS_LIBRARIES ${FREETYPE_LIBRARIES})

if(ENABLE_MAD)
  find_package(Mad)
  if(MAD_FOUND)
    add_definitions(-DENABLE_MAD)
    include_directories(${MAD_INCLUDE_DIR})
    list(APPEND XOREOS_LIBRARIES ${MAD_LIBRARY})
  else()
    set(ENABLE_MAD OFF)
  endif()
endif()

if(ENABLE_FAAD)
  find_package(Faad)
  if(FAAD_FOUND)
    add_definitions(-DENABLE_FAAD)
    include_directories(${FAAD_INCLUDE_DIR})
    list(APPEND XOREOS_LIBRARIES ${FAAD_LIBRARY})
  else()
    set(ENABLE_FAAD OFF)
  endif()
endif()

if(ENABLE_VORBIS)
  find_package(Ogg)
  find_package(Vorbis)
  if(OGG_FOUND AND VORBIS_FOUND)
    add_definitions(-DENABLE_VORBIS)
    include_directories(${OGG_INCLUDE_DIR} ${VORBIS_INCLUDE_DIR})
    list(APPEND XOREOS_LIBRARIES ${OGG_LIBRARY} ${VORBIS_LIBRARIES})
  else()
    set(ENABLE_VORBIS OFF)
  endif()
endif()

if(ENABLE_XVIDCORE)
  find_package(XviD)
  if(XVID_FOUND)
    add_definitions(-DENABLE_XVIDCORE)
    include_directories(${XVID_INCLUDE_DIR})
    list(APPEND XOREOS_LIBRARIES ${XVID_LIBRARY})
  else()
    set(ENABLE_XVIDCORE OFF)
  endif()
endif()

if(ENABLE_VPX)
  find_package(VPX)
  if(VPX_FOUND)
    add_definitions(-DENABLE_VPX)
    include_directories(${VPX_INCLUDE_DIR})
    list(APPEND XOREOS_LIBRARIES ${VPX_LIBRARY})
  else()
    set(ENABLE_VPX OFF)
  endif()
endif()

if(ENABLE_LZMA)
  find_package(LibLZMA)
  if(LIBLZMA_FOUND)
    add_definitions(-DENABLE_LZMA)
    include_directories(${LIBLZMA_INCLUDE_DIR})
    list(APPEND XOREOS_LIBRARIES ${LIBLZMA_LIBRARY})
  else()
    set(ENABLE_LZMA OFF)
  endif()
endif()

if(ENABLE_XML)
  find_package(LibXml2)
  if(LIBXML2_FOUND)
    add_definitions(-DENABLE_XML)
    include_directories(${LIBXML2_INCLUDE_DIR})
    list(APPEND XOREOS_LIBRARIES ${LIBXML2_LIBRARIES})
  else()
    set(ENABLE_XML OFF)
  endif()
endif()

if(ICONV_SECOND_ARGUMENT_IS_CONST)
  add_definitions(-DICONV_CONST=const)
else(ICONV_SECOND_ARGUMENT_IS_CONST)
  add_definitions(-DICONV_CONST=)
endif(ICONV_SECOND_ARGUMENT_IS_CONST)

add_definitions(-DGLM_FORCE_CTOR_INIT)
add_definitions(-DGLEW_NO_GLU)

set(GLEW_FOUND 0)
if(NOT GLEW_INTERNAL)
  find_package(GLEW)
endif()

if(NOT GLEW_FOUND)
  message(STATUS "Using internal GLEW library")

  add_definitions(-DGLEW_STATIC)
  set(GLEW_LIBRARIES external_glew)

  add_definitions(-DXOREOS_INTERNAL_GLEW)
  set(INTERNAL_GLEW 1)

  parse_automake(external/glew/rules.mk)
else()
  message(STATUS "Using external GLEW library")
endif()
include_directories(${GLEW_INCLUDE_DIRS})
list(APPEND XOREOS_LIBRARIES ${GLEW_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
list(APPEND XOREOS_LIBRARIES ${OPENGL_LIBRARIES})

# pthreads
if(NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "MinGW")
  find_package(Threads)
endif()

set(HAVE_PTHREAD ${CMAKE_USE_PTHREADS_INIT})
if(CMAKE_USE_PTHREADS_INIT)
  set(PTHREAD_LIBS ${CMAKE_THREAD_LIBS_INIT})
  list(APPEND XOREOS_LIBRARIES ${PTHREAD_LIBS})
endif()

# use our own version of lua, as we will have to support the Witcher lua scripts
parse_automake(external/lua/rules.mk)
list(APPEND XOREOS_LIBRARIES external_lua)

parse_automake(external/toluapp/rules.mk)
list(APPEND XOREOS_LIBRARIES external_toluapp)

# use our own version of imgui.
parse_automake(external/imgui/rules.mk)
list(APPEND XOREOS_LIBRARIES external_imgui)

# use the opengl3 imgui renderer with glew.
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)

# libmspack
parse_automake(external/mspack/rules.mk)

function(add_engine ENGINE_NAME ENGINE_FULLNAME ENGINE_DEPS)
  option(ENABLE_${ENGINE_NAME} "Enable building with support for ${ENGINE_FULLNAME}" ON)
  if(ENABLE_${ENGINE_NAME} AND ENGINE_DEPS)
    set(MISSING_DEPS "")
    foreach(ENGINE_DEP ${ENGINE_DEPS})
      if(NOT ENABLE_${ENGINE_DEP})
        set(ENABLE_${ENGINE_NAME} OFF)
        set(ENABLE_${ENGINE_NAME} OFF PARENT_SCOPE)
        set(MISSING_DEPS "${MISSING_DEPS} ${ENGINE_DEP}")
      endif()
    endforeach(ENGINE_DEP)
    if(NOT ENABLE_${ENGINE_NAME})
      message(STATUS "Disabling ${ENGINE_FULLNAME} as the following dependencies are missing: ${MISSING_DEPS}")
    endif()
  endif()
  if(ENABLE_${ENGINE_NAME})
    add_definitions(-DENABLE_${ENGINE_NAME})
  endif()
endfunction()

add_engine(NWN "Neverwinter Nights" "")
add_engine(NWN2 "Neverwinter Nights 2" "")
add_engine(KOTOR "Star Wars: Knights of the Old Republic" "LZMA")
add_engine(KOTOR2 "Star Wars: Knights of the Old Republic II: The Sith Lords" "KOTOR")
add_engine(JADE "Jade Empire" "")
add_engine(WITCHER "The Witcher" "")
add_engine(SONIC "Sonic Chronicles: The Dark Brotherhood" "")
add_engine(DRAGONAGE "Dragon Age: Origins" "XML")
add_engine(DRAGONAGE2 "Dragon Age II" "XML")


# -------------------------------------------------------------------------
# custom unit testing target, because CMake's make test is pretty useless
add_custom_target(check ${CMAKE_COMMAND} -E echo CWD=${CMAKE_BINARY_DIR}
                  COMMAND ${CMAKE_COMMAND} -E echo CMD=${CMAKE_CTEST_COMMAND} -C $<CONFIG>
                  COMMAND ${CMAKE_COMMAND} -E echo ----------------------------------
                  COMMAND ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1 ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})


# -------------------------------------------------------------------------
# xoreos main targets, parsed from the Automake rules.mk files
include_directories(${PROJECT_SOURCE_DIR})
add_definitions(-DPACKAGE_STRING="xoreos ${xoreos_VERSION}")

parse_automake(src/rules.mk)

# unit tests depend on xoreos proper
foreach(AM_TARGET ${AM_TARGETS})
  add_dependencies(check ${AM_TARGET})
endforeach()

foreach(AM_PROGRAM ${AM_PROGRAMS})
  target_link_libraries(${AM_PROGRAM} ${XOREOS_LIBRARIES})
endforeach()

# install programs to bin dir
install(
 TARGETS ${AM_PROGRAMS}
 RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# -------------------------------------------------------------------------
# xoreos man pages and docs
parse_automake(man/rules.mk)

# install man pages
install(
  FILES ${AM_MAN1_MANS}
  DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
)
install(
  FILES ${AM_MAN6_MANS}
  DESTINATION ${CMAKE_INSTALL_MANDIR}/man6
)

parse_automake(doc/rules.mk)

# install docs
install(
  FILES ${AM_DOCS}
  DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

# -------------------------------------------------------------------------
# unit tests, parsed from the Automake rules.mk files
enable_testing()
parse_automake(tests/rules.mk)

# they should be build on make check, but not make all
foreach(AM_TARGET ${AM_TARGETS})
  set_target_properties(${AM_TARGET} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE EXCLUDE_FROM_ALL TRUE)
  add_dependencies(check ${AM_TARGET})

  # ctest counts skipped tests as failed... -.-
  get_target_property(TARGET_SOURCES ${AM_TARGET} SOURCES)
  foreach(TARGET_SOURCE ${TARGET_SOURCES})
    set_source_files_properties(${TARGET_SOURCE} PROPERTIES APPEND PROPERTY COMPILE_DEFINITIONS "SKIP_RETURN_CODE=0")
  endforeach()
endforeach()

foreach(AM_PROGRAM ${AM_PROGRAMS})
  target_link_libraries(${AM_PROGRAM} ${XOREOS_LIBRARIES})
  add_test(NAME ${AM_PROGRAM} COMMAND ${AM_PROGRAM})
endforeach()

# -------------------------------------------------------------------------
# uninstall target
# Code taken from https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake
if(NOT TARGET uninstall)
  configure_file("${CMAKE_SOURCE_DIR}/cmake/CMakeUninstall.cmake" "${CMAKE_BINARY_DIR}/CMakeUninstall.cmake" IMMEDIATE @ONLY)
  add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/CMakeUninstall.cmake)
endif()


# -------------------------------------------------------------------------
# try to add version information from git to src/version/version.cpp
# this is not 100% clean, and doesn't reconfigure when there's only a local change since last
#  build (so it does not always add the .dirty suffix)

# get build timestamp, but heed SOURCE_DATE_EPOCH
set(DATE_FMT "%Y-%m-%dT%H:%M:%SZ")
if(DEFINED ENV{SOURCE_DATE_EPOCH})
  execute_process(COMMAND "date" "-u" "-d" "@$ENV{SOURCE_DATE_EPOCH}" "+${DATE_FMT}" OUTPUT_VARIABLE XOREOS_BUILDDATE OUTPUT_STRIP_TRAILING_WHITESPACE RESULT_VARIABLE DATE_RETCODE ERROR_QUIET)
  if(NOT "${DATE_RETCODE}" STREQUAL "0")
    execute_process(COMMAND "date" "-u" "-r" "$ENV{SOURCE_DATE_EPOCH}" "+${DATE_FMT}" OUTPUT_VARIABLE XOREOS_BUILDDATE OUTPUT_STRIP_TRAILING_WHITESPACE RESULT_VARIABLE DATE_RETCODE ERROR_QUIET)
    if(NOT "${DATE_RETCODE}" STREQUAL "0")
      string(TIMESTAMP XOREOS_BUILDDATE "${DATE_FMT}" UTC)
    endif()
  endif()
else()
  string(TIMESTAMP XOREOS_BUILDDATE "${DATE_FMT}" UTC)
endif()

set_property(SOURCE src/version/version.cpp APPEND PROPERTY COMPILE_DEFINITIONS XOREOS_BUILDDATE="${XOREOS_BUILDDATE}")

find_package(Git)
if(GIT_FOUND)
  execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMAKE_CURRENT_SOURCE_DIR} describe --long --match desc/* OUTPUT_VARIABLE XOREOS_REVDESC OUTPUT_STRIP_TRAILING_WHITESPACE)
  string(REGEX REPLACE "desc/(.*)-([^-]*)-([^-]*)" "\\1+\\2.\\3" XOREOS_REVDESC "${XOREOS_REVDESC}")

  execute_process(
    COMMAND ${GIT_EXECUTABLE} -C ${CMAKE_CURRENT_SOURCE_DIR} update-index --refresh --unmerged
    COMMAND ${GIT_EXECUTABLE} -C ${CMAKE_CURRENT_SOURCE_DIR} diff-index --quiet HEAD
    OUTPUT_QUIET ERROR_QUIET
    RESULT_VARIABLE XOREOS_REVDIRT
  )
  if(NOT XOREOS_REVDIRT EQUAL 0)
    set(XOREOS_REVDESC "${XOREOS_REVDESC}.dirty")
  endif()

  string(REGEX REPLACE "^[^+]*\\+" "" XOREOS_REV "${XOREOS_REVDESC}")
  message(STATUS "Building version: ${XOREOS_REVDESC}")

  set_property(SOURCE src/version/version.cpp APPEND PROPERTY COMPILE_DEFINITIONS XOREOS_REVDESC="${XOREOS_REVDESC}")
  set_property(SOURCE src/version/version.cpp APPEND PROPERTY COMPILE_DEFINITIONS XOREOS_REV="${XOREOS_REV}")

  # use .git/logs/HEAD to track if git state has changed and if we need to reconfigure
  if(EXISTS ${CMAKE_SOURCE_DIR}/.git/logs/HEAD)
    configure_file(${CMAKE_SOURCE_DIR}/.git/logs/HEAD .dirstamp)
  endif()
endif()


# -------------------------------------------------------------------------
# documentation
if(XOREOS_BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly")
  endif()

  add_custom_target(doxygen
    COMMAND ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    SOURCES ${PROJECT_SOURCE_DIR}/Doxyfile
  )
  add_custom_target(doxygen-clean
    COMMAND rm -rf doc/doxygen/html/
    COMMAND rm -rf doc/doxygen/latex/
    COMMAND rm -rf doc/doxygen/man/
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endif()

message(STATUS "")
message(STATUS "Enabled engines:")
message(STATUS "	Neverwinter Nights: ${ENABLE_NWN}")
message(STATUS "	Neverwinter Nights 2: ${ENABLE_NWN2}")
message(STATUS "	Star Wars: Knights of the Old Republic: ${ENABLE_KOTOR}")
message(STATUS "	Star Wars: Knights of the Old Republic II: The Sith Lords: ${ENABLE_KOTOR2}")
message(STATUS "	Jade Empire: ${ENABLE_JADE}")
message(STATUS "	The Witcher: ${ENABLE_WITCHER}")
message(STATUS "	Sonic Chronicles: The Dark Brotherhood: ${ENABLE_SONIC}")
message(STATUS "	Dragon Age: Origins: ${ENABLE_DRAGONAGE}")
message(STATUS "	Dragon Age II: ${ENABLE_DRAGONAGE2}")
message(STATUS "")
message(STATUS "Enabled libraries:")
message(STATUS "	libmad: ${ENABLE_MAD}")
message(STATUS "	libvorbis: ${ENABLE_VORBIS}")
message(STATUS "	libfaad: ${ENABLE_FAAD}")
message(STATUS "	xvidcore: ${ENABLE_XVIDCORE}")
message(STATUS "	libvpx: ${ENABLE_VPX}")
message(STATUS "	liblzma: ${ENABLE_LZMA}")
message(STATUS "	libxml2: ${ENABLE_XML}")
message(STATUS "")
