#ifndef INC_201704_TEST_SERIALIZE_HPP
#define INC_201704_TEST_SERIALIZE_HPP

#include <cassert>
#include "../serialize/parse/parse_list.hpp"
#include "../graph/vertex.hpp"
#include "../graph/directed_graph.hpp"
#include "../graph/undirected_graph.hpp"

namespace test
{
    namespace serialize
    {
        namespace parse
        {
            namespace parse_list
            {
                template <typename T>
                void test_case(std::string_view string, std::vector<T> expected)
                {
                    std::istringstream stream{std::string{string}};
                    ds_expr::serialize::parse::parse_list<T> parser(stream);
                    auto extracted = parser.extract_list();
                    assert(extracted == expected);
                }
                inline void test()
                {
                    using vector_t = std::vector<std::string>;
                    test_case("{}", vector_t{});
                    test_case("{(a)}", vector_t{"a"});
                    test_case("{(one), (two ),( three), (space middle)}", vector_t{"one", "two ", " three", "space middle"});
                    test_case("{(\\)),(\\\\),(\\,),(\\})}", vector_t{")", "\\", "\\,", "\\}"});
                }
            }
            inline void test()
            {
                parse_list::test();
            }
        }
        inline void test_basic_serialize()
        {
            lzhlib::detail::vertex<std::string> v(std::in_place, "vertex");
            std::ostringstream stream;
            stream << v;
        }
        inline void test()
        {
            parse::test();
            test_basic_serialize();
        }
    }
}

#endif //INC_201704_TEST_SERIALIZE_HPP
