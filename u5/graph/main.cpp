#include <cassert>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <forward_list>

struct node {
  // directed edges
  std::vector<node *> neighbors;
};

struct graph {
  std::forward_list<node> nodes;
  int num_nodes;
  int num_edges;
};

graph read_graph(char const* filename);

int main(int argc, char ** argv) {
  try {
    if (2 != argc)
      throw(std::invalid_argument("usage: graph <filename>"));
    auto g = read_graph(argv[1]);
    std::cout << "the graph has " << g.num_nodes
              << " nodes and " << g.num_edges << " edges\n";
    // TODO continue from here
  } catch(std::exception const& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }
  return 0;
}

node * id_to_node(int id, graph & g, std::unordered_map<int, node *> & dict) {
  auto ret_pair = dict.emplace(id, nullptr);
  node *& n = ret_pair.first->second;
  if (ret_pair.second) {  // node does not exist yet
    g.nodes.push_front(node());
    n = &g.nodes.front();
  }
  assert(nullptr != n);
  assert(dict.count(id));
  return n;
}

graph read_graph(char const* filename) {
  std::ifstream graph_file(filename);
  std::string line;
  char const* DELIMITER = " \t";
  std::unordered_map<int, node *> node_dict;
  graph g;
  g.num_nodes = 0;
  while (std::getline(graph_file, line)) {
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

