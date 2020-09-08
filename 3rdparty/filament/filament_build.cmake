include(ExternalProject)
if(FILAMENT_SOURCE_ROOT)
    if(EXISTS "${FILAMENT_SOURCE_ROOT}")
        set(FILAMENT_ROOT "${FILAMENT_SOURCE_ROOT}")
    else()
        message(FATAL_ERROR "Filament sources not found in ${FILAMENT_SOURCE_ROOT}")
    endif()
else()
    set(FILAMENT_SOURCE_ROOT ${CMAKE_BINARY_DIR}/downloads/filament-1.8.1)
    if (NOT EXISTS ${FILAMENT_SOURCE_ROOT}/README.md)
        set(DOWNLOAD_PATH ${CMAKE_BINARY_DIR}/downloads)
        file(MAKE_DIRECTORY ${DOWNLOAD_PATH})
        set(TAR_PWD ${DOWNLOAD_PATH})
        file(MAKE_DIRECTORY ${TAR_PWD})
        if (NOT EXISTS ${ARCHIVE_FILE})
            set(ARCHIVE_FILE ${CMAKE_BINARY_DIR}/downloads/filament_source.tgz)

            set(DOWNLOAD_URL "https://github.com/google/filament/archive/v1.8.1.tar.gz")

            file(DOWNLOAD ${DOWNLOAD_URL} ${ARCHIVE_FILE} SHOW_PROGRESS STATUS DOWNLOAD_RESULT)
        endif()
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${ARCHIVE_FILE} WORKING_DIRECTORY ${TAR_PWD})
    endif()
endif()

set(FILAMENT_ROOT "${CMAKE_BINARY_DIR}/filament-binaries")

# TODO: don't build samples

ExternalProject_Add(
    ext_filament
    PREFIX filament
    SOURCE_DIR ${FILAMENT_SOURCE_ROOT}
    UPDATE_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_C_COMPILER=${FILAMENT_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${FILAMENT_CXX_COMPILER}
        -DCMAKE_INSTALL_PREFIX=${FILAMENT_ROOT}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DUSE_STATIC_CRT=${STATIC_WINDOWS_RUNTIME}
        -DUSE_STATIC_LIBCXX=ON
)

set(filament_LIBRARIES filameshio filament filamat_lite filamat filaflat filabridge geometry backend bluegl ibl image meshoptimizer smol-v utils)
