# CMakeList.txt : CMake project for Shell, include source and define
cmake_minimum_required (VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Shell")

if(WIN32)
    add_definitions(-D_WIN32)
else()
    add_definitions(-DPOSIX)
endif()

# Configure text files to copy to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Test.txt
              ${CMAKE_CURRENT_BINARY_DIR}/Test.txt COPYONLY)

# Add source files
set(SOURCES
    Shell.cpp
    Interpreter.cpp
    ShellMemory.cpp
)

# Add header files
set(HEADERS
    Shell.h
    Interpreter.h
    ShellMemory.h
)

# Add source to this project's executable.
add_executable(Shell ${SOURCES} ${HEADERS})

if (CMAKE_VERSION VERSION_GREATER 3.20)
  set_property(TARGET Shell PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
