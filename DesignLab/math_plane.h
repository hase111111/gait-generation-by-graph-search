
//! @file math_plane.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_MATH_PLANE_H_
#define DESIGNLAB_MATH_PLANE_H_

#include <array>

#include "math_vector3.h"

namespace designlab {

//! @todo アドホックな実装なので、後でリファクタリングすること

struct PlaneRect final {
  std::array<Vector3, 4> corners;  //!< 時計回り or 反時計回りの4頂点
  Vector3 normal;                  //!< 法線ベクトル
};

struct Plane {
  Vector3 p1, p2, p3;

  // 法線ベクトル取得
  Vector3 GetNormal() const { return (p2 - p1).Cross(p3 - p1).GetNormalized(); }

  // 平面方程式のd項（ax + by + cz + d = 0）
  float GetD() const {
    auto normal = GetNormal();
    return -normal.Dot(p1);
  }

  // 点との距離
  float DistanceToPoint(const Vector3& point) const {
    auto normal = GetNormal();
    float d = GetD();
    return std::abs(normal.Dot(point) + d);
  }
};

// 点と平面との距離を計算
inline double pointToPlaneDistance(const Vector3& p, const Plane& plane) {
  const Vector3 normal =
      (plane.p2 - plane.p1).Cross(plane.p3 - plane.p1).GetNormalized();
  const double d = -normal.Dot(plane.p1);
  return std::abs(normal.Dot(p) + d);
}

inline Plane detectPlaneRANSAC(const std::vector<Vector3>& points,
                               int maxIterations, float distanceThreshold,
                               int minInliers) {
  Plane bestPlane{};
  int bestInlierCount = 0;

  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist(0,
                                          static_cast<int>(points.size()) - 1);

  for (int iter = 0; iter < maxIterations; ++iter) {
    // ランダムな3点選出
    auto p1 = points[dist(rng)];
    auto p2 = points[dist(rng)];
    auto p3 = points[dist(rng)];

    if ((p2 - p1).Cross(p3 - p1).GetLength() < 1e-6f)
      continue;  // 直線上 or 面積ゼロ

    Plane candidate{p1, p2, p3};

    int inliers = 0;
    for (const auto& pt : points) {
      if (candidate.DistanceToPoint(pt) < distanceThreshold) {
        ++inliers;
      }
    }

    if (inliers > bestInlierCount && inliers >= minInliers) {
      bestPlane = candidate;
      bestInlierCount = inliers;
    }
  }

  return bestPlane;
}

inline Vector3 powerIterationEigenVector(
    const std::array<std::array<float, 3>, 3>& cov, int iterations = 10) {
  Vector3 v{1, 1, 1};  // 初期ベクトル

  for (int i = 0; i < iterations; ++i) {
    Vector3 v_new;
    v_new.x = cov[0][0] * v.x + cov[0][1] * v.y + cov[0][2] * v.z;
    v_new.y = cov[1][0] * v.x + cov[1][1] * v.y + cov[1][2] * v.z;
    v_new.z = cov[2][0] * v.x + cov[2][1] * v.y + cov[2][2] * v.z;
    v = v_new.GetNormalized();
  }

  return v;
}

inline PlaneRect fitRectangleToInliers(
    const std::vector<designlab::Vector3>& inliers, designlab::Vector3 normal) {
  using namespace designlab;

  // 1. 法線を正規化（念のため）
  normal.Normalize();

  // 2. 法線に直交する2軸を定義
  const Vector3 dummy = std::abs(normal.Dot(Vector3(1, 0, 0))) < 0.9f
                            ? Vector3(1, 0, 0)
                            : Vector3(0, 1, 0);
  const Vector3 axis1 = normal.Cross(dummy).GetNormalized();  // 第1軸
  const Vector3 axis2 = normal.Cross(axis1).GetNormalized();  // 第2軸

  // 3. 重心を計算
  Vector3 centroid = Vector3::GetZeroVec();
  for (const auto& pt : inliers) {
    centroid += pt;
  }
  centroid /= static_cast<float>(inliers.size());

  // 4. 射影して最小矩形を定義（axis1, axis2上でmin/maxをとる）
  float min1 = FLT_MAX, max1 = -FLT_MAX;
  float min2 = FLT_MAX, max2 = -FLT_MAX;

  for (const auto& pt : inliers) {
    Vector3 d = pt - centroid;
    float u = d.Dot(axis1);
    float v = d.Dot(axis2);
    min1 = (std::min)(min1, u);
    max1 = (std::max)(max1, u);
    min2 = (std::min)(min2, v);
    max2 = (std::max)(max2, v);
  }

  // 5. 矩形の4頂点を構成
  PlaneRect rect;
  rect.normal = normal;
  rect.corners[0] = centroid + min1 * axis1 + min2 * axis2;
  rect.corners[1] = centroid + max1 * axis1 + min2 * axis2;
  rect.corners[2] = centroid + max1 * axis1 + max2 * axis2;
  rect.corners[3] = centroid + min1 * axis1 + max2 * axis2;

  return rect;
}

inline std::vector<PlaneRect> detectMultiplePlanes(
    std::vector<Vector3> points, int maxPlanes = 5, int maxIterations = 1000,
    float distanceThreshold = 0.01f, int minInliers = 100) {
  std::vector<PlaneRect> detectedPlanes;

  for (int i = 0;
       i < maxPlanes && points.size() >= static_cast<size_t>(minInliers); ++i) {
    // 1平面検出
    Plane bestPlane =
        detectPlaneRANSAC(points, maxIterations, distanceThreshold, minInliers);

    // インライア抽出
    std::vector<Vector3> inliers;
    std::vector<Vector3> remaining;

    for (const auto& pt : points) {
      if (pointToPlaneDistance(pt, bestPlane) < distanceThreshold) {
        inliers.push_back(pt);
      } else {
        remaining.push_back(pt);
      }
    }

    if (inliers.size() < static_cast<size_t>(minInliers)) {
      break;  // もう十分な平面が見つからない
    }

    // PCA + 矩形フィッティング
    PlaneRect fitted = fitRectangleToInliers(inliers, bestPlane.GetNormal());
    detectedPlanes.push_back(fitted);

    // 残り点群で再実行
    points = std::move(remaining);
  }

  return detectedPlanes;
}

}  // namespace designlab

#endif  // DESIGNLAB_MATH_PLANE_H_
