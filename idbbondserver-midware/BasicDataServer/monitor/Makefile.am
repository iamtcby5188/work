AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I .. -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_LIBRARIES= libmonitor.a

libmonitor_a_SOURCES= account_monitor.cpp \
		bondsmallview_monitor.cpp \
		tradinginstitution_monitor.cpp \
		bondtrader_monitor.cpp \
		brokerbindingtrader_monitor.cpp \
		chinabonddeviatedvalue_monitor.cpp \
        institutionrating_monitor.cpp
		

