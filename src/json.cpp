#include "json.h"

#include <stdexcept>

#include "json_ref.h"

using namespace json_cpp;
using namespace json_cpp::inner::json_model;
using namespace json_cpp::inner::utils;

using std::string;
using std::pair;
using std::runtime_error;
using std::initializer_list;
using std::make_shared;
using std::shared_ptr;

Json::Json(string const& str)
    : value_(new JsonString(str))
{}

Json::Json(double num)
    : value_(new JsonNumber(num))
{}

Json::Json(bool b)
    : value_(new JsonBool(b))
{}

Json::Json(initializer_list<pair<const string, Json>> const &lst) {
    auto obj = make_shared<JsonObject>();
    auto &map = obj->value();
    for(auto &p: lst) {
        map.insert(p.first, p.second.value_);
    }
    value_ = std::move(obj);
}

Json::Json(JsonRef const &json_ref)
    : value_(CopyJsonTree(json_ref.value_ref_))
{}

Json::Json(Json const &other) 
    : value_(CopyJsonTree(other.value_))
{}

Json::Json(Json &&other) noexcept
    : value_(std::move(other.value_))
{}

Json::~Json() {}

Json &Json::operator=(Json const& other) {
    if(this != &other) {
        value_ = CopyJsonTree(other.value_);
    }
    return *this;
}

Json &Json::operator=(Json &&other) noexcept {
    if(this != &other) {
        value_ = std::move(other.value_);
    }
    return *this;
}

inline string InvalidOperation(string const &prefix, JType const &t) {
    return prefix + JTypeUtils::ToString(t);
}

JsonRef const Json::operator[](string const &field_name) const {
    if(value_->type() == JType::JOBJECT) {
        return JsonRef(as<JsonObject>(value_)->value()[field_name]);
    }
    throw runtime_error(InvalidOperation("attempt to access field on ", value_->type()));
}

JsonRef Json::operator[](string const &field_name) {
    if(value_->type() == JType::JOBJECT) {
        return JsonRef(as<JsonObject>(value_)->value()[field_name]);
    }
    throw runtime_error(InvalidOperation("attempt to access field on ", value_->type()));
}

JsonRef const Json::operator[](size_type index) const {
    if(value_->type() == JType::JARRAY) {
        return JsonRef(as<JsonArray>(value_)->value()[index]);
    }
    throw runtime_error(InvalidOperation("attempt to index ", value_->type()));
}

JsonRef Json::operator[](size_type index) {
    if(value_->type() == JType::JARRAY) {
        return JsonRef(as<JsonArray>(value_)->value()[index]);
    }
    throw runtime_error(InvalidOperation("attempt to index ", value_->type()));
}

Json &Json::operator+=(Json const& val) {
    if(value_->type() == JType::JARRAY) {
        as<JsonArray>(value_)->value().push_back(val.value_);
        return *this;
    }
    throw runtime_error(InvalidOperation("attempt to append element to ", value_->type()));
}

Json::object_iterator Json::ObjectBegin() {
    if(value_->type() == JType::JOBJECT) {
        return MakeMappingBegin(as<JsonObject>(value_)->value().begin(), &Json::ProxyEntry);
    }
    throw runtime_error(InvalidOperation("attempt to access object iterator on ", value_->type()));
}

Json::object_const_iterator Json::ObjectBegin() const {
    if(value_->type() == JType::JOBJECT) {
        return MakeMappingBegin(as<JsonObject>(value_)->value().begin(), &Json::ProxyEntryConst);
    }
    throw runtime_error(InvalidOperation("attempt to access object iterator on ", value_->type()));
}

Json::object_iterator Json::ObjectEnd() {
    if(value_->type() == JType::JOBJECT) {
        return MakeMappingBegin(as<JsonObject>(value_)->value().end(), &Json::ProxyEntry);
    }
    throw runtime_error(InvalidOperation("attempt to access object iterator on ", value_->type()));
}

Json::object_const_iterator Json::ObjectEnd() const {
    if(value_->type() == JType::JOBJECT) {
        return MakeMappingBegin(as<JsonObject>(value_)->value().end(), &Json::ProxyEntryConst);
    }
    throw runtime_error(InvalidOperation("attempt to access object iterator on ", value_->type()));
}

Json::array_iterator Json::ArrayBegin() {
    if(value_->type() == JType::JARRAY) {
        return MakeMappingBegin(as<JsonArray>(value_)->value().begin(), &Json::Proxy);
    }
    throw runtime_error(InvalidOperation("attempt to access array iterator on ", value_->type()));
}

Json::array_const_iterator Json::ArrayBegin() const {
    if(value_->type() == JType::JARRAY) {
        return MakeMappingBegin(as<JsonArray>(value_)->value().begin(), &Json::ProxyConst);
    }
    throw runtime_error(InvalidOperation("attempt to access array iterator on ", value_->type()));
}

Json::array_iterator Json::ArrayEnd() {
    if(value_->type() == JType::JARRAY) {
        return MakeMappingBegin(as<JsonArray>(value_)->value().end(), &Json::Proxy);
    }
    throw runtime_error(InvalidOperation("attempt to access array iterator on ", value_->type()));
}

Json::array_const_iterator Json::ArrayEnd() const {
    if(value_->type() == JType::JARRAY) {
        return MakeMappingBegin(as<JsonArray>(value_)->value().end(), &Json::ProxyConst);
    }
    throw runtime_error(InvalidOperation("attempt to access array iterator on ", value_->type()));
}

inline string BadConversion(JType const &from, string const &to) {
    string msg = "attempt to convert ";
    return msg + JTypeUtils::ToString(from) + " to " + to;
}

string const &Json::AsString() const {
    if(!value_ || value_->type() != JType::JSTRING) {
        throw runtime_error(BadConversion(value_->type(), "string"));
    }
    return as<JsonString>(value_)->value();
}

double Json::AsDouble() const {
    if(!value_ || value_->type() != JType::JNUMBER) {
        throw runtime_error(BadConversion(value_->type(), "double"));
    }
    return as<JsonNumber>(value_)->value();
}

bool Json::AsBool() const {
    if(!value_ || value_->type() != JType::JBOOL) {
        throw runtime_error(BadConversion(value_->type(), "bool"));
    }
    return as<JsonBool>(value_)->value();
}

nullptr_t Json::AsNull() const {
    if(value_) {
        throw runtime_error(BadConversion(value_->type(), "nullptr"));
    }
    return nullptr;
}

// static members

Json Json::obj(initializer_list<pair<const string, Json>> const &lst) {
    return Json(lst);
}

Json Json::arr(initializer_list<Json> const &lst) {
    auto arr = make_shared<JsonArray>();
    auto &vec = arr->value();
    for(auto &l: lst) {
        vec.push_back(l.value_);
    }
    Json json;
    json.value_ = std::move(arr);
    return json;
}