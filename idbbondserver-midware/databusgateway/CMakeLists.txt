PROJECT(databusgateway)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(TARGET databusgateway)
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

IF(WIN32)
SET(INC_PATH D:\\svn\\cb\\inc)
SET(LIB_PATH D:\\svn\\cb\\lib)
SET(MSGBUS_INC D:\\svn\\CSvr\\include)
SET(INCLUDE_PATH D:\\svn\\BSvr\\include)
ELSE()
SET(COMMON_PATH ${PROJECT_SOURCE_DIR}/../../cb)
SET(LIBRARY_PATH ../lib)
SET(MSGBUS_INC /opt/dist/include)
ENDIF()

IF(CMAKE_BUILD_TYPE STREQUAL Debug)
IF(WIN32)
SET(LOG4CPLUS log4cplus-vc100-mdd)
SET(MSGBUS_LIB D:\\svn\\CSvr\\lib\\debug)
SET(LIBRARY_PATH D:\\svn\\BSvr\\lib\\debug)
ELSE()
SET(LOG4CPLUS log4cplusd)
ENDIF()
ELSE()
IF(WIN32)
SET(LOG4CPLUS log4cplus-vc100-md)
SET(MSGBUS_LIB D:\\svn\\CSvr\\lib\\release)
SET(LIBRARY_PATH D:\\svn\\BSvr\\lib\\release)
ELSE()
SET(LOG4CPLUS log4cplus)
ENDIF()
ENDIF()

INCLUDE_DIRECTORIES(
	${INC_PATH}
	${INCLUDE_PATH}
	${MSGBUS_INC}
)

LINK_DIRECTORIES(
	${LIB_PATH}
	${LIBRARY_PATH}
	${MSGBUS_LIB}
	)

ADD_EXECUTABLE(${TARGET} ${DIR_SRCS})

TARGET_LINK_LIBRARIES(${TARGET} log4cplusd  boutild msgbus sdbus qpid_engine )

