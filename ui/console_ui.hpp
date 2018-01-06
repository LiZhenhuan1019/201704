#ifndef INC_201704_CONSOLE_UI_HPP
#define INC_201704_CONSOLE_UI_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <map>
#include <variant>
#include <cassert>
#include "../graph/graph.hpp"
#include "../serialize/serialize.hpp"
#include "../algorithm/algorithm.hpp"
#include "../algorithm/value_wrapper.hpp"

#ifdef _WIN32
    constexpr char clear_screen_command[] = "cls";
#elif defined(__unix__)
    constexpr char clear_screen_command[] = "clear";
#endif

namespace ds_expr
{
    namespace ui
    {
        using namespace std::literals;
        template <typename Vertex_value_t, typename Edge_value_t>
        class console_ui
        {
            using vertex_value_type = Vertex_value_t;
            using edge_value_type = Edge_value_t;
            using vertex_id_type = lzhlib::vertex_id;
            using directed_graph_type = lzhlib::directed_graph<algorithm::value_wrapper<vertex_value_type>, algorithm::value_wrapper<edge_value_type>>;
            using undirected_graph_type = lzhlib::undirected_graph<algorithm::value_wrapper<vertex_value_type>, algorithm::value_wrapper<edge_value_type>>;
            using variant_type = std::variant<directed_graph_type , undirected_graph_type>; //use std::variant to support both directed and undirected graph
            using optional_type = std::optional<variant_type>;
            using graphs_type = std::vector<optional_type>;
            struct bad_input : std::runtime_error
            {
                using std::runtime_error::runtime_error;
            };
            struct fail_input : std::runtime_error
            {
                using std::runtime_error::runtime_error;
            };
            struct eof_input : std::runtime_error
            {
                using std::runtime_error::runtime_error;
            };
            struct vertex_not_exists : std::logic_error
            {
                vertex_not_exists(vertex_id_type id, char const *function_name)
                    : logic_error("Vertex doesn't exists in function '"s + function_name + "'.")
                {}
            };

        public:
            console_ui()
                : directed_graphs(1)
            {
            }

            void execute()
            {
                clear_screen();
                print_menu();
                print_info();
                while (!quit)
                {
                    print_wait_input();
                    auto button = input_value(0, (int) commands.size());
                    clear_screen();
                    print_menu();
                    print_input(button);
                    try
                    {
                        commands[button].act(*this);
                    }
                    catch (std::logic_error const &e)
                    {
                        print_error();
                        std::cout << "Exception caught: " << e.what() << "\n";
                    }
                    catch (std::bad_optional_access const &e)
                    {
                        print_error();
                        std::cout << "Exception caught : " << e.what() << "\nThe graph/vertex/edge may not exists.\n";
                    }
                    print_info();
                }
            }

        private:
            void exit()
            {
                quit = true;
            }

            void create()
            {
                std::cout << "Please select the type of graph.\n"
                          "0 represents directed graph, 1 represents undirected graph.\n";
                auto type = input_value(0 ,2);
                if(type)
                    current_graph() = undirected_graph_type{};
                else
                    current_graph() = directed_graph_type{};
                std::cout << "Please input the list of vertices value.\n";
                std::string_view syntax_prompt = "语法：定义以'{'开始，以'}'结束，二者之间为以'('和')'包围元素，以','分隔元素的列表;\n"
                    "列表中每个元素可以是任意字符串，但若其中包含')'这样的字符则需在之前添加'\\'进行转义。\n"
                    "列表中每一个元素将成为对应结点的值，元素在列表中的位置将成为对应结点的id（从0起始）。\n"
                    "空格可在任意地方添加，但元素中的空格将被视为元素的一部分。\n"
                    "例子： {(first vertex), ( with spaces ) , (without_space)}\n"
                    R"~(将得到一个含有3个顶点的图，其中id为0的顶点的值为"first vertex"，)~""\n"
                    R"~(    id为1的顶点的值为" with spaces "，id为2的顶点的值为"without_space"。)~""\n";
                std::cout << syntax_prompt;
                std::istringstream stream(input_line<std::string>());
                serialize::parse::parse_list<vertex_value_type> parser(stream);
                auto vertices = parser.extract_list();
                std::visit(
                    [&vertices](auto &g)
                    {
                        for (auto &value : vertices)
                            g.add_vertex(std::move(value));
                    }, *current_graph());
                print_ok();
            }

