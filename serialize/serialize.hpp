#ifndef INC_201704_SERIALIZE_HPP
#define INC_201704_SERIALIZE_HPP

#include <istream>
#include <vector>
#include <set>
#include "parse/parse_list.hpp"
#include "basic_serialize.hpp"

namespace ds_expr
{
    namespace serialize
    {

        template <typename T>
        std::istream &operator>>(std::istream &in, std::vector<T> &vector)
        {
            vector = parse::parse_list<T>(in).extract_list();
            return in;
        }
        template <typename T>
        std::ostream &operator<<(std::ostream &out, std::vector<T> const &vector)
        {
            serialize(out, vector.begin(), vector.end());
            return out;
        }
        template <typename T, typename Compare>
        std::ostream &operator<<(std::ostream &out, std::set<T, Compare> const &set)
        {
            serialize(out, set.begin(), set.end());
            return out;
        }
    }
}
#endif //INC_201704_SERIALIZE_HPP
