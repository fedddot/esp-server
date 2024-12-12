# This component demonstrates how to add an existing third-party library as a component
# to ESP-IDF build system.
#
# Since we are wrapping the library inside a component,
# the component has to be registered first:
idf_component_register()

# To build a third-party library, ExternalProject CMake module can be used.
# ExternalProject offers many features which are impossible to demonstrate
# in a single example. Please refer to its documentation for more info:
#   https://cmake.org/cmake/help/latest/module/ExternalProject.html
include(ExternalProject)

# Define the location where mcu_server will be installed:
set(MCU_SERVER_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/mcu_server)

# This function downloads the project, calls CMake to configure it,
# builds the project and installs it to the specified location:
externalproject_add(mcu_server_proj
    # Download the source code of the third party project from the following URL.
    # (Two URLs are provided, the 2nd one is the mirror for Chinese users)
    URL https://github.com/fedddot/mcu_server.git
    # (Downloading is not the only option; the library can also be located in your source tree.
    #  Consult ExternalProject_Add function documentation for other options.)

    # Specify arguments to be passed when running CMake for this subproject.
    # Note that ExternalProject_Add also works with non-CMake projects, so this
    # is just an example.
    CMAKE_ARGS
        # Use the same CMake toolchain file as for the main project.
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        # mcu_server-specific settings: disable building everything except for the static library
        -Dmcu_server_BUILD_TESTING=FALSE
        -Dmcu_server_SHARED_LIBS=FALSE
        # Pass the install directory to the subproject.
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>

    # These options are set so that Ninja immediately outputs
    # the subproject build to the terminal. Otherwise it looks like the
    # build process "hangs" while the subproject is being built.
    USES_TERMINAL_DOWNLOAD TRUE
    USES_TERMINAL_CONFIGURE TRUE
    INSTALL_DIR ${MCU_SERVER_INSTALL_DIR}
)

# Now that the subproject build is set up, we need to consume the results
# of the build: the header file and the static library.
# To do this, define an imported CMake library:
add_prebuilt_library(mcu_server_lib "${MCU_SERVER_INSTALL_DIR}/lib/libmcu_server.a"
                     # tinyxml calls certain C++ support library functions (_Unwind_Resume and similar)
                     # so a dependency on IDF's cxx component is added here:
                     PRIV_REQUIRES cxx)
target_include_directories(mcu_server_lib INTERFACE "${MCU_SERVER_INSTALL_DIR}/include")
add_dependencies(mcu_server_lib mcu_server_proj)

# Link the imported library to the current component.
target_link_libraries(${COMPONENT_LIB} INTERFACE mcu_server_lib)

# To use mcu_server in another component, add 'mcu_server' (the name of this component)
# to PRIV_REQUIRES or REQUIRES list its idf_component_register call.
# See ../../main/CMakeLists.txt for an example.
