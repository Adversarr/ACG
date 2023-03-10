#pragma once

#include <acore/geometry/common.hpp>
#include <acore/math/access.hpp>
#include <autils/god/god.hpp>
#include <autils/log.hpp>
#include <vector>

#include "common.hpp"

namespace acg::spatial {

template <typename F, typename D, int dim, UInt32 subdivision_count = 4, UInt32 max_depth = 4>
class SubDivisionAABB {
public:
  static constexpr UInt32 subdivision_count_total_ = utils::god::pow<dim>(subdivision_count);
  static_assert(max_depth > 0, "Max depth should be greater than 0.");
  static_assert(utils::god::pow<max_depth>(subdivision_count_total_)
                    < std::numeric_limits<size_t>::max(),
                "subdivision too large.");

  /**
   * @brief Get the leaf count of subdivision.
   *
   * @return
   */
  size_t Size() const noexcept;

  /**
   * @brief Insert a given aabb into the tree.
   *
   * @param aabb
   * @return
   */
  size_t Insert(const AABB<D, F, dim> &aabb);

  /**
   * @brief Get Visualize data, first = position, second = topo.
   */
  std::pair<Field<F, dim>, geometry::topology::LineList> Visualize() const;

  /**
   * @brief Clean all the aabbs.
   */
  void Clear();

  /**
   * @brief Insert from iterator
   */
  template <typename B, typename E> void Insert(B beg, E end);

  /***
   * @brief Query intersecting aabbs.
   */
  std::vector<size_t> Query(const AABB<void, F, dim> &aabb) const;

  std::vector<std::pair<size_t, size_t>> QueryInternal() const;

  explicit SubDivisionAABB(F unit_length = 1.0, F epsilon = 1e-6)
      : epsilon_(epsilon),
        unit_(unit_length),
        indexer_(std::make_from_tuple<NdRangeIndexer<dim>>(
            utils::god::duplicate<dim>(subdivision_count))) {}

  struct Node {
    AABB<void, F, dim> box_;
    F unit_;
    size_t depth_;
    std::array<size_t, subdivision_count_total_> sub_nodes_;
    std::vector<size_t> leafs_;

    AABB<void, F, dim> GetChildAABB(utils::god::IndexTuple<dim> index) const;

    size_t GetAABBIndex(const AABB<D, F, dim> &aabb) const;

    explicit Node(F unit, AABB<void, F, dim> box, size_t depth)
        : box_(box), unit_(unit), depth_(depth) {
      std::fill(std::begin(sub_nodes_), std::end(sub_nodes_), InvalidSubscript);
    }
  };

  AABB<D, F, dim> &Get(size_t id);

  const AABB<D, F, dim> &Get(size_t id) const;

private:
  // @brief Query the entry node's id for aabb.
  size_t FindEntry(const AABB<D, F, dim> &aabb) const;

  // @brief return the sequence for visit given node.
  //
  // [parent = Entry, Sub1, Sub2, ... , Destination]
  //
  // for large leafs, return [InvalidSubscript ... ]
  std::array<size_t, max_depth> FindVisitSequence(const AABB<D, F, dim> &aabb);

  size_t PutAABBData(const AABB<D, F, dim> &aabb);

  size_t PutNode(F local_unit, const AABB<void, F, dim> &range, size_t depth);

  size_t EnsureSubDivision(size_t parent, size_t child);

  size_t EnsureEntry(const AABB<D, F, dim> &aabb);

  void QueryVisit(std::vector<size_t> &retval, size_t node, const AABB<void, F, dim> &aabb) const;
  
  void QueryVisitInternal(std::vector<std::pair<size_t, size_t>> &retval, size_t node) const;

  std::vector<AABB<D, F, dim>> data_;
  std::vector<Node> nodes_;
  std::vector<size_t> entry_nodes_;

  // @brief for those AABB larger than U x U x U, store them directly.
  std::vector<size_t> large_leafs_;

  // @brief Indicates the maximal length for the root aabb.
  const F epsilon_ = std::numeric_limits<F>::min();
  const F unit_;
  NdRangeIndexer<dim> indexer_;
};

}  // namespace acg::spatial
#include "details/subdivision-inl.hpp"
