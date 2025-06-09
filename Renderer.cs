using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public static class Renderer
{
    static Font m_DefaultFont = Raylib.GetFontDefault();

    public static void DrawLabel(string _Text, Vector2 _Position, Color _Color, int _FontSize = 16)
    {
        int textWidth = Raylib.MeasureText(_Text, _FontSize);
        Raylib.DrawTextEx(m_DefaultFont, _Text, _Position - new Vector2(textWidth / 2, 0), _FontSize, 5, _Color);
    }

    public static void DrawArrow(Vector2 _Start, Vector2 _End, Color _Color, int _Thickness = 2)
    {
        Raylib.DrawLineEx(_Start, _End, _Thickness, _Color);
    }

    public static void DrawBox(Box _Box, Color? _Color = null)
    {
        Color color = _Color.HasValue
            ? _Color.Value
            : _Box.IsStatic
                ? Color.Red
                : Color.Blue;

        if (_Box.IsStatic)
            Raylib.DrawRectangle((int)_Box.Rectangle.X, (int)_Box.Rectangle.Y, (int)_Box.Rectangle.Width, (int)_Box.Rectangle.Height, color);
        else
            Raylib.DrawRectangleLinesEx(_Box.Rectangle, 6, color);

        Vector2 center = _Box.Rectangle.Center();
        if (_Box.Velocity != Vector2.Zero)
        {
            DrawArrow(center, center + _Box.Velocity, Color.DarkPurple, 5);
            DrawLabel("Velocity", center + _Box.Velocity, Color.DarkPurple);
        }

        if (_Box.Acceleration != Vector2.Zero)
        {
            DrawArrow(center, center + _Box.Acceleration, Color.Green, 3);
            DrawLabel("Acceleration", center + _Box.Acceleration, Color.Green);
        }

        if (_Box.NetForce != Vector2.Zero)
        {
            DrawArrow(center, center + _Box.NetForce, Color.Black, 1);
            DrawLabel("NetForce", center + _Box.NetForce, Color.Black);
        }
    }

    public static void DrawState(State _State)
    {
        for (uint i = 0; i < _State.Boxes.Length; i++)
        {
            Renderer.DrawBox(_State.Boxes[i], i == 0 ? Color.Green : null);
        }
    }
}