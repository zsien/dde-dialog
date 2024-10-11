cmake_minimum_required(VERSION 3.16.0)
project(dialog VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GNUInstallDirs)

set(DATA_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/dde-dialog" CACHE PATH "Data directory")

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core Quick DBus)
find_package(yaml-cpp REQUIRED)

qt_standard_project_setup()

qt_policy(SET QTP0001 NEW)

pkg_check_modules(libsystemd REQUIRED IMPORTED_TARGET libsystemd)

pkg_search_module(DBUS REQUIRED dbus-1)
pkg_get_variable(DBUS_DATA_DIR dbus-1 datadir)
if(DBUS_DATA_DIR)
    set(DBUS_DATA_DIR "${DBUS_DATA_DIR}/dbus-1")
else()
    set(DBUS_DATA_DIR "${CMAKE_INSTALL_DATADIR}/dbus-1")
endif()

message(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

qt_generate_dbus_interface(src/daemon/Dialog.h org.deepin.dde.dialog1.xml OPTIONS -a)
qt_generate_dbus_interface(src/agent/AgentInterface.h org.deepin.dde.dialog1.agent.xml
    OPTIONS
        -a
        -t Action=(sssss)
)
add_custom_target(dialog_xml ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/org.deepin.dde.dialog1.xml
    COMMENT "Helper target for XML stuff."
)
add_custom_target(agent_xml ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/org.deepin.dde.dialog1.agent.xml
    COMMENT "Helper target for XML stuff."
)

add_subdirectory(src)

install(
    FILES misc/dbus/policy.d/org.deepin.dde.dialog1.conf
    DESTINATION ${DBUS_DATA_DIR}/system.d
)
