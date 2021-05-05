#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include "include/Volume.h"
#include "include/IsoSurface.h"

TEST(VolumeTest, BasicAssertions)
{
    Volume volume("Data/Carp.inf", "Data/Carp.raw");

    EXPECT_EQ(glm::ivec3(256, 256, 256), volume.shape());

    glm::vec3 voxel_size = volume.voxel_size();
    EXPECT_FLOAT_EQ(1.0, voxel_size.x);
    EXPECT_FLOAT_EQ(0.5, voxel_size.y);
    EXPECT_FLOAT_EQ(1.0, voxel_size.z);
}

TEST(IsoSurfaceTest, BasicAssertions)
{
    Volume volume("Data/Carp.inf", "Data/Carp.raw");
    IsoSurface iso_surface(volume);

    glm::vec3 shape = iso_surface.shape();
    EXPECT_FLOAT_EQ(256.0, shape.x);
    EXPECT_FLOAT_EQ(128.0, shape.y);
    EXPECT_FLOAT_EQ(256.0, shape.z);

    iso_surface.iso_value() = volume.average();
    EXPECT_FLOAT_EQ(volume.average(), iso_surface.iso_value());

    // TODO test interpolation

    iso_surface.run();
    EXPECT_EQ(iso_surface.vertices().size(), iso_surface.normals().size());
}
