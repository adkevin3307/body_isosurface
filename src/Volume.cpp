#include "Volume.h"

#include <stdexcept>
#include <fstream>
#include <unistd.h>

using namespace std;

Volume::Volume()
    : m_byte_size(0), m_min(0.0), m_max(0.0), m_inf_file(""), m_raw_file(""), m_type(CONSTANT::TYPE::CHAR), m_endian(CONSTANT::ENDIAN::LITTLE), m_shape(0), m_voxel_size(0.0)
{
    short int word = 0x0001;
    char* check = (char*)&word;
    this->m_machine_endian = (check[0] ? CONSTANT::ENDIAN::LITTLE : CONSTANT::ENDIAN::BIG);
}

Volume::Volume(string inf_file, string raw_file)
    : Volume::Volume()
{
    if (access(inf_file.c_str(), R_OK) != 0) {
        throw runtime_error(inf_file + " not exist");
    }
    if (access(raw_file.c_str(), R_OK) != 0) {
        throw runtime_error(raw_file + " not exist");
    }

    this->m_inf_file = inf_file;
    this->m_raw_file = raw_file;

    this->load_inf_file();
    this->load_raw_file();
}

Volume::Volume(Volume const& rhs)
    : m_byte_size(rhs.m_byte_size),
      m_min(rhs.m_min), m_max(rhs.m_max),
      m_inf_file(rhs.m_inf_file), m_raw_file(rhs.m_raw_file),
      m_type(rhs.m_type),
      m_endian(rhs.m_endian), m_machine_endian(rhs.m_machine_endian),
      m_shape(rhs.m_shape),
      m_voxel_size(rhs.m_voxel_size),
      m_data(rhs.m_data)
{
}

Volume::Volume(Volume&& rhs)
    : m_byte_size(rhs.m_byte_size),
      m_min(rhs.m_min), m_max(rhs.m_max),
      m_inf_file(rhs.m_inf_file), m_raw_file(rhs.m_raw_file),
      m_type(rhs.m_type),
      m_endian(rhs.m_endian), m_machine_endian(rhs.m_machine_endian),
      m_shape(std::move(rhs.m_shape)),
      m_voxel_size(std::move(rhs.m_voxel_size)),
      m_data(std::move(rhs.m_data))
{
}

Volume::~Volume()
{
    this->m_data.clear();
    this->m_data.shrink_to_fit();
}

void Volume::load_inf_file()
{
    string s;
    fstream file;

    file.open(this->m_inf_file, ios::in);

    if (!file.is_open()) {
        throw runtime_error("cannot open " + this->m_inf_file);
    }

    while (getline(file, s)) {
        if (s[0] == '#') continue;

        s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
        transform(s.begin(), s.end(), s.begin(), ::tolower);

        auto it = s.find('=');
        if (it == string::npos) continue;

        string key = s.substr(0, it);
        string value = s.substr(it + 1);

        if (key.find("file") != string::npos) {
            continue;
        }
        else if (key.find("endian") != string::npos || key.find("indian") != string::npos) {
            this->m_endian = (value.find('l') != string::npos ? CONSTANT::ENDIAN::LITTLE : CONSTANT::ENDIAN::BIG);
        }
        else if (key.find("resolution") != string::npos) {
            replace(value.begin(), value.end(), 'x', ':');
            sscanf(value.c_str(), "%d:%d:%d", &(this->m_shape.z), &(this->m_shape.y), &(this->m_shape.x));
        }
        else if (key.find("voxel") != string::npos || key.find("ratio") != string::npos) {
            replace(value.begin(), value.end(), 'x', ':');
            sscanf(value.c_str(), "%f:%f:%f", &(this->m_voxel_size.z), &(this->m_voxel_size.y), &(this->m_voxel_size.x));
        }
        else if (key.find("sample") != string::npos || key.find("value") != string::npos) {
            if (value.find("f") != string::npos) {
                this->m_byte_size = 4;
                this->m_type = CONSTANT::TYPE::FLOAT;
            }
            else if (value.find("char") != string::npos || value == "b" || value == "ub") {
                this->m_byte_size = 1;
                this->m_type = ((value.find("u") != string::npos) ? CONSTANT::TYPE::UNSIGNED_CHAR : CONSTANT::TYPE::CHAR);
            }
            else if (value.find("short") != string::npos || value == "s" || value == "us") {
                this->m_byte_size = 2;
                this->m_type = ((value.find("u") != string::npos) ? CONSTANT::TYPE::UNSIGNED_SHORT : CONSTANT::TYPE::SHORT);
            }
            else if (value.find("int") != string::npos || value == "i" || value == "ui") {
                this->m_byte_size = 4;
                this->m_type = ((value.find("u") != string::npos) ? CONSTANT::TYPE::UNSIGNED_INT : CONSTANT::TYPE::INT);
            }
        }
    }

    file.close();

    this->m_data.resize(this->m_shape.x * this->m_shape.y * this->m_shape.z);
}