            void destroy()
            {
                if (!current_graph())
                    return print_error();
                current_graph().reset();
                print_ok();
            }

            void locate_vertex()
            {
                std::cout << "Please input the value of vertex to locate.\n";
                std::visit(
                    [this](auto &g)
                    {
                        auto value = input_line<vertex_value_type>();
                        for (auto id = g.first_vertex(); id != g.end_vertex(); id = g.next_vertex(id))
                        {
                            if (g.value(id).value == value)
                                print_value(id);
                            return print_ok();
                        }
                        print_error();
                    }, *current_graph());
            }

            void get_vertex()
            {
                std::cout << "Please input the id of vertex to show.\n";
                auto id = input_line<vertex_id_type>();
                check_exists(id, __func__);
                auto &value = std::visit([&id](auto &g)->auto&{return g.value(id).value;}, *current_graph());
                std::cout << "The vertex ";
                print_value(value);
                print_ok();
            }

            void assign_vertex()
            {
                std::cout << "Please input the id of vertex to change.\n";
                auto id = input_line<vertex_id_type>();
                check_exists(id, __func__);
                std::cout << "Please input the value to change to.\n";
                auto value = input_line<vertex_value_type>();
                std::visit([&id, &value](auto &g){g.value(id).value = value;}, *current_graph());
                print_ok();
            }

            void first_adjacent()
            {
                std::cout << "Please input the id of the vertex.\n";
                auto id = input_line<vertex_id_type>();
                check_exists(id, __func__);
                auto const &edges = std::visit([&id](auto &g)->auto&{return g.associated_edges(id);}, *current_graph());
                if (edges.empty())
                    return print_null();
                print_value(edges.begin()->opposite_vertex());
                print_ok();
            }

            void next_adjacent()
            {
                std::cout << "Please input the id of the vertex.\n";
                auto id = input_line<vertex_id_type>();
                check_exists(id, __func__);
                auto const &edges = std::visit([&id](auto &g)->auto&{return g.associated_edges(id);}, *current_graph());
                if (edges.empty())
                    return print_null();
                std::cout << "Please input the id of the previous adjacent\n";
                auto previous = input_line<vertex_id_type>();
                check_exists(id, __func__);
                for (auto iter = edges.begin(); iter != edges.end(); ++iter)
                    if (iter->opposite_vertex() == previous)
                    {
                        if (++iter == edges.end())
                            return print_null();
                        print_value(iter->opposite_vertex());
                        return print_ok();
                    }
                print_error();
            }

            void add_vertex()
            {
                std::cout << "Please input the value of the vertex.\n";
                auto value = input_line<vertex_value_type>();
                std::visit([&value](auto &g){g.add_vertex(algorithm::value_wrapper{std::move(value)});}, *current_graph());
                print_ok();
            }

            void remove_vertex()
            {
                std::cout << "Please input the id of the vertex to delete.\n";
                auto id = input_line<vertex_id_type>();
                check_exists(id, __func__);
                std::visit([&id](auto &g){g.remove_vertex(id);}, *current_graph());
                print_ok();
            }

            void get_edge()
            {
                std::cout << "Please input the id of the first vertex.\n";
                auto first = input_line<vertex_id_type>();
                check_exists(first, __func__);
                std::cout << "Please input the id of the second vertex.\n";
                auto second = input_line<vertex_id_type>();
                check_exists(second, __func__);
                std::visit([this, &first, &second](auto &g)
                           {
                               auto edge = g.get_edge(first, second);
                               print_value(g.value(edge).value);
                           }, *current_graph());
                print_ok();
            }

            void add_edge()
            {
                std::cout << "Existing edge will be replaced by new edge.\n";
                std::cout << "Please input the id of the first vertex.\n";
                auto first = input_line<vertex_id_type>();
                check_exists(first, __func__);
                std::cout << "Please input the id of the second vertex.\n";
                auto second = input_line<vertex_id_type>();
                check_exists(second, __func__);
                std::cout << "Please input the value of the edge.\n";
                auto value = input_line<edge_value_type>();
                std::visit([&first, &second, &value](auto &g)
                           {
                               g.add_edge(first, second, algorithm::value_wrapper{std::move(value)});
                           }, *current_graph());
                print_ok();
            }

