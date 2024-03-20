
if (NOT VCPKG_CONFIGURED)
    set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")

    if (NOT EXISTS "${VCPKG_ROOT}")
        message(STATUS "${VCPKG_ROOT} cannot find vcpkg directory. Executing git clone, please wait...")
        execute_process(COMMAND "git" "clone" "https://github.com/microsoft/vcpkg" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" OUTPUT_QUIET)
    endif()

    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

    if(WIN32)
        set(VCPKG_TARGET_TRIPLET "x64-windows-static")
        set(VCPKG_BINARY "${VCPKG_ROOT}/vcpkg.exe")
    elseif(UNIX)
        set(VCPKG_TARGET_TRIPLET "x64-linux")
        set(VCPKG_BINARY "${VCPKG_ROOT}/vcpkg")
    endif()

    set(VCPKG_CONFIGURED TRUE)

    if(NOT EXISTS "${VCPKG_BINARY}")
        message(STATUS "${VCPKG_BINARY} not found, bootstrapping vcpkg, please wait...")
        if (WIN32)            
            execute_process(COMMAND "powershell" "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/bootstrap-vcpkg.bat" OUTPUT_QUIET)
        elseif(UNIX)
            execute_process(COMMAND "sh" "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/bootstrap-vcpkg.sh" OUTPUT_QUIET)
        endif()
    endif()

    message(STATUS "VCPKG configured with no errors")
endif()

function(vcpgk_install PackageName)
    execute_process(COMMAND "${VCPKG_BINARY}" "install" "${PackageName}:${VCPKG_TARGET_TRIPLET}" "--clean-after-build")
endfunction()

function(vcpgk_install_not_found vcpkg_name cmake_name)
    find_package(${cmake_name} QUIET)
    if(NOT ${cmake_name}_FOUND)
        message(STATUS "${vcpkg_name} not found, installing...")
        vcpgk_install(${vcpkg_name})
        else()
        message(STATUS "${vcpkg_name} is already installed")
    endif()
endfunction()