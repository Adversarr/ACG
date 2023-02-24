#pragma once
#include <memory>
#include <vector>

#include "acore/common.hpp"
#include "mesh.hpp"
namespace acg {
namespace geometry {

// TODO: Implemntation required
template <typename V = Index , typename F = Index , typename E = Index > class HalfEdgeMesh {
public:
  // Decleration.
  struct Vertex;
  struct Face;
  struct Edge;

  struct Edge {
    E data;
    Face* face;
    Edge *twin, next, prev;
    Vertex* origin;

    Edge(E d, Face* f, Edge* t, Edge* n, Edge* p, Vertex* o)
        : face(f), twin(t), next(n), prev(p), origin(o), data(d) {}
  };
  struct Face {
    F data;
    Edge* edge;

    Face(F d, Edge* e) : data(d), edge(e) {}
  };

  struct Vertex {
    V data;
    Edge* edge;
    Vertex(V d, Edge* e) : data(d), edge(e) {}
  };

  struct VertexOutEdgeIteratorWrapper {
    struct Iterator {
      Edge* edge;
      Edge* start;

      Iterator operator++() const {
        Iterator it;
        it.edge = edge->next;
        it.start = edge->start;

        return it;
      }

      Iterator operator==(const Iterator& rhs) const {
        return std::tie(edge, start) == std::tie(rhs.edge, rhs.start);
      }
    };

    Iterator begin() const;

    Iterator end() const;

    HalfEdgeMesh& mesh;
  };

private:
  explicit HalfEdgeMesh(const topology::TriangleList& mesh);

  std::vector<std::unique_ptr<Edge>> edges_;

  std::vector<std::unique_ptr<Vertex>> vertices_;

  std::vector<std::unique_ptr<Face>> faces_;
};

}  // namespace geometry

}  // namespace acg
