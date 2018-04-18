AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I .. -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_LIBRARIES= libservice.a

libservice_a_SOURCES= bondbestquote_service.cpp \
		bondquote_service.cpp \
		bonddeal_service.cpp \
		dcsbonddeal_service.cpp \
		bondcompletedquote_service.cpp \
		login_service.cpp \
		sync_service.cpp \
		qb_sync_service.cpp \
		bondbestquote_ftp_upload_service.cpp \
		bondbestquote_shch_ftp_upload_service.cpp \
		service_manager.cpp
		

