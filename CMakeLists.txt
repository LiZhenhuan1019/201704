cmake_minimum_required(VERSION 3.9)
project(201704)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER MATCHES .*clang\\+\\+.*)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ ")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -fexec-charset=GBK")
endif ()

add_executable(201704
        main.cpp
        graph/directed_graph.hpp
        graph/edge.hpp
        graph/edge_id.hpp
        graph/graph.hpp
        graph/graph_base.hpp
        graph/undirected_graph.hpp
        graph/vertex.hpp
        graph/vertex_id.hpp
        repository/dynamic_repository.hpp
        repository/repository.hpp
        repository/stock_id.hpp
        serialize/parse/parse_list.hpp serialize/basic_serialize.hpp serialize/parse/basic_parse.hpp test/test.hpp test/test_serialize.hpp serialize/serialize_exception.hpp test/test_graph.hpp serialize/serialize.hpp algorithm/value_wrapper.hpp algorithm/algorithm.hpp test/test_algorithm.hpp ui/console_ui.hpp)