void Volume::load_raw_file()
{
    fstream file;
    char* byte_data = new char[this->m_shape.x * this->m_shape.y * this->m_shape.z * this->m_byte_size];

    file.open(this->m_raw_file, ios::in | ios::binary);

    if (!file.is_open()) {
        throw runtime_error("cannot open " + this->m_raw_file);
    }

    file.read(byte_data, this->m_shape.x * this->m_shape.y * this->m_shape.z * this->m_byte_size);

    file.close();

    for (size_t i = 0; i < this->m_data.size(); i++) {
        int byte_index = i * this->m_byte_size;

        float value = 0.0;
        switch (this->m_type) {
            case CONSTANT::TYPE::FLOAT:
                value = this->endian<float>(byte_index, byte_data);

                break;
            case CONSTANT::TYPE::UNSIGNED_CHAR:
                value = (float)this->endian<unsigned char>(byte_index, byte_data);

                break;
            case CONSTANT::TYPE::CHAR:
                value = (float)this->endian<char>(byte_index, byte_data);

                break;
            case CONSTANT::TYPE::UNSIGNED_SHORT:
                value = (float)this->endian<unsigned short>(byte_index, byte_data);

                break;
            case CONSTANT::TYPE::SHORT:
                value = (float)this->endian<short>(byte_index, byte_data);

                break;
            case CONSTANT::TYPE::UNSIGNED_INT:
                value = (float)this->endian<unsigned int>(byte_index, byte_data);

                break;
            case CONSTANT::TYPE::INT:
                value = (float)this->endian<int>(byte_index, byte_data);

                break;
            default:
                break;
        }

        this->m_data[i].first = value;

        if (i == 0) {
            this->m_min = value;
            this->m_max = value;
        }

        this->m_max = max(this->m_max, value);
        this->m_min = min(this->m_min, value);
    }

    delete[] byte_data;
}

glm::vec3 Volume::gradient(int x, int y, int z)
{
    glm::vec3 result;

    glm::ivec3 largest(min(this->m_shape.x - 1, x + 1), min(this->m_shape.y - 1, y + 1), min(this->m_shape.z - 1, z + 1));
    glm::ivec3 smallest(max(0, x - 1), max(0, y - 1), max(0, z - 1));
    glm::vec3 delta = (glm::vec3(largest) - glm::vec3(smallest)) * this->m_voxel_size;

    result.x = (this->operator()(largest.x, y, z).first - this->operator()(smallest.x, y, z).first);
    result.y = (this->operator()(x, largest.y, z).first - this->operator()(x, smallest.y, z).first);
    result.z = (this->operator()(x, y, largest.z).first - this->operator()(x, y, smallest.z).first);

    return (result / delta);
}

void Volume::calculate_gradient()
{
    for (auto i = 0; i < this->m_shape.x; i++) {
        for (auto j = 0; j < this->m_shape.y; j++) {
            for (auto k = 0; k < this->m_shape.z; k++) {
                this->operator()(i, j, k).second = this->gradient(i, j, k);
            }
        }
    }
}

glm::ivec3 const& Volume::shape() const
{
    return this->m_shape;
}

glm::vec3 const& Volume::voxel_size() const
{
    return this->m_voxel_size;
}

pair<float, glm::vec3> const& Volume::operator()(int x, int y, int z) const
{
    size_t index = x * this->m_shape.y * this->m_shape.z + y * this->m_shape.z + z;

    if (index >= this->m_data.size()) {
        throw out_of_range("index for volume data out of range");
    }

    return this->m_data[index];
}

pair<float, glm::vec3>& Volume::operator()(int x, int y, int z)
{
    size_t index = x * this->m_shape.y * this->m_shape.z + y * this->m_shape.z + z;

    if (index >= this->m_data.size()) {
        throw out_of_range("index for volume data out of range");
    }

    return this->m_data[index];
}

pair<float, glm::vec3> const& Volume::operator()(glm::ivec3 index) const
{
    return this->operator()(index.x, index.y, index.z);
}

pair<float, glm::vec3>& Volume::operator()(glm::ivec3 index)
{
    return this->operator()(index.x, index.y, index.z);
}
