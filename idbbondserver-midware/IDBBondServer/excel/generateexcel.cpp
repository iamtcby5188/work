#include <sstream>
#include "generateexcel.h"
#include "include/fid.h"
#include "util/excel/excelengine.h"
#include "config.h"
using namespace GenerateExcels;

GenerateExcel::GenerateExcel(std::string fileName)
{    
    std::string save_file_path = Config::singleton()->getValue("Ftp.LocalPath", "./excel/");

    m_fileName = save_file_path + fileName;
}

GenerateExcel::~GenerateExcel()
{
}

bool GenerateExcel::SaveBondDataBase(const BondQuoteCacheVecPtr& cache_vec)
{
    ExcelEngine xls;

    if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
    {
        LOGGER_ERROR("Can't find head.txt");
        return false;
    }
   
    xls.SetColumnWidth(1, 80);
    xls.SetColumnWidth(1, 80);
    xls.SetColumnWidth(1, 70);
    xls.SetColumnWidth(1, 70);
    xls.SetColumnWidth(1, 140);
    xls.SetColumnWidth(1, 70);
    xls.SetColumnWidth(1, 70);
    xls.SetColumnWidth(1, 140);

    xls.StartNewLine(18);
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "剩余期限");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "代码");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "bid价格");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "bid量");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "bid机构");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "ofr价格");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "ofr量");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "ofr机构");
    xls.EndCurrLine();

    int tableR = cache_vec->size();

    std::string bondType;
    std::string lastType = "";
    std::string price;

    //write data to excel
    for (int i = 0; i < tableR; ++i)
    {
        BondQuoteCachePtr cache = cache_vec->at(i); 
        bondType = cache->bond_type;        

        if(bondType != "" && bondType != lastType)
        {
            xls.StartNewLine(16);
            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), "");
            xls.EndCurrLine();
            xls.StartNewLine(16);
            xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),"x:str"), Utf8ToAnsi(bondType).c_str());
            xls.EndCurrLine();
            lastType = bondType;
        }
        xls.StartNewLine(16);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), cache->time_to_maturity.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), cache->bond_id.c_str()); 

        if(cache->symbol == 1)
        {
            if(cache->price == INVALIDVALUE)
            {
                if(cache->rebate == "1")
                {
                    if(cache->return_point != INVALIDVALUE)
                        price = "--F" + GetStringFromDouble(cache->return_point);
                    else
                        price = "平价返";
                }
                else
                {
                    price = "Bid";
                }
            }
            else
            {
                price = GetStringFromDouble(cache->price);
                if(cache->rebate == "1")
                {
                    price += "+F";
                    if(cache->return_point != INVALIDVALUE)
                        price += GetStringFromDouble(cache->return_point);
                }
            }


            xls.FillExcelCell(CELL_NORMAL(41,BDR_SOLID(0),"x:str"), price.c_str());

            if(cache->volume == INVALIDVALUE || cache->volume == 0)
                xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), "--");
            else
                xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), (int)cache->volume); 

            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), Utf8ToAnsi(cache->institution_name.c_str()).c_str());
            xls.EndCurrLine();
        }
        else
        {
            if(cache->price == INVALIDVALUE)
            {
                if(cache->rebate == "1")
                {
                    if(cache->return_point != INVALIDVALUE)
                        price = "--F" + GetStringFromDouble(cache->return_point);
                    else
                        price = "平价返";
                }
                else
                {
                    price = "Ofr";
                }
            }
            else
            {
                price = GetStringFromDouble(cache->price);
                if(cache->rebate == "1")
                {
                    price += "+F";
                    if(cache->return_point != INVALIDVALUE)
                        price += GetStringFromDouble(cache->return_point);
                }
            }
            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),""), ""); 
            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),""), ""); 
            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),""), ""); 

            xls.FillExcelCell(CELL_NORMAL(42,BDR_SOLID(0),"x:str"), price.c_str());
      
            if(cache->volume == INVALIDVALUE || cache->volume == 0)
                xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), "--");
            else
                xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), (int)cache->volume);

            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), Utf8ToAnsi(cache->institution_name.c_str()).c_str());
            xls.EndCurrLine();
        }
    }
    
    xls.EndExeclFile();

    return true;
}

