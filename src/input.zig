const std = @import("std");
const events = @import("events.zig");

const Event = events.Event;
const EventType = events.EventType;
const Key = events.Key;

/// Persistent input state updated each frame from the event queue.
/// Tracks which keys are held, mouse position, and per-frame mouse delta.
pub const Input = struct {
    keys_held: std.AutoHashMap(Key, void),
    mouse_x: f32 = 0,
    mouse_y: f32 = 0,
    mouse_dx: f32 = 0,
    mouse_dy: f32 = 0,
    scroll_y: f32 = 0,

    pub fn init(allocator: std.mem.Allocator) Input {
        return Input{
            .keys_held = std.AutoHashMap(Key, void).init(allocator),
        };
    }

    pub fn deinit(self: *Input) void {
        self.keys_held.deinit();
    }

    /// Process all events for this frame. Call once per frame before systems run.
    pub fn update(self: *Input, event_queue: []const Event) void {
        self.mouse_dx = 0;
        self.mouse_dy = 0;
        self.scroll_y = 0;

        for (event_queue) |event| {
            switch (event.eventType) {
                EventType.KeyPressed => {
                    self.keys_held.put(event.key, {}) catch {};
                },
                EventType.KeyReleased => {
                    _ = self.keys_held.remove(event.key);
                },
                EventType.MouseMoved => {
                    self.mouse_x = event.mouse_x;
                    self.mouse_y = event.mouse_y;
                    self.mouse_dx += event.mouse_x_rel;
                    self.mouse_dy += event.mouse_y_rel;
                },
                EventType.MouseWheel => {
                    self.scroll_y += event.scroll_y;
                },
                else => {},
            }
        }
    }

    pub fn isKeyHeld(self: *const Input, key: Key) bool {
        return self.keys_held.contains(key);
    }
};
