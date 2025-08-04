using System.Numerics;
using Raylib_cs;

namespace Turnip;

public class Text : Node
{
    int m_TextSize = 24;

    [Serialize]
    public string SText = string.Empty;

    public Text(
        string _ID,
        Node? _Parent,
        string _Text,
        int _TextSize = 24,
        Size _Size = new Size(),
        Color? _Color = null
    )
        : base(_ID, _Parent, _Size, _Color)
    {
        m_TextSize = _TextSize;
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
