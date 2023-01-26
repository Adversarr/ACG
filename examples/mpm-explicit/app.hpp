#include <acg_core/math/common.hpp>
#include <memory>

/* Mpm3d in taichi
import numpy as np

import taichi as ti

ti.init(arch=ti.gpu)

#dim, n_grid, steps, dt = 2, 128, 20, 2e-4
#dim, n_grid, steps, dt = 2, 256, 32, 1e-4
dim, n_grid, steps, dt = 3, 32, 25, 4e-4
#dim, n_grid, steps, dt = 3, 64, 25, 2e-4
#dim, n_grid, steps, dt = 3, 128, 25, 8e-5

n_particles = n_grid**dim // 2**(dim - 1)
dx = 1 / n_grid

p_rho = 1
p_vol = (dx * 0.5)**2
p_mass = p_vol * p_rho
gravity = 9.8
bound = 3
E = 400

F_x = ti.Vector.field(dim, float, n_particles)
F_v = ti.Vector.field(dim, float, n_particles)
F_C = ti.Matrix.field(dim, dim, float, n_particles)
F_J = ti.field(float, n_particles)

F_grid_v = ti.Vector.field(dim, float, (n_grid, ) * dim)
F_grid_m = ti.field(float, (n_grid, ) * dim)

neighbour = (3, ) * dim


@ti.kernel
def substep():
    for I in ti.grouped(F_grid_m):
        F_grid_v[I] = ti.zero(F_grid_v[I])
        F_grid_m[I] = 0
    ti.loop_config(block_dim=n_grid)
    for p in F_x:
        Xp = F_x[p] / dx
        base = int(Xp - 0.5)
        fx = Xp - base
        w = [0.5 * (1.5 - fx)**2, 0.75 - (fx - 1)**2, 0.5 * (fx - 0.5)**2]
        stress = -dt * 4 * E * p_vol * (F_J[p] - 1) / dx**2
        affine = ti.Matrix.identity(float, dim) * stress + p_mass * F_C[p]
        for offset in ti.static(ti.grouped(ti.ndrange(*neighbour))):
            dpos = (offset - fx) * dx
            weight = 1.0
            for i in ti.static(range(dim)):
                weight *= w[offset[i]][i]
            F_grid_v[base +
                     offset] += weight * (p_mass * F_v[p] + affine @ dpos)
            F_grid_m[base + offset] += weight * p_mass
    for I in ti.grouped(F_grid_m):
        if F_grid_m[I] > 0:
            F_grid_v[I] /= F_grid_m[I]
        F_grid_v[I][1] -= dt * gravity
        cond = (I < bound) & (F_grid_v[I] < 0) | \
               (I > n_grid - bound) & (F_grid_v[I] > 0)
        F_grid_v[I] = ti.select(cond, 0, F_grid_v[I])
    ti.loop_config(block_dim=n_grid)
    for p in F_x:
        Xp = F_x[p] / dx
        base = int(Xp - 0.5)
        fx = Xp - base
        w = [0.5 * (1.5 - fx)**2, 0.75 - (fx - 1)**2, 0.5 * (fx - 0.5)**2]
        new_v = ti.zero(F_v[p])
        new_C = ti.zero(F_C[p])
        for offset in ti.static(ti.grouped(ti.ndrange(*neighbour))):
            dpos = (offset - fx) * dx
            weight = 1.0
            for i in ti.static(range(dim)):
                weight *= w[offset[i]][i]
            g_v = F_grid_v[base + offset]
            new_v += weight * g_v
            new_C += 4 * weight * g_v.outer_product(dpos) / dx**2
        F_v[p] = new_v
        F_x[p] += dt * F_v[p]
        F_J[p] *= 1 + dt * new_C.trace()
        F_C[p] = new_C


@ti.kernel
def init():
    for i in range(n_particles):
        F_x[i] = ti.Vector([ti.random() for i in range(dim)]) * 0.4 + 0.15
        F_J[i] = 1


def T(a):
    if dim == 2:
        return a

    phi, theta = np.radians(28), np.radians(32)

    a = a - 0.5
    x, y, z = a[:, 0], a[:, 1], a[:, 2]
    cp, sp = np.cos(phi), np.sin(phi)
    ct, st = np.cos(theta), np.sin(theta)
    x, z = x * cp + z * sp, z * cp - x * sp
    u, v = x, y * ct + z * st
    return np.array([u, v]).swapaxes(0, 1) + 0.5


def main():
    init()
    gui = ti.GUI('MPM3D', background_color=0x112F41)
    while gui.running and not gui.get_event(gui.ESCAPE):
        for s in range(steps):
            substep()
        pos = F_x.to_numpy()
        if export_file:
            writer = ti.tools.PLYWriter(num_vertices=n_particles)
            writer.add_vertex_pos(pos[:, 0], pos[:, 1], pos[:, 2])
            writer.export_frame(gui.frame, export_file)
        gui.circles(T(pos), radius=1.5, color=0x66ccff)
        gui.show()


if __name__ == '__main__':
    main()
*/

using namespace acg;

/* Grid:
 * (0, 0) (0, 1)  ...  (0 , 0, #g)
 *  ...    ...    ...   ...
 * (#g, 0) ...    ...  (#g, #g, #g)
 * */

class MpmExplictApp {
public:
  MpmExplictApp();

  void Run();

  void SubStep();

  void Init();

  void P2G();

  void Advection();

  void G2P();

  void Projection();

  inline Index GetGridIndex(Index i, Index j, Index k) const {
    assert(0 <= i && i < n_grid_ + 1);
    assert(0 <= j && j < n_grid_ + 1);
    assert(0 <= k && k < n_grid_ + 1);
    return k + (n_grid_ + 1) * (j + (n_grid_ + 1) * i);
  }

  Index n_grid_{48};
  Index grid_size_;

  Vec3f grav_ = {0, 0, -9.8};

  Index steps_{32};

  F64 simulation_range_{1};
  F64 dx_;
  F64 dt_ = 4e-3;

  F64 rho_ = 1e3;
  F64 particle_mass_;
  F64 particle_vol_;
  F64 E_ = 4;
  F64 weight_sum;

  Index n_particles_;
  Field<F64, 3> grid_velocity_;
  Field<F64, 1> grid_mass_;

  Field<F64, 9> particle_C_;
  Field<F64, 1> particle_J_;
  Field<F64, 3> particle_position_;
  Field<F64, 3> particle_velocity_;
};
