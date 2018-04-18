AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_PROGRAMS= ProductFavorServer

ProductFavorServer_SOURCES= main.cpp service/productfavor_service.cpp service/service_manager.cpp \
	business/productfavor_business.cpp \
	cache/cache_manager.cpp 

ProductFavorServer_LDADD= -lIDBCommon -llog4cxx -lmysqlcppconn-static -lqpid_engine -lsdbus -lboost_locale -lboost_regex -lboost_date_time
ProductFavorServer_LDFLAGS= -L$(top_srcdir)/linux -L$(top_srcdir)/deploy_young/IDBBond/BasicDataServer/lib -L$(top_srcdir)/../ssmq/lib/linux

all-am:$(all-am) CPPROGRAMS

CPPROGRAMS:
	mv $(noinst_PROGRAMS) $(top_srcdir)/linux
