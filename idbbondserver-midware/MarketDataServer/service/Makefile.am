AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I .. -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_LIBRARIES= libservice.a

libservice_a_SOURCES= bondbestquote_service.cpp \
		bondquote_service.cpp \
		bonddeal_service.cpp \
		bondcompletedquote_service.cpp \
        dcsbonddeal_service.cpp \
		sync_service.cpp \
		heartbeat_service.cpp \
		service_manager.cpp
		

