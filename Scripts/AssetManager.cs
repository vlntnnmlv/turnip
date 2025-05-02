using System.IO;
using System.Collections.Generic;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using System;
using System.Linq;

public static class AssetManager
{
    static Dictionary<string, Texture2D> _textures = new();
    static Dictionary<string, SpriteFont> _fonts = new();
    static Texture2D _defaultTexture;

    const string FONTS_DIRECTORY = "Fonts";
    const string TEXTURES_DIRECTORY = "Textures";

    public static string GetDirectoryNameFromAssetType(Type assetType)
    {
        string directoryName = string.Empty;

        if (assetType == typeof(SpriteFont))
            directoryName = FONTS_DIRECTORY;
        
        if (assetType == typeof(Texture2D))
            directoryName =  TEXTURES_DIRECTORY;

        if (directoryName == string.Empty)
            throw new Exception($"ERROR! Unsupported asset type: {assetType}");

        return Path.Combine(_game.Content.RootDirectory, directoryName);
    }

    static Game _game;

    public static void Init(Game game)
    {
        _game = game;

        ParseAssetsInDirectory(GetDirectoryNameFromAssetType(typeof(SpriteFont)), ref _fonts);
        ParseAssetsInDirectory(GetDirectoryNameFromAssetType(typeof(Texture2D)), ref _textures);

        _defaultTexture = new Texture2D(game.GraphicsDevice, 1, 1);
        _defaultTexture.SetData(new Color[] { Color.White });
    }

    static void ParseAssetsInDirectory<T>(string assetsDirectoryName, ref Dictionary<string, T> results)
    {
        string assetGroup = Path.GetRelativePath(_game.Content.RootDirectory, assetsDirectoryName);
        foreach (string file in Directory.GetFiles(assetsDirectoryName))
        {
            string assetName = Path.GetFileNameWithoutExtension(file);
            string assetID = GetAssetID(assetGroup, assetName);

            results[assetID] = _game.Content.Load<T>(Path.Combine(assetGroup, assetName));
        }

        foreach (string directory in Directory.GetDirectories(assetsDirectoryName))
        {
            ParseAssetsInDirectory(directory, ref results);
        }
    }

    static string GetAssetID(string assetGroup, string assetName)
    {
        if (assetGroup == TEXTURES_DIRECTORY || assetGroup == FONTS_DIRECTORY)
            return assetName;

        string assetGroupPrefix = assetGroup.Substring(assetGroup.IndexOf(Path.DirectorySeparatorChar) + 1).Replace(Path.DirectorySeparatorChar, '.');
        string assetID = $"{assetGroupPrefix}.{assetName}";

        return assetID.ToLowerInvariant();
    }

    public static Texture2D GetTexture(string textureID)
    {
        return _textures[textureID];
    }

    public static Texture2D GetDefaultTexture()
    {
        return _defaultTexture;
    }

    public static SpriteFont GetFont(string fontID)
    {
        return _fonts[fontID];
    }
}