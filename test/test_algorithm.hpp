#ifndef INC_201704_TEST_ALGORITHM_HPP
#define INC_201704_TEST_ALGORITHM_HPP
#include "../graph/graph.hpp"
#include "../algorithm/algorithm.hpp"

namespace test
{
    namespace algorithm
    {
        inline void test()
        {
            using ds_expr::algorithm::value_wrapper;
            lzhlib::directed_graph<value_wrapper<std::string>, value_wrapper<std::string>> graph;
            auto a = graph.add_vertex(value_wrapper{"a"});
            auto b = graph.add_vertex(value_wrapper{"b"});
            auto c = graph.add_vertex(value_wrapper{"c"});
            auto d = graph.add_vertex(value_wrapper{"d"});
            graph.add_edge(a, b, "a->b");
            graph.add_edge(a, c, "a->c");
            graph.add_edge(b, d, "b->d");
            graph.add_edge(c, b, "c->b");
            std::vector<std::string> iterate_result;
            ds_expr::algorithm::depth_first_search(graph, [&iterate_result](auto &s)
            { iterate_result.push_back(s); });
            assert(iterate_result == (std::vector<std::string>{"a", "b", "d", "c"}));
            assert(ds_expr::algorithm::find(graph, [](auto &v)
            { return v == "c"; }) == c);
            iterate_result.clear();
            ds_expr::algorithm::breadth_first_search(graph, [&iterate_result](auto &s)
            { iterate_result.push_back(s); });
            assert(iterate_result == (std::vector<std::string>{"a", "b", "c", "d"}));
        }
    }
}
#endif //INC_201704_TEST_ALGORITHM_HPP
