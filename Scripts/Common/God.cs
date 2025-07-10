using Raylib_cs;

namespace BasicKafana;

public static class God
{
    public static bool DebugPhysics { get; set; }
    public static bool DebugUI { get; set; }
    public static float UIAnimationRate { get; set; } = 0.002f;

    static Font? m_Font;
    public static Font Font
    {
        get
        {
            if (m_Font != null)
                return m_Font.Value;

            // TODO: Make this font look nice
            m_Font = Raylib.LoadFont("Resources/Fonts/PlayfairDisplayBlack.ttf");
            Raylib.SetTextureFilter(m_Font.Value.Texture, TextureFilter.Bilinear);

            return m_Font.Value;
        }
    }
}
