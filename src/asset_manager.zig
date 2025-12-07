const std = @import("std");

const AssetLoader = @import("asset_loader.zig").AssetLoader;

pub const AssetType = enum {
    TEXTURE,
    SHADER,
    FONT,
};

pub const AssetManager = struct {};
