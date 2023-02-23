#include <acore/access.hpp>
#include <aport/triangle/node_loader.hpp>
#include <autils/common.hpp>
namespace acg::port::triangle {

NodeLoader::NodeLoader(std::istream& input_stream)
    : input_stream_(input_stream), status_(Status::kFailedPrecondition) {}

Status NodeLoader::GetStatus() const { return status_; }

const types::DynamicField<F64>& NodeLoader::GetData() const {
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
  input_stream_ >> num_points_ >> dimension_ >> num_attributes_;
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
  auto accessor = access(nodes_);
  Index offset = 0;
  for (Index i = 0; i < num_points_; ++i) {
    Index point_id;
    input_stream_ >> point_id;
    if (i == 0) UNLIKELY {
        offset = point_id;
      }
    for (Index j = 0; j < dimension_; ++j) {
      input_stream_ >> accessor(i - offset)(j);
    }
  }

  status_ = Status::kOk;
}

}  // namespace acg::port::triangle
