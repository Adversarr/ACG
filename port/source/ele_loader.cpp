#include "acg_port/triangle/ele_loader.hpp"

#include "acg_utils/log.hpp"

namespace acg::port::triangle {

EleLoader::EleLoader(std::istream& input_stream, bool has_tetra_index)
    : input_stream_(input_stream),
      status_(Status::kFailedPrecondition),
      has_tetra_index_(has_tetra_index) {}

Status EleLoader::GetStatus() const { return status_; }

const types::DynamicField<Index>& EleLoader::GetData() const {
  ACG_DEBUG_CHECK(status_ == Status::kOk, "Access to unavailable data. status code = {}",
                  static_cast<int>(status_));
  return tetra_;
}

void EleLoader::Load() {
  if (!input_stream_.good()) {
    ACG_ERROR("Failed to get any information from InputStream!");
    status_ = Status::kUnavailable;
    return;
  }

  // Load first line.
  input_stream_ >> num_triangles_ >> nodes_per_triangle_ >> num_attributes_;
  if (input_stream_.fail()) {
    ACG_ERROR("Failed to get any information from InputStream!");
    status_ = Status::kUnavailable;
    return;
  }

  if (num_attributes_ != 0) {
    ACG_ERROR(
        "Ele loader found num attributes not equal to zero. However, this class does not try to "
        "get any attribute. (#attr = {})",
        num_attributes_);
    status_ = Status::kUnimplemented;
    return;
  }

  tetra_.resize(nodes_per_triangle_, num_triangles_);
  Index triangle_id = 0;
  for (Index i = 0; i < num_triangles_; ++i) {
    if (has_tetra_index_) {
      input_stream_ >> triangle_id;
      if (triangle_id >= num_triangles_) {
        ACG_ERROR("Got Triangle ID {} greater than num triangles {}.", triangle_id, num_triangles_);
        status_ = Status::kDataLoss;
        return;
      }
    }
    for (Index j = 0; j < nodes_per_triangle_; ++j) {
      input_stream_ >> tetra_(j, triangle_id);
    }

    if (input_stream_.fail()) {
      ACG_ERROR("Failed to load {}-th line from InputStream.", i + 1);
      status_ = Status::kUnavailable;
      return;
    }
    triangle_id += 1;
  }

  status_ = Status::kOk;
}

}  // namespace acg::port::triangle
