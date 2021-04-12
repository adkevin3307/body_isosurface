#include "IsoSurface.h"

using namespace std;

IsoSurface::IsoSurface()
    : m_iso_value(0.0)
{
}

IsoSurface::IsoSurface(Volume const& volume)
    : m_volume(volume)
{
}

IsoSurface::~IsoSurface()
{
    this->m_vertices.clear();
    this->m_vertices.shrink_to_fit();

    this->m_normals.clear();
    this->m_normals.shrink_to_fit();
}

pair<glm::vec3, glm::vec3> IsoSurface::interpolation(glm::ivec3 p1, glm::ivec3 p2)
{
    glm::vec3 voxel_size = this->m_volume.voxel_size();

    pair<float, glm::vec3> voxel_1 = this->m_volume(p1);
    pair<float, glm::vec3> voxel_2 = this->m_volume(p2);

    float v1 = voxel_1.first;
    float v2 = voxel_2.first;
    glm::vec3 n1 = voxel_1.second;
    glm::vec3 n2 = voxel_2.second;

    if (glm::length(n1) >= numeric_limits<float>::epsilon()) n1 = glm::normalize(n1);
    if (glm::length(n2) >= numeric_limits<float>::epsilon()) n2 = glm::normalize(n2);

    if (fabs(this->m_iso_value - v1) < numeric_limits<float>::epsilon()) return make_pair(glm::vec3(p1) * voxel_size, n1);
    if (fabs(this->m_iso_value - v2) < numeric_limits<float>::epsilon()) return make_pair(glm::vec3(p2) * voxel_size, n2);
    if (fabs(v1 - v2) < numeric_limits<float>::epsilon()) return make_pair(glm::vec3(p1) * voxel_size, n1);

    float mu = (this->m_iso_value - v1) / (v2 - v1);
    glm::vec3 coordinate = glm::vec3(p1) + mu * (glm::vec3(p2) - glm::vec3(p1));
    glm::vec3 normal = n1 + mu * (n2 - n1);

    if (glm::length(normal) >= numeric_limits<float>::epsilon()) normal = glm::normalize(normal);

    return make_pair(coordinate * voxel_size, normal);
}

void IsoSurface::run()
{
    vector<pair<glm::vec3, glm::vec3>> v(12);

    for (auto i = 0; i < this->m_volume.shape().x - 1; i++) {
        for (auto j = 0; j < this->m_volume.shape().y - 1; j++) {
            for (auto k = 0; k < this->m_volume.shape().z - 1; k++) {
                int index = 0;

                if (this->m_volume(i, j, k).first < this->m_iso_value) index |= 1;
                if (this->m_volume(i, j, k + 1).first < this->m_iso_value) index |= 2;
                if (this->m_volume(i, j + 1, k + 1).first < this->m_iso_value) index |= 4;
                if (this->m_volume(i, j + 1, k).first < this->m_iso_value) index |= 8;
                if (this->m_volume(i + 1, j, k).first < this->m_iso_value) index |= 16;
                if (this->m_volume(i + 1, j, k + 1).first < this->m_iso_value) index |= 32;
                if (this->m_volume(i + 1, j + 1, k + 1).first < this->m_iso_value) index |= 64;
                if (this->m_volume(i + 1, j + 1, k).first < this->m_iso_value) index |= 128;

                if (CONSTANT::EDGETABLE[index] == 0) continue;
                if (CONSTANT::EDGETABLE[index] & 1) v[0] = this->interpolation(glm::ivec3(i, j, k), glm::ivec3(i, j, k + 1));
                if (CONSTANT::EDGETABLE[index] & 2) v[1] = this->interpolation(glm::ivec3(i, j, k + 1), glm::ivec3(i, j + 1, k + 1));
                if (CONSTANT::EDGETABLE[index] & 4) v[2] = this->interpolation(glm::ivec3(i, j + 1, k + 1), glm::ivec3(i, j + 1, k));
                if (CONSTANT::EDGETABLE[index] & 8) v[3] = this->interpolation(glm::ivec3(i, j, k), glm::ivec3(i, j + 1, k));
                if (CONSTANT::EDGETABLE[index] & 16) v[4] = this->interpolation(glm::ivec3(i + 1, j, k), glm::ivec3(i + 1, j, k + 1));
                if (CONSTANT::EDGETABLE[index] & 32) v[5] = this->interpolation(glm::ivec3(i + 1, j, k + 1), glm::ivec3(i + 1, j + 1, k + 1));
                if (CONSTANT::EDGETABLE[index] & 64) v[6] = this->interpolation(glm::ivec3(i + 1, j + 1, k + 1), glm::ivec3(i + 1, j + 1, k));
                if (CONSTANT::EDGETABLE[index] & 128) v[7] = this->interpolation(glm::ivec3(i + 1, j, k), glm::ivec3(i + 1, j + 1, k));
                if (CONSTANT::EDGETABLE[index] & 256) v[8] = this->interpolation(glm::ivec3(i, j, k), glm::ivec3(i + 1, j, k));
                if (CONSTANT::EDGETABLE[index] & 512) v[9] = this->interpolation(glm::ivec3(i, j, k + 1), glm::ivec3(i + 1, j, k + 1));
                if (CONSTANT::EDGETABLE[index] & 1024) v[10] = this->interpolation(glm::ivec3(i, j + 1, k + 1), glm::ivec3(i + 1, j + 1, k + 1));
                if (CONSTANT::EDGETABLE[index] & 2048) v[11] = this->interpolation(glm::ivec3(i, j + 1, k), glm::ivec3(i + 1, j + 1, k));

                for (auto vertex = 0; CONSTANT::TRIANGLETABLE[index][vertex] != -1 && vertex < 16; vertex += 3) {
                    for (auto delta_vertex = 0; delta_vertex < 3; delta_vertex++) {
                        this->m_vertices.push_back(v[CONSTANT::TRIANGLETABLE[index][vertex + delta_vertex]].first.x);
                        this->m_vertices.push_back(v[CONSTANT::TRIANGLETABLE[index][vertex + delta_vertex]].first.y);
                        this->m_vertices.push_back(v[CONSTANT::TRIANGLETABLE[index][vertex + delta_vertex]].first.z);

                        this->m_normals.push_back(v[CONSTANT::TRIANGLETABLE[index][vertex + delta_vertex]].second.x);
                        this->m_normals.push_back(v[CONSTANT::TRIANGLETABLE[index][vertex + delta_vertex]].second.y);
                        this->m_normals.push_back(v[CONSTANT::TRIANGLETABLE[index][vertex + delta_vertex]].second.z);
                    }
                }
            }
        }
    }
}

glm::vec3 const IsoSurface::shape() const
{
    return (glm::vec3(this->m_volume.shape()) * this->m_volume.voxel_size());
}

vector<float> const& IsoSurface::vertices() const
{
    return this->m_vertices;
}

vector<float> const& IsoSurface::normals() const
{
    return this->m_normals;
}

float const& IsoSurface::iso_value() const
{
    return this->m_iso_value;
}

float& IsoSurface::iso_value()
{
    return this->m_iso_value;
}