            void remove_edge()
            {
                std::cout << "Please input the id of the first vertex.\n";
                auto first = input_line<vertex_id_type>();
                check_exists(first, __func__);
                std::cout << "Please input the id of the second vertex.\n";
                auto second = input_line<vertex_id_type>();
                check_exists(second, __func__);
                std::visit([&first, &second](auto &g)
                           {
                               g.remove_edge(first, second);
                           }, *current_graph());
                print_ok();
            }

            void dfs_iterate()
            {
                std::visit([](auto &g)
                           {
                               algorithm::depth_first_search(g, [](auto const &value)
                               {
                                   std::cout << " Visit vertex'" + value + "'.\n";
                               });
                           }, *current_graph());
                print_ok();
            }

            void bfs_iterate()
            {
                std::visit([](auto &g)
                           {
                               algorithm::breadth_first_search(g, [](auto const &value)
                               {
                                   std::cout << " Visit vertex'" + value + "'.\n";
                               });
                           }, *current_graph());
                print_ok();
            }

            void save()
            {
                std::ofstream file(save_file_name);
                file << *this;
                if (file.good())
                    print_ok();
                else
                    print_error();
            }

            void load()
            {
                std::ifstream file(save_file_name);
                file >> *this;
                if (file.good())
                    print_ok();
                else
                    print_error();
            }

            void add_graph()
            {
                std::cout << "Please enter the position to insert the graph.\n";
                std::cout << "The position should be in the range of [0, " << directed_graphs.size() + 1 << ").\n";
                auto index = input_index();
                if (index > directed_graphs.size())
                    return print_error();
                if (current_graph_index >= index)
                    ++current_graph_index;
                directed_graphs.insert(std::next(directed_graphs.begin(), index), directed_graph_type{});
                print_ok();
            }

            void select_graph()
            {
                std::cout << "Please enter the index of graph to select.\n";
                std::cout << "The index should be in the range of [0, " << directed_graphs.size() << ").\n";
                auto index = input_index();
                if (index >= directed_graphs.size())
                    return print_error();
                current_graph_index = index;
                print_ok();
            }

            void remove_graph()
            {
                if (directed_graphs.size() == 1)
                    return print_error();
                std::cout << "Please enter the index of the graph to remove.\n";
                std::cout << "The index should be in the range of [0, " << directed_graphs.size() << ").\n";
                auto index = input_index();
                if (index >= directed_graphs.size())
                    return print_error();
                if (index < current_graph_index)
                    --current_graph_index;
                directed_graphs.erase(std::next(directed_graphs.begin(), index));
                print_ok();
            }

            void print_menu()
            {
                std::cout << "Menu for graph sample\n";
                std::cout << "---------------------------\n";
                for (auto i = 0u; i < commands.size(); ++i)
                {
                    std::string item = std::to_string(i) + ". " + commands[i].name;
                    std::cout << item << "\n";
                }
            }

            void print_info()
            {
                std::cout << "Current selected graph: " << current_graph_index << "\n";
                std::cout << "Number of total directed_graphs: " << directed_graphs.size() << "\n";
            }

            void print_wait_input()
            {
                std::cout << "press the number to execute the corresponding command.\n";
            }

            static void eat_line(std::istream &i)
            {
                i.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //跳过直到换行符, 这样就到了下一行.
            }

            auto input_index(std::size_t start = 0)
            {
                return input_value<std::size_t>(start, std::numeric_limits<int>::max() - 1);
            }

            template <typename U>
            static auto input_line(std::istream &in = std::cin)
            {
                while (true)
                {
                    try
                    {
                        std::string str;
                        getline(in, str);
                        U u;
                        serialize::parse::assign_value(u, str);
                        return u;
                    }
                    catch (serialize::exception::stream_fail const &e)
                    {
                        std::cout << "Please input a valid value!\n";
                        std::cin.clear();
                    }
                }
            }
            template <typename U>
            static void input_line(std::istream &in, U &u)
            {
                u = input_line<U>(in);
            }

            template <typename U>
            static auto input_value()
            {
                while (true)
                {
                    try
                    {
                        U input = wait_for_input<U>();
                        return input;
                    }
                    catch (fail_input const &)
                    {
                        std::cout << "Please input a valid value!\n";
                        std::cin.clear();
                    }
                }
            }

