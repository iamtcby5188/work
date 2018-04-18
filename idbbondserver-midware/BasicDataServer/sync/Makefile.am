AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I .. -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_LIBRARIES= libsync.a

libsync_a_SOURCES= account_sync.cpp \
		bondsmallview_sync.cpp \
		bondtrader_sync.cpp \
		brokerbindingtrader_sync.cpp \
		chinabonddeviatedvalue_sync.cpp \
		tradinginstitution_sync.cpp \
        institutionrating_sync.cpp
		

