#include "graph.hpp"

#include <iomanip>

namespace aco {

Graph::Graph(size_t num_vertex) : graph_(num_vertex) {}

Distance Graph::GetEdge(Vertex from, Vertex to) const {
  return graph_.Get(from, to);
}

void Graph::SetEdge(Vertex from, Vertex to, Distance dist) {
  graph_.Set(from, to, dist);
}

bool Graph::HasEdge(Vertex from, Vertex to) const {
  return GetEdge(from, to) != 0;
}

std::vector<Vertex> Graph::GetNeighbours(Vertex v) const {
  std::vector<Vertex> neighbours;
  for (Vertex neigh = 0; neigh < Size(); ++neigh) {
    if (HasEdge(v, neigh)) {
      neighbours.push_back(neigh);
    }
  }
  return neighbours;
}

size_t Graph::Size() const { return graph_.Size(); }

std::ostream &operator<<(std::ostream &os, const Graph &graph) {
  auto num_vertex = graph.Size();
  for (size_t from = 0; from < num_vertex; ++from) {
    for (size_t to = 0; to < num_vertex; ++to) {
      if (graph.HasEdge(from, to)) {
        os << std::setw(5) << graph.GetEdge(from, to) << " ";
      } else {
        os << std::setw(5) << "- ";
      }
    }
    os << "\n";
  }
  return os;
}

}  // namespace aco