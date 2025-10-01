// Copyright 2025 Valentin Namleev

#pragma once

namespace feyerverx {
class ICamera {
public:
    virtual ~ICamera() = default;
    virtual void setView() const = 0;
    virtual void resizeView(float width, float height) = 0;
};

class CameraOrthogonal : public ICamera {
public:
    virtual ~CameraOrthogonal() override = default;
    CameraOrthogonal(float left, float right, float top, float bottom, float near = 0.0f,
                     float far = 1000.0f);

public:
    virtual void setView() const override;
    virtual void resizeView(float width, float height) override;

private:
    float m_left;
    float m_right;
    float m_top;
    float m_bottom;
    float m_near;
    float m_far;
};
} // namespace feyerverx
