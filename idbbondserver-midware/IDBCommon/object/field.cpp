#include "field.h"

void GetFieldString(const sdbus::VariantMap& mp, int fid, Field<std::string>& field)
{
    std::string val;
    if (mp.GetString(fid, val)) {
        field = val;
    }
}

void GetFieldInt(const sdbus::VariantMap& mp, int fid, Field<int>& field)
{
    int val;
    if (mp.GetInt32(fid, val)) {
        field = val;
    }
}

void GetFieldDouble(const sdbus::VariantMap& mp, int fid, Field<double>& field)
{
    double val;
    if (mp.GetDouble(fid, val)) {
        field = val;
    }
}

void GetFieldBool(const sdbus::VariantMap& mp, int fid, Field<bool>& field)
{
    bool val;
    if (mp.GetBool(fid, val)) {
        field = val;
    }
}

void GetFieldString(const sdbus::Message* msg, int fid, Field<std::string>& field)
{
	std::string val;
	if (msg->GetString(fid, val)) {
		field = val;
	}
}

void GetFieldInt(const sdbus::Message* msg, int fid, Field<int>& field)
{
	int val;
	if (msg->GetInt32(fid, val)) {
		field = val;
	}
}

void GetFieldDouble(const sdbus::Message* msg, int fid, Field<double>& field)
{
	double val;
	if (msg->GetDouble(fid, val)) {
		field = val;
	}
}

void GetFieldBool(const sdbus::Message* msg, int fid, Field<bool>& field)
{
	bool val;
	if (msg->GetBool(fid, val)) {
		field = val;
	}
}