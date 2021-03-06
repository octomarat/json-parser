#ifndef __JSON_H__
#define __JSON_H__

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <cstddef>
#include <initializer_list>

#include "json_model.h"
#include "mapping_iterator.h"
#include "json_mutable.h"
#include "json_ref.h"
#include "const_json_ref.h"

namespace my_cpp_json {

class ConstJsonRef;

class Json: public internal::proxy_impl::JsonMutable<JsonRef, ConstJsonRef> {
    friend class JsonRef;
    friend class ConstJsonRef;
public:
    Json() = default;
    Json(std::nullptr_t np): Json() {}
    Json(std::string const& str);
    Json(char const *c_str): Json(std::string(c_str)) {}
    Json(double num);
    Json(int num): Json(static_cast<double>(num)) {}
    Json(bool b);
    Json(std::initializer_list<std::pair<const std::string, Json>> const &lst);

    Json(Json const &other);
    Json(Json &&other) noexcept;
    ~Json() { /*TODO use recursive remove to prevent stack overflow*/ }

    Json &operator=(Json const &other);
    Json &operator=(Json &&other) noexcept;

    // conversions between ConstJsonRef, JsonRef and Json
    Json(JsonRef const &json_ref);
    Json(ConstJsonRef const &const_json_ref);
    
    Json &operator=(JsonRef const &json_ref);
    Json &operator=(ConstJsonRef const &const_json_ref);

    Json &operator+=(Json const& elem);
    Json &operator+=(JsonRef const& elem);
    Json &operator+=(ConstJsonRef const& elem);

    // these functions are meant to be used as literals 
    // so they have short lower case names
    static Json obj(std::initializer_list<std::pair<const std::string, Json>> const &lst = {});
    static Json arr(std::initializer_list<Json> const &lst = {});
protected:
    JsonValuePtr const &Value() const override { return value_; }
    JsonValuePtr &Value() override { return value_; }
    
    JsonRef ProxyJsonRef(JsonValuePtr &val) override { return JsonRef(val); }
    ConstJsonRef ProxyConstJsonRef(JsonValuePtr &val) const override { return ConstJsonRef(val); }
private:
    JsonValuePtr value_;
};

}

#endif