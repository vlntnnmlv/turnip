const std = @import("std");

// TODO:!!!
const Registry = @import("ecs.zig").Registry;
const ComponentsViewIterator = @import("components_meta.zig").ComponentsViewIterator;

pub const System = struct {
    const VTable = struct {
        run: *const fn (*anyopaque) void,
        deinit: *const fn (*anyopaque) void = skip,
    };

    ptr: *anyopaque,
    vtable: *const VTable,

    pub fn create(
        ptr: anytype,
        comptime run_function: fn (@TypeOf(ptr)) void,
    ) System {
        const PtrType = @TypeOf(ptr);
        const Erased = struct {
            fn run(erased: *anyopaque) void {
                const self: PtrType = @ptrCast(@alignCast(erased));
                run_function(self);
            }

            fn deinit(erased: *anyopaque) void {
                _ = erased;
            }
        };

        return System{
            .ptr = ptr,
            .vtable = &VTable{
                .run = Erased.run,
                .deinit = Erased.deinit,
            },
        };
    }

    fn skip(_: *anyopaque) void {}

    pub fn run(self: System) void {
        self.vtable.run(self.ptr);
    }

    pub fn deinit(self: System) void {
        self.vtable.deinit(self.ptr);
    }
};
