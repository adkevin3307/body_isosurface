#pragma once

#include <vector>
#include <string>
#include <utility>
#include <map>
#include <glm/glm.hpp>

#include "Volume.h"
#include "BufferManagement.h"

class Mesh {
private:
    Buffer m_buffer;
    glm::vec3 m_shape;
    int m_size;

public:
    Mesh();
    Mesh(Buffer buffer, glm::vec3 shape, int size);
    Mesh(Mesh const& rhs);
    Mesh(Mesh&& rhs);
    ~Mesh();

    Mesh& operator=(Mesh const& rhs);
    Mesh& operator=(Mesh&& rhs);

    Buffer const& buffer() const;
    glm::vec3 const shape() const;
    int const size() const;
};

class MeshManagement {
private:
    static std::map<std::string, Volume> m_volumes;
    static std::vector<Mesh> m_datas;

public:
    MeshManagement();
    ~MeshManagement();

    MeshManagement(MeshManagement const& rhs) = delete;
    MeshManagement(MeshManagement&& rhs) = delete;
    MeshManagement& operator=(MeshManagement const& rhs) = delete;
    MeshManagement& operator=(MeshManagement&& rhs) = delete;

    static size_t size;

    static void add(std::string name, Volume& volume);
    static void add(Buffer& buffer, glm::vec3& shape, int size);

    static void clear();

    static Volume const& get(std::string name);
    static Mesh const& get(size_t index);

    static bool exist(std::string name);
};
