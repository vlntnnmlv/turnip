const std = @import("std");

const Registry = @import("ecs.zig").Registry;
const ComponentsViewIterator = @import("components_meta.zig").ComponentsViewIterator;

pub fn System(comptime ComponentTypes: anytype) type {
    return struct {
        const Self = @This();
        pub const SystemComponentsViewIterator = ComponentsViewIterator(ComponentTypes);

        registry: *const Registry,

        pub fn init(registry: *const Registry) Self {
            return Self{ .registry = registry };
        }

        pub fn run(self: *Self, comptime process: anytype) void {
            var components = self.registry.view(ComponentTypes);
            defer components.deinit();

            while (components.next()) |view| {
                process(view);
            }
        }
    };
}
