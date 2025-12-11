const std = @import("std");

const fey = @import("fey");

const App = fey.app.App;

const AssetType = fey.asset_manager.AssetType;
const AssetReference = fey.asset_manager.AssetReference;
const TextureReference = fey.asset_manager.TextureReference;

const components = fey.ecs.components;
const Camera = components.Camera;
const Transform2D = components.Transform2D;
const Sprite = components.Sprite;

const print = std.debug.print;

// API
// 1. Create an app.
// 2. Specify component types.
// 3. Set up scenes
// 4. Specify renderers.
// 5. Specify systems.
// 6. Start the application loop.

const Position = struct { x: f32, y: f32 };
const Velocity = struct { dx: f32, dy: f32 };

const NamedComponentsTuple = fey.ecs.meta.NamedComponentsTuple;

fn movementSystem(view: NamedComponentsTuple(.{Transform2D})) void {
    view.transform2d.rectangle.y += 0.1;
}

pub fn main() !void {
    const allocator = std.heap.c_allocator;

    // 1. Create an app
    const width = 800;
    const height = 600;
    var app = try App.init(allocator, "feyerverx", width, height);
    defer app.deinit();

    const main_scene = try app.add("Main", .{ .background_color = 0x363636ff });
    const camera_entity = try main_scene.registry.create();

    // 3. Set up scenes
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

    const asset_reference = try app.asset_manager.loadAsset(fey.asset_manager.AssetType.TEXTURE, "bean");
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
            .texture_reference = asset_reference,
        });
    }

    // 5. Specify systems
    try main_scene.addSystem(.{Transform2D}, movementSystem);

    try app.run();
}
