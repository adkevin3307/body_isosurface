#include "MeshManagement.h"

#include <stdexcept>

using namespace std;

Mesh::Mesh()
    : m_shape(glm::vec3(0.0)), m_size(0)
{
}

Mesh::Mesh(Buffer buffer, glm::vec3 shape, int size)
    : m_buffer(buffer), m_shape(shape), m_size(size)
{
}

Mesh::Mesh(Mesh const& rhs)
    : m_buffer(rhs.m_buffer), m_shape(rhs.m_shape), m_size(rhs.m_size)
{
}

Mesh::Mesh(Mesh&& rhs)
    : m_buffer(move(rhs.m_buffer)), m_shape(rhs.m_shape), m_size(rhs.m_size)
{
}

Mesh::~Mesh()
{
}

Mesh& Mesh::operator=(Mesh const& rhs)
{
    if (this == &rhs) return *this;

    this->m_buffer = rhs.m_buffer;
    this->m_shape = rhs.m_shape;
    this->m_size = rhs.m_size;

    return *this;
}

Mesh& Mesh::operator=(Mesh&& rhs)
{
    if (this == &rhs) return *this;

    this->m_buffer = move(rhs.m_buffer);
    this->m_shape = rhs.m_shape;
    this->m_size = rhs.m_size;

    return *this;
}

Buffer const& Mesh::buffer() const
{
    return this->m_buffer;
}

glm::vec3 const Mesh::shape() const
{
    return this->m_shape;
}

int const Mesh::size() const
{
    return this->m_size;
}

vector<Mesh> MeshManagement::m_datas;
map<string, Volume> MeshManagement::m_volumes;
size_t MeshManagement::size;

MeshManagement::MeshManagement()
{
}

MeshManagement::~MeshManagement()
{
    MeshManagement::m_volumes.clear();

    MeshManagement::m_datas.clear();
    MeshManagement::m_datas.shrink_to_fit();
}

void MeshManagement::add(string name, Volume& volume)
{
    MeshManagement::m_volumes[name] = volume;
}

void MeshManagement::add(Buffer& buffer, glm::vec3& shape, int size)
{
    Mesh mesh(buffer, shape, size);

    MeshManagement::m_datas.push_back(mesh);

    MeshManagement::size = m_datas.size();
}

void MeshManagement::clear()
{
    MeshManagement::size = 0;

    MeshManagement::m_datas.clear();
}

Volume const& MeshManagement::get(std::string name)
{
    if (MeshManagement::m_volumes.find(name) == MeshManagement::m_volumes.end()) {
        throw out_of_range(name + " key in get not found");
    }

    return MeshManagement::m_volumes[name];
}

Mesh const& MeshManagement::get(size_t index)
{
    if (index >= MeshManagement::m_datas.size()) {
        throw out_of_range("index " + to_string(index) + " in get not found");
    }

    return MeshManagement::m_datas[index];
}

bool MeshManagement::exist(string name)
{
    return (MeshManagement::m_volumes.find(name) != MeshManagement::m_volumes.end());
}
