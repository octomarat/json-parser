#ifndef __JSON_MUTABLE_H__
#define __JSON_MUTABLE_H__

#include "json_basic.h"
#include "mapping_iterator.h"

namespace json_cpp {

class JsonRef;

// TODO prohibit pointers creation
class JsonMutable: public JsonBasic {
public:
    using ObjectIterator = 
        inner::utils::MappingIterator<
            inner::json_model::JsonObject::iterator,
            std::pair<std::string const, JsonRef>
        >;
    using ArrayIterator =  
        inner::utils::MappingIterator<
            inner::json_model::JsonArray::iterator,
            JsonRef
        >;

    using JsonBasic::operator[];
    using JsonBasic::operator[];

    JsonRef operator[](std::string const &field_name);
    JsonRef operator[](ArraySizeType index);

    using JsonBasic::ObjectBegin;
    using JsonBasic::ObjectEnd;
    using JsonBasic::ArrayBegin;
    using JsonBasic::ArrayEnd;

    ObjectIterator ObjectBegin();
    ObjectIterator ObjectEnd();

    ArrayIterator ArrayBegin();
    ArrayIterator ArrayEnd();
protected:
    virtual JsonValuePtr &Value() = 0;

    void AddElemToArray(JsonBasic const &elem);
};

}

#endif