AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I .. -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_LIBRARIES= libdatamanager.a

libdatamanager_a_SOURCES= quote_data_manager.cpp \
		deal_data_manager.cpp \
		referquote_data_manager.cpp \
		bestquote_data_manager.cpp \
		data_manager.cpp \
        dcs_deal_data_manager.cpp \
		valuation_data_manager.cpp
