/*
 * issuinginstitution_cache.h
 *
 *  Created on: June 30, 2014
 *      Author: theochen
 */

#ifndef ISSUINGINSTITUTION_CACHE_H_
#define ISSUINGINSTITUTION_CACHE_H_

#include "cache_def.h"

class IssuingInstitutionCache;
typedef std::tr1::shared_ptr<IssuingInstitutionCache> IssuingInstitutionCachePtr;
typedef std::tr1::shared_ptr<std::vector<IssuingInstitutionCachePtr> > IssuingInstitutionCacheVecPtr;

class IssuingInstitutionCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
    static std::string SQL_TAG;
public:
    IssuingInstitutionCache();
    virtual ~IssuingInstitutionCache();
	std::string getPrimaryKey();

public:
	std::string id;
    std::string institution_code;
    std::string institution_type;
    std::string full_name_c;
	std::string delflag;
    std::string Pin_Yin;
    std::string Pin_Yin_Full;
    std::string institution_subtype;
    std::string institution_rating;

public:
    //inline IssuingInstitutionCache& operator == (const IssuingInstitutionCache& cache) {
    //      if (&cache == this)
    //      {
    //          return *this;
    //      }
    //      this->institution_code = cache.institution_code;
    //      this->institution_type = cache.institution_type;
    //      this->full_name_c = cache.full_name_c;
    //      this->Pin_Yin = cache.Pin_Yin;
    //      this->Pin_Yin_Full = cache.Pin_Yin_Full;
    //      this->institution_rating = cache.institution_rating;
    //      this->institution_subtype = cache.institution_subtype;

    //      return *this;
    //}

    bool operator == (const IssuingInstitutionCache &rhs);
    bool operator != (const IssuingInstitutionCache &rhs);

public:
    TYPE_DESCRIPTOR(
        (KEY(id, HASHED /*| INDEXED*/),
		 KEY(institution_code, HASHED /*| INDEXED*/),
         FIELD(institution_type),
         FIELD(full_name_c),
		 FIELD(delflag),
         FIELD(Pin_Yin),
         FIELD(Pin_Yin_Full),
         FIELD(institution_subtype),
         FIELD(institution_rating)
        )
    );
};


/*
#define KEY(x, index) \
    (*FASTDB_NS::dbDescribeField(new FASTDB_NS::dbFieldDescriptor(#x, (char*)&x-(char*)this, \
                                                                  sizeof(x), index), x))
                                                                  */

#endif /* ISSUINGINSTITUTION_CACHE_H_ */
