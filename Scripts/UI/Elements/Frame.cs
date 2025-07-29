using Raylib_cs;

namespace Turnip;

public class Frame : Node
{
    public Frame(string _ID, Node _Parent, Size _Size = new Size())
        : base(_ID, _Parent, _Size)
    {
        new Image(
            "background",
            this,
            new ImageInfo { Texture = God.Texture },
            _Color: new Color(240, 250, 255)
        );

        new Image(
            "frame",
            this,
            new ImageInfo { Texture = Resources.LoadTexture("frame_internal"), Patch = new LRTB(8) }
        );
    }
}
