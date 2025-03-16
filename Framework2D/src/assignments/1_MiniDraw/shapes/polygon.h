#pragma once

#include <vector>

#include "shape.h"

namespace USTC_CG
{
class Polygon : public Shape
{
   public:
    Polygon() = default;

    Polygon(std::vector<float> x_list, std::vector<float> y_list);

    virtual ~Polygon() = default;

    void draw(const Config& config) const override;

    void update(float x, float y) override;

    void add_control_point(float x, float y) override;

    float get_first_point_x() const
    {
        return x_list_[0];
    }

    float get_first_point_y() const
    {
        return y_list_[0];
    }

   private:
    std::vector<float> x_list_, y_list_;
};
}  // namespace USTC_CG