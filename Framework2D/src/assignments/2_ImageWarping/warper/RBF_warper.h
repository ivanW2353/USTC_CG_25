// HW2_TODO: Implement the RBFWarper class
#pragma once

#include "warper.h"

namespace USTC_CG
{
class RBFWarper : public Warper
{
   public:
    RBFWarper() = default;
    virtual ~RBFWarper() = default;

    // HW2_TODO: Implement the warp(...) function with RBF interpolation
    Point2D warp(const Point2D& p) const override;

    // HW2_TODO: other functions or variables if you need
    RBFWarper(const std::vector<std::pair<Point2D, Point2D>>& control_points);

   private:
    
    void compute_affine_transform();
    void compute_radial_weights();

    std::vector<std::pair<Point2D, Point2D>> control_points_;
    Eigen::Matrix2f A_;        // ����任����
    Eigen::Vector2f b_;        // ƽ������
    Eigen::VectorXf alpha_x_;  // x������Ȩ��
    Eigen::VectorXf alpha_y_;  // y������Ȩ��
    std::vector<float> r_;     // ÿ�����Ƶ�ľֲ��뾶
};
}  // namespace USTC_CG