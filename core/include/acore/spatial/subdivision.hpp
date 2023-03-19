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
  using container_type = std::vector<std::pair<AABB<F, dim>, D>>;
  // using iterator_type = typename container_type::iterator;
  // using const_iterator_type = typename container_type::const_iterator_type;
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
  size_t Insert(const std::pair<AABB<F, dim>, D> &data);

  /**
   * @brief Get Visualize data, first = position, second = topo.
   */
  std::pair<Field<F, dim>, types::topology::LineList> Visualize() const;

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
  std::vector<size_t> Query(const AABB<F, dim> &aabb) const;

  std::vector<std::pair<size_t, size_t>> QueryInternal() const;

  explicit SubDivisionAABB(F unit_length = 1.0, F epsilon = 1e-6)
      : epsilon_(epsilon),
        unit_(unit_length),
        indexer_(std::make_from_tuple<NdRangeIndexer<dim>>(
            utils::god::duplicate<dim>(subdivision_count))) {}

  struct Node {
    AABB<F, dim> box_;
    F unit_;
    std::vector<size_t> leafs_;
    std::array<size_t, subdivision_count_total_> sub_nodes_;
    AABB<F, dim> GetChildAABB(utils::god::IndexTuple<dim> index) const;
    explicit Node(F unit, AABB<F, dim> box) : box_(box), unit_(unit) {
      std::fill(std::begin(sub_nodes_), std::end(sub_nodes_), InvalidSubscript);
    }
  };

  std::pair<AABB<F, dim>, D> &Get(size_t id);

  const std::pair<AABB<F, dim>, D> &Get(size_t id) const;

private:
  // @brief Query the entry node's id for aabb.
  size_t FindEntry(const AABB<F, dim> &aabb) const;

  // @brief return the sequence for visit given node.
  //
  // [parent = Entry, Sub1, Sub2, ... , Destination]
  //
  // for large leafs, return [InvalidSubscript ... ]
  std::array<size_t, max_depth> FindVisitSequence(const AABB<F, dim> &aabb);

  size_t PutAABBData(const std::pair<AABB<F, dim>, D> &aabb);

  size_t PutNode(F local_unit, const AABB<F, dim> &range);

  size_t EnsureSubDivision(size_t parent, size_t child);

  size_t EnsureEntry(const AABB<F, dim> &aabb);

  void QueryVisit(std::vector<size_t> &retval, size_t node, const AABB<F, dim> &aabb) const;

  void QueryVisitInternal(std::vector<std::pair<size_t, size_t>> &retval, size_t node) const;

  std::vector<std::pair<AABB<F, dim>, D>> data_;
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
