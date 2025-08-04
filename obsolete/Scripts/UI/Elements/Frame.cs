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
        ).IgnoreEvents = true;

        new Image(
            "frame",
            this,
            new ImageInfo { Texture = Resources.LoadTexture("frame_internal"), Patch = new LRTB(8) }
        ).IgnoreEvents = true;

        // Button closeButton = new Button(
        //     "close_button",
        //     this,
        //     new ButtonInfo
        //     {
        //         Normal = new ImageInfo
        //         {
        //             Texture = Resources.LoadTexture("button_close"),
        //             Patch = new LRTB(4),
        //         },
        //         Hovered = new ImageInfo
        //         {
        //             Texture = Resources.LoadTexture("button_close_hovered"),
        //             Patch = new LRTB(4),
        //         },
        //         Pressed = new ImageInfo
        //         {
        //             Texture = Resources.LoadTexture("button_close_pressed"),
        //             Patch = new LRTB(4),
        //         },
        //         Action = Remove,
        //     },
        //     new Size
        //     {
        //         AxisX = SizeType.END,
        //         AxisY = SizeType.START,
        //         Width = 32,
        //         Height = 32,
        //     }
        // );
        // closeButton.Padding = new LRTB(0, 1, 1, 0);
        // Console.WriteLine($"Frame close button uid: {closeButton.UID}");
    }
}
