#ifndef INC_201704_BASIC_SERIALIZE_HPP
#define INC_201704_BASIC_SERIALIZE_HPP

#include <istream>
#include <vector>
#include "parse/parse_list.hpp"

namespace ds_expr
{
    namespace serialize
    {
        template <typename T>
        std::istream &operator>>(std::istream &in, std::vector<T> &vector)
        {
            vector = parse::parse_list(in).extract_list();
            return in;
        }
        template <typename T>
        std::ostream &operator<<(std::ostream &out, std::vector<T> const &vector)
        {
            out << "{";
            for (auto const &element : vector)
                out << "(" << element << ")";
            out << "}";
            return out;
        }

        template <typename Graph>
        std::istream &operator>>(std::istream &in, Graph &graph)
        {

        }
    }
}
#endif //INC_201704_BASIC_SERIALIZE_HPP
