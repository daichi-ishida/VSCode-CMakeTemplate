#include <openvdb/openvdb.h>
#include <openvdb/tools/Dense.h>

#include <cmath>
#include <vector>
#include <algorithm>


double smoothstep(double edge0, double edge1, double x) 
{
    x = std::min(std::max((x - edge0) / (edge1 - edge0), 0.0), 1.0);
    // Evaluate polynomial
    return x * x * (3 - 2 * x);
}

constexpr int N = 64;
constexpr int DIM_X = 2*N;
constexpr int DIM_Y = N;
constexpr int DIM_Z = N;

int main()
{

    double center_x = 0.5*static_cast<double>(DIM_X);
    double center_y = 0.9*static_cast<double>(DIM_Y);
    double center_z = 0.5*static_cast<double>(DIM_Z);
    double sphere_radius = 0.05 * static_cast<double>(DIM_Y);

    std::vector<double> density;
    for(unsigned int k = 0; k < DIM_Z; ++k)
        for(unsigned int j = 0; j < DIM_Y; ++j)
            for(unsigned int i = 0; i < DIM_X; ++i)
            {
                double d_x = static_cast<double>(i) + 0.5 - center_x;
                double d_y = static_cast<double>(j) + 0.5 - center_y;
                double d_z = static_cast<double>(k) + 0.5 - center_z;

                double distance = std::sqrt(d_x*d_x + d_y*d_y + d_z*d_z);
                double sdf_radius = sphere_radius + 1.0;

                double value = 1.0 - smoothstep(sphere_radius, sdf_radius, distance);

                density.push_back(value);
            }

    // The grid spacing
    const double dx = 1.0;

    // Initialize the OpenVDB library.
    openvdb::initialize();

    double ox = -0.5 * dx * static_cast<double>(DIM_X);
    double oy = -0.5 * dx * static_cast<double>(DIM_Y);
    double oz = 0;

    // density
    openvdb::DoubleGrid::Ptr densityGrid = openvdb::DoubleGrid::create(0.0);
    densityGrid->setName("density");
    openvdb::math::Transform::Ptr transform = openvdb::math::Transform::createLinearTransform(dx);
    transform->postTranslate(openvdb::math::Vec3d(ox, oy, oz));
    densityGrid->setTransform(transform);
    densityGrid->setGridClass(openvdb::GRID_FOG_VOLUME);

    auto densityAccessor = densityGrid->getAccessor();
    for(unsigned int z = 0; z < DIM_Z; ++z)
        for(unsigned int y = 0; y < DIM_Y; ++y)
            for(unsigned int x = 0; x < DIM_X; ++x)
            {
                int offset = x + (y + z * DIM_Y) * DIM_X;
                openvdb::Coord xyz(x, y, z);
                densityAccessor.setValue(xyz, density[offset]);
            }

    // Add the grid pointer to a container.
    openvdb::GridPtrVec grids;
    grids.push_back(densityGrid);

    // Create a VDB file object.
    openvdb::io::File file("smoke.vdb");

    // Write out the contents of the container.
    file.write(grids);
    file.close();
}