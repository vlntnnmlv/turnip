const std = @import("std");

const fey = @import("fey");
const App = fey.app;

const print = std.debug.print;

// API
// 1. Create an app.
// 2. Specify component types.
// 3. Specify renderers.
// 4. Specify systems.
// 5. Set up scenes
// 6. Start the application loop.

pub fn main() !void {
    var app = try App.init("feyerverx", 800, 600);
    defer app.deinit();
    app.run();

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
