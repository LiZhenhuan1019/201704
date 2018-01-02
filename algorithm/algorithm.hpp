#ifndef INC_201704_ALGORITHM_HPP
#define INC_201704_ALGORITHM_HPP

#include <queue>
#include "value_wrapper.hpp"
namespace ds_expr
{
    namespace algorithm
    {
        template <typename Graph_t, typename Predicate>
        auto find(Graph_t &graph, Predicate predicate)
        {
            for (auto id = graph.first_vertex(); id != graph.end_vertex(); id = graph.next_vertex(id))
                if (predicate(graph.value(id).value))
                    return id;
            return graph.end_vertex();
        }
        namespace detail
        {
            template <typename Graph_t, typename Id_t, typename Callable>
            void depth_first_search_impl(Graph_t &graph, Id_t id, Callable callable)
            {
                if (graph.value(id).number)
                    return;
                graph.value(id).number = 1;
                callable(graph.value(id).value);
                for (auto edge_ref : graph.associated_edges(id))
                    depth_first_search_impl(graph, edge_ref.opposite_vertex(), callable);
            }
        }
        template <typename Graph_t, typename Callable>
        void depth_first_search(Graph_t &graph, Callable callable)
        {
            for (auto id = graph.first_vertex(); id != graph.end_vertex(); id = graph.next_vertex(id))
                graph.value(id).number = 0;
            for (auto id = graph.first_vertex(); id != graph.end_vertex(); id = graph.next_vertex(id))
                if (graph.value(id).number == 0)
                    detail::depth_first_search_impl(graph, id, callable);
        }
        namespace detail
        {
            template <typename Graph_t, typename Id_t, typename Callable>
            void breadth_first_search_impl(Graph_t &graph, Id_t root, Callable callable)
            {
                if (graph.value(root).number == 0)
                {
                    graph.value(root).number = 1;
                    std::queue<Id_t> queue;
                    queue.push(root);
                    while (!queue.empty())
                    {
                        auto vertex = queue.front();
                        queue.pop();
                        callable(graph.value(vertex).value);
                        for (auto const &edge_ref : graph.associated_edges(vertex))
                        {
                            auto opposite_vertex = edge_ref.opposite_vertex();
                            if (graph.value(opposite_vertex).number == 0)
                            {
                                graph.value(opposite_vertex).number = 1;
                                queue.push(opposite_vertex);
                            }
                        }
                    }
                }
            }
        }
        template <typename Graph_t, typename Callable>
        void breadth_first_search(Graph_t &graph, Callable callable)
        {
            for (auto id = graph.first_vertex(); id != graph.end_vertex(); id = graph.next_vertex(id))
                graph.value(id).number = 0;
            for (auto root = graph.first_vertex(); root != graph.end_vertex(); root = graph.next_vertex(root))
                detail::breadth_first_search_impl(graph, root, callable);
        }
    }
}

#endif //INC_201704_ALGORITHM_HPP
