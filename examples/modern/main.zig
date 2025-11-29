const std = @import("std");
const fey = @import("fey");

const print = std.debug.print;

// Three possible stacks:
//    |Window|  |Graphics|
// 1.  Raylib     Raylib    <-- For now, let's focus in the easiest one and desgin a common API
// 2.   SDL        bgfx
// 3   custom     custom

// So, what interface do we need?
// We need to be able to:
//  1. create a window with a @title, @width and @height
//  2. pull input events (key presses, mouse movement)
//  3. render stuff (textures, models, etc)

// API
// 1. Create a window
// 2. Specify additional components types if needed
// 3. Specify additional systems if needed
// 4. Start the application loop

pub fn main() !void {
    // const datas = [_]std.builtin.Type.EnumField{
    //     .{ .name = "D1", .value = 1 },
    //     .{ .name = "D2", .value = 2 },
    //     .{ .name = "D3", .value = 3 },
    //     .{ .name = "D4", .value = 4 },
    //     .{ .name = "D5", .value = 5 },
    //     .{ .name = "D6", .value = 6 },
    //     .{ .name = "D7", .value = 7 },
    //     .{ .name = "D8", .value = 8 },
    //     .{ .name = "D9", .value = 9 },
    //     .{ .name = "D10", .value = 10 },
    //     .{ .name = "D11", .value = 11 },
    //     .{ .name = "D12", .value = 12 },
    //     .{ .name = "D13", .value = 13 },
    // };

    // const DataEnum = @Type(.{ .@"enum" = .{
    //     .decls = &.{},
    //     .tag_type = u16,
    //     .fields = &datas,
    //     .is_exhaustive = true,
    // } });

    // const Key = enum {};
    // const d = DataEnum.D7;
    // print(":>{}\n", .{d});
    var window = try fey.window.create("feyerverx", 800, 600, .{ .backendType = fey.window.BackendType.SDL_BGFX });
    window.run();

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
