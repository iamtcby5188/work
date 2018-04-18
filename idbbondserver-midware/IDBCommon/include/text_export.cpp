/*==================================================================================================================
                                    Copyright (C) 2014 SUMSCOPE L.P.
                                          ALL RIGHTS RESERVED
====================================================================================================================
File description:
    Text Export Utility class
====================================================================================================================
   Date       Name                                  Description of Change
13-Aug-2014   James Xu                              Initial version
19-Aug-2014   James Xu                              Add getExpBestVolume and 
$HISTORY$
===================================================================================================================*/

#include "text_export.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "common.h"
#include "constants.h"

#include <algorithm>

/*std::string getExpTTM(const std::string& time_to_maturity, int width) {
    std::ostringstream oss;

    oss << std::setw(width) << std::setiosflags(std::ios::left) << time_to_maturity;

    return oss.str();
}*/

std::string getExpTTM(const std::string& time_to_maturity, const int& days, int width) {
    std::ostringstream oss, ss;
    ss << time_to_maturity;
    if (days > 0) {
        ss << "(" << GBKToUTF8("休") << days << ")";
    }
    oss << std::setw(width) << std::setiosflags(std::ios::left) << ss.str();
    
    return oss.str();
}

std::string getExpTTMInMonth(const std::string& time_to_maturity, int width, bool has_option)
{
    std::string ttm = time_to_maturity;
    if (ttm.length() == 0) {
        return "";
    }

    if (!has_option)
    {
        float maturity = atof(time_to_maturity.substr(0, time_to_maturity.length() - 1).c_str());
        int length = time_to_maturity.length();
        std::string unit = time_to_maturity.substr(length - 1, 1);
        if (unit == "D" && maturity > 30)
        {
            ttm = FloatToString( Round(maturity / 30.0, 1) ) + "M";
        } 
    }

    std::ostringstream oss;
    oss << std::setw(width) << std::setiosflags(std::ios::left) << ttm;
    return oss.str();
}

std::string getExpBondShortName(const std::string& bond_short_name, int width)
{
    std::ostringstream oss;
    if (width == 0)
    {
        return bond_short_name;
    }

    int cnt = CountChineseWords(bond_short_name);
    int delta = width - (bond_short_name.size() - cnt);

    if ( cnt > delta ) 
    {
        cnt = delta;
    }
    oss << std::setw(width) << std::setiosflags(std::ios::left) << bond_short_name << std::setw(cnt) << ' ';

    return oss.str();
}

std::string getExpBondCode(const std::string& bond_code, int width)
{
    std::ostringstream oss;

    oss << std::setw(width) << std::setiosflags(std::ios::left) << bond_code;

    return oss.str();
}


std::string getExpPrice(double price, const std::string& fan_dian_flag, double fan_dian, int symbol, int price_type, int width) // 0-基本报价 1-成交价格 2-作废价格
{
    std::string sPrice;

    if (price != INVALIDVALUE) // 价格不为空
    {
        if (fan_dian_flag == "1") // 返点
        {
            if (fan_dian == 0 || fan_dian == INVALIDVALUE)
            {
                sPrice =  sPrice + Format(price, 2, 4) + "F";
            } 
            else
            {                   
                sPrice = sPrice + Format(price, 2, 4) + "F" + Format(fan_dian, 2, 4);
            }

        }
        else // 不返点
        {
            sPrice = Format(price, 2, 4);
        }  
    }

    else 
    {
        if (fan_dian_flag == "1")
        {
            if (fan_dian == 0 || fan_dian == INVALIDVALUE)
            {

                sPrice = sPrice + GBKToUTF8("平价返");
            } 
            else
            {
                sPrice = sPrice + Format(price, 2, 4) + "F" + Format(fan_dian, 2, 4);
            }
        }

        else
        {
            if (price_type == 0)
            {
                sPrice =  sPrice + (symbol == 1 ? "Bid" : "Ofr");
            } 
        }
    }

    if (sPrice.empty())
    {
        return sPrice;
    }
    std::ostringstream oss;
    if (sPrice.size() < width )
    {
        oss << std::setw(width) << std::setiosflags(std::ios::left) << sPrice;
        return oss.str();
    }

    return sPrice;
}

