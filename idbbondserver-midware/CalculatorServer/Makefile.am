AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/IDBCommon/mysqlcppconn/linux/include -I $(top_srcdir)/../SDBUS/include

noinst_PROGRAMS= CalculatorServer

CalculatorServer_SOURCES= main.cpp service/calculator_service.cpp \
	business/calculator_business.cpp business/calculator_data_loader.cpp 

CalculatorServer_LDADD= -lIDBCommon -lQuantLib -lSSCalculator -llog4cxx -lmysqlcppconn-static -lqpid_engine -lsdbus -lboost_locale -lboost_regex 
CalculatorServer_LDFLAGS= -L./lib/linux -L$(top_srcdir)/linux -L$(top_srcdir)/IDBCommon/mysqlcppconn/linux/lib \
	 -L$(top_srcdir)/../SDBUS/lib/linux

all-am:$(all-am) CPPROGRAMS

CPPROGRAMS:
	mv $(noinst_PROGRAMS) $(top_srcdir)/linux
