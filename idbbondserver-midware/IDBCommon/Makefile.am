AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I include -I fastdb/inc -I $(top_srcdir)/../ssmq/inc

noinst_LIBRARIES = libIDBCommon.a

libIDBCommon_a_SOURCES=config.cpp connection_manager.cpp db/connection_pool.cpp db/dbconnector.cpp \
include/common.cpp include/constants.cpp include/logger.cpp include/pinyin.cpp include/text_export.cpp include/cache_def.cpp \
        service/service.cpp \
        service/send_service.cpp \
        service/publish_manager.cpp \
        sync/thread_sync.cpp \
        util/excel/excelengine.cpp util/pugixml/pugixml.cpp  \
        fastdb/class.cpp \
		fastdb/compiler.cpp \
		fastdb/container.cpp \
		fastdb/cursor.cpp \
		fastdb/database.cpp \
		fastdb/file.cpp \
        fastdb/hashtab.cpp \
		fastdb/localcli.cpp \
		fastdb/query.cpp \
		fastdb/repsock.cpp \
		fastdb/rtree.cpp \
        fastdb/server.cpp \
		fastdb/stdtp.cpp \
		fastdb/symtab.cpp \
		fastdb/sync.cpp \
		fastdb/ttree.cpp \
		fastdb/unisock.cpp \
        fastdb/wwwapi.cpp \
		fastdb/xml.cpp \
		cache/cache_center.cpp \
		cache/cache_monitor.cpp \
        msg2db/msg_log_service.cpp \
		msg2db/msg_helper.cpp \
        monitor/chinabondvaluation_monitor.cpp \
        monitor/monitor.cpp \
        object/field.cpp \
        cache/model/account_cache.cpp \
        cache/model/account_group_cache.cpp \
        cache/model/account_permission_cache.cpp \
		cache/model/bondbestquote_cache.cpp \
		cache/model/bonddeal_cache.cpp \
		cache/model/dcsbonddeal_cache.cpp \
        cache/model/bondgroup_cache.cpp \
        cache/model/bondgroupdetail_cache.cpp \
		cache/model/bondquote_cache.cpp \
		cache/model/bondquoterefer_cache.cpp \
        cache/model/bondsmallview_cache.cpp \
        cache/model/bondtrader_cache.cpp \
        cache/model/bondtypeselective_cache.cpp \
        cache/model/brokerbindingtrader_cache.cpp \
        cache/model/chinabonddeviatedvalue_cache.cpp \
        cache/model/chinabondvaluation_cache.cpp \
        cache/model/constants_cache.cpp \
        cache/model/holiday_info_cache.cpp \
        cache/model/institutionrating_cache.cpp \
        cache/model/issuer_info_cache.cpp \
        cache/model/issuinginstitution_cache.cpp \
        cache/model/logintoken_cache.cpp \
        cache/model/province_info_cache.cpp \
        cache/model/selectiveinfo_cache.cpp \
        cache/model/sw_sector_info_cache.cpp \
        cache/model/system_company_cache.cpp \
        cache/model/tradinginstitution_cache.cpp \
        cache/model/version_cache.cpp \
        cache/controller/account_cachecontroller.cpp \
        cache/controller/bondbestquote_cachecontroller.cpp \
		cache/controller/bonddeal_cachecontroller.cpp \
        cache/controller/dcsbonddeal_cachecontroller.cpp \
        cache/controller/bondgroup_cachecontroller.cpp \
        cache/controller/bondgroupdetail_cachecontroller.cpp \
        cache/controller/bondquote_cachecontroller.cpp \
        cache/controller/bondquoterefer_cachecontroller.cpp \
        cache/controller/bondsmallview_cachecontroller.cpp \
        cache/controller/brokerbindingtrader_cachecontroller.cpp \
        cache/controller/chinabondvaluation_cachecontroller.cpp \
        cache/controller/logintoken_cachecontroller.cpp \
        business/BaseBusiness.cpp \
		business/httpclient.cpp \
        business/SystemCompanyBusiness.cpp \
        business/VersionBusiness.cpp \
        increment/increment_cache.cpp \
        increment/increment_cache_manager.cpp



all-am:$(all-am) libIDBCommon.a CPLIBRARIES

CPLIBRARIES:
	mv $(noinst_LIBRARIES) $(top_srcdir)/linux