bool GenerateExcel::SaveBondDataICAP(const BondQuoteCacheVecPtr& cache_vec)
{
    ExcelEngine xls;

    if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
    {
        LOGGER_ERROR("Can't find head.txt");
        return false;
    }

    xls.SetColumnWidth(1, 80);
    xls.SetColumnWidth(1, 80);
    xls.SetColumnWidth(1, 70);
    xls.SetColumnWidth(1, 70);
    xls.SetColumnWidth(1, 140);
    xls.SetColumnWidth(1, 70);
    xls.SetColumnWidth(1, 70);
    xls.SetColumnWidth(1, 140);

    xls.StartNewLine(18);
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "剩余期限");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "代码");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "bid价格");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "bid量");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "bid机构");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "ofr价格");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "ofr量");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "ofr机构");
    xls.EndCurrLine();


    int tableR = cache_vec->size();

    std::string bondType;
    std::string lastType = "";
    std::string price;

    for (int i = 0; i < tableR; ++i)
    {
        BondQuoteCachePtr cache = cache_vec->at(i); 

        bondType = cache->bond_type;
        if(bondType != "" && bondType != lastType)
        {
            xls.StartNewLine(16);
            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), "");
            xls.EndCurrLine();
            xls.StartNewLine(16);            
            xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),"x:str"), Utf8ToAnsi(bondType).c_str());
            lastType = bondType;
            xls.EndCurrLine();
        }

        xls.StartNewLine(16);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), cache->time_to_maturity.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), cache->bond_id.c_str()); 

        price =  GetPriceWithR(cache->price, cache->symbol, cache->rebate.c_str(), cache->return_point);

        if(cache->symbol == 1)
        {
            xls.FillExcelCell(CELL_NORMAL(41,BDR_SOLID(0),"x:str"), price.c_str());
            if(cache->volume == INVALIDVALUE || cache->volume == 0)
                xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), "--");
            else
                xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), (int)cache->volume);
            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), Utf8ToAnsi(cache->institution_name.c_str()).c_str());
            xls.EndCurrLine();
        }
        else
        {
            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),""), ""); 
            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),""), ""); 
            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),""), ""); 
            xls.FillExcelCell(CELL_NORMAL(42,BDR_SOLID(0),"x:str"), price.c_str());
            if(cache->volume == INVALIDVALUE || cache->volume == 0)
                xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), "--");
            else
                xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), (int)cache->volume);
            xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), Utf8ToAnsi(cache->institution_name.c_str()).c_str());
            xls.EndCurrLine();
        }
    }

    xls.EndExeclFile();

    return true;
}

