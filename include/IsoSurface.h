#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Volume.h"

std::pair<glm::vec3, glm::vec3> interpolation(std::pair<VOXEL, VOXEL> voxel, std::pair<glm::ivec3, glm::ivec3> index, glm::vec3 voxel_size, float value);

class IsoSurface {
private:
    Volume m_volume;
    float m_iso_value;
    std::vector<float> m_vertices;
    std::vector<float> m_normals;

public:
    IsoSurface();
    IsoSurface(Volume const& volume);
    ~IsoSurface();

    IsoSurface(IsoSurface const& rhs) = delete;
    IsoSurface(IsoSurface&& rhs) = delete;
    IsoSurface& operator=(IsoSurface const& rhs) = delete;
    IsoSurface& operator=(IsoSurface&& rhs) = delete;

    void run();

    std::vector<float> const& vertices() const;
    std::vector<float> const& normals() const;

    float const& iso_value() const;
    float& iso_value();

    friend std::ostream& operator<<(std::ostream& os, const IsoSurface& iso_surface)
    {
        os << "iso value: " << iso_surface.m_iso_value << '\n';
        os << "vertices: " << iso_surface.m_vertices.size() << '\n';
        os << "normals: " << iso_surface.m_normals.size();

        return os;
    }
};
