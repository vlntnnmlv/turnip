using Raylib_cs;
using System.Numerics;

namespace BasicKafana;

public class Meristem
{
    public Meristem(Rectangle _Rect)
    {
        Rect = _Rect;
    }

    public void Draw(Meristem? _Parent, string _ID)
    {
        Rectangle worldRect;

        if (_Parent == null)
            worldRect = Rect;
        else
            worldRect = Rect.Move(_Parent.Rect.Position);

        Raylib.DrawRectangleLinesEx(worldRect, 1, Color.Red);

        Font font = Raylib.GetFontDefault();
        int textWidth = Raylib.MeasureText(_ID, 24);
        Raylib.DrawTextEx(font, _ID, worldRect.Center() - new Vector2(textWidth / 2, 0), 24, 5, Color.Red);
    }

    public Rectangle Rect;
}