bool GenerateExcel::SaveBondDealDataPATR(const BondDealCacheVecPtr& cache_vec)
{
    ExcelEngine xls;

    if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
    {
        LOGGER_ERROR("Can't find head.txt");
        return false;
    }

    xls.SetColumnWidth(1,80);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,85);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);

    xls.StartNewLine(18);
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Reference");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Bond Name");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Bond Code");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Payment");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Quntity");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Yield");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Clean Price");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Settle Date");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Buyer");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Buyer Trader");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Buyer Broker");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Seller");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Seller Trader");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Seller Broker");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Full Rate");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Full Value");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Maturity date");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Days");
    xls.EndCurrLine();

    int tableR = cache_vec->size();

    for (int i = 0; i < tableR; ++i)
    {
        BondDealCachePtr cache = cache_vec->at(i); 
        xls.StartNewLine(16);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),"");
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bond_short_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->bond_id.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->delivery_type).c_str());		

        std::stringstream ss;
        std::string num;
        ss << cache->volume / 100.0;
        ss >> num;
        num += "m";
        ss.clear();

        std::string pirce = GetStringFromDouble(cache->price) + "%";
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),num.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),pirce.c_str());

        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->net_price > 0 ? GetStringFromDouble(cache->net_price).c_str(): "");
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->settlement_date.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bid_institution_name).c_str());

        std::string bidAgent = cache->bid_trader_name;
        std::string ofrAgent = cache->ofr_trader_name;

        if(cache->bid_bridge == "1")
            bidAgent = bidAgent + " " + cache->bid_bridge;
        if(cache->ofr_bridge == "1")
            ofrAgent = ofrAgent + " " + cache->ofr_bridge;

        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(bidAgent).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bid_broker_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->ofr_institution_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(ofrAgent).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->ofr_broker_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->full_price > 0 ? GetStringFromDouble(cache->full_price).c_str(): "");

        double full = 0;
        if(cache->full_price == INVALIDVALUE || cache->volume == INVALIDVALUE || cache->volume < 0 || cache->full_price < 0){
            full = 0;
        }else{
            full = cache->full_price*cache->volume;
        }

        ss << full;
        ss >> num;
        ss.clear();

        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),num != "0"? num.c_str(): "");
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->maturity_date.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->time_to_maturity.c_str());
        xls.EndCurrLine();
    }

    xls.EndExeclFile();

    return true;
}

bool GenerateExcel::SaveBondDealDataBGC(const BondDealCacheVecPtr& cache_vec)
{
    ExcelEngine xls;

    if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
    {
        LOGGER_ERROR("Can't find head.txt");
        return false;
    }

    xls.SetColumnWidth(1,80);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,85);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);

    xls.StartNewLine(18);
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "交易时间");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "债券代码");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "简称");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "收益率");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "成交量");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "期限");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "买入方");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "交易员");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Broker");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "卖出方");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "交易员");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "Broker");
    xls.EndCurrLine();

    int tableR = cache_vec->size();

    for (int i = 0; i < tableR; ++i)
    {
        BondDealCachePtr cache = cache_vec->at(i); 

        xls.StartNewLine(16);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),"");
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->settlement_date.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->bond_id.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bond_short_name).c_str());

        std::string price = GetStringFromDouble(cache->price) + "%";
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),price.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),(int)cache->volume);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->time_to_maturity).c_str());

        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bid_institution_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bid_trader_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bid_broker_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->ofr_institution_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->ofr_trader_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->ofr_broker_name).c_str());
        xls.EndCurrLine();
    }

    xls.EndExeclFile();

    return true;
}

bool GenerateExcel::SaveBondDealDataICAP(const BondDealCacheVecPtr& cache_vec)
{
    ExcelEngine xls;

    if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
    {
        LOGGER_ERROR("Can't find head.txt");
        return false;
    }

    xls.SetColumnWidth(1,80);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,80);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);

    xls.StartNewLine(18);
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "成交日期");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "代码");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "简称");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "评级");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "到期期限");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "成交收益率");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "卖方");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "买方");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "量");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "备注");
    xls.EndCurrLine();

    int tableR = cache_vec->size();

    for (int i = 0; i < tableR; ++i)
    {
        BondDealCachePtr cache = cache_vec->at(i); 
        xls.StartNewLine(16);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),"");
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->settlement_date.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->bond_id.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bond_short_name).c_str());

        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->issuer_rating.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->time_to_maturity.c_str());
        std::string price = GetStringFromDouble(cache->price) + "%";

        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),price.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->ofr_institution_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bid_institution_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),(int)cache->volume);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->description).c_str());
        xls.EndCurrLine();
    }
    xls.EndExeclFile();

    return true;
}

