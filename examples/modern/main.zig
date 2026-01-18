const std = @import("std");

const fey = @import("fey");

const App = fey.app.App;

const AssetType = fey.asset_manager.AssetType;
const AssetReference = fey.asset_manager.AssetReference;
const AssetLoader = fey.asset_loader.AssetLoader;

const fbx = fey.asset_loader.fbx;

const components = fey.ecs.components;
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

fn movementSystem(view: ComponentsView(.{Transform2D})) void {
    view.transform2d.rectangle.y += 0.1;
}

pub fn main() !void {
    const allocator = std.heap.c_allocator;

    // 1. Create an app
    const width = 800;
    const height = 600;
    var app = try App.init(allocator, "feyerverx", width, height);
    defer app.deinit();

    // 3. Set up scenes
    const main_scene = try app.add("Main", .{ .background_color = 0x363636ff });
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
        .position = Vec3{ .x = 0.0, .y = 0.0, .z = -1.0 },
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

    // var cube_model = try AssetLoader.loadFBX("Lowpoly_tree_sample");
    // defer cube_model.deinit();
    // const cube_entity = try main_scene.registry.create();
    // try cube_entity.add(Mesh, Mesh{
    //     .vertices = try cube_model.vertices(allocator),
    //     .indices = cube_model.triangles(),
    //     .uvs = try cube_model.uvs(allocator),
    // });

    // 5. Specify systems
    // try main_scene.addSystem(.{Transform2D}, movementSystem);

    const callback_wrapper = struct {
        pub fn move(scene: *fey.scenes.Scene, event: Event) void {
            var camera_views = scene.registry.view(.{Camera});
            while (camera_views.next()) |camera_view| {
                if (event.key == Key.D) {
                    // camera_view.camera.position.x += 1.0;
                    camera_view.camera.moveRelative(.{ .x = 10.0, .y = 0.0, .z = 0.0 });
                }
                if (event.key == Key.A) {
                    // camera_view.camera.position.x -= 10.0;
                    camera_view.camera.moveRelative(.{ .x = -10.0, .y = 0.0, .z = 0.0 });
                }
                if (event.key == Key.W) {
                    // camera_view.camera.position.y -= 10.0;
                    camera_view.camera.moveRelative(.{ .x = 0.0, .y = -10.0, .z = 0.0 });
                }
                if (event.key == Key.S) {
                    // camera_view.camera.position.y += 10.0;
                    camera_view.camera.moveRelative(.{ .x = 0.0, .y = 10.0, .z = 0.0 });
                }

                // TODO: Clean up camera component and controlls
                if (event.key == Key.K) {
                    // camera_view.camera.position.y += 10.0;
                    camera_view.camera.moveRelative(.{ .x = 0.0, .y = 0.0, .z = 1.0 });
                }
                if (event.key == Key.L) {
                    // camera_view.camera.position.y += 10.0;
                    camera_view.camera.moveRelative(.{ .x = 0.0, .y = 0.0, .z = -1.0 });
                }

                if (event.key == Key.Z) {
                    camera_view.camera.rotation.y += 0.1;
                }
                if (event.key == Key.X) {
                    camera_view.camera.rotation.y -= 0.1;
                }
                if (event.key == Key.C) {
                    camera_view.camera.rotation.z += 0.1;
                }
            }
        }
    };
    try main_scene.addEventCallback(callback_wrapper.move);
    try app.run();
}

// add system ( .{component types}, process(components, context), context);

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