            template <typename U>
            static auto input_value(U const &lower_bound, U const &upper_bound)
            {
                assert(lower_bound < upper_bound);
                std::cout << "The value should be in the range of [" << lower_bound << ", " << upper_bound << ").\n";
                auto input = input_value<U>();
                while (input < lower_bound || input >= upper_bound)
                {
                    std::cout << "The value is out of range, please input again.\n";
                    std::cout << "The value should be in the range of [" << lower_bound << ", " << upper_bound
                              << ").\n";
                    input = input_value<U>();
                }
                return input;
            }

            template <typename U>
            static auto wait_for_input()
            {
                U input;
                std::cin >> input;
                eat_line(std::cin);
                if (std::cin.bad())
                    throw bad_input("irrecoverable input stream error.");
                else if (std::cin.fail())
                    throw fail_input("input failed.(formatting or extraction error.)");
                else if (std::cin.eof())
                    throw eof_input("input stream reached EOF.");
                print_input(input);
                return input;
            }

            void print_ok()
            {
                std::cout << "result :OK\n";
            }

            void print_error()
            {
                std::cout << "result: ERROR\n";
            }

            void print_null()
            {
                std::cout << "result : null\n";
            }

            template <typename U>
            void print_value(U const &value)
            {
                std::cout << "value: " << value << "\n";
            }

            using action = decltype(std::mem_fn(&console_ui::exit));
            struct command
            {
                action act;
                char const *name;
            };

            template <typename ...T1, typename ...T2>
            static std::vector<command> make_commands(std::pair<T1, T2> &&...t)
            {
                return {command{std::mem_fn(t.first), t.second}...};
            }

            bool quit = false;
            std::size_t current_graph_index = 0;
            graphs_type directed_graphs;

            inline static auto commands = make_commands(std::make_pair(&console_ui::exit, "Exit"),
                                                        std::make_pair(&console_ui::create, "Create"),
                                                        std::make_pair(&console_ui::destroy, "Destroy"),
                                                        std::make_pair(&console_ui::locate_vertex, "Locate"),
                                                        std::make_pair(&console_ui::get_vertex, "GetVertex"),
                                                        std::make_pair(&console_ui::assign_vertex, "AssignVertex"),
                                                        std::make_pair(&console_ui::first_adjacent, "FirstAdjVex"),
                                                        std::make_pair(&console_ui::next_adjacent, "NextAdjVex"),
                                                        std::make_pair(&console_ui::add_vertex, "AddVertex"),
                                                        std::make_pair(&console_ui::remove_vertex, "RemoveVertex"),
                                                        std::make_pair(&console_ui::get_edge, "GetEdge"),
                                                        std::make_pair(&console_ui::add_edge, "AddEdge"),
                                                        std::make_pair(&console_ui::remove_edge, "RemoveEdge"),
                                                        std::make_pair(&console_ui::dfs_iterate, "DfsIterate"),
                                                        std::make_pair(&console_ui::bfs_iterate, "BfsIterate"),
                                                        std::make_pair(&console_ui::save, "Save"),
                                                        std::make_pair(&console_ui::load, "Load"),
                                                        std::make_pair(&console_ui::add_graph, "AddGraph"),
                                                        std::make_pair(&console_ui::select_graph, "SelectGraph"),
                                                        std::make_pair(&console_ui::remove_graph, "RemoveGraph")
            );
            auto &current_graph()
            {
                return directed_graphs[current_graph_index];
            }
            bool vertex_exists(vertex_id_type id)
            {
                return std::visit([&id](auto &g)
                                  {
                                      return g.is_id_valid(id);
                                  }, *current_graph());
            }
            void check_exists(vertex_id_type id, char const *function_name)
            {
                if (!vertex_exists(id))
                    throw vertex_not_exists(id, function_name);
            }

            inline static const std::string save_file_name = "data.save";

            template <typename U>
            static void print_input(U const &value)
            {
                std::cout << "Input : " << value << "\n";
            }

            static void clear_screen()
            {
                std::system(clear_screen_command);
            }

            friend std::ostream &operator<<(std::ostream &out, console_ui const &ui)
            {
                using namespace serialize;
                out << ui.current_graph_index << " ";
                out << ui.directed_graphs;
                return out;
            }

            friend std::istream &operator>>(std::istream &in, console_ui &ui)
            {
                using namespace serialize;
                in >> ui.current_graph_index;
                in >> ui.directed_graphs;
                return in;
            }
        };
    }

}

#endif //INC_201704_CONSOLE_UI_HPP
