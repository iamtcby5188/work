AUTOMAKE_OPTIONS=foreign

SUBDIRS=service \
	business \
	datamanager \
	sync \
	cache \
	monitor

INCLUDES=-I . -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_PROGRAMS= IDBBondServer

IDBBondServer_SOURCES= DCSConnectionService.cpp main.cpp \
	excel/generateexcel.cpp \
	MessageResendService.cpp \
	tprfa_service.cpp 
	 
IDBBondServer_LDADD= \
	service/libservice.a \
	business/libbusiness.a \
	datamanager/libdatamanager.a \
	sync/libsync.a \
	cache/libcache.a \
	monitor/libmonitor.a \
	-lIDBCommon -llog4cxx -lmysqlcppconn-static -lqpid_engine -lsdbus -lboost_locale  -lboost_regex -lboost_date_time
IDBBondServer_LDFLAGS= -L$(top_srcdir)/linux -L$(top_srcdir)/deploy_young/IDBBond/BasicDataServer/lib -L$(top_srcdir)/../ssmq/lib/linux

all-am:$(all-am) CPPROGRAMS

CPPROGRAMS:
	mv $(noinst_PROGRAMS) $(top_srcdir)/linux
	cp ./Resource/sqlquery.xml $(top_srcdir)/linux
