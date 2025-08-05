// Copyright 2025 Valentin Namleev

#pragma once

#include <Vector2.hpp>

#include "./imageInfo.hpp"
#include "./node.hpp"

namespace turnip {
class Image : public Node {
public:
    Image(const std::string &_ID, turnip::ImageInfo _ImageInfo, turnip::Size _Size, ::Color _Color)
        : turnip::Node(_ID, _Size, _Color) {
        m_ImageInfo = _ImageInfo;
    }

    static std::shared_ptr<Image>
    Create(const std::string &_ID, const std::shared_ptr<Node> &_Parent, ImageInfo _ImageInfo,
           turnip::Size _Size = turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL},
           ::Color _Color = {255, 255, 255, 255}) {
        auto image = std::make_shared<Image>(_ID, _ImageInfo, _Size, _Color);
        image->SetParent(_Parent);
        return image;
    }

    ~Image() override = default;
    const turnip::ImageInfo &ImageInfo() const { return m_ImageInfo; }

    void Render() override {
        Node::Render();

        NPatchInfo patchInfo =
            NPatchInfo{Rectangle{0, 0, static_cast<float>(m_ImageInfo.texture.width),
                                 static_cast<float>(m_ImageInfo.texture.height)},
                       static_cast<int>(m_ImageInfo.patch.left),
                       static_cast<int>(m_ImageInfo.patch.top),
                       static_cast<int>(m_ImageInfo.patch.right),
                       static_cast<int>(m_ImageInfo.patch.bottom),
                       NPATCH_NINE_PATCH};

        DrawTextureNPatch(m_ImageInfo.texture, patchInfo, WorldRect(), {0, 0}, 0, Color());
    }

private:
    turnip::ImageInfo m_ImageInfo;
};
} // namespace turnip
