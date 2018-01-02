#ifndef INC_201704_BASIC_SERIALIZE_HPP
#define INC_201704_BASIC_SERIALIZE_HPP

#include <istream>

namespace ds_expr
{
    namespace serialize
    {
        template <typename T>
        std::istream &operator>>(std::istream &in, std::optional<T> &optional)
        {
            int i = 0;
            in >> i;
            if (i)
            {
                T t;
                in >> t;
                optional = t;
            } else
                optional = std::nullopt;
            return in;
        }
        template <typename T>
        std::ostream &operator<<(std::ostream &out, std::optional<T> const &optional)
        {
            if (optional)
            {
                out << 1 << " " << *optional;
            } else
                out << 0 << " ";
            return out;
        }
        template <typename T, typename ...Escaped>
        std::ostream &output_with_escape(std::ostream &out, T const &t, Escaped ...escaped)
        {
            std::ostringstream stream;
            stream << t;
            auto output = stream.str();
            for (auto iter = output.begin(); iter != output.end(); ++iter)
            {
                if (*iter == '\\' || ((*iter == escaped) || ...))
                {
                    iter = output.insert(iter, '\\');
                    ++iter;
                }
            }
            return out << output;
        }
    }
}
#endif //INC_201704_BASIC_SERIALIZE_HPP
