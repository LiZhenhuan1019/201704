#ifndef INC_201704_BASIC_SERIALIZE_HPP
#define INC_201704_BASIC_SERIALIZE_HPP

#include <istream>
#include <optional>
#include <variant>

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
        template <typename T1, typename T2>
        std::istream &operator>>(std::istream &in, std::variant<T1, T2> &variant)
        {
            std::size_t index = 0;
            in >> index;
            switch (index)
            {
            case 0:
            {
                T1 t;
                in >> t;
                variant = t;
                break;
            }
            case 1:
                T2 t;
                in >> t;
                variant = t;
                break;
            }
            return in;
        }
        template <typename T1, typename T2>
        std::ostream &operator<<(std::ostream &out, std::variant<T1, T2> const &variant)
        {
            out << variant.index() << " ";
            std::visit([&out](auto &t)
                       {
                           out << t;
                       }, variant);
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
