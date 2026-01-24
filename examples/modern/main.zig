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
const EventType = fey.events.EventType;
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

    // 3. Set up worlds
    // TODO: In future add Scene as a wrapper around the World,
    // which will add utilities like set default bg color, set time scale, etc
    const main_world = try app.add("Main");

    const camera_entity = try main_world.registry.create();
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
        .position = Vec3{ .x = 0.0, .y = 0.0, .z = -50.0 },
        .rotation = Vec3{ .x = 0, .y = 0.7, .z = 0 },
    });

    const asset_reference = try app.asset_manager.loadAsset(fey.asset_manager.AssetType.TEXTURE, "hand");
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

    var cube_model = try AssetLoader.loadFBX("cube_tr");
    // TODO: deinit freezes the application...probably because triangles data is now owned by bgfx and FBX struct
    // defer cube_model.deinit();
    const cube_entity = try main_world.registry.create();
    try cube_entity.add(Mesh, Mesh{
        .vertices = try cube_model.vertices(allocator),
        .indices = cube_model.triangles(),
        .uvs = try cube_model.uvs(allocator),
    });
    try cube_entity.add(Sprite, Sprite{ .texture_reference = asset_reference });

    // 5. Specify systems

    // Camera movement system
    // var data = {};
    // const CameraMovementSystem = struct {
    //     pub fn run(view: ComponentsView(.{Camera}), dt: f32, d: *anyopaque) void {
    //         _ = d;
    //         view.camera.rotation.y += 0.001 * dt;
    //     }
    // };
    // try main_world.addSystem(.{Camera}, &CameraMovementSystem.run, &data);

    // Camera control system
    const ControlInfo = struct {
        w: bool = false,
        a: bool = false,
        s: bool = false,
        d: bool = false,
        k: bool = false,
        l: bool = false,
    };

    const CameraControlSystemContext = struct {
        events_q: *std.ArrayList(Event),
        control_info: *ControlInfo,
    };

    const CameraControlSystem = struct {
        pub fn run(view: ComponentsView(.{Camera}), dt: f32, d: *anyopaque) void {
            const context: *CameraControlSystemContext = @ptrCast(@alignCast(d));
            const move_speed = 0.5 * dt;
            const rotation_speed = 0.001 * dt;

            while (context.events_q.pop()) |event| {
                std.debug.print("event: {}\n", .{event});
                const pressed = event.eventType == EventType.KeyPressed;
                if (pressed) {
                    switch (event.key) {
                        Key.W => context.control_info.w = true,
                        Key.A => context.control_info.a = true,
                        Key.S => context.control_info.s = true,
                        Key.D => context.control_info.d = true,
                        Key.K => context.control_info.k = true,
                        Key.L => context.control_info.l = true,
                        else => {},
                    }
                }
                const released = event.eventType == EventType.KeyReleased;
                if (released) {
                    switch (event.key) {
                        Key.W => context.control_info.w = false,
                        Key.A => context.control_info.a = false,
                        Key.S => context.control_info.s = false,
                        Key.D => context.control_info.d = false,
                        Key.K => context.control_info.k = false,
                        Key.L => context.control_info.l = false,
                        else => {},
                    }
                }

                // switch (event.key) {
                //     Key.W => context.control_info.w = event.eventType == EventType.KeyPressed, //view.camera.moveRelative(.{ .x = 0.0, .y = -move_speed, .z = 0.0 }),
                //     Key.A => context.control_info.a = event.eventType == EventType.KeyPressed, //view.camera.moveRelative(.{ .x = -move_speed, .y = 0.0, .z = 0.0 }),
                //     Key.S => context.control_info.s = event.eventType == EventType.KeyPressed, //view.camera.moveRelative(.{ .x = 0.0, .y = move_speed, .z = 0.0 }),
                //     Key.D => context.control_info.d = event.eventType == EventType.KeyPressed, //view.camera.moveRelative(.{ .x = move_speed, .y = 0.0, .z = 0.0 }),
                //     // Key.K => view.camera.moveRelative(.{ .x = 0.0, .y = 0.0, .z = move_speed }),
                //     // Key.L => view.camera.moveRelative(.{ .x = 0.0, .y = 0.0, .z = -move_speed }),
                //     // Key.Z => view.camera.rotation.y += rotation_speed,
                //     // Key.X => view.camera.rotation.y -= rotation_speed,
                //     // Key.C => view.camera.rotation.z += rotation_speed,
                //     else => {},
                // }
            }

            if (context.control_info.w) {
                view.camera.moveRelative(.{ .x = 0.0, .y = 0.0, .z = move_speed });
            }

            if (context.control_info.a) {
                view.camera.moveRelative(.{ .x = -move_speed, .y = 0.0, .z = 0.0 });
            }

            if (context.control_info.s) {
                view.camera.moveRelative(.{ .x = 0.0, .y = 0.0, .z = -move_speed });
            }

            if (context.control_info.d) {
                view.camera.moveRelative(.{ .x = move_speed, .y = 0.0, .z = 0.0 });
            }

            if (context.control_info.k) {
                view.camera.rotate(0, rotation_speed);
            }

            if (context.control_info.l) {
                view.camera.rotate(0, -rotation_speed);
            }
        }
    };

    var control_info = ControlInfo{};
    var context = CameraControlSystemContext{ .events_q = &app.events_q, .control_info = &control_info };
    try main_world.addSystem(.{Camera}, &CameraControlSystem.run, &context);

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
