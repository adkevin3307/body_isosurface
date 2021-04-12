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

class Volume {
private:
    int m_byte_size;
    float m_min, m_max;
    std::string m_inf_file, m_raw_file;
    CONSTANT::TYPE m_type;
    CONSTANT::ENDIAN m_endian, m_machine_endian;
    glm::ivec3 m_shape;
    glm::vec3 m_voxel_size;
    std::vector<std::pair<float, glm::vec3>> m_data;

    template<typename T>
    T endian(int index, char* data)
    {
        T value;

        if (this->m_machine_endian != this->m_endian) {
            std::reverse(data + index, data + index + this->m_byte_size);
        }
        std::memcpy(&value, data + index, this->m_byte_size);

        return value;
    }

    void load_inf_file();
    void load_raw_file();

    glm::vec3 gradient(int x, int y, int z);
    void calculate_gradient();

public:
    Volume();
    Volume(std::string inf_file, std::string raw_file);
    ~Volume();

    std::pair<float, glm::vec3> const& operator()(int x, int y, int z) const;
    std::pair<float, glm::vec3>& operator()(int x, int y, int z);

    Volume(Volume const& rhs) = delete;
    Volume(Volume&& rhs) = delete;
    Volume& operator=(Volume const& rhs) = delete;
    Volume& operator=(Volume&& rhs) = delete;

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
        os << "voxel size: " << glm::to_string(volume.m_voxel_size) << '\n';

        return os;
    }
};
