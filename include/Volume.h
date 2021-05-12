#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <utility>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "constant.h"

struct VOXEL {
    float value;
    glm::vec3 normal;
};

class Volume {
private:
    int m_byte_size;
    float m_min, m_max;
    std::string m_inf_file, m_raw_file;
    CONSTANT::TYPE m_type;
    CONSTANT::ENDIAN m_endian, m_machine_endian;
    glm::ivec3 m_shape;
    glm::vec3 m_voxel_size;
    std::vector<VOXEL> m_data;

    template<typename T>
    T endian(int index, char* data)
    {
        T value;

        std::memcpy(&value, data + index, this->m_byte_size);
        if (this->m_machine_endian != this->m_endian) {
            std::reverse(&value, &value + this->m_byte_size);
        }

        return value;
    }

    void load_inf_file();
    void load_raw_file();
    void gradient();

public:
    Volume();
    Volume(std::string inf_file, std::string raw_file);
    Volume(Volume const& rhs);
    Volume(Volume&& rhs);
    ~Volume();

    Volume& operator=(Volume const& rhs);
    Volume& operator=(Volume&& rhs);

    glm::ivec3 const& shape() const;
    glm::vec3 const& voxel_size() const;
    float const average() const;
    float const min_value() const;
    float const max_value() const;

    VOXEL const& operator()(int x, int y, int z) const;
    VOXEL& operator()(int x, int y, int z);
    VOXEL const& operator()(glm::ivec3 index) const;
    VOXEL& operator()(glm::ivec3 index);

    friend std::ostream& operator<<(std::ostream& os, const Volume& volume)
    {
        std::map<CONSTANT::TYPE, std::string> type_map = {
            { CONSTANT::TYPE::UNSIGNED_CHAR, "unsigned char" },
            { CONSTANT::TYPE::CHAR, "char" },
            { CONSTANT::TYPE::UNSIGNED_SHORT, "unsigned short" },
            { CONSTANT::TYPE::SHORT, "short" },
            { CONSTANT::TYPE::UNSIGNED_INT, "unsigned int" },
            { CONSTANT::TYPE::INT, "int" },
            { CONSTANT::TYPE::FLOAT, "float" }
        };

        std::map<CONSTANT::ENDIAN, std::string> endian_map = {
            { CONSTANT::ENDIAN::LITTLE, "little" },
            { CONSTANT::ENDIAN::BIG, "big" }
        };

        os << "inf file: " << volume.m_inf_file << '\n';
        os << "raw file: " << volume.m_raw_file << '\n';
        os << "endian: " << endian_map[volume.m_endian] << '\n';
        os << "type: " << type_map[volume.m_type] << '\n';
        os << "shape: " << glm::to_string(volume.m_shape) << '\n';
        os << "min value: " << volume.m_min << '\n';
        os << "max value: " << volume.m_max << '\n';
        os << "voxel size: " << glm::to_string(volume.m_voxel_size);

        return os;
    }
};
