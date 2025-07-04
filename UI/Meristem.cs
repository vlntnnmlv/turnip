using Raylib_cs;
using System.Numerics;

namespace BasicKafana;

public class Meristem
{
    public Meristem(Rectangle _Rect)
    {
        Rect = _Rect;
    }

    public Rectangle WorldRect { get; set; }

    public virtual void Draw(Node _Node)
    {
        if (_Node.Parent == null)
            WorldRect = Rect;
        else
            WorldRect = Rect.Move(_Node.Parent.Meristem.Rect.Position);

        // Texture2D m_FloorTexture = Raylib.LoadTexture("frame.png");
        // NPatchInfo patchInfo = new NPatchInfo()
        // {
        //     Source = new Rectangle(Vector2.Zero, m_FloorTexture.Width, m_FloorTexture.Height),
        //     Left = 5,
        //     Right = 5,
        //     Top = 5,
        //     Bottom = 5,
        //     Layout = NPatchLayout.NinePatch
        // };
        // Raylib.DrawTextureNPatch(m_FloorTexture, patchInfo, worldRect, Vector2.Zero, 0, Color.White);

        if (Garden.DebugUI)
        {
            Raylib.DrawRectangleLinesEx(WorldRect, 1, Color.Red);

            Font font = Raylib.GetFontDefault();
            int fontSize = 8;
            Vector2 textSize = Raylib.MeasureTextEx(font, _Node.ID, fontSize, 0.0f);
            Raylib.DrawTextEx(font, _Node.ID, WorldRect.Center() - textSize / 2, fontSize, 5, Color.Red);
        }
    }

    public Rectangle Rect;
}