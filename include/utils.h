#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <imgui/imgui.h>

#include "Timer.h"
#include "Volume.h"
#include "IsoSurface.h"
#include "MeshManagement.h"

namespace UTILS {
    inline void load(std::string filename)
    {
        if (MeshManagement::exist(filename)) {
            std::cout << "==================================================" << '\n';
            std::cout << "already exist" << '\n';
            std::cout << "==================================================" << '\n';

            return;
        }

        Timer::begin();
        Volume volume("Data" / std::filesystem::path(filename + ".inf"), "Data" / std::filesystem::path(filename + ".raw"));
        Timer::end();

        std::cout << "==================================================" << '\n';
        std::cout << volume << '\n';
        std::cout << "time: " << Timer::time() << '\n';
        std::cout << "==================================================" << '\n';

        MeshManagement::add(filename, volume);
    }

    inline void show(std::string filename, float iso_value)
    {
        if (!MeshManagement::exist(filename)) {
            load(filename);
        }

        IsoSurface iso_surface(MeshManagement::get(filename));
        iso_surface.iso_value() = iso_value;

        Timer::begin();
        iso_surface.run();
        Timer::end();

        std::cout << "==================================================" << '\n';
        std::cout << iso_surface << '\n';
        std::cout << "time: " << Timer::time() << '\n';
        std::cout << "==================================================" << '\n';

        glm::vec3 shape = MeshManagement::get(filename).shape();
        glm::vec3 voxel_size = MeshManagement::get(filename).voxel_size();

        shape = shape * voxel_size;

        std::vector<float> vertices = iso_surface.vertices();
        std::vector<float> normals = iso_surface.normals();

        assert(vertices.size() == normals.size());

        std::vector<GLfloat> data;
        data.reserve(vertices.size() + normals.size());
        for (size_t i = 0; i < vertices.size() && i < normals.size(); i += 3) {
            data.push_back(vertices[i]);
            data.push_back(vertices[i + 1]);
            data.push_back(vertices[i + 2]);

            data.push_back(normals[i]);
            data.push_back(normals[i + 1]);
            data.push_back(normals[i + 2]);
        }

        Buffer buffer = BufferManagement::generate();

        BufferManagement::bind(buffer);
        BufferManagement::fill(data);
        BufferManagement::set(0, 3, 6, 0);
        BufferManagement::set(1, 3, 6, 3 * sizeof(GLfloat));
        BufferManagement::unbind();

        MeshManagement::add(buffer, shape, data.size());
    }

    inline void GUI()
    {
        static std::vector<std::string> datasets;

        if (datasets.size() == 0) {
            for (auto entry : std::filesystem::directory_iterator("Data")) {
                auto it = entry.path().filename().string().find(".inf");

                if (it != std::string::npos) {
                    std::string basename = entry.path().stem();

                    if (std::filesystem::exists("Data" / std::filesystem::path(basename + ".raw"))) {
                        datasets.push_back(basename);
                    }
                }
            }

            sort(datasets.begin(), datasets.end());
        }

        static float iso_value = 0.0;
        static std::string select_data = datasets.front();

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(CONSTANT::WIDTH / 3.0, CONSTANT::HEIGHT / 3.0), ImGuiCond_Once);

        ImGui::Begin("Control");
        ImGui::SetWindowFontScale(1.2);

        if (ImGui::BeginCombo("## Data", select_data.c_str())) {
            for (auto data : datasets) {
                bool selected = (select_data == data);

                if (ImGui::Selectable(data.c_str(), selected)) select_data = data;
                if (selected) ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::SameLine();
        if (ImGui::Button("Load")) {
            load(select_data);
            iso_value = MeshManagement::get(select_data).average();
        }

        if (MeshManagement::exist(select_data)) {
            ImGui::SliderFloat("Iso Value", &iso_value, MeshManagement::get(select_data).min_value(), MeshManagement::get(select_data).max_value());

            if (ImGui::Button("Show")) {
                show(select_data, iso_value);
            }

            ImGui::SameLine();
            if (ImGui::Button("Clean")) {
                MeshManagement::clear();
            }
        }

        ImGui::End();
    }
}; // namespace UTILS
