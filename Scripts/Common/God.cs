using Raylib_cs;

namespace BasicKafana;

public class God
{
    static God? m_Instance;
    public static God Instance
    {
        get
        {
            m_Instance ??= new God();
            return m_Instance;
        }
        private set { m_Instance = value; }
    }

    public static void SetInstance(God _Instance)
    {
        Instance = _Instance;
    }

    public bool DebugPhysics { get; set; }
    public bool DebugUI { get; set; }
    public float UIAnimationRate { get; set; }
    public string FontName = "PlayfairDisplayBlack";

    static Font? m_Font;
    public static Font Font
    {
        get
        {
            if (m_Font.HasValue)
                return m_Font.Value;

            // TODO: Make this font look nice
            m_Font = Raylib.LoadFont($"Resources/Fonts/{Instance.FontName}.ttf");
            Raylib.SetTextureFilter(m_Font.Value.Texture, TextureFilter.Bilinear);

            return m_Font.Value;
        }
    }

    static Texture2D? m_Texture;
    public static Texture2D Texture
    {
        get
        {
            if (m_Texture.HasValue)
                return m_Texture.Value;

            RenderTexture2D t = Raylib.LoadRenderTexture(1, 1);
            Raylib.BeginTextureMode(t);
            Raylib.DrawPixel(0, 0, Color.White);
            Raylib.EndTextureMode();
            Raylib.SetTextureFilter(t.Texture, TextureFilter.Trilinear);

            return t.Texture;
        }
    }

    static Shader? m_NoiseShader;
    public static Shader NoiseShader
    {
        get
        {
            if (m_NoiseShader.HasValue)
                return m_NoiseShader.Value;

            m_NoiseShader = Raylib.LoadShader(null, "./Resources/Shaders/noise.glsl");

            return m_NoiseShader.Value;
        }
    }
}
