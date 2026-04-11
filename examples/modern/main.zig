const std = @import("std");
const fey = @import("fey");

const App = fey.app.App;

const AssetType = fey.asset_manager.AssetType;
const Camera = fey.components.Camera;
const Transform2D = fey.components.Transform2D;
const Sprite = fey.components.Sprite;
const Mesh = fey.components.Mesh;

const OrthoCameraController = fey.systems.OrthoCameraController;

const AssetLoader = fey.asset_loader.AssetLoader;
const Vec3 = fey.geometry.zlm.Vec3;

// TODO: Discuss general workflow
// ecs may be too much boilerplate sometimes...
// how to mix ecs with global state/smth else?
pub fn main() !void {
    const allocator = std.heap.c_allocator;

    // 1. Create an app
    const width = 800;
    const height = 600;
    var app = try App.init(allocator, "feyerverx", width, height);
    defer app.deinit();

    // 3. Set up worlds
    // TODO: In future add Scene as a wrapper around the World,
    // which will add utilities like set default bg color, set time scale, etc
    const main_world = try app.add("Main");

    const camera_entity = try main_world.registry.create();
    try camera_entity.add(Camera, Camera{
        .view_type = Camera.ViewType.ORTHOGONAL,
        .options = .{
            .view_rectangle = fey.geometry.Rectangle{
                .x = 0,
                .y = 0,
                .width = width,
                .height = height,
            },
        },
        .position = Vec3{ .x = 0.0, .y = 2.0, .z = -5.0 },
        .rotation = Vec3{ .x = 0, .y = 0, .z = 0 },
    });

    const asset_reference = try app.asset_manager.loadAsset(fey.asset_manager.AssetType.TEXTURE, "hive");
    const grass_reference = try app.asset_manager.loadAsset(fey.asset_manager.AssetType.TEXTURE, "grass");
    var i: f32 = 0.0;
    while (i < 10) {
        const sprite_entity = try main_world.registry.create();
        try sprite_entity.add(Transform2D, Transform2D{
            .rectangle = .{
                .x = i * 100,
                .y = 0,
                .width = 100,
                .height = 100,
            },
        });
        try sprite_entity.add(Sprite, Sprite{ .texture_reference = asset_reference });

        i += 1.0;
    }
    i = 0;
    var j: f32 = 0.0;

    while (i < 100) {
        j = 0;
        while (j < 100) {
            const sprite_entity = try main_world.registry.create();
            try sprite_entity.add(Transform2D, Transform2D{
                .rectangle = .{
                    .x = i * 10,
                    .y = j * 10,
                    .width = 25,
                    .height = 25,
                },
            });
            try sprite_entity.add(Sprite, Sprite{ .texture_reference = grass_reference });
            j += 1.0;
        }
        i += 1.0;
    }
    // 5. Load a 3D model
    const fbx_file = try AssetLoader.loadFBX(allocator, "cube_tr");
    const cube_mesh = try Mesh.fromFBX(allocator, fbx_file);
    const cube_entity = try main_world.registry.create();
    try cube_entity.add(Mesh, cube_mesh);
    try cube_entity.add(Sprite, Sprite{ .texture_reference = asset_reference });

    // 6. Camera controller (WASD + mouse look)
    var camera_controller = OrthoCameraController.init(&app.input, width, height);
    try main_world.addSystem(.{Camera}, &OrthoCameraController.system, camera_controller.context());

    try app.run();
}
