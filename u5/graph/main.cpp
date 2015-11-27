#include <cassert>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <forward_list>
#include <random>
#include <atomic>

#define RANDOM_TEST_ITERATIONS 1000000

using namespace std;

template <typename T> using atomic_vector = std::vector<std::atomic<T>>;

struct node 
{
    // directed edges
    std::vector<node *> neighbors;
    int index;
    int id;
};

struct graph 
{
    std::forward_list<node> nodes;
    int num_nodes;
    int num_edges;
};

graph read_graph(char const* filename);
int bfs(graph & g, node * const u, node * const v);
void bfs_(graph & g, node * const u, node * const v, atomic_vector<int> & D, const int ds);

void calculate_mean(graph & g);
void random_test(graph & g);

int main(int argc, char ** argv) 
{
    try
    {
        if (2 != argc)
            throw(std::invalid_argument("usage: graph <filename>"));
        auto g = read_graph(argv[1]);
        std::cout << "the graph has " << g.num_nodes
                  << " nodes and " << g.num_edges << " edges\n";

        //random_test(g);
        calculate_mean(g);

    }
    catch(std::exception const& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}

void random_test(graph & g)
{
    random_device random_device;
    default_random_engine random_generator(random_device());
    uniform_int_distribution<uint> random_graph_index(0, g.num_nodes - 1);

    vector<node*> nodes;
    nodes.reserve(g.num_nodes);

    for ( node & nd : g.nodes )
    {
        nodes.push_back(&nd);
    }

    for(int i = 0; i < RANDOM_TEST_ITERATIONS; ++i)
    {
        int index_u;
        int index_v;

        do
        {
            index_u = random_graph_index(random_generator);
            index_v = random_graph_index(random_generator);
        }
        while(index_u == index_v);

        node * u = nodes[index_u];
        node * v = nodes[index_v];

        int d = bfs(g, u, v);

        cout << "D(" << u->id << ", " << v->id << ") = " << d << endl;
    }
}

void calculate_mean(graph & g)
{
    double s = 0;

    for(node & u : g.nodes)
    {
        for(node & v : g.nodes)
        {
            int d = bfs(g, &u, &v);
            cout << "D(" << u.id << ", " << v.id << ") = " << d << endl;
            s += d;
        }
    }

    s /= g.num_nodes;

    cout << "Average distance: " << s << endl;
}

int bfs(graph & g, node * const u, node * const v)
{
    atomic_vector<int> D(g.num_nodes);

    for(uint i = 0; i < D.size(); ++i)
        D[i] = -1;

    D[u->index] = 0;

    #pragma omp parallel default(none) shared(D,g)
    {
        #pragma omp single nowait
        bfs_(g, u, v, D, 0);
    }

    // Return path if Dv != -1, returns -1 if there is no such path
    return D[v->index];
}

void bfs_(graph & g, node * const u, node * const v, atomic_vector<int> &D, const int ds)
{
    for(node * n : u->neighbors)
    {
        if(n == v)
        {
            if(D[v->index].load() == -1 || D[v->index].load() > ds + 1)
                D[v->index].store(ds + 1);
        }
        else
        {
            if(D[n->index].load() != -1 && D[n->index].load() <= ds + 1)
                continue;
            else
                D[n->index].store(ds + 1);
        }

        #pragma omp task default(none) shared(g,D) firstprivate(n) if(( D[v->index].load() != -1 && D[v->index].load() > ds + 2) || D[v->index].load() == -1)
        bfs_(g, n, v, D, ds + 1);
    }
}

node * id_to_node(int id, graph & g, std::unordered_map<int, node *> & dict) 
{
    auto ret_pair = dict.emplace(id, nullptr);
    node *& n = ret_pair.first->second;
    if (ret_pair.second)
    {  // node does not exist yet
        g.nodes.push_front(node());
        n = &g.nodes.front();

        //assign the index from node dictionary
        n->index = dict.size() - 1;

        //Assign the id from src file
        n->id = id;

    }
    assert(nullptr != n);
    assert(dict.count(id));
    return n;
}

graph read_graph(char const* filename) 
{
    std::ifstream graph_file(filename);
    std::string line;
    char const* DELIMITER = " \t";
    std::unordered_map<int, node *> node_dict;
    graph g;
    g.num_nodes = 0;
    g.num_edges = 0;
    while (std::getline(graph_file, line))
    {
        if('#' == line[0] || line.empty())
            continue;  // ignore comments and empty lines

        int from_id = std::atoi(std::strtok(&line[0], DELIMITER));
        auto * from_node = id_to_node(from_id, g, node_dict);
        int to_id   = std::atoi(std::strtok(nullptr, DELIMITER));
        auto * to_node = id_to_node(to_id, g, node_dict);
        from_node->neighbors.push_back(to_node);
        ++g.num_edges;

    }

    g.num_nodes = node_dict.size();
    return g;
}

