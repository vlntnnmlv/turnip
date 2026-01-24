const std = @import("std");

const zlm = @import("zlm").as(f32);
const geometry = @import("geometry.zig");
const asset_manager = @import("asset_manager.zig");

const Rectangle = geometry.Rectangle;
const AssetReference = asset_manager.AssetReference;

pub const Transform = struct { position: zlm.Vec3, scale: zlm.Vec3, rotation: zlm.Vec3 };
pub const Transform2D = struct { rectangle: Rectangle };

pub const Camera = struct {
    pub const ViewType = enum(u1) {
        ORTHOGONAL,
        PERSPECTIVE,
    };

    pub const Options = struct {
        view_rectangle: Rectangle,
        fov: f32 = 90.0,
        target: zlm.Vec3 = zlm.Vec3.zero,
        up: zlm.Vec3 = zlm.Vec3{ .x = 0.0, .y = 1.0, .z = 0.0 },
    };

    view_type: ViewType,
    near: f32 = 0.1,
    far: f32 = 1000.0,
    position: zlm.Vec3 = zlm.Vec3.zero,
    rotation: zlm.Vec3 = zlm.Vec3.zero,
    options: Options,

    // Helper to get forward vector from rotation
    pub fn getForward(self: Camera) zlm.Vec3 {
        const pitch = self.rotation.x;
        const yaw = self.rotation.y;

        return zlm.Vec3.new(@sin(yaw) * @cos(pitch), @sin(pitch), @cos(yaw) * @cos(pitch)).normalize();
    }

    // Helper to get right vector
    pub fn getRight(self: Camera) zlm.Vec3 {
        const forward = self.getForward();
        return forward.cross(self.options.up).normalize();
    }

    // Get target point (position + forward)
    pub fn getTarget(self: Camera) zlm.Vec3 {
        return self.position.add(self.getForward());
    }

    // Rotate camera
    pub fn rotate(self: *Camera, delta_pitch: f32, delta_yaw: f32) void {
        self.rotation.x += delta_pitch;
        self.rotation.y += delta_yaw;

        // Clamp pitch to prevent flipping
        const max_pitch: f32 = 1.5; // ~85 degrees
        if (self.rotation.x > max_pitch) self.rotation.x = max_pitch;
        if (self.rotation.x < -max_pitch) self.rotation.x = -max_pitch;

        // Keep yaw in reasonable range
        const two_pi = 2.0 * std.math.pi;
        if (self.rotation.y > two_pi) self.rotation.y -= two_pi;
        if (self.rotation.y < -two_pi) self.rotation.y += two_pi;
    }

    // Move relative to camera orientation
    pub fn moveRelative(self: *Camera, offset: zlm.Vec3) void {
        const forward = self.getForward();
        const right = self.getRight();
        const up = forward.cross(right).normalize();

        const movement = right.scale(offset.x)
            .add(up.scale(offset.y))
            .add(forward.scale(offset.z));

        self.position = self.position.add(movement);
    }
};

pub const Sprite = struct { texture_reference: AssetReference };

pub const Mesh = struct {
    vertices: []zlm.Vec3,
    indices: []i32,
    uvs: []zlm.Vec2,
};
