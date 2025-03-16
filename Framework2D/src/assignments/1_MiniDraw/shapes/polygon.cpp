#include "polygon.h"

#include <imgui.h>

namespace USTC_CG
{
Polygon::Polygon(std::vector<float> x_list, std::vector<float> y_list)
    : x_list_(x_list),
      y_list_(y_list)
{
}

void Polygon::draw(const Config& config) const
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // Draw the polygon
    for (size_t i = 0; i < x_list_.size() - 1; i++)
    {
        draw_list->AddLine(
            ImVec2(config.bias[0] + x_list_[i], config.bias[1] + y_list_[i]),
            ImVec2(
                config.bias[0] + x_list_[(i + 1) % x_list_.size()],
                config.bias[1] + y_list_[(i + 1) % y_list_.size()]),
            IM_COL32(
                config.line_color[0],
                config.line_color[1],
                config.line_color[2],
                config.line_color[3]),
            config.line_thickness);
    }
}

void Polygon::update(float x, float y)
{
    x_list_.back() = x;
    y_list_.back() = y;
}

void Polygon::add_control_point(float x, float y)
{
    x_list_.push_back(x);
    y_list_.push_back(y);
}
}  // namespace USTC_CG

