AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_PROGRAMS= QQ2IDBServer

QQ2IDBServer_SOURCES= main.cpp transform_service.cpp add_bond_quote.cpp message.cpp

QQ2IDBServer_LDADD= -lIDBCommon -llog4cxx -lmysqlcppconn-static -lqpid_engine -lsdbus -lboost_locale -lboost_regex 
QQ2IDBServer_LDFLAGS= -L$(top_srcdir)/linux -L$(top_srcdir)/deploy_young/IDBBond/BasicDataServer/lib -L$(top_srcdir)/../ssmq/lib/linux

all-am:$(all-am) CPPROGRAMS

CPPROGRAMS:
	mv $(noinst_PROGRAMS) $(top_srcdir)/linux
