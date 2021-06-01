#include "IsoSurface.h"

using namespace std;

pair<glm::vec3, glm::vec3> interpolation(pair<VOXEL, VOXEL> voxel, pair<glm::ivec3, glm::ivec3> index, glm::vec3 voxel_size, float value)
{
    float v1 = voxel.first.value;
    float v2 = voxel.second.value;
    glm::vec3 n1 = voxel.first.normal;
    glm::vec3 n2 = voxel.second.normal;

    if (glm::length(n1) >= numeric_limits<float>::epsilon()) n1 = glm::normalize(n1);
    if (glm::length(n2) >= numeric_limits<float>::epsilon()) n2 = glm::normalize(n2);

    if (fabs(value - v1) < numeric_limits<float>::epsilon()) return make_pair(glm::vec3(index.first) * voxel_size, n1);
    if (fabs(value - v2) < numeric_limits<float>::epsilon()) return make_pair(glm::vec3(index.second) * voxel_size, n2);
    if (fabs(v1 - v2) < numeric_limits<float>::epsilon()) return make_pair(glm::vec3(index.first) * voxel_size, n1);

    float mu = (value - v1) / (v2 - v1);
    glm::vec3 coordinate = glm::vec3(index.first) + mu * (glm::vec3(index.second) - glm::vec3(index.first));
    glm::vec3 normal = n1 + mu * (n2 - n1);

    if (glm::length(normal) >= numeric_limits<float>::epsilon()) normal = glm::normalize(normal);

    return make_pair(coordinate * voxel_size, normal);
}

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

void IsoSurface::run()
{
    constexpr const int base[][6] = {
        { 0, 0, 0, 0, 0, 1 },
        { 0, 0, 1, 0, 1, 1 },
        { 0, 1, 1, 0, 1, 0 },
        { 0, 0, 0, 0, 1, 0 },
        { 1, 0, 0, 1, 0, 1 },
        { 1, 0, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 0 },
        { 1, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 0, 1 },
        { 0, 1, 1, 1, 1, 1 },
        { 0, 1, 0, 1, 1, 0 },
    };

    #pragma omp parallel
    {
        pair<glm::vec3, glm::vec3> v[12];
        vector<pair<glm::vec3, glm::vec3>> local_v;

        #pragma omp for collapse(3)
        for (auto i = 0; i < this->m_volume.shape().x - 1; i++) {
            for (auto j = 0; j < this->m_volume.shape().y - 1; j++) {
                for (auto k = 0; k < this->m_volume.shape().z - 1; k++) {
                    int index = 0;

                    if (this->m_volume(i, j, k).value < this->m_iso_value) index |= 1;
                    if (this->m_volume(i, j, k + 1).value < this->m_iso_value) index |= 2;
                    if (this->m_volume(i, j + 1, k + 1).value < this->m_iso_value) index |= 4;
                    if (this->m_volume(i, j + 1, k).value < this->m_iso_value) index |= 8;
                    if (this->m_volume(i + 1, j, k).value < this->m_iso_value) index |= 16;
                    if (this->m_volume(i + 1, j, k + 1).value < this->m_iso_value) index |= 32;
                    if (this->m_volume(i + 1, j + 1, k + 1).value < this->m_iso_value) index |= 64;
                    if (this->m_volume(i + 1, j + 1, k).value < this->m_iso_value) index |= 128;

                    if (CONSTANT::EDGETABLE[index] == 0) continue;
                    for (auto v_index = 0; v_index < 12; v_index++) {
                        if (CONSTANT::EDGETABLE[index] & (1 << v_index)) {
                            glm::ivec3 p1(i + base[v_index][0], j + base[v_index][1], k + base[v_index][2]);
                            glm::ivec3 p2(i + base[v_index][3], j + base[v_index][4], k + base[v_index][5]);

                            VOXEL v1 = this->m_volume(p1);
                            VOXEL v2 = this->m_volume(p2);

                            v[v_index] = interpolation(make_pair(v1, v2), make_pair(p1, p2), this->m_volume.voxel_size(), this->m_iso_value);
                        }
                    }

                    for (auto vertex = 0; CONSTANT::TRIANGLETABLE[index][vertex] != -1 && vertex < 16; vertex += 3) {
                        local_v.push_back(v[CONSTANT::TRIANGLETABLE[index][vertex + 0]]);
                        local_v.push_back(v[CONSTANT::TRIANGLETABLE[index][vertex + 1]]);
                        local_v.push_back(v[CONSTANT::TRIANGLETABLE[index][vertex + 2]]);
                    }
                }
            }
        }

        #pragma omp critical
        {
            for (size_t i = 0; i < local_v.size(); i += 3) {
                this->m_vertices.push_back(local_v[i + 0].first.x);
                this->m_vertices.push_back(local_v[i + 0].first.y);
                this->m_vertices.push_back(local_v[i + 0].first.z);

                this->m_vertices.push_back(local_v[i + 1].first.x);
                this->m_vertices.push_back(local_v[i + 1].first.y);
                this->m_vertices.push_back(local_v[i + 1].first.z);

                this->m_vertices.push_back(local_v[i + 2].first.x);
                this->m_vertices.push_back(local_v[i + 2].first.y);
                this->m_vertices.push_back(local_v[i + 2].first.z);

                this->m_normals.push_back(local_v[i + 0].second.x);
                this->m_normals.push_back(local_v[i + 0].second.y);
                this->m_normals.push_back(local_v[i + 0].second.z);

                this->m_normals.push_back(local_v[i + 1].second.x);
                this->m_normals.push_back(local_v[i + 1].second.y);
                this->m_normals.push_back(local_v[i + 1].second.z);

                this->m_normals.push_back(local_v[i + 2].second.x);
                this->m_normals.push_back(local_v[i + 2].second.y);
                this->m_normals.push_back(local_v[i + 2].second.z);
            }
        }
    }
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
