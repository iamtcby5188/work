AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I .. -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_LIBRARIES= libsync.a

libsync_a_SOURCES= bondbestquote_sync.cpp \
		bondquote_sync.cpp \
		bondreferquote_sync.cpp \
		bonddeal_sync.cpp \
		chinabondvaluation_sync.cpp \
		dcsbonddeal_sync.cpp
		
