/**
 * @brief Port for Triangle
 *
 * @url https://www.cs.cmu.edu/~quake/triangle.html
 */

#pragma once
#include <acore/geometry/common.hpp>
#include <acore/math/common.hpp>
#include <autils/result.hpp>

namespace acg {

namespace data {

namespace triangle {

/**
 * @brief Load .ele formated data from stream
 *
 * @intro
 *        .ele files
 *        First line: <# of triangles> <nodes per triangle> <# of attributes>
 *        Remaining lines: <triangle #> <node> <node> <node> ... [attributes]
 */
class EleLoader {
public:
  explicit EleLoader(std::istream& input_stream, bool has_tetra_index = true);

  void Load();

  Status GetStatus() const;

  const types::DynamicField<Index>& GetData() const;

private:
  std::istream& input_stream_;

  Status status_;

  types::DynamicField<Index> tetra_;

public:
  Index num_triangles_;
  Index num_attributes_;
  Index nodes_per_triangle_;
  const bool has_tetra_index_;
};

}  // namespace triangle
}  // namespace data
}  // namespace acg
