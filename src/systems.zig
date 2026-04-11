const std = @import("std");
const components = @import("components.zig");
const input_mod = @import("input.zig");
const ecs = @import("fey_ecs");

const Camera = components.Camera;
const Input = input_mod.Input;
const Key = @import("events.zig").Key;
const ComponentsView = ecs.meta.ComponentsView;

/// 2D orthographic camera controller driven by keyboard (WASD).
/// Pans position along screen axes. Scroll wheel zooms the view.
///
/// Usage:
///   var controller = OrthoCameraController.init(&app.input);
///   try world.addSystem(.{Camera}, &OrthoCameraController.system, controller.context());
pub const OrthoCameraController = struct {
    input: *Input,
    pan_speed: f32 = 500.0,
    zoom_speed: f32 = 0.1,
    min_zoom: f32 = 0.1,
    max_zoom: f32 = 10.0,
    zoom: f32 = 1.0,
    base_width: f32 = 0,
    base_height: f32 = 0,
    up_key: Key = Key.W,
    down_key: Key = Key.S,
    left_key: Key = Key.A,
    right_key: Key = Key.D,

    pub fn init(input: *Input, width: f32, height: f32) OrthoCameraController {
        return OrthoCameraController{
            .input = input,
            .base_width = width,
            .base_height = height,
        };
    }

    pub fn context(self: *OrthoCameraController) *anyopaque {
        return @ptrCast(self);
    }

    pub fn system(view: ComponentsView(.{Camera}), dt: f32, ctx: *anyopaque) void {
        const self: *OrthoCameraController = @ptrCast(@alignCast(ctx));
        const inp = self.input;

        const move = self.pan_speed * dt;

        // Keyboard panning — axis-aligned, no rotation
        if (inp.isKeyHeld(self.left_key)) view.camera.position.x -= move;
        if (inp.isKeyHeld(self.right_key)) view.camera.position.x += move;
        if (inp.isKeyHeld(self.up_key)) view.camera.position.y -= move;
        if (inp.isKeyHeld(self.down_key)) view.camera.position.y += move;

        // Scroll wheel zoom
        if (inp.scroll_y != 0) {
            self.zoom *= 1.0 + inp.scroll_y * self.zoom_speed;
            self.zoom = std.math.clamp(self.zoom, self.min_zoom, self.max_zoom);
            view.camera.options.view_rectangle.width = self.base_width / self.zoom;
            view.camera.options.view_rectangle.height = self.base_height / self.zoom;
        }
    }
};

/// First-person camera controller driven by keyboard (WASD + QE) and mouse look.
///
/// Usage:
///   var controller = FPSCameraController.init(&app.input);
///   try world.addSystem(.{Camera}, &FPSCameraController.system, controller.context());
pub const FPSCameraController = struct {
    input: *Input,
    move_speed: f32 = 20.0,
    look_sensitivity: f32 = 5.0,
    forward_key: Key = Key.W,
    back_key: Key = Key.S,
    left_key: Key = Key.A,
    right_key: Key = Key.D,
    up_key: Key = Key.Q,
    down_key: Key = Key.E,

    pub fn init(input: *Input) FPSCameraController {
        return FPSCameraController{ .input = input };
    }

    pub fn context(self: *FPSCameraController) *anyopaque {
        return @ptrCast(self);
    }

    pub fn system(view: ComponentsView(.{Camera}), dt: f32, ctx: *anyopaque) void {
        const self: *FPSCameraController = @ptrCast(@alignCast(ctx));
        const inp = self.input;

        const move = self.move_speed * dt;
        const look = self.look_sensitivity * dt;

        // Mouse look
        if (inp.mouse_dx != 0 or inp.mouse_dy != 0) {
            view.camera.rotate(-inp.mouse_dy * look, -inp.mouse_dx * look);
        }

        // Keyboard movement
        var movement = @import("zlm").as(f32).Vec3.zero;

        if (inp.isKeyHeld(self.forward_key)) movement.z += move;
        if (inp.isKeyHeld(self.back_key)) movement.z -= move;
        if (inp.isKeyHeld(self.left_key)) movement.x -= move;
        if (inp.isKeyHeld(self.right_key)) movement.x += move;
        if (inp.isKeyHeld(self.up_key)) movement.y += move;
        if (inp.isKeyHeld(self.down_key)) movement.y -= move;

        if (movement.x != 0 or movement.y != 0 or movement.z != 0) {
            view.camera.moveRelative(movement);
        }
    }
};
