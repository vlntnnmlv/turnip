const std = @import("std");

pub const System = struct {
    component_types: std.ArrayList(type),
    allocator: std.mem.Allocator,

    pub fn init(comptime allocator: std.mem.Allocator, comptime ComponentTypes: anytype) System {
        return System{
            .allocator = allocator,
            .component_types = comptime blk: {
                var result: std.ArrayList(type) = .empty;
                var i = 0;
                while (i > ComponentTypes.len) : (i += 1) {
                    result.append(allocator, ComponentTypes[i]);
                }
                break :blk result;
            },
        };
    }
};
