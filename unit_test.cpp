#include <utility>
#include <glm/glm.hpp>
#include <gtest/gtest.h>

#include "include/Volume.h"
#include "include/IsoSurface.h"

using namespace std;

TEST(VolumeTest, BasicAssertions)
{
    Volume volume("Data/Carp.inf", "Data/Carp.raw");

    EXPECT_EQ(glm::ivec3(256, 256, 256), volume.shape());

    glm::vec3 voxel_size = volume.voxel_size();
    EXPECT_FLOAT_EQ(1.0, voxel_size.x);
    EXPECT_FLOAT_EQ(0.5, voxel_size.y);
    EXPECT_FLOAT_EQ(1.0, voxel_size.z);
}

void interpolation_unit_test(pair<VOXEL, VOXEL> voxel, pair<glm::ivec3, glm::ivec3> index, float value, pair<glm::vec3, glm::vec3> answer)
{
    glm::vec3 voxel_size(1.0);

    glm::vec3 vertex, normal;
    pair<glm::vec3, glm::vec3> result;
    glm::vec3 answer_vertex = answer.first;
    glm::vec3 answer_normal = glm::normalize(answer.second);

    result = interpolation(voxel, index, voxel_size, value);
    vertex = result.first;
    normal = result.second;

    EXPECT_FLOAT_EQ(vertex.x, answer_vertex.x);
    EXPECT_FLOAT_EQ(vertex.y, answer_vertex.y);
    EXPECT_FLOAT_EQ(vertex.z, answer_vertex.z);

    EXPECT_FLOAT_EQ(normal.x, answer_normal.x);
    EXPECT_FLOAT_EQ(normal.y, answer_normal.y);
    EXPECT_FLOAT_EQ(normal.z, answer_normal.z);
}

TEST(InterpolationTest, BasicAssertions)
{
    VOXEL v1{
        .value = 0.0,
        .normal = glm::vec3(1.0, 0.0, 0.0)
    };

    VOXEL v2{
        .value = 0.0,
        .normal = glm::vec3(0.0, 1.0, 0.0)
    };

    glm::ivec3 p1(0, 0, 0);
    glm::ivec3 p2(1, 1, 1);

    interpolation_unit_test(make_pair(v1, v2), make_pair(p1, p2), 1.0, make_pair(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0)));

    v1.value = 10.0;
    v2.value = 20.0;

    interpolation_unit_test(make_pair(v1, v2), make_pair(p1, p2), 15.0, make_pair(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.0)));
    interpolation_unit_test(make_pair(v1, v2), make_pair(p1, p2), 10.0, make_pair(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0)));
    interpolation_unit_test(make_pair(v1, v2), make_pair(p1, p2), 20.0, make_pair(glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, 0.0)));

    v1.value = -10.0;
    v2.value = 10.0;

    interpolation_unit_test(make_pair(v1, v2), make_pair(p1, p2), 0.0, make_pair(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.0)));
    interpolation_unit_test(make_pair(v1, v2), make_pair(p1, p2), 5.0, make_pair(glm::vec3(0.75, 0.75, 0.75), glm::vec3(0.25, 0.75, 0.0)));
    interpolation_unit_test(make_pair(v1, v2), make_pair(p1, p2), -5.0, make_pair(glm::vec3(0.25, 0.25, 0.25), glm::vec3(0.75, 0.25, 0.0)));
}

TEST(IsoSurfaceTest, BasicAssertions)
{
    Volume volume("Data/Carp.inf", "Data/Carp.raw");
    IsoSurface iso_surface(volume);

    iso_surface.iso_value() = volume.average();
    EXPECT_FLOAT_EQ(volume.average(), iso_surface.iso_value());

    iso_surface.run();
    EXPECT_EQ(iso_surface.vertices().size(), iso_surface.normals().size());
}