std::string getExpNotes(const std::string& bargain_flag, const std::string& oco_flag, const std::string& price_description, const std::string& company_id)
{
    std::string note;
    bool isBargainOnly = false;
    std::string delimiter = ",";
    if (company_id == kCompanyICAP )
    {
        delimiter = " ";
    }

    if ( bargain_flag == "1" )
    {
        isBargainOnly = true;
        note = note + "*";
    }
    else if ( bargain_flag == "2" )
    {
        isBargainOnly = true;
        note = note + "**";
    }

    if ( oco_flag == "1" )
    {
        isBargainOnly = false;
        note = note.empty() ? ("oco") : (note + delimiter + "oco");
    }
    else if ( oco_flag == "2" )
    {
        isBargainOnly = false;
        note = note.empty() ? GBKToUTF8("打包") : (note + delimiter + GBKToUTF8("打包"));
    }

    if ( !price_description.empty() ) 
    {
        isBargainOnly = false;
        note = note.empty() ? price_description : (note + delimiter + price_description);
    }

    if ( !note.empty() && !isBargainOnly )
    {
        note = "(" + note + ")";   
    }

    return note;
}

std::string getExpVolume(double volume, int symbol, int width, const std::string& company_id)
{
    std::ostringstream oss;
    std::stringstream ssVolume;
    ssVolume << Format(volume, 0, 4);
    std::string strVolume;

    if ( symbol == kBidQuote )
    {
        if ( volume == 0 || volume == DOUBLE_NULL )
        {
            if (company_id == kCompanyICAP)
            {
				strVolume = ssVolume.str();
            }
            else
            {
                strVolume = "--/--";
            }          
        }
        else
        {
            strVolume = ssVolume.str() + "/--";
        }          
    }
    else
    {
		if (volume == 0 || volume == DOUBLE_NULL)
        {            
            if (company_id == kCompanyICAP)
            {
				strVolume = ssVolume.str();
            }
            else
            {
                strVolume = "--/--";
            }
        }
        else
        {
            if (company_id == kCompanyICAP)
            {
                strVolume = ssVolume.str();
            }
            else
            {
                strVolume = "--/" + ssVolume.str();
            }           
        }           
    }

    oss << std::setw(width) << std::setiosflags(std::ios::left) << strVolume;
    
    return oss.str();
}

std::string getExpVolume(double volume)
{
    if (volume == 0 || volume == DOUBLE_NULL)
    {
        return "--";
    }

    std::stringstream ssVolume;
    ssVolume << Format(volume, 0, 4);

    return ssVolume.str();
}

std::string getExpBestVolume(double bid_volume, double ofr_volume)
{
    std::ostringstream oss;
    std::stringstream ssBidVolume;
    std::stringstream ssOfrVolume;
    ssBidVolume << Format(bid_volume, 0, 4);
    ssOfrVolume << Format(ofr_volume, 0, 4);

    if ((bid_volume == 0 || bid_volume == DOUBLE_NULL) && (ofr_volume == 0 || ofr_volume == DOUBLE_NULL))
    {
        return "--/--";
    }
    else if (ofr_volume == 0 || ofr_volume == DOUBLE_NULL)
    {
        oss << ssBidVolume.str() << "/--";
    }
    else if (bid_volume == 0 || bid_volume == DOUBLE_NULL)
    {
        oss << "--/" << ssOfrVolume.str();
    }
    else
    {
        oss << ssBidVolume.str() << "/" << ssOfrVolume.str();
    }
    
    return oss.str();
}

std::string getExpRatings(const std::string& issuer_rating, const std::string& bond_rating, const std::string& company_id)
{
    std::string ratings;

    if ( !issuer_rating.empty() && !bond_rating.empty() )
    {
        if ( issuer_rating == bond_rating )
        {
            if (issuer_rating != "A-1")
            {
                ratings = issuer_rating;
            }        
        }
        else
        {
            if (issuer_rating == "A-1")
            {
                ratings = bond_rating;
            }
            else if (bond_rating == "A-1")
            {
                ratings = issuer_rating;
            }
            else
            {
                ratings = issuer_rating + "/" + bond_rating;
            }
            
        }
    }
    else if ( !issuer_rating.empty() )
    {
        if (issuer_rating != "A-1")
        {
            ratings = issuer_rating;
        }      
    }
    else if ( !bond_rating.empty() )
    {
        if (bond_rating != "A-1")
        {
            ratings = bond_rating;
        }     
    }

    std::ostringstream oss;
    if (company_id == kCompanyICAP)
    {
        oss << std::setw(9) << std::setiosflags(std::ios::left) << ratings;
        return oss.str();
    }
    else if (company_id == kCompanyPATR)
    {
        oss << std::setw(8) << std::setiosflags(std::ios::left) << ratings;
        return oss.str();
    }

    return ratings;
}

std::string getExpRatings(const std::string& issuer_rating, int width)
{
    std::stringstream oss;

    if ( !issuer_rating.empty())
    {
        oss << std::setw(width) << std::setiosflags(std::ios::left) << issuer_rating;
        return oss.str();
    }
 
    return issuer_rating;
}

std::string getExpListedDate(int listed_date)
{
    std::ostringstream oss;
    oss << listed_date;
    return oss.str();
}

