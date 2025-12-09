const std = @import("std");
const bgfx = @import("bgfx.zig").bgfx;

const AssetLoader = @import("asset_loader.zig").AssetLoader;

pub const AssetType = enum {
    TEXTURE,
};

pub const AssetReference = struct {
    asset_type: AssetType,
    id: u16 = 0,
};

pub const AssetManager = struct {
    allocator: std.mem.Allocator,
    assets_map: std.StringHashMap(AssetReference),

    pub fn init(allocator: std.mem.Allocator) AssetManager {
        return AssetManager{
            .allocator = allocator,
            .assets_map = std.StringHashMap(AssetReference).init(allocator),
        };
    }

    pub fn deinit(self: *AssetManager) void {
        var assets_iterator = self.assets_map.valueIterator();
        while (assets_iterator.next()) |asset| {
            switch (asset.asset_type) {
                AssetType.TEXTURE => bgfx.bgfx_destroy_texture(.{ .idx = asset.id }),
            }
        }

        self.assets_map.deinit();
    }

    pub fn loadAsset(self: *AssetManager, asset_type: AssetType, name: []const u8) !AssetReference {
        if (self.assets_map.get(name)) |asset| {
            return asset;
        }

        var asset_reference = AssetReference{ .asset_type = asset_type };
        switch (asset_type) {
            AssetType.TEXTURE => {
                const texture_handle = try AssetLoader.loadTexture(name);
                asset_reference.id = texture_handle.idx;
            },
        }

        try self.assets_map.put(name, asset_reference);

        return asset_reference;
    }
};
