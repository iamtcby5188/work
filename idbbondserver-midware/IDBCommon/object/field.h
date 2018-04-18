#ifndef _FIELD_H_
#define _FIELD_H_

#include <sdbus/variant.h>
#include <sdbus/message.h>

#define SETIFASSIGNED(A, B) \
    if (B.assigned()) { \
        A = B; \
    } \

template<class T>
class Field
{
public:
    Field() : assigned_(false) {}
    Field(const Field<T>& rhs) : value_(rhs.value_), assigned_(false) {}
    Field(const T& value) : value_(value), assigned_(false) {}
    Field& operator= (const Field<T>& rhs) {value_ = rhs.value_; assigned_ = true; return *this;}
    Field& operator= (const T& value) {value_ = value; assigned_ = true; return *this;}

    operator T () {return value_;}
    operator const T() const {return value_;}

    const T& ref() const {return value_;}

    bool assigned() const {return assigned_;}

private:
    T value_;
    bool assigned_;
};

void GetFieldString(const sdbus::VariantMap& mp, int fid, Field<std::string>& field);
void GetFieldInt(const sdbus::VariantMap& mp, int fid, Field<int>& field);
void GetFieldDouble(const sdbus::VariantMap& mp, int fid, Field<double>& field);
void GetFieldBool(const sdbus::VariantMap& mp, int fid, Field<bool>& field);
void GetFieldString(const sdbus::Message* msg, int fid, Field<std::string>& field);
void GetFieldInt(const sdbus::Message* msg, int fid, Field<int>& field);
void GetFieldDouble(const sdbus::Message* msg, int fid, Field<double>& field);
void GetFieldBool(const sdbus::Message* msg, int fid, Field<bool>& field);

#endif
