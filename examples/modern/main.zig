const std = @import("std");
const fey = @import("fey");

const print = std.debug.print;

// API
// 1. Create an app
// 2. Specify additional components types if needed
// 3. Specify additional systems if needed
// 4. Start the application loop

pub fn main() !void {
    var app = try fey.app.create("feyerverx", 800, 600, .{ .background_color = 0x696969ff });
    defer app.dispose();
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
