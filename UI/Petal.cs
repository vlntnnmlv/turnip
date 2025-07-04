using Raylib_cs;
using System.Numerics;

namespace BasicKafana;

public struct PetalInfo
{
    public Texture2D Texture;
    public Rectangle Patch;
}

public class Petal : Meristem
{
    PetalInfo m_Info;

    public Petal(Rectangle _Rect, PetalInfo _Info) : base(_Rect)
    {
        m_Info = _Info;
    }

    public override void Draw(Node _Node)
    {
        base.Draw(_Node);

        NPatchInfo patchInfo = new NPatchInfo()
        {
            Source = new Rectangle(Vector2.Zero, m_Info.Texture.Width, m_Info.Texture.Height),
            Left = (int)m_Info.Patch.X,
            Right = (int)m_Info.Patch.Y,
            Top = (int)m_Info.Patch.Width,
            Bottom = (int)m_Info.Patch.Height,
            Layout = NPatchLayout.NinePatch
        };

        Raylib.DrawTextureNPatch(m_Info.Texture, patchInfo, WorldRect, Vector2.Zero, 0, Color.White);
    }
}