std::string getExpPriceAndNotes(const std::string& price, const std::string& notes, int symbol, const std::string& company_id)
{
    std::string priceNotes; 
    std::ostringstream oss;
    
    if (company_id == kCompanyTP)
    {
        priceNotes =  (symbol ==  1 ? (price + notes + "/--") : ("--/" + price + notes));
        int length = priceNotes.size() - CountChineseWords(priceNotes);
        if (length < 10)
        {
            oss << std::setw(10) << std::setiosflags(std::ios::left) << priceNotes;
            return oss.str();
        } 
        else
        {
            return priceNotes;
        }
    }
    else if (company_id == kCompanyICAP)
    {
        priceNotes = (symbol == 1 ? (price + notes) : ("--/" + price + notes));
    }

    return priceNotes;
}

std::string getExpPriceAndNotes(const std::string& bid_price, const std::string& bid_notes, const std::string& ofr_price, const std::string& ofr_notes)
{
    std::string priceNotes;
   
    if (bid_price.empty() && ofr_price.empty())
    {
        priceNotes = "--/--";
    }
    else if (!bid_price.empty() && ofr_price.empty())
    {
        priceNotes = bid_price + bid_notes + "/--";
    } 
    else if (bid_price.empty() && !ofr_price.empty())
    {
        priceNotes = "--/" + ofr_price + ofr_notes;
    }
    else
    {
        priceNotes = bid_price + bid_notes + "/" + ofr_price + ofr_notes;
    }
    int length = priceNotes.size() - CountChineseWords(priceNotes);

    std::ostringstream oss;
    if (length < 10)
    {
        oss << std::setw(10) << std::setiosflags(std::ios::left) << priceNotes;
    } 
    else
    {
        return priceNotes;
    }

    return oss.str();
}

std::string getExpPriceNotesVolume(const std::string& bid_price, const std::string& bid_notes, double bid_volume, const std::string& ofr_price, const std::string& ofr_notes, double ofr_volume)
{
    std::string priceNotesVolume;
    std::stringstream ssBidVolume;
    ssBidVolume << Format(bid_volume, 0, 4);

    std::stringstream ssOfrVolume;
    ssOfrVolume << Format(ofr_volume, 4);

    if (bid_price.empty() && ofr_price.empty())
    {
        priceNotesVolume = "--/--";
    }
    else if (bid_price.empty())
    {
        priceNotesVolume = "--/" + ofr_price + ofr_notes + " " + (ofr_volume == 0 ? "" : ssOfrVolume.str());
    } 
    else if (ofr_price.empty())
    {
        priceNotesVolume = bid_price + bid_notes + " " + (bid_volume == 0 ? "" : ssBidVolume.str()) + "/--";
    }
    else
    {
        if (bid_volume == ofr_volume)
        {
            priceNotesVolume = bid_price + bid_notes + "/" + ofr_price + ofr_notes + " " + (ofr_volume == 0 ? "" : ssOfrVolume.str());
        }
        else
        {
            priceNotesVolume = bid_price + bid_notes + " " + (bid_volume == 0 ? "" : ssBidVolume.str()) + "/" + ofr_price + ofr_notes + " " + (ofr_volume == 0 ? "" : ssOfrVolume.str());

        }
    }

    return priceNotesVolume;
}

std::string getExpDealDirection(const std::string& deal_direction)
{
    if (deal_direction == "traded")
    {
        return "trd";
    } 
    else if (deal_direction == "taken")
    {
        return "tkn";
    }
    else
    {
        return "gvn";
    }
}

std::string getRateIndex(const std::string& rate_index, int width, const std::string& company_id)
{
    std::string rateIndex = rate_index;
    if (company_id == kCompanyICAP)
    {
        if (rate_index == "SHIBOR")
        {
            rateIndex = "shib";
        }
        else if (rate_index == "DEPO")
        {
            rateIndex = "depo";
        }

        std::ostringstream oss;

        oss << std::setw(width) << std::setiosflags(std::ios::left) << rateIndex;
        return oss.str();
    }

    // 首字母大写，其余小写
    std::transform (rateIndex.begin() + 1, rateIndex.end(), rateIndex.begin() + 1, ::tolower);  
    
    std::ostringstream oss;
    oss << std::setw(width) << std::setiosflags(std::ios::left) << rateIndex;

    return oss.str();
}

std::string getCouponRate(const double& coupon_rate)
{
	std::string cr_string;
	std::stringstream ss;
	ss.precision(2);
	ss<<std::fixed<<coupon_rate;
	ss>>cr_string;
	return cr_string;
}

std::string getExpMaturityHolidays(const int& days) {
    if (days > 0) {
        std::stringstream ss;
        ss << "(" << GBKToUTF8("休") << days << ")";
        return ss.str();
    }
    return "";
}