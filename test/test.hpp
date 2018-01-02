#ifndef INC_201704_TEST_HPP
#define INC_201704_TEST_HPP

#include "test_serialize.hpp"
#include "test_graph.hpp"
#include "test_algorithm.hpp"

namespace test
{
    inline void test()
    {
        serialize::test();
        graph::test();
        algorithm::test();
    }
}

#endif //INC_201704_TEST_HPP
