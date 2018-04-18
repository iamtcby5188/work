AUTOMAKE_OPTIONS=foreign

SUBDIRS=service \
	business \
	datamanager \
	sync \
	cache

INCLUDES=-I . -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_PROGRAMS= MarketDataServer

MarketDataServer_SOURCES= main.cpp
	 
MarketDataServer_LDADD= \
	service/libservice.a \
	business/libbusiness.a \
	datamanager/libdatamanager.a \
	sync/libsync.a \
	cache/libcache.a \
	-lIDBCommon -llog4cxx -lmysqlcppconn-static -lqpid_engine -lsdbus -lboost_locale  -lboost_regex -lboost_date_time
MarketDataServer_LDFLAGS= -L$(top_srcdir)/linux -L$(top_srcdir)/deploy_young/IDBBond/BasicDataServer/lib -L$(top_srcdir)/../ssmq/lib/linux

all-am:$(all-am) CPPROGRAMS

CPPROGRAMS:
	mv $(noinst_PROGRAMS) $(top_srcdir)/linux
