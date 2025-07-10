using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public class Text : Node
{
    int m_TextSize = 24;
    public string SText = string.Empty;
    public Color Color;

    public Text(string _ID, string _Text, Color _Color, int _TextSize = 24, Size _Size = new Size())
        : base(_ID, _Size)
    {
        m_TextSize = _TextSize;
        Color = _Color;
        SText = _Text;
    }

    public override void Draw()
    {
        Vector2 textSize = Raylib.MeasureTextEx(God.Font, SText, m_TextSize, 5f);

        Raylib.DrawTextEx(
            God.Font,
            SText,
            WorldRect.Center() - textSize / 2,
            m_TextSize,
            5f,
            Color
        );
    }
}
