#include <limits>

#include "RBF_warper.h"

namespace USTC_CG
{
RBFWarper::RBFWarper(
    const std::vector<std::pair<Point2D, Point2D>>& control_points)
    : control_points_(control_points)
{
    // Step 1: 计算每个控制点的局部半径 r_i
    const size_t n = control_points_.size();
    r_.resize(n);
    for (size_t i = 0; i < n; ++i)
    {
        float min_dist = std::numeric_limits<float>::max();
        for (size_t j = 0; j < n; ++j)
        {
            if (i == j)
                continue;
            float dist =
                (control_points_[i].first - control_points_[j].first).norm();
            if (dist < min_dist)
                min_dist = dist;
        }
        r_[i] = min_dist;
    }

    // Step 2: 计算仿射变换
    compute_affine_transform();

    // Step 3: 计算径向基函数权重
    compute_radial_weights();
}

void RBFWarper::compute_affine_transform()
{
    const size_t n = control_points_.size();
    Eigen::MatrixXf M(n, 3);
    Eigen::VectorXf q_x(n), q_y(n);

    for (size_t i = 0; i < n; ++i)
    {
        const auto& p = control_points_[i].first;
        const auto& q = control_points_[i].second;
        M(i, 0) = p.x();
        M(i, 1) = p.y();
        M(i, 2) = 1.0f;
        q_x(i) = q.x();
        q_y(i) = q.y();
    }

    // 最小二乘解仿射参数
    Eigen::Vector3f aff_x =
        M.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(q_x);
    Eigen::Vector3f aff_y =
        M.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(q_y);

    A_ << aff_x[0], aff_x[1], aff_y[0], aff_y[1];
    b_ << aff_x[2], aff_y[2];
}

void RBFWarper::compute_radial_weights()
{
    const size_t n = control_points_.size();
    Eigen::MatrixXf G(n, n);

    // 构建基函数矩阵 G
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            float d =
                (control_points_[i].first - control_points_[j].first).norm();
            G(i, j) = 1.0f / std::sqrt(d * d + r_[j] * r_[j]);
        }
    }

    // 计算残差
    Eigen::VectorXf r_x(n), r_y(n);
    for (size_t i = 0; i < n; ++i)
    {
        const auto& p = control_points_[i].first;
        const auto& q = control_points_[i].second;
        Eigen::Vector2f q_pred = A_ * p + b_;
        r_x(i) = q.x() - q_pred.x();
        r_y(i) = q.y() - q_pred.y();
    }

    // 解线性方程组
    alpha_x_ = G.colPivHouseholderQr().solve(r_x);
    alpha_y_ = G.colPivHouseholderQr().solve(r_y);
}

Point2D RBFWarper::warp(const Point2D& p) const
{
    // 仿射部分
    Eigen::Vector2f result = A_ * p + b_;

    // 径向基部分
    for (size_t j = 0; j < control_points_.size(); ++j)
    {
        float d = (p - control_points_[j].first).norm();
        float g_val = 1.0f / std::sqrt(d * d + r_[j] * r_[j]);
        result.x() += alpha_x_(j) * g_val;
        result.y() += alpha_y_(j) * g_val;
    }

    return result;
}
}  // namespace USTC_CG