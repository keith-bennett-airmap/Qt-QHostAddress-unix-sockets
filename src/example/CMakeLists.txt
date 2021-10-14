
FILE(GLOB QT_QHOSTADDRESS_UNIX_SOCKETS_SRC "*.cpp")
FILE(GLOB QT_QHOSTADDRESS_UNIX_SOCKETS_HDR "*.hpp")
SET(REMOVE_ITEM QT_QHOSTADDRESS_UNIX_SOCKETS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

SET(QT_QHOSTADDRESS_UNIX_SOCKETS_LIB_DEPENDENCIES
	${Boost_LIBRARIES}
	Qt5::Widgets
	Qt5::Network
)

ADD_LIBRARY(
	qt_qhostaddress_unix_sockets_lib
	${QT_QHOSTADDRESS_UNIX_SOCKETS_SRC}
	${QT_QHOSTADDRESS_UNIX_SOCKETS_HDR}
)
TARGET_LINK_LIBRARIES(
	qt_qhostaddress_unix_sockets_lib
	${QT_QHOSTADDRESS_UNIX_SOCKETS_LIB_DEPENDENCIES}
)

TARGET_INCLUDE_DIRECTORIES(
	qt_qhostaddress_unix_sockets_lib
	INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
)

INSTALL(
	TARGETS qt_qhostaddress_unix_sockets_lib
	DESTINATION lib
)

ADD_EXECUTABLE(
	qt_qhostaddress_unix_sockets
	main.cpp
)

TARGET_LINK_LIBRARIES(
	qt_qhostaddress_unix_sockets
	qt_qhostaddress_unix_sockets_lib
	Qt5::Core
)
