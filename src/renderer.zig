const std = @import("std");
const geometry = @import("geometry.zig");

const Texture = struct {};
const RenderTextureT = fn (rectangle: geometry.Rectangle) void;

const Renderer = struct {
    renderTexture: RenderTextureT,

    pub fn create(renderTexture: RenderTextureT) Renderer {
        return Renderer{
            .renderTexture = renderTexture,
        };
    }
};

const RendererRaylib = Renderer.create();
