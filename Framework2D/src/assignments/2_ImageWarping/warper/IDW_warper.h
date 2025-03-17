// HW2_TODO: Implement the IDWWarper class
#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include "warper.h"

namespace USTC_CG
{
class IDWWarper : public Warper
{
   public:
    IDWWarper() = default;

    virtual ~IDWWarper() = default;

    // HW2_TODO: Implement the warp(...) function with IDW interpolation
    Point2D warp(const Point2D& p) const override;

    // HW2_TODO: other functions or variables if you need
    IDWWarper(
        const std::vector<std::pair<Point2D, Point2D>>& control_points,
        float mu = 2.0f);

   private:
    std::vector<std::pair<Point2D, Point2D>> control_points_;
    std::vector<Eigen::Matrix2f> T_;
    float mu_;

    void precompute_transformations();
};
}  // namespace USTC_CG