#ifndef __JSON_SERIALIZER_H__
#define __JSON_SERIALIZER_H__

#include <iostream>

#include "my_cpp_json.h"

namespace my_cpp_json {

class JsonSerializer {
public:
    void PrettyPrint(Json const &json, std::ostream &os, unsigned int indent_size = 4);
    void CompactPrint(Json const &json, std::ostream &os);

    std::string PrettyPrint(Json const &json, unsigned int indent_size = 4);
    std::string CompactPrint(Json const &json);
private:
    void Print(
        ConstJsonRef const &json,
        std::ostream &os,
        unsigned int cur_indent
    );
    void MaybeEndl(std::ostream &os);
    void MaybeSpaces(std::ostream &os, unsigned int spaces_number);

    unsigned int indent_size_ = 4;
    bool compact_mode_ = false;
};

}

#endif