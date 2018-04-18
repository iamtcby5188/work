#ifndef ADD_BOND_QUOTE_H_
#define ADD_BOND_QUOTE_H_

#include "sdbus/connection.h"
#include "message.h"
#include "include/common.h"
#include <list>

namespace bcserver_service_transform {

    class AddBondQuote
    {
    public:
        AddBondQuote(const sdbus::Message *msg);

        bool sendquote(sdbus::Message &reply);
        void processError();

    protected:
        BOND_VARIANT_MAP_LIST mSrc_;
        BOND_VARIANT_MAP_LIST mDes_;
        std::list<int> errList_;

    private:
        void addError(QQ2IDB_ERROR_CODE);
        void removeError(QQ2IDB_ERROR_CODE);
        void copyUsefulDataForQuote();
    };

}  // namespace bcserver_service_transform

#endif // ADD_BOND_QUOTE_H_