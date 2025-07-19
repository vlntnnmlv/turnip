using BasicKafana;
using Raylib_cs;

public static class Resources
{
    public static Texture2D LoadTexture(string _Name)
    {
        Texture2D texture = Raylib.LoadTexture($"Resources/Textures/{_Name}.png");

        Raylib.SetTextureFilter(texture, TextureFilter.Point);
        return texture;
    }
}
