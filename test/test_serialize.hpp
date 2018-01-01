#ifndef INC_201704_TEST_SERIALIZE_HPP
#define INC_201704_TEST_SERIALIZE_HPP

#include <cassert>
#include "../serialize/parse/parse_list.hpp"

namespace test
{
    namespace serialize
    {
        namespace parse
        {
            namespace parse_list
            {
                template <typename T>
                inline void test_case(std::string_view string, std::vector<T> expected)
                {
                    std::istringstream stream{std::string{string}};
                    ds_expr::serialize::parse::parse_list<T> parser(stream);
                    auto extracted = parser.extract_list();
                    assert(extracted == expected);
                }
            }
            inline void test()
            {
                using vector_t = std::vector<std::string>;
                parse_list::test_case("{}", vector_t{});
                parse_list::test_case("{(a)}", vector_t{"a"});
                parse_list::test_case("{(one), (two ),( three), (space middle)}", vector_t{"one", "two ", " three", "space middle"});
                parse_list::test_case("{(\\)),(\\\\),(\\,),(\\})}", vector_t{")", "\\", "\\,", "\\}"});
            }
        }
        inline void test()
        {
            parse::test();
        }
    }

}

#endif //INC_201704_TEST_SERIALIZE_HPP
