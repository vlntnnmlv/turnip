// // Copyright 2025 Valentin Namleev
//
// #pragma once
//
// #include <bx/math.h>
// #include <memory>
//
// #include "math/vector.hpp"
//
// #include "identifiable.hpp"
// #include "rectangleOffset.hpp"
//
// namespace feyerverx {
// class ICamera : public IIdentifiable {
// public:
//     explicit ICamera(const std::string &id, RectangleOffset viewport, float near, float far);
//     virtual ~ICamera() = default;
//     virtual void setView(uint16_t viewID) const = 0;
//
//     [[nodiscard]] RectangleOffset viewport() const noexcept;
//     [[nodiscard]] float near() const noexcept;
//     [[nodiscard]] float far() const noexcept;
//     void updateViewport(RectangleOffset viewportScaled, Vector2f windowSize);
//
// private:
//     RectangleOffset m_viewportScaled;
//     RectangleOffset m_viewport;
//     float m_near;
//     float m_far;
// };
//
// class CameraOrthogonal final : public ICamera {
// public:
//     static std::unique_ptr<ICamera> create(RectangleOffset viewport, float near = 0.0f,
//                                                         float far = 1000.0f);
//
//     CameraOrthogonal(const CameraOrthogonal &other) = delete;
//     CameraOrthogonal &operator=(const CameraOrthogonal &other) = delete;
//     CameraOrthogonal(CameraOrthogonal &&other) = default;
//     CameraOrthogonal &operator=(CameraOrthogonal &&other) = delete;
//
//     ~CameraOrthogonal() override = default;
//
//     void setView(uint16_t viewID) const override;
//
// private:
//     CameraOrthogonal(RectangleOffset viewport, float near = 0.0f, float far = 1000.0f);
// };
//
// class CameraPerspective final : public ICamera {
// public:
//     static std::unique_ptr<ICamera> create(RectangleOffset viewport, Vector3f position,
//                                                         float FOV = 120.0f, float near = 0.0f,
//                                                         float far = 1000.0f);
//
//     CameraPerspective(const CameraPerspective &other) = delete;
//     CameraPerspective &operator=(const CameraPerspective &other) = delete;
//     CameraPerspective(CameraPerspective &&other) = default;
//     CameraPerspective &operator=(CameraPerspective &&other) = delete;
//
//     ~CameraPerspective() override = default;
//
//     void setView(uint16_t viewID) const override;
//
// private:
//     CameraPerspective(RectangleOffset viewport, Vector3f position, float FOV, float near = 0.0f,
//                       float far = 1000.0f);
//
//     void updateVectors() {
//         static bx::Vec3 forward{0.0f, 0.0f, -1.0f};
//         static bx::Vec3 upWorld{0.0f, -1.0f, 0.0f};
//
//         const bx::Vec3 eye{m_eye.x, m_eye.y, m_eye.z};
//
//         const bx::Quaternion rotation =
//             bx::fromEuler(bx::Vec3{m_rotation.x, m_rotation.y, m_rotation.z});
//
//         const bx::Vec3 lookingAt = bx::add(eye, bx::mul(forward, rotation));
//         m_lookingAt = Vector3f{lookingAt.x, lookingAt.y, lookingAt.z};
//
//         const bx::Vec3 up = bx::mul(upWorld, rotation);
//         m_up = Vector3f{up.x, up.y, up.z};
//     }
//
//     float m_FOV;
//
//     Vector3f m_rotation{};
//     Vector3f m_eye{};
//     Vector3f m_lookingAt{};
//     Vector3f m_up{};
// };
// } // namespace feyerverx
