#pragma once

#include <string>
#include <vector>

namespace aco {

template <typename T>
class AdjacencyMatrix final {
 public:
  explicit AdjacencyMatrix(size_t size) : size_(size), matrix_(size * size) {}

  T Get(size_t from, size_t to) const { return matrix_[from * size_ + to]; }

  void Set(size_t from, size_t to, T value) {
    matrix_[from * size_ + to] = value;
  }

  void Add(size_t from, size_t to, T delta) {
    matrix_[from * size_ + to] += delta;
  }

  size_t Size() const { return size_; }

 private:
  size_t size_;
  std::vector<T> matrix_;
};

using Vertex = size_t;
using Distance = double;
using Route = std::vector<Vertex>;

class Graph final {
 public:
  explicit Graph(size_t num_vertex);

  Distance GetEdge(Vertex from, Vertex to) const;
  void SetEdge(Vertex from, Vertex to, Distance dist);
  bool HasEdge(Vertex from, Vertex to) const;

  std::vector<Vertex> GetNeighbours(Vertex v) const;

  size_t Size() const;

 private:
  AdjacencyMatrix<Distance> graph_;
};

}  // namespace aco