bool GenerateExcel::SaveBondDealDataTPBCO(const BondDealCacheVecPtr& cache_vec)
{
    ExcelEngine xls;

    if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
    {
        LOGGER_ERROR("Can't find head.txt");
        return false;
    }

    xls.SetColumnWidth(1,110);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,80);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);

    xls.StartNewLine(18);
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "债券代码");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "简称");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "评级");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "收益率");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "成交量");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "剩余天数");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "买入佣金");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "卖出佣金");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "买入方");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "交易员");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "卖出方");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "交易员");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "备注");
    xls.EndCurrLine();

    int tableR = cache_vec->size();
    double totalVolume = 0;
    double totalBrokerrage = 0;

    int i = 0;

    for(i = 0; i < tableR; ++i)
    {
        BondDealCachePtr cache = cache_vec->at(i);
        std::stringstream ss;
        std::string num;
        ss << i+1;
        ss >> num;
        ss.clear();
        xls.StartNewLine(16);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), (GetDateString() + "(" + num + ")").c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->bond_id.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), Utf8ToAnsi(cache->bond_short_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bond_rating).c_str());

        std::string price = GetStringFromDouble(cache->price) + "%";
        double vol = cache->volume / 1000.0;
        totalVolume += cache->volume / 10000.0;

        ss << vol;
        ss >> num;
        ss.clear();

        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),price.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),num.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->time_to_maturity.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),GetStringFromDouble(cache->bid_brokerrage).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),GetStringFromDouble(cache->ofr_brokerrage).c_str());

        if(abs(cache->bid_brokerrage) >= FLOATING_PRECISION){
            totalBrokerrage += cache->bid_brokerrage;
        }
        if(abs(cache->ofr_brokerrage) >= FLOATING_PRECISION){
            totalBrokerrage += cache->ofr_brokerrage;
        }

        std::string bidAgent = cache->bid_trader_name;
        std::string ofrAgent = cache->ofr_trader_name;

        if(cache->bid_bridge == "2")
            bidAgent = bidAgent + " 过券";
        if(cache->ofr_bridge == "2")
            ofrAgent = ofrAgent + " 过券";

        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bid_institution_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(bidAgent).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->ofr_institution_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(ofrAgent).c_str());
        std::string remarks = Utf8ToAnsi(cache->bid_broker_name) + " " + Utf8ToAnsi(cache->ofr_broker_name);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),remarks.c_str());
        xls.EndCurrLine();
    }
    std::stringstream ts;
    std::string n;
    std::string m;
    std::string v;
    ts << i;
    ts >> n;
    ts.clear();
    ts << totalVolume;
    ts >> m;
    ts.clear();
    ts << totalBrokerrage;
    ts >> v;
    std::string totalInfo = "成交笔数" + n + "笔 成交数量" + m + "亿 成交金额" + v + "元";	
    xls.StartNewLine(16);
    xls.FillExcelCell(CELL_COLSPAN(14,32,BDR_SOLID(0),"x:str"),totalInfo.c_str());
    xls.EndCurrLine();

    xls.EndExeclFile();
    return true;
}

bool GenerateExcel::SaveBondDealDataTPBNC(const BondDealCacheVecPtr& cache_vec)
{
    ExcelEngine xls;

    if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
    {
        LOGGER_ERROR("Can't find head.txt");
        return false;
    }

    xls.SetColumnWidth(1,110);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,80);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,70);
    xls.SetColumnWidth(1,80);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);
    xls.SetColumnWidth(1,105);

    xls.StartNewLine(18);
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "债券代码");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "简称");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "成交价格");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "收益率");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "成交量");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "剩余天数");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "买入佣金");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "卖出佣金");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "买入方");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "交易员");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "卖出方");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "交易员");
    xls.FillExcelCell(CELL_NORMAL(40,BDR_SOLID(0),""), "备注");
    xls.EndCurrLine();

    int tableR = cache_vec->size();
    double totalVolume = 0;
    double totalBrokerrage = 0;

    int i = 0;

    for (i = 0; i < tableR; ++i)
    {
        BondDealCachePtr cache = cache_vec->at(i);
        std::stringstream ss;
        std::string num;
        ss << i+1;
        ss >> num;

        // 计算价格字段和返点字段
        std::string price_string, fan_dian_string;
        GetPriceStrings(cache->price, cache->rebate, cache->return_point, 0, 1, price_string, fan_dian_string);

        ss.clear();
        xls.StartNewLine(16);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), (GetDateString() + "(" + num + ")").c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), cache->bond_id.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), Utf8ToAnsi(cache->bond_short_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"), price_string.c_str());

        std::string price = GetStringFromDouble(cache->price) + "%";
        double vol = cache->volume / 1000.0;
        totalVolume += cache->volume / 10000.0;

        ss << vol;
        ss >> num;
        ss.clear();        

        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),price.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),num.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),cache->time_to_maturity.c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),GetStringFromDouble(cache->bid_brokerrage).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),GetStringFromDouble(cache->ofr_brokerrage).c_str());

        if(abs(cache->bid_brokerrage) >= FLOATING_PRECISION){
            totalBrokerrage += cache->bid_brokerrage;
        }
        if(abs(cache->ofr_brokerrage) >= FLOATING_PRECISION){
            totalBrokerrage += cache->ofr_brokerrage;
        }

        std::string bidAgent = cache->bid_trader_name;
        std::string ofrAgent = cache->ofr_trader_name;
        if(cache->bid_bridge == "2")
            bidAgent = bidAgent + " 过券";
        if(cache->ofr_bridge == "2")
            ofrAgent = ofrAgent + " 过券";

        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->bid_institution_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(bidAgent).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(cache->ofr_institution_name).c_str());
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),Utf8ToAnsi(ofrAgent).c_str());
        std::string remarks = Utf8ToAnsi(cache->bid_broker_name) + " " + Utf8ToAnsi(cache->ofr_broker_name);
        xls.FillExcelCell(CELL_NORMAL(32,BDR_SOLID(0),"x:str"),remarks.c_str());
        xls.EndCurrLine();
    }

    std::stringstream ts;
    std::string n;
    std::string m;
    std::string v;
    ts << i;
    ts >> n;
    ts.clear();
    ts << totalVolume;
    ts >> m;
    ts.clear();
    ts << totalBrokerrage;
    ts >> v;
    std::string totalInfo = "成交笔数" + n + "笔 成交数量" + m + "亿 成交金额" + v + "元";

    xls.StartNewLine(16);
    xls.FillExcelCell(CELL_COLSPAN(14,32,BDR_SOLID(0),"x:str"),totalInfo.c_str());
    xls.EndCurrLine();

    xls.EndExeclFile();
    return true;
}

