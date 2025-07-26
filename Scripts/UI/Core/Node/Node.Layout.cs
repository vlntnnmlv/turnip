using Raylib_cs;

namespace Turnip;

public struct Size
{
    [Serialize]
    public SizeType AxisX;

    [Serialize]
    public SizeType AxisY;

    [Serialize]
    public float? X;

    [Serialize]
    public float? Y;

    [Serialize]
    public float? Width;

    [Serialize]
    public float? Height;
}

public enum SizeType
{
    FILL = 0,
    START = 1,
    END = 2,
    CENTER = 3,
    ABSOLUTE = 4,
}

public partial class Node
{
    [Serialize]
    public Size Size;

    [Serialize]
    public LRTB Padding = new(0);

    [Serialize]
    public LRTB Margin = new(0);

    public virtual void Measure()
    {
        // Parent is simple node
        LRTB padding = Parent?.Padding ?? new LRTB(0);
        Rectangle parentRect =
            Parent?.Rect ?? new Rectangle(0, 0, Globals.WINDOW_WIDTH, Globals.WINDOW_HEIGHT);

        switch (Size.AxisX)
        {
            case SizeType.FILL:
                Rect.Width =
                    parentRect.Width - padding.Left - padding.Right - Margin.Left - Margin.Right;
                break;
            case SizeType.START:
            case SizeType.END:
            case SizeType.CENTER:
            case SizeType.ABSOLUTE:
                Rect.Width = Size.Width ?? 0;
                break;
        }

        switch (Size.AxisY)
        {
            case SizeType.FILL:
                Rect.Height =
                    parentRect.Height - padding.Top - padding.Bottom - Margin.Top - Margin.Bottom;
                break;
            case SizeType.START:
            case SizeType.END:
            case SizeType.CENTER:
            case SizeType.ABSOLUTE:
                Rect.Height = Size.Height ?? 0;
                break;
        }

        // foreach (Node child in Children)
        // {
        //     Size size = child.Size;
        //     switch (size.AxisX)
        //     {
        //         case SizeType.FILL:
        //             child.Rect.Width = Rect.Width - Padding.Left - Padding.Right;
        //             break;
        //         case SizeType.START:
        //         case SizeType.END:
        //         case SizeType.CENTER:
        //             child.Rect.Width = size.Width ?? 0.0f;
        //             break;
        //         case SizeType.ABSOLUTE:
        //             child.Rect.Width = size.Width ?? 0;
        //             break;
        //     }

        //     switch (size.AxisY)
        //     {
        //         case SizeType.FILL:
        //             child.Rect.Height = Rect.Height - Padding.Top - Padding.Bottom;
        //             break;
        //         case SizeType.START:
        //         case SizeType.END:
        //         case SizeType.CENTER:
        //             child.Rect.Height = size.Height ?? 0.0f;
        //             break;
        //         case SizeType.ABSOLUTE:
        //             child.Rect.Height = size.Height ?? 0;
        //             break;
        //     }
        // }
    }

    public virtual void Arrange()
    {
        // Parent is simple node
        LRTB padding = Parent?.Padding ?? new LRTB(0);
        Rectangle parentRect =
            Parent?.Rect ?? new Rectangle(0, 0, Globals.WINDOW_WIDTH, Globals.WINDOW_HEIGHT);

        switch (Size.AxisX)
        {
            case SizeType.FILL:
            case SizeType.START:
                Rect.X = padding.Left + Margin.Left;
                break;
            case SizeType.END:
                Rect.X = parentRect.Width - padding.Right - Margin.Right - Rect.Width;
                break;
            case SizeType.CENTER:
                Rect.X = parentRect.Center().X - Rect.Width / 2;
                break;
            case SizeType.ABSOLUTE:
                Rect.X = Size.X ?? 0;
                break;
        }

        switch (Size.AxisY)
        {
            case SizeType.FILL:
            case SizeType.START:
                Rect.Y = padding.Top + Margin.Top;
                break;
            case SizeType.END:
                Rect.Y = parentRect.Height - padding.Bottom - Margin.Bottom - Rect.Height;
                break;
            case SizeType.CENTER:
                Rect.Y = parentRect.Center().X - Rect.Height / 2;
                break;
            case SizeType.ABSOLUTE:
                Rect.Y = Size.Y ?? 0;
                break;
        }

        // foreach (Node child in Children)
        // {
        //     Size size = child.Size;
        //     switch (size.AxisX)
        //     {
        //         case SizeType.FILL:
        //         case SizeType.START:
        //             child.Rect.X = Padding.Left;
        //             break;
        //         case SizeType.END:
        //             child.Rect.X = Rect.Width - Padding.Right - child.Rect.Width;
        //             break;
        //         case SizeType.CENTER:
        //             child.Rect.X = Rect.Center().X - child.Rect.Width / 2.0f;
        //             break;
        //         case SizeType.ABSOLUTE:
        //             child.Rect.X = size.X ?? 0;
        //             break;
        //     }

        //     switch (size.AxisY)
        //     {
        //         case SizeType.FILL:
        //         case SizeType.START:
        //             child.Rect.Y = Padding.Top;
        //             break;
        //         case SizeType.END:
        //             child.Rect.Y = Rect.Height - Padding.Bottom - child.Rect.Height;
        //             break;
        //         case SizeType.CENTER:
        //             child.Rect.Y = Rect.Center().Y - child.Rect.Height / 2.0f;
        //             break;
        //         case SizeType.ABSOLUTE:
        //             child.Rect.Y = size.Y ?? 0;
        //             break;
        //     }
        // }
    }

    public void ProcessLayout(float _DeltaTime)
    {
        Measure();
        Arrange();
        RealRect = RealRect.Lerp(Rect, God.Instance.UIAnimationRate * _DeltaTime);

        PlaceInWorld();
    }
}
