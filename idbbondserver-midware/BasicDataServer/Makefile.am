AUTOMAKE_OPTIONS=foreign

SUBDIRS=service \
	business \
	sync \
	monitor \
	cache

INCLUDES=-I . -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_PROGRAMS= BasicDataServer

BasicDataServer_SOURCES= main.cpp CRMConnectionService.cpp CrmProto.pb.cc
	 
BasicDataServer_LDADD= \
	service/libservice.a \
	business/libbusiness.a \
	sync/libsync.a \
	monitor/libmonitor.a \
	cache/libcache.a \
	-lIDBCommon -llog4cxx -lmysqlcppconn-static -lprotobuf -lprotobuf-lite -lprotoc -lqpid_engine -lsdbus -lboost_locale  -lboost_regex -lboost_date_time
BasicDataServer_LDFLAGS= -L$(top_srcdir)/linux -L$(top_srcdir)/deploy_young/IDBBond/BasicDataServer/lib -L$(top_srcdir)/../ssmq/lib/linux

all-am:$(all-am) CPPROGRAMS

CPPROGRAMS:
	mv $(noinst_PROGRAMS) $(top_srcdir)/linux
