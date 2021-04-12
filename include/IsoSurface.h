#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Volume.h"

class IsoSurface {
private:
    Volume m_volume;
    float m_iso_value;
    std::vector<float> m_vertices;
    std::vector<float> m_normals;

    std::pair<glm::vec3, glm::vec3> interpolation(glm::ivec3 p1, glm::ivec3 p2);

public:
    IsoSurface();
    IsoSurface(Volume const& volume);
    ~IsoSurface();

    IsoSurface(IsoSurface const& rhs) = delete;
    IsoSurface(IsoSurface&& rhs) = delete;
    IsoSurface& operator=(IsoSurface const& rhs) = delete;
    IsoSurface& operator=(IsoSurface&& rhs) = delete;

    void run();

    glm::vec3 const shape() const;
    std::vector<float> const& vertices() const;
    std::vector<float> const& normals() const;

    float const& iso_value() const;
    float& iso_value();

    friend std::ostream& operator<<(std::ostream& os, const IsoSurface& iso_surface)
    {
        os << "iso value: " << iso_surface.m_iso_value << '\n';
        os << "shape: " << glm::to_string(iso_surface.shape()) << '\n';
        os << "vertices: " << iso_surface.m_vertices.size() << '\n';
        os << "normals: " << iso_surface.m_normals.size();

        return os;
    }
};
