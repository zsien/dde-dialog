set(BIN_NAME dialog-agent)

set(DIALOG_AGENT_SRCS
    main.cpp
    Agent.h
    Agent.cpp
    AgentInterface.h
    AgentInterface.cpp
    DialogModel.h
    DialogModel.cpp
    ../common/DBusTypes.h
    ../common/DBusTypes.cpp
)

qt_add_dbus_interface(DIALOG_AGENT_SRCS ${CMAKE_BINARY_DIR}/org.deepin.dde.dialog1.xml DialogInterface)
qt_add_dbus_adaptor(DIALOG_AGENT_SRCS org.deepin.dde.dialog1.agent.xml AgentInterface.h
    AgentInterface
)

qt_add_executable(${BIN_NAME})

qt_add_qml_module(${BIN_NAME}
    URI agent
    VERSION 1.0
    QML_FILES
        Main.qml
)

add_dependencies(${BIN_NAME} dialog_xml)

target_sources(${BIN_NAME} PRIVATE
    ${DIALOG_AGENT_SRCS}
)

target_link_libraries(${BIN_NAME} PRIVATE
    Qt6::Quick
    Qt6::DBus
)

install(TARGETS ${BIN_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
