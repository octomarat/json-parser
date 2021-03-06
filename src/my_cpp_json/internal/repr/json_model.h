#ifndef __JSON_MODEL_H__
#define __JSON_MODEL_H__

#include <string>
#include <map>
#include <vector>
#include <initializer_list>
#include <memory>

#include "json_type.h"
#include "ordered_hashmap.h"

namespace my_cpp_json { namespace internal { namespace repr {

class JsonValue {
public:
    virtual JType type() const = 0;
    virtual ~JsonValue() {}
};

class JsonString: public JsonValue {
public:
    explicit JsonString(std::string const &s): value_(s) {}

    JType type() const override { return JType::JSTRING; }
    std::string &value() { return value_; }
private:
    std::string value_;
};

class JsonNumber: public JsonValue {
public:
    explicit JsonNumber(double v): value_(v) {}

    JType type() const override { return JType::JNUMBER; }
    double &value() { return value_; }
private:
    double value_ = 0.0;
};

class JsonObject: public JsonValue {
public:
    using iterator = 
        utils::OrderedHashMap<std::string, std::shared_ptr<JsonValue>>::iterator;
    using const_iterator = 
        utils::OrderedHashMap<std::string, std::shared_ptr<JsonValue>>::const_iterator;
    
    JsonObject() = default;
    explicit JsonObject(
        std::initializer_list<std::pair<std::string const, std::shared_ptr<JsonValue>>> const &lst
    );

    JType type() const override { return JType::JOBJECT; }
    utils::OrderedHashMap<std::string, std::shared_ptr<JsonValue>> &value() { return value_; }
private:
    utils::OrderedHashMap<std::string, std::shared_ptr<JsonValue>> value_;
};

class JsonArray: public JsonValue {
public:
    using size_type = std::vector<std::shared_ptr<JsonValue>>::size_type;
    using iterator = std::vector<std::shared_ptr<JsonValue>>::iterator;
    using const_iterator = std::vector<std::shared_ptr<JsonValue>>::const_iterator;

    JsonArray() = default;
    explicit JsonArray(std::initializer_list<std::shared_ptr<JsonValue>> const &lst);

    JType type() const override { return JType::JARRAY; }
    std::vector<std::shared_ptr<JsonValue>> &value() { return value_; }
private:
    std::vector<std::shared_ptr<JsonValue>> value_;
};

class JsonBool: public JsonValue {
public:
    explicit JsonBool(bool b): value_(b) {}

    JType type() const override { return JType::JBOOL; }
    bool &value() { return value_; }
private:
    bool value_ = false;
};

std::shared_ptr<JsonValue> CopyJsonTree(std::shared_ptr<JsonValue> const &original);

template<typename T>
struct GetJType {};

template<>
struct GetJType<JsonString> {
    static JType const value = JType::JSTRING;
};

template<>
struct GetJType<JsonNumber> {
    static JType const value = JType::JNUMBER;
};

template<>
struct GetJType<JsonObject> {
    static JType const value = JType::JOBJECT;
};

template<>
struct GetJType<JsonArray> {
    static JType const value = JType::JARRAY;
};

template<>
struct GetJType<JsonBool> {
    static JType const value = JType::JBOOL;
};

}}}

#endif