cmake_minimum_required(VERSION 3.10)
project(201704)

set(CMAKE_CXX_STANDARD 17)

add_executable(201704
        main.cpp
        include/graph/directed_graph.h
        include/graph/edge.h
        include/graph/edge_id.h
        include/graph/graph.h
        include/graph/graph_base.h
        include/graph/undirected_graph.h
        include/graph/vertex.h
        include/graph/vertex_id.h
        include/repository/dynamic_repository.h
        include/repository/repository.h
        include/repository/stock_id.h
        include/parse/parse_vertex_list.h)