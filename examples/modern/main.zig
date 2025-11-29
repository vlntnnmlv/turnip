const std = @import("std");
const fey = @import("fey");

const print = std.debug.print;

// Three possible stacks:
//    |Window|  |Graphics|
// 1.  Raylib     Raylib    <-- For now, let's focus in the easiest one and desgin a common API
// 2.   SDL        bgfx
// 3   custom     custom

// API
// 1. Create a window
// 2. Specify additional components types if needed
// 3. Specify additional systems if needed
// 4. Start the application loop

fn ftest(i: i32) !void {
    if (i < 0) return error.Boom;

    std.debug.print("Hi!\n", .{});
}
pub fn main() !void {
    var window = try fey.window.create("feyerverx", 800, 600, .{ .backendType = fey.backend.BackendType.SDL_BGFX });
    window.run();

    // const FType = fn (i32) void;
    // const f: FType = ftest;
    // f();

    // var registry = fey.ecs.Registry.create();
    // defer registry.deinit();

    // const entitiesCount = 10;
    // const componentsCount = 10;

    // for (0..entitiesCount) |_| {
    //     const e = try registry.createEntity();
    //     for (0..componentsCount) |_| {
    //         try registry.addComponent(e, fey.ecs.Position, fey.ecs.Position{ .x = 4, .y = 100 });
    //         try registry.addComponent(e, fey.ecs.Character, fey.ecs.Character{ .id = 324 });
    //         try registry.addComponent(e, fey.ecs.Boid, fey.ecs.Boid{ .k = 0.23453245 });
    //     }
    // }

    // try fey.loop.run(&registry);
}
