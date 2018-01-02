#ifndef INC_201704_BASIC_PARSE_HPP
#define INC_201704_BASIC_PARSE_HPP

#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include "../serialize_exception.hpp"

namespace ds_expr
{
    namespace serialize
    {
        namespace parse
        {
            namespace exception
            {
                using namespace std::literals;

                struct parse_failed : std::logic_error
                {
                    explicit parse_failed(char const *function_name, std::string const &reason = "")
                        : logic_error("Parse failed in '"s + function_name + "'." + reason)
                    {}
                };
            }
            namespace detail
            {
                using namespace std::literals;

                template <typename Char_T>
                struct parse_helper
                {
                    using char_type = Char_T;
                    parse_helper(std::basic_istream<char_type> &in)
                        : in(in)
                    {}
                    template <typename T>
                    bool read(T const &expected)
                    {
                        auto pos = save_pos();
                        eat_space();
                        if (raw_read(expected))
                            return true;
                        else
                            return retrieve_pos(pos), false;
                    }
                    template <typename ...T>
                    bool read(T const &...expected)
                    {
                        return ((read(expected)) || ... );
                    }
                    template <typename T>
                    bool peek(T const &expected)
                    {
                        auto pos = save_pos();
                        eat_space();
                        auto success = raw_read(expected);
                        retrieve_pos(pos);
                        return success;
                    }
                    template <typename ...T>
                    bool peek(T const &...expected)
                    {
                        return ((peek(expected)) || ...);
                    }
                    template <typename T>
                    bool peek_without_skipping_spaces(T const &expected)
                    {
                        auto pos = save_pos();
                        auto success = raw_read(expected);
                        retrieve_pos(pos);
                        return success;
                    }
                    template <typename ...T>
                    bool peek_without_skipping_spaces(T const &...expected)
                    {
                        return ((peek_without_skipping_spaces(expected)) || ...);
                    }
                    template <typename T>
                    void force_read(T const &expected)
                    {
                        if (!read(expected))
                            throw exception::parse_failed(__func__, "Expect character '"s + expected + "'.");
                    }
                    template <typename... Escaped>
                    void unescape(Escaped... escaped)
                    {
                        auto pos = in.tellg();
                        if (read('\\'))
                        {
                            if (peek('\\', escaped...))
                                return;
                            in.seekg(pos);
                        }
                    }
                    template <typename ...Escaped>
                    auto get(Escaped... escaped)
                    {
                        unescape(escaped...);
                        return in.get();
                    }
                    template <typename... Stops>
                    std::string read_until(Stops... stops)
                    {
                        std::string result;
                        while (in)
                        {
                            if (peek_without_skipping_spaces(stops...))
                                return result;
                            else
                            {
                                if (in)
                                    result.push_back((char) get(stops...));
                            }
                        }
                        serialize::exception::throw_exception(in, __func__);
                    }
                    void eat_space()
                    {
                        while (in && std::isspace(in.peek()))
                            in.get();
                    }
                private:
                    std::basic_istream<char_type> &in;
                    bool raw_read(char_type c)
                    {
                        char input = in.get();
                        return input == c;
                    }
                    bool raw_read(std::basic_string_view<char_type> word)
                    {
                        for (auto expected : word)
                            if (in.get() != expected)
                                return false;
                        return true;
                    }
                    auto save_pos()
                    {
                        return in.tellg();
                    }
                    template <typename Pos>
                    void retrieve_pos(Pos pos)
                    {
                        in.seekg(pos);
                        if (in.eof())
                            in.clear();
                        if (in.bad())
                            in.clear();
                        if (!in.good())
                            in.clear();
                    }
                };
            }
        }
    }
}
#endif //INC_201704_BASIC_PARSE_HPP
