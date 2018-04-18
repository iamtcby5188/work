AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I .. -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_LIBRARIES= libdatamanager.a

libdatamanager_a_SOURCES= bond_data_manager.cpp \
		institution_data_manager.cpp \
		trader_data_manager.cpp \
		account_data_manager.cpp \
		broker_binding_data_manager.cpp \
		deviated_value_data_manager.cpp \
		data_manager.cpp