//dcs
bool GenerateExcel::SaveBondDealDataPATR(const DcsBondDealCacheVecPtr& cache_vec)
{
	ExcelEngine xls;

	if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
	{
		std::cout << "Can't find head.txt" << std::endl;
		return false;
	}

	xls.SetColumnWidth(1, 80);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 85);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);

	xls.StartNewLine(18);
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Reference");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Bond Name");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Bond Code");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Payment");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Quntity");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Yield");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Clean Price");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Settle Date");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Buyer");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Buyer Trader");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Buyer Broker");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Seller");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Seller Trader");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Seller Broker");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Full Rate");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Full Value");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Maturity date");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Days");
	xls.EndCurrLine();

	int tableR = cache_vec->size();

	for (int i = 0; i < tableR; ++i)
	{
		DcsBondDealCachePtr cache = cache_vec->at(i);
		xls.StartNewLine(16);
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), "");
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bond_short_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->bond_id.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->delivery_type).c_str());

		std::stringstream ss;
		std::string num;
		ss << cache->volume / 100.0;
		ss >> num;
		num += "m";
		ss.clear();

		std::string pirce = GetStringFromDouble(cache->price) + "%";
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), num.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), pirce.c_str());

		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->net_price > 0 ? GetStringFromDouble(cache->net_price).c_str() : "");
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->settlement_date.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bid_institution_name).c_str());

		std::string bidAgent = cache->bid_trader_name;
		std::string ofrAgent = cache->ofr_trader_name;

		if (cache->bid_bridge == "1")
			bidAgent = bidAgent + " " + cache->bid_bridge;
		if (cache->ofr_bridge == "1")
			ofrAgent = ofrAgent + " " + cache->ofr_bridge;

		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(bidAgent).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bid_broker_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->ofr_institution_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(ofrAgent).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->ofr_broker_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->full_price > 0 ? GetStringFromDouble(cache->full_price).c_str() : "");

		double full = 0;
		if (cache->full_price == INVALIDVALUE || cache->volume == INVALIDVALUE || cache->volume < 0 || cache->full_price < 0){
			full = 0;
		}
		else{
			full = cache->full_price*cache->volume;
		}

		ss << full;
		ss >> num;
		ss.clear();

		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), num != "0" ? num.c_str() : "");
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->maturity_date.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->time_to_maturity.c_str());
		xls.EndCurrLine();
	}

	xls.EndExeclFile();

	return true;
}

bool GenerateExcel::SaveBondDealDataBGC(const DcsBondDealCacheVecPtr& cache_vec)
{
	ExcelEngine xls;

	if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
	{
		std::cout << "Can't find head.txt" << std::endl;
		return false;
	}

	xls.SetColumnWidth(1, 80);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 85);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);

	xls.StartNewLine(18);
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "交易时间");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "债券代码");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "简称");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "收益率");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "成交量");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "期限");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "买入方");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "交易员");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Broker");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "卖出方");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "交易员");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "Broker");
	xls.EndCurrLine();

	int tableR = cache_vec->size();

	for (int i = 0; i < tableR; ++i)
	{
		DcsBondDealCachePtr cache = cache_vec->at(i);

		xls.StartNewLine(16);
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), "");
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->settlement_date.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->bond_id.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bond_short_name).c_str());

		std::string price = GetStringFromDouble(cache->price) + "%";
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), price.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), (int)cache->volume);
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->time_to_maturity).c_str());

		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bid_institution_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bid_trader_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bid_broker_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->ofr_institution_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->ofr_trader_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->ofr_broker_name).c_str());
		xls.EndCurrLine();
	}

	xls.EndExeclFile();

	return true;
}

bool GenerateExcel::SaveBondDealDataICAP(const DcsBondDealCacheVecPtr& cache_vec)
{
	ExcelEngine xls;

	if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
	{
		std::cout << "Can't find head.txt" << std::endl;
		return false;
	}

	xls.SetColumnWidth(1, 80);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 80);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);

	xls.StartNewLine(18);
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "成交日期");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "代码");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "简称");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "评级");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "到期期限");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "成交收益率");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "卖方");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "买方");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "量");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "备注");
	xls.EndCurrLine();

	int tableR = cache_vec->size();

	for (int i = 0; i < tableR; ++i)
	{
		DcsBondDealCachePtr cache = cache_vec->at(i);
		xls.StartNewLine(16);
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), "");
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->settlement_date.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->bond_id.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bond_short_name).c_str());

		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->issuer_rating.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->time_to_maturity.c_str());
		std::string price = GetStringFromDouble(cache->price) + "%";

		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), price.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->ofr_institution_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bid_institution_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), (int)cache->volume);
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->description).c_str());
		xls.EndCurrLine();
	}
	xls.EndExeclFile();

	return true;
}

