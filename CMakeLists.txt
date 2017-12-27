cmake_minimum_required(VERSION 3.10)
project(201704)

set(CMAKE_CXX_STANDARD 17)

add_executable(201704
        main.cpp
        include/graph/directed_graph.hpp
        include/graph/edge.hpp
        include/graph/edge_id.hpp
        include/graph/graph.hpp
        include/graph/graph_base.hpp
        include/graph/undirected_graph.hpp
        include/graph/vertex.hpp
        include/graph/vertex_id.hpp
        include/repository/dynamic_repository.hpp
        include/repository/repository.hpp
        include/repository/stock_id.hpp
        include/parse/parse_vertex_list.hpp)