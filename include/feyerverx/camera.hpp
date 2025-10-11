// Copyright 2025 Valentin Namleev

#pragma once
#include <memory>

namespace feyerverx {
class ICamera {
public:
    virtual ~ICamera() = default;
    virtual void setView() const = 0;
    virtual void resizeView(float width, float height) = 0;
};

class CameraOrthogonal : public ICamera {
public:
    static std::unique_ptr<ICamera> createUniquePointer(float left, float right, float top,
                                                        float bottom, float near = 0.0f,
                                                        float far = 1000.0f);

    CameraOrthogonal(const CameraOrthogonal &other) = delete;
    CameraOrthogonal &operator=(const CameraOrthogonal &other) = delete;
    CameraOrthogonal(CameraOrthogonal &&other) = default;
    CameraOrthogonal &operator=(CameraOrthogonal &&other) = delete;

    ~CameraOrthogonal() override = default;

    void setView() const override;
    void resizeView(float width, float height) override;

private:
    CameraOrthogonal(float left, float right, float top, float bottom, float near = 0.0f,
                     float far = 1000.0f);

private:
    float m_left;
    float m_right;
    float m_top;
    float m_bottom;
    float m_near;
    float m_far;
};
} // namespace feyerverx
