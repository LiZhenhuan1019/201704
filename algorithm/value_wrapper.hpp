#ifndef INC_201704_VALUE_WRAPPER_HPP
#define INC_201704_VALUE_WRAPPER_HPP

#include <istream>
#include <ostream>
namespace ds_expr
{
    namespace algorithm
    {
        namespace detail
        {
            template <typename T>
            using remove_cvrf_t = std::remove_cv_t<std::remove_reference_t<T>>;
        }
        template <typename T, typename Number_t = int>
        struct value_wrapper
        {
            using value_type = std::remove_reference_t<T>;
            using number_type = Number_t;
            value_wrapper() = default;
            template <typename U, std::enable_if_t<!std::is_same_v<std::remove_cv_t<U>, value_wrapper>, int> = 0>
            value_wrapper(U &&u, number_type number = 0)
                :value(std::forward<U>(u)), number(number)
            {}
            T value;
            number_type number;

            friend std::istream &operator>>(std::istream &in, value_wrapper &value)
            {
                in >> value.value >> value.number;
                return in;
            }
            friend std::ostream &operator<<(std::ostream &out, value_wrapper const &value)
            {
                out << value.value << " " << value.number << " ";
                return out;
            }
        };

        template <typename T, typename Number_t = int>
        value_wrapper(T
        &&t,
        Number_t = 0
        ) ->
        value_wrapper<detail::remove_cvrf_t<T>, Number_t>;
        template <typename Char_t, size_t N, typename Number_t = int>
        value_wrapper(Char_t( &)[N], Number_t = 0
        ) ->
        value_wrapper<std::basic_string<std::remove_const_t<Char_t>>, Number_t>;
    }
}


#endif //INC_201704_VALUE_WRAPPER_HPP
