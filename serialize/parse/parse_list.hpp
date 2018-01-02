#ifndef INC_201704_PARSE_LIST_H
#define INC_201704_PARSE_LIST_H

#include <optional>
#include "basic_parse.hpp"
#include "../basic_serialize.hpp"

namespace ds_expr
{
    namespace serialize
    {
        namespace parse
        {
            template <typename T>
            void assign_value(T &value, std::string const &input)
            {
                std::istringstream stream(input);
                stream >> value;
            }
            void assign_value(std::string &value, std::string const &input)
            {
                value = input;
            }

            template <typename T>
            class parse_list
            {
            public:
                using value_type = T;
                using vector_type = std::vector<value_type>;
                explicit parse_list(std::istream &in)
                    : in(in), helper(in)
                {}
                vector_type extract_list()
                {
                    helper.force_read(open_brace);
                    auto list = get_list();
                    helper.force_read(close_brace);
                    return list;
                }

            private:
                vector_type get_list()
                {
                    if (auto value = get_value())
                    {
                        std::vector<value_type> left_operand = {*value};
                        while (helper.read(separator))
                            left_operand.push_back(force_get_value());
                        return left_operand;
                    } else
                        return vector_type{};
                }
            public:
                std::optional<value_type> get_value()
                {
                    if (helper.read(begin_paren))
                    {
                        auto value = read_raw_value();
                        helper.force_read(close_paren);
                        return value;
                    }
                    return std::nullopt;
                }
                value_type force_get_value()
                {
                    helper.force_read(begin_paren);
                    auto value = read_raw_value();
                    helper.force_read(close_paren);
                    return value;
                }
            private:
                value_type read_raw_value()
                {
                    value_type value;
                    assign_value(value, helper.read_until(close_paren));
                    return value;
                }
                static constexpr char open_brace = '{', close_brace = '}', separator = ',', begin_paren = '(', close_paren = ')';
                std::istream &in;
                detail::parse_helper<char> helper;
            };
        }
    }

}

#endif //INC_201704_PARSE_LIST_H
