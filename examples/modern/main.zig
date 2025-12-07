const std = @import("std");

const fey = @import("fey");

const App = fey.app.App;

const components = fey.components;
const Camera = components.Camera;
const Transform2D = components.Transform2D;
const Sprite = components.Sprite;

const print = std.debug.print;

// API
// 1. Create an app.
// 2. Specify component types.
// 3. Specify renderers.
// 4. Specify systems.
// 5. Set up scenes
// 6. Start the application loop.

pub fn main() !void {
    const allocator = std.heap.c_allocator;

    const width = 800;
    const height = 600;
    var app = try App.init(allocator, "feyerverx", width, height);
    defer app.deinit();

    const main_scene = try app.add("Main", .{ .background_color = 0x363636ff });

    const camera_entity = try main_scene.registry.create();
    try main_scene.registry.add(camera_entity, Camera, Camera{
        .view_type = Camera.ViewType.ORTHOGONAL,
        .options = .{
            .view_rectangle = fey.geometry.Rectangle{
                .x = 0,
                .y = 0,
                .width = width,
                .height = height,
            },
        },
    });

    const textureHandle = try fey.assetLoader.AssetLoader.loadTexture("bean");
    for (0..10) |i| {
        const sprite_entity = try main_scene.registry.create();
        try main_scene.registry.add(sprite_entity, Transform2D, Transform2D{
            .rectangle = .{
                .x = @as(f32, @floatFromInt(i)) * 100,
                .y = 0,
                .width = 100,
                .height = 100,
            },
        });
        try main_scene.registry.add(sprite_entity, Sprite, Sprite{
            .texture_reference = .{
                .idx = textureHandle.idx,
            },
        });
    }

    try app.run();
}
