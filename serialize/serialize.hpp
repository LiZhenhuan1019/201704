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
        template <typename T, typename Compare>
        std::istream &operator>>(std::istream &in, std::set<T, Compare> &set)
        {
            std::vector<T> vector;
            in >> vector;
            std::copy(vector.begin(), vector.end(), std::inserter(set, set.end()));
            return in;
        }
        template <typename Iter>
        std::ostream &serialize(std::ostream &out, Iter begin, Iter end)
        {
            out << "{";
            if (begin != end)
            {
                auto iter = begin;
                out << "(";
                output_with_escape(out, *iter, ')') << ")";
                for (++iter; iter != end; ++iter)
                {
                    out << ",(";
                    output_with_escape(out, *iter, ')') << ")";
                }
            }
            out << "}";
            return out;
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
