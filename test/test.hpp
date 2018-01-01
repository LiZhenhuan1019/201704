#ifndef INC_201704_TEST_HPP
#define INC_201704_TEST_HPP

#include "test_serialize.hpp"
#include "test_graph.hpp"

namespace test
{
    inline void test()
    {
        serialize::test();
        graph::test();
    }
}

#endif //INC_201704_TEST_HPP
