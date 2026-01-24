const std = @import("std");
const fey = @import("fey");

const App = fey.app.App;

const AssetType = fey.asset_manager.AssetType;
const AssetReference = fey.asset_manager.AssetReference;
const AssetLoader = fey.asset_loader.AssetLoader;

const fbx = fey.asset_loader.fbx;

const components = fey.components;
const Camera = components.Camera;
const Transform2D = components.Transform2D;
const Sprite = components.Sprite;
const Mesh = components.Mesh;

const Event = fey.events.Event;
const Key = fey.events.Key;

const Vec3 = fey.geometry.zlm.Vec3;

const print = std.debug.print;

// API
// 1. Create an app.
// 2. Specify component types.
// 3. Set up scenes
// 4. Specify renderers.
// 5. Specify systems.
// 6. Start the application loop.

const ComponentsView = fey.ecs.meta.ComponentsView;

const MovementSystemData = struct { time: f32, dt: f32 };
fn movementSystem(view: ComponentsView(.{Transform2D}), dt: f32, data: *anyopaque) void {
    _ = view;
    _ = dt;
    _ = data;

    // const v = @sin(dt);
    // view.transform2d.rectangle.y += 2 * v;
}

pub fn main() !void {
    const allocator = std.heap.c_allocator;

    // 1. Create an app
    const width = 800;
    const height = 600;
    var app = try App.init(allocator, "feyerverx", width, height);
    defer app.deinit();

    // 3. Set up scenes
    const main_scene = try app.add("Main");

    const camera_entity = try main_scene.registry.create();

    try camera_entity.add(Camera, Camera{
        .view_type = Camera.ViewType.PERSPECTIVE,
        .options = .{
            .view_rectangle = fey.geometry.Rectangle{
                .x = 0,
                .y = 0,
                .width = width,
                .height = height,
            },
        },
        .position = Vec3{ .x = 0.0, .y = 0.0, .z = -20.0 },
        .rotation = Vec3{ .x = 0, .y = 0.7, .z = 0 },
    });

    const asset_reference = try app.asset_manager.loadAsset(fey.asset_manager.AssetType.TEXTURE, "hand");
    var i: f32 = 0.0;
    while (i < 10) {
        const sprite_entity = try main_scene.registry.create();
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

    // var cube_model = try AssetLoader.loadFBX("cube_tr");
    // // TODO: deinit freezes the application...
    // // defer cube_model.deinit();
    // const cube_entity = try main_scene.registry.create();
    // try cube_entity.add(Mesh, Mesh{
    //     .vertices = try cube_model.vertices(allocator),
    //     .indices = cube_model.triangles(),
    //     .uvs = try cube_model.uvs(allocator),
    // });
    // try cube_entity.add(Sprite, Sprite{ .texture_reference = asset_reference });

    // 5. Specify systems
    var data = MovementSystemData{ .dt = 0, .time = 0 };
    try main_scene.addSystem(.{Transform2D}, movementSystem, &data);

    const CameraMovementSystem = struct {
        pub fn run(view: ComponentsView(.{Camera}), dt: f32, d: *anyopaque) void {
            _ = d;
            std.debug.print("DT: {}\n", .{dt});
            view.camera.rotation.y += 0.001 * dt;
        }
    };

    try main_scene.addSystem(.{Camera}, &CameraMovementSystem.run, &data);

    _ = struct {
        pub fn move(scene: *fey.scenes.Scene, event: Event) void {
            var camera_views = scene.registry.view(.{Camera});
            while (camera_views.next()) |camera_view| {
                switch (event.key) {
                    Key.D => camera_view.camera.moveRelative(.{ .x = 10.0, .y = 0.0, .z = 0.0 }),
                    Key.A => camera_view.camera.moveRelative(.{ .x = -10.0, .y = 0.0, .z = 0.0 }),
                    Key.W => camera_view.camera.moveRelative(.{ .x = 0.0, .y = -10.0, .z = 0.0 }),
                    Key.S => camera_view.camera.moveRelative(.{ .x = 0.0, .y = 10.0, .z = 0.0 }),
                    Key.K => camera_view.camera.moveRelative(.{ .x = 0.0, .y = 0.0, .z = 10.0 }),
                    Key.L => camera_view.camera.moveRelative(.{ .x = 0.0, .y = 0.0, .z = -10.0 }),
                    Key.Z => camera_view.camera.rotation.y += 0.1,
                    Key.X => camera_view.camera.rotation.y -= 0.1,
                    Key.C => camera_view.camera.rotation.z += 0.1,
                    else => {},
                }
            }
        }
    };

    // try main_scene.addEventCallback(callback_wrapper.move);
    try app.run();
}

//////////////////////////

// const std = @import("std");
// const fey = @import("fey");

// const AssetLoader = fey.asset_loader.AssetLoader;

// pub fn main() !void {
//     const allocator = std.heap.c_allocator;

//     const model = try AssetLoader.loadFBX("Lowpoly_tree_sample");
//     const vs = model.children.get("Objects").?.children.get("Geometry").?.children.get("Vertices").?.properties.items[0];
//     var points: std.ArrayList(Vec3) = .empty;
//     for (vs.data.ArrayDouble, 0..) |v, i| {
//         const part = i % 3;
//         if (part == 0) {
//             try points.append(allocator, Vec3{ .x = v, .y = 0, .z = 0 });
//         }
//         if (part == 1) {
//             points.items[i / 3].y = v;
//         }
//         if (part == 2) {
//             points.items[i / 3].z = v;
//         }
//     }
//     for (points.items) |p| {
//         std.debug.print("{any}\n", .{p});
//     }
//     // std.debug.print("Model version: {}\n", .{model.version});
// }
