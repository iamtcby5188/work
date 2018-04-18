AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I .. -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_LIBRARIES= libservice.a

libservice_a_SOURCES= bondservice.cpp \
		management_service.cpp \
		sync_service.cpp \
		service_manager.cpp \
        productfavor_service.cpp