bool GenerateExcel::SaveBondDealDataTPBCO(const DcsBondDealCacheVecPtr& cache_vec)
{
	ExcelEngine xls;

	if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
	{
		std::cout << "Can't find head.txt" << std::endl;
		return false;
	}

	xls.SetColumnWidth(1, 110);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 80);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);

	xls.StartNewLine(18);
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "债券代码");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "简称");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "评级");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "收益率");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "成交量");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "剩余天数");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "买入佣金");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "卖出佣金");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "买入方");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "交易员");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "卖出方");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "交易员");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "备注");
	xls.EndCurrLine();

	int tableR = cache_vec->size();
	double totalVolume = 0;
	double totalBrokerrage = 0;

	int i = 0;

	for (i = 0; i < tableR; ++i)
	{
		DcsBondDealCachePtr cache = cache_vec->at(i);
		std::stringstream ss;
		std::string num;
		ss << i + 1;
		ss >> num;
		ss.clear();
		xls.StartNewLine(16);
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), (GetDateString() + "(" + num + ")").c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->bond_id.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bond_short_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bond_rating).c_str());

		std::string price = GetStringFromDouble(cache->price) + "%";
		double vol = cache->volume / 1000.0;
		totalVolume += cache->volume / 10000.0;

		ss << vol;
		ss >> num;
		ss.clear();

		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), price.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), num.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->time_to_maturity.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), GetStringFromDouble(cache->bid_brokerrage).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), GetStringFromDouble(cache->ofr_brokerrage).c_str());

		if (abs(cache->bid_brokerrage) >= FLOATING_PRECISION){
			totalBrokerrage += cache->bid_brokerrage;
		}
		if (abs(cache->ofr_brokerrage) >= FLOATING_PRECISION){
			totalBrokerrage += cache->ofr_brokerrage;
		}

		std::string bidAgent = cache->bid_trader_name;
		std::string ofrAgent = cache->ofr_trader_name;

		if (cache->bid_bridge == "2")
			bidAgent = bidAgent + " 过券";
		if (cache->ofr_bridge == "2")
			ofrAgent = ofrAgent + " 过券";

		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bid_institution_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(bidAgent).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->ofr_institution_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(ofrAgent).c_str());
		std::string remarks = Utf8ToAnsi(cache->bid_broker_name) + " " + Utf8ToAnsi(cache->ofr_broker_name);
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), remarks.c_str());
		xls.EndCurrLine();
	}
	std::stringstream ts;
	std::string n;
	std::string m;
	std::string v;
	ts << i;
	ts >> n;
	ts.clear();
	ts << totalVolume;
	ts >> m;
	ts.clear();
	ts << totalBrokerrage;
	ts >> v;
	std::string totalInfo = "成交笔数" + n + "笔 成交数量" + m + "亿 成交金额" + v + "元";
	xls.StartNewLine(16);
	xls.FillExcelCell(CELL_COLSPAN(14, 32, BDR_SOLID(0), "x:str"), totalInfo.c_str());
	xls.EndCurrLine();

	xls.EndExeclFile();
	return true;
}

