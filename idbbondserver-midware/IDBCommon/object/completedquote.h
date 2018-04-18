/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Best Quote Structure
====================================================================================================================
   Date       Name                                  Description of Change
23-July-2014  Yudan chen                            Initial version
$HISTORY$
===================================================================================================================*/
#ifndef _COMPLETED_QUOTE_H_
#define _COMPLETED_QUOTE_H_

#include "sdbus/string.h"
#include "include/constants.h"
#include <string>

/*typedef struct
{
    sdbus::string bid_quote_id;
    sdbus::string bond_key_listed_market;
    sdbus::string bid_issue_institution_id;
    sdbus::string bid_trader_id;
    sdbus::string bid_institution_trader_name_;  // »ú¹¹(½»Ò×Ô±)
    double bid_volume_;                             // BidÊýÁ¿£¬µ¥Î»£¨Ôª£©
    sdbus::string bid_broker_name_;              // bid·½¾­¼ÍÈË
    sdbus::string bid_description_;              // bid·½±¸×¢
    sdbus::string bid_oco_flag_;                 // OCO±êÊ¶, 0-´ú±í²»ÏÔÊ¾£¬1-ÏÔÊ¾oco  2-oco´ò°ü
    sdbus::string bid_bargain_flag_;             // Òé¼Û±êÊ¶, È¡Öµ 0, 1, 2
    double        bid_price_;                    // bid±¨¼Û
    sdbus::string bid_fan_dian_flag_;            // ÊÇ·ñ·µµã£¬ ¡°1¡±- ÓÐ·µµã£¬¡°0¡±- ÎÞ·µµã
    double        bid_fan_dian_;                 // bid·µµã
    sdbus::string bid_internal_flag_;            // 1-Õý³£±¨¼Û£¬2-ÄÚ²¿±¨¼Û
    sdbus::string bid_price_string_;
    sdbus::string bid_fan_dian_string_;

    sdbus::string ofr_quote_id;
	sdbus::string ofr_issue_institution_id;
	sdbus::string ofr_trader_id;
    sdbus::string ofr_institution_trader_name_;  // »ú¹¹(½»Ò×Ô±)
    double ofr_volume_;                             // ofrÊýÁ¿£¬µ¥Î»£¨Ôª£©
    sdbus::string ofr_broker_name_;              // ofr·½¾­¼ÍÈË
    sdbus::string ofr_description_;              // ofr·½±¸×¢
    sdbus::string ofr_oco_flag_;                 // OCO±êÊ¶, 0-´ú±í²»ÏÔÊ¾£¬1-ÏÔÊ¾oco  2-oco´ò°ü
    sdbus::string ofr_bargain_flag_;             // Òé¼Û±êÊ¶, È¡Öµ 0, 1, 2
    double        ofr_price_;                    // ofr±¨¼Û
    sdbus::string ofr_fan_dian_flag_;            // ÊÇ·ñ·µµã£¬ ¡°1¡±- ÓÐ·µµã£¬¡°0¡±- ÎÞ·µµã
    double        ofr_fan_dian_;                 // ofr·µµã
    sdbus::string ofr_internal_flag_;            // 1-Õý³£±¨¼Û£¬2-ÄÚ²¿±¨¼Û
    sdbus::string ofr_price_string_;
    sdbus::string ofr_fan_dian_string_;

    bool          is_best_quote_;                // true×îÓÅ±¨¼Û false´ÎÓÅ±¨¼Û
} CompletedQuoteDetail;*/
class CompletedQuoteDetail {
public:
    CompletedQuoteDetail():bid_volume_(DECIMAL_NULL), bid_price_(DECIMAL_NULL), bid_fan_dian_(DECIMAL_NULL), ofr_volume_(DECIMAL_NULL), ofr_price_(DECIMAL_NULL), ofr_fan_dian_(DECIMAL_NULL), is_best_quote_(false) {}
    sdbus::string bid_quote_id;
    sdbus::string bond_key_listed_market;
    sdbus::string bid_issue_institution_id;
    sdbus::string bid_trader_id;
    sdbus::string bid_institution_trader_name_;  
    double bid_volume_;                          
    sdbus::string bid_broker_name_;              
    sdbus::string bid_description_;              
    sdbus::string bid_oco_flag_;                 
    sdbus::string bid_bargain_flag_;             
    double        bid_price_;                    
    sdbus::string bid_fan_dian_flag_;            
    double        bid_fan_dian_;                 
    sdbus::string bid_internal_flag_;            
    sdbus::string bid_price_string_;
    sdbus::string bid_fan_dian_string_;

    sdbus::string ofr_quote_id;
    sdbus::string ofr_issue_institution_id;
    sdbus::string ofr_trader_id;
    sdbus::string ofr_institution_trader_name_;  
    double ofr_volume_;                          
    sdbus::string ofr_broker_name_;              
    sdbus::string ofr_description_;              
    sdbus::string ofr_oco_flag_;                 
    sdbus::string ofr_bargain_flag_;             
    double        ofr_price_;                    
    sdbus::string ofr_fan_dian_flag_;            
    double        ofr_fan_dian_;                 
    sdbus::string ofr_internal_flag_;            
    sdbus::string ofr_price_string_;
    sdbus::string ofr_fan_dian_string_;

    bool          is_best_quote_;                
};

typedef std::tr1::shared_ptr<CompletedQuoteDetail> CompletedQuoteDetailPtr;
typedef std::tr1::shared_ptr<std::vector<CompletedQuoteDetailPtr> > CompletedQuoteDetailVecPtr;

#endif // _COMPLETED_QUOTE_H_
