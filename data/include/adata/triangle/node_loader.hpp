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
 *        .node files
 *        First line: [# of points] [dimension (must be 3)] [# of attributes] [# of boundary markers
 * (0 or 1)] Remaining lines: [point #] [x] [y] [z] [optional attributes] [optional boundary marker]
 */
class NodeLoader {
public:
  explicit NodeLoader(std::istream& input_stream);

  void Load();

  Status GetStatus() const;

  const types::DynamicField<Float64>& GetData() const;

  const std::vector<bool>& GetBoundaryData() const;

  Index GetOffset() const;

private:
  std::istream& input_stream_;

  Status status_;

  types::DynamicField<Float64> nodes_;

  std::vector<bool> node_is_boundary_;

public:
  Index num_points_;
  Index num_attributes_;
  Index dimension_;
  Index offset_;
};

}  // namespace triangle
}  // namespace data
}  // namespace acg