bool GenerateExcel::SaveBondDealDataTPBNC(const DcsBondDealCacheVecPtr& cache_vec)
{
	ExcelEngine xls;

	if (ERR_WRITE_HEAD_FAILED == xls.CreateExeclFile(m_fileName.c_str()))
	{
		std::cout << "Can't find head.txt" << std::endl;
		return false;
	}

	xls.SetColumnWidth(1, 110);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 80);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 70);
	xls.SetColumnWidth(1, 80);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);
	xls.SetColumnWidth(1, 105);

	xls.StartNewLine(18);
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "债券代码");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "简称");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "成交价格");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "收益率");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "成交量");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "剩余天数");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "买入佣金");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "卖出佣金");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "买入方");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "交易员");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "卖出方");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "交易员");
	xls.FillExcelCell(CELL_NORMAL(40, BDR_SOLID(0), ""), "备注");
	xls.EndCurrLine();

	int tableR = cache_vec->size();
	double totalVolume = 0;
	double totalBrokerrage = 0;

	int i = 0;

	for (i = 0; i < tableR; ++i)
	{
		DcsBondDealCachePtr cache = cache_vec->at(i);
		std::stringstream ss;
		std::string num;
		ss << i + 1;
		ss >> num;

		// 计算价格字段和返点字段
		std::string price_string, fan_dian_string;
		GetPriceStrings(cache->price, cache->rebate, cache->return_point, 0, 1, price_string, fan_dian_string);

		ss.clear();
		xls.StartNewLine(16);
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), (GetDateString() + "(" + num + ")").c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->bond_id.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bond_short_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), price_string.c_str());

		std::string price = GetStringFromDouble(cache->price) + "%";
		double vol = cache->volume / 1000.0;
		totalVolume += cache->volume / 10000.0;

		ss << vol;
		ss >> num;
		ss.clear();

		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), price.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), num.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), cache->time_to_maturity.c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), GetStringFromDouble(cache->bid_brokerrage).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), GetStringFromDouble(cache->ofr_brokerrage).c_str());

		if (abs(cache->bid_brokerrage) >= FLOATING_PRECISION){
			totalBrokerrage += cache->bid_brokerrage;
		}
		if (abs(cache->ofr_brokerrage) >= FLOATING_PRECISION){
			totalBrokerrage += cache->ofr_brokerrage;
		}

		std::string bidAgent = cache->bid_trader_name;
		std::string ofrAgent = cache->ofr_trader_name;
		if (cache->bid_bridge == "2")
			bidAgent = bidAgent + " 过券";
		if (cache->ofr_bridge == "2")
			ofrAgent = ofrAgent + " 过券";

		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->bid_institution_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(bidAgent).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(cache->ofr_institution_name).c_str());
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), Utf8ToAnsi(ofrAgent).c_str());
		std::string remarks = Utf8ToAnsi(cache->bid_broker_name) + " " + Utf8ToAnsi(cache->ofr_broker_name);
		xls.FillExcelCell(CELL_NORMAL(32, BDR_SOLID(0), "x:str"), remarks.c_str());
		xls.EndCurrLine();
	}

	std::stringstream ts;
	std::string n;
	std::string m;
	std::string v;
	ts << i;
	ts >> n;
	ts.clear();
	ts << totalVolume;
	ts >> m;
	ts.clear();
	ts << totalBrokerrage;
	ts >> v;
	std::string totalInfo = "成交笔数" + n + "笔 成交数量" + m + "亿 成交金额" + v + "元";

	xls.StartNewLine(16);
	xls.FillExcelCell(CELL_COLSPAN(14, 32, BDR_SOLID(0), "x:str"), totalInfo.c_str());
	xls.EndCurrLine();

	xls.EndExeclFile();
	return true;
}

std::string GenerateExcel::GetStringFromDouble(double data)
{
    std::stringstream ss;
    ss << data;
    return ss.str();
}

std::string GenerateExcel::GetDateString()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%Y-%m-%d",timeinfo);
    std::string str(buffer);

    return str;
}

std::string GenerateExcel::GetPriceWithR(double _price, int _type, sdbus::string rebate, double returnPoint)
{
    std::string tempPrice = "";
    try {
        if (_price == INVALIDVALUE)
        {
            if(rebate == "1"){
                if (returnPoint == INVALIDVALUE) {
                    return "f";
                } else {
                    return "f" + GetStringFromDouble(returnPoint);
                }
            }else{
                if (_type == 1)
                {
                    return "Bid";
                } 
                else
                {
                    return "Ofr";
                }
            }
        }
        else
        {
            if(rebate == "1"){
                if (returnPoint == INVALIDVALUE) {
                    return GetStringFromDouble(_price) + "f";
                } else {
                    return GetStringFromDouble(_price) + "f" + GetStringFromDouble(returnPoint);
                }
            }
            return GetStringFromDouble(_price);
        }
    } 
    catch (int) 
    {
        return "";
    }    
}