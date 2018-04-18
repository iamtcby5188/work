AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_PROGRAMS= VersionServer

VersionServer_SOURCES= main.cpp version_service.cpp 

VersionServer_LDADD= -lIDBCommon -llog4cxx -lmysqlcppconn-static -lqpid_engine -lsdbus -lboost_locale -lboost_regex 
VersionServer_LDFLAGS= -L$(top_srcdir)/linux -L$(top_srcdir)/deploy_young/IDBBond/BasicDataServer/lib -L$(top_srcdir)/../ssmq/lib/linux

all-am:$(all-am) CPPROGRAMS

CPPROGRAMS:
	mv $(noinst_PROGRAMS) $(top_srcdir)/linux
