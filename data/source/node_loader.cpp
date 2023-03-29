#include <acore/math/view.hpp>
#include <acore/math/utilities.hpp>
#include <adata/triangle/node_loader.hpp>
#include <autils/common.hpp>

namespace acg::data::triangle {

NodeLoader::NodeLoader(std::istream& input_stream)
    : input_stream_(input_stream), status_(Status::kFailedPrecondition) {}

Status NodeLoader::GetStatus() const { return status_; }

const types::DynamicField<Float64>& NodeLoader::GetData() const {
  ACG_DEBUG_CHECK(status_ == Status::kOk, "Access to unavailable data. status code = {}",
                  static_cast<int>(status_));
  return nodes_;
}

void NodeLoader::Load() {
  if (!input_stream_.good()) {
    ACG_ERROR("Failed to get any information from InputStream!");
    status_ = Status::kUnavailable;
    return;
  }

  // Load first line.
  int num_bc = 0;
  input_stream_ >> num_points_ >> dimension_ >> num_attributes_ >> num_bc;
  if (input_stream_.fail()) {
    ACG_ERROR("Failed to get any information from InputStream!");
    status_ = Status::kUnavailable;
    return;
  }

  if (num_attributes_ != 0) {
    ACG_ERROR(
        "Node loader found num attributes not equal to zero. However, this class does not try to "
        "get any attribute. (#attr = {})",
        num_attributes_);
    status_ = Status::kUnimplemented;
    return;
  }
  nodes_.resize(dimension_, num_points_);
  node_is_boundary_.resize(num_points_);
  auto accessor = view(nodes_);
  offset_ = 0;
  for (Index i = 0; i < num_points_; ++i) {
    Index point_id;
    input_stream_ >> point_id;
    if (i == 0) {
      offset_ = point_id;
    }

    point_id -= offset_;
    for (Index j = 0; j < dimension_; ++j) {
      input_stream_ >> accessor(point_id)(j);
    }

    if (num_bc > 0) {
      int is_bd = 0;
      input_stream_ >> is_bd;
      node_is_boundary_[point_id] = (is_bd == 1);
    }
  }
  status_ = Status::kOk;
}

Index NodeLoader::GetOffset() const { return offset_; }
const std::vector<bool>& NodeLoader::GetBoundaryData() const { return node_is_boundary_; }

}  // namespace acg::data::triangle
