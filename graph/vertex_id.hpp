#ifndef GRAPH_VERTEX_ID_H
#define GRAPH_VERTEX_ID_H

#include <cstddef>    //for std::size_t
#include "../repository/stock_id.hpp"

namespace lzhlib
{

    class vertex_id
    {
        friend struct invalid_vertex;

        template <class, class>
        friend
        class graph_base;

    private:
        explicit vertex_id(stock_id i)
            : id_(i)
        {}
    public:
        vertex_id() = default;
        bool operator<(vertex_id rhs) const
        {
            return id_ < rhs.id_;
        }
        bool operator==(vertex_id rhs) const
        {
            return id_ == rhs.id_;
        }
        bool operator!=(vertex_id rhs) const
        {
            return !(*this == rhs);
        }
    private:
        auto id() const noexcept
        {
            return id_;
        }
        stock_id id_{lzhlib::invalid_id};

        friend std::istream &operator>>(std::istream &in, vertex_id &id)
        {
            std::size_t i;
            in >> i;
            id.id_ = stock_id(i);
            return in;
        }
        friend std::ostream &operator<<(std::ostream &out, vertex_id id)
        {
            out << id.id().id() << " ";
            return out;
        }
    };

    struct invalid_vertex
    {
        static constexpr vertex_id id{};
    };
    constexpr vertex_id invalid_vertex_id = invalid_vertex::id;

}
#endif //GRAPH_VERTEX_ID_H
