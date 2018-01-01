#ifndef INC_201704_SERIALIZE_EXCEPTION_HPP
#define INC_201704_SERIALIZE_EXCEPTION_HPP

#include <stdexcept>

namespace ds_expr
{
    namespace serialize
    {
        namespace exception
        {
            using namespace std::literals;

            struct stream_error : std::runtime_error
            {
                stream_error(char const *error_name, char const *function_name)
                    : runtime_error("Stream error '"s + error_name + "' in function '" + function_name + "'.")
                {}
            };

            struct stream_bad : stream_error
            {
                explicit stream_bad(char const *function_name)
                    : stream_error("bad", function_name)
                {}
            };

            struct stream_fail : stream_error
            {
                explicit stream_fail(char const *function_name)
                    : stream_error("fail", function_name)
                {}
            };

            struct stream_eof : stream_error
            {
                explicit stream_eof(char const *function_name)
                    : stream_error("eof", function_name)
                {}
            };
            [[noreturn]] void throw_exception(std::istream const&in, char const* function_name)
            {
                if(in.eof())
                    throw stream_eof(function_name);
                else if(in.bad())
                    throw stream_bad(function_name);
                else if(in.fail())
                    throw stream_fail(function_name);
                assert(0);
            }
        }
    }
}
#endif //INC_201704_SERIALIZE_EXCEPTION_HPP
