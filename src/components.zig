const zlm = @import("zlm").as(f32);
const Rectangle = @import("geometry.zig").Rectangle;

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
        near: f32 = -1.0,
        far: f32 = 1.0,
    };

    view_type: ViewType,
    options: Options,
};

pub const TextureReference = struct {
    idx: u16,
};
pub const Sprite = struct { texture_reference: TextureReference };
