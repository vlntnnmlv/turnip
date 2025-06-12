using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public static class Renderer
{
    static Texture2D m_CrateTexture = Raylib.LoadTexture("crate.png");
    static Texture2D m_FloorTexture = Raylib.LoadTexture("floor.png");

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

        Raylib.DrawTexturePro(m_CrateTexture, new Rectangle(Vector2.Zero, m_CrateTexture.Width, m_CrateTexture.Height), _Box.Rectangle, Vector2.Zero, 0, color);

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

    public static void DrawFloor()
    {
        int gridWidth = (int)MathF.Ceiling(0.5f + Globals.WINDOW_WIDTH / m_FloorTexture.Width);
        int gridHeight = (int)MathF.Ceiling(0.5f + Globals.WINDOW_HEIGHT / m_FloorTexture.Height);

        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                Raylib.DrawTexturePro(
                    m_FloorTexture,
                    new Rectangle(Vector2.Zero, m_FloorTexture.Width, m_FloorTexture.Height),
                    new Rectangle(Vector2.Zero + new Vector2(i * m_FloorTexture.Width, j * m_FloorTexture.Height), m_FloorTexture.Width, m_FloorTexture.Height),
                    Vector2.Zero,
                    0,
                    Color.White
                );
            }
        }
    }

    public static void DrawState(State _State)
    {
        DrawFloor();

        for (uint i = 0; i < _State.Boxes.Length; i++)
        {
            DrawBox(_State.Boxes[i], i == 0 ? Color.Green : null);
        }

        DrawArrow(_State.Boxes[0].Rectangle.Center(), _State.Boxes[0].Rectangle.Center() + _State.ManualControl, new Color(155, 12, 96));
    }
}