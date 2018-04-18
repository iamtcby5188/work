AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I .. -I include -I $(top_srcdir)/IDBCommon -I $(top_srcdir)/IDBCommon/include \
	-I $(top_srcdir)/../ssmq/inc

noinst_LIBRARIES= libbusiness.a

libbusiness_a_SOURCES= bondbestquote_business.cpp \
		bondbestquotecalc_business.cpp \
		bondquote_business.cpp \
		bonddeal_business.cpp \
		dcsbonddeal_business.cpp \
		bondcompletedquote_business.cpp \
		login_business.cpp \
		bondbestquote_ftp_upload_business.cpp \
		bondbestquote_shch_ftp_upload_business.cpp \
		calculator_business.cpp \
		ftpClient.cpp \
		oper_log.cpp
