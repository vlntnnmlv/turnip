using System.Numerics;
using Raylib_cs;

namespace Turnip;

public struct ImageInfo
{
    public Texture2D Texture;
    public LRTB Patch;
}

public class Image : Node
{
    ImageInfo m_Info;
    public ImageInfo Info
    {
        set => m_Info = value;
    }

    public Image(
        string _ID,
        Node? _Parent,
        ImageInfo _Info,
        Size _Size = new Size(),
        Color? _Color = null
    )
        : base(_ID, _Parent, _Size, _Color)
    {
        m_Info = _Info;
    }

    public override void Draw()
    {
        base.Draw();

        NPatchInfo patchInfo = new NPatchInfo()
        {
            Source = new Rectangle(Vector2.Zero, m_Info.Texture.Width, m_Info.Texture.Height),
            Left = m_Info.Patch.Left,
            Right = m_Info.Patch.Right,
            Top = m_Info.Patch.Top,
            Bottom = m_Info.Patch.Bottom,
            Layout = NPatchLayout.NinePatch,
        };

        Raylib.DrawTextureNPatch(m_Info.Texture, patchInfo, WorldRect, Vector2.Zero, 0, Color);
    }
}
