#include "IDW_warper.h"

namespace USTC_CG
{
IDWWarper::IDWWarper(
    const std::vector<std::pair<Point2D, Point2D>>& control_points,
    float mu,
    float epsilon)
    : mu_(mu),
      epsilon_(epsilon)
{
    // Step 1: ȥ�ؿ��Ƶ㣨�����������꾫ȷƥ�䣩
    control_points_ = control_points;
    auto unique_compare = [](const auto& a, const auto& b)
    { return a.first.x() == b.first.x() && a.first.y() == b.first.y(); };
    std::sort(
        control_points_.begin(),
        control_points_.end(),
        [](const auto& a, const auto& b) { return a.first.x() < b.first.x(); });
    auto last = std::unique(
        control_points_.begin(), control_points_.end(), unique_compare);
    control_points_.erase(last, control_points_.end());

    // Step 2: Ԥ����任����
    precompute_transformations();
}

void IDWWarper::precompute_transformations()
{
    const size_t n = control_points_.size();
    T_.resize(n);

    for (size_t i = 0; i < n; ++i)
    {
        const auto& p_i = control_points_[i].first;
        const auto& q_i = control_points_[i].second;

        Eigen::Matrix2f A = Eigen::Matrix2f::Zero();
        Eigen::Matrix2f B = Eigen::Matrix2f::Zero();

        for (size_t j = 0; j < n; ++j)
        {
            if (j == i)
                continue;

            const auto& p_j = control_points_[j].first;
            const Eigen::Vector2f dp = p_j - p_i;
            const float distance = dp.norm();
            if (distance < 1e-7f)
                continue;  // �����ظ��㣨��ȥ�أ�

            const float sigma = 1.0f / std::pow(distance, mu_);
            A += sigma * dp * dp.transpose();

            const Eigen::Vector2f dq = control_points_[j].second - q_i;
            B += sigma * dq * dp.transpose();
        }

        // ���򻯾���
        A += epsilon_ * Eigen::Matrix2f::Identity();

        // ��� T_i = B * A^{-1}
        T_[i] = B * A.inverse();
    }
}

Point2D IDWWarper::warp(const Point2D& p) const
{
    // Step 1: �������꾫ȷƥ��
    for (const auto& cp : control_points_)
    {
        if (p.x() == cp.first.x() && p.y() == cp.first.y())
        {
            return cp.second;
        }
    }

    // Step 2: ����Ȩ��
    std::vector<float> weights;
    float sum_weights = 0.0f;

    for (const auto& cp : control_points_)
    {
        const Eigen::Vector2f dp = p - cp.first;
        const float distance = dp.norm() + 1e-7f;  // ��ֹ����0
        const float sigma = 1.0f / std::pow(distance, mu_);
        weights.push_back(sigma);
        sum_weights += sigma;
    }

    // Step 3: ��һ��Ȩ��
    if (sum_weights < 1e-7f)
    {  // �������о����������
        return Point2D(0, 0);
    }
    for (auto& w : weights)
    {
        w /= sum_weights;
    }

    // Step 4: ��Ȩ��ֵ
    Point2D result(0, 0);
    for (size_t i = 0; i < control_points_.size(); ++i)
    {
        const Eigen::Vector2f dp = p - control_points_[i].first;
        const Eigen::Vector2f f_i = control_points_[i].second + T_[i] * dp;
        result += weights[i] * f_i;
    }

    return result;
}
}  // namespace USTC_CG