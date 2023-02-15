#include <acore/all.hpp>
#include <acore/common.hpp>
#include <set>

class App {
public:
  void Init();

  void Step();

  void AddSpring(acg::Index, acg::Index);

  acg::Field<float, 3> position_;
  acg::Field<float, 3> origin_position_;

  acg::Field<float, 3> velocity_;

  acg::geometry::topology::TriangleList faces_;

  acg::Index n_grids_ = 5;

  std::set<std::pair<acg::Index, acg::Index>> springs_;

  float dt_{0.001};

  float k_{1000};
};
