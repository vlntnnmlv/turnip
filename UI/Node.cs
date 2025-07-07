using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public struct Size
{
    public SizeType AxisX;
    public SizeType AxisY;
    public float? Width;
    public float? Height;
}

public enum SizeType
{
    FILL,
    START,
    END,
    CENTER,
}

public class Node : ANode<Node>
{
    public Node(string _ID, Rectangle _Rect, Size _Size = new Size())
        : base(_ID)
    {
        Rect = _Rect;
        Size = _Size;
    }

    public Size Size;
    public LRTB Padding = new(0);
    public Rectangle Rect;
    public Rectangle WorldRect;

    public virtual void Measure()
    {
        foreach (Node child in Children)
        {
            Size size = child.Size;
            switch (size.AxisX)
            {
                case SizeType.FILL:
                    child.Rect.Width = Rect.Width - Padding.Left - Padding.Right;
                    break;
                case SizeType.START:
                case SizeType.END:
                case SizeType.CENTER:
                    child.Rect.Width = size.Width ?? 0.0f;
                    break;
            }

            switch (size.AxisY)
            {
                case SizeType.FILL:
                    child.Rect.Height = Rect.Height - Padding.Top - Padding.Bottom;
                    break;
                case SizeType.START:
                case SizeType.END:
                case SizeType.CENTER:
                    child.Rect.Height = size.Height ?? 0.0f;
                    break;
            }
        }
    }

    public virtual void Arrange()
    {
        foreach (Node child in Children)
        {
            Size size = child.Size;
            switch (size.AxisX)
            {
                case SizeType.FILL:
                case SizeType.START:
                    child.Rect.X = Padding.Left;
                    break;
                case SizeType.END:
                    child.Rect.X = Rect.Width - Padding.Right - child.Rect.Width;
                    break;
                case SizeType.CENTER:
                    child.Rect.X = Rect.Center().X - child.Rect.Width / 2.0f;
                    break;
            }

            switch (size.AxisY)
            {
                case SizeType.FILL:
                case SizeType.START:
                    child.Rect.Y = Padding.Top;
                    break;
                case SizeType.END:
                    child.Rect.Y = Rect.Height - Padding.Bottom - child.Rect.Height;
                    break;
                case SizeType.CENTER:
                    child.Rect.Y = Rect.Center().Y - child.Rect.Height / 2.0f;
                    break;
            }
        }
    }

    public void PlaceInWorld()
    {
        WorldRect = Parent == null ? Rect : Rect.Move(Parent.Rect.Position);
    }

    public virtual void Draw()
    {
        // if (_Node.Parent == null)
        //     WorldRect = Rect;
        // else
        //     WorldRect = Rect.Move(_Node.Parent.Meristem.Rect.Position);

        // float spacing = 20;
        // Rectangle padding = new Rectangle(20, 20, 20, 20);

        // if (_Node.Parent?.ID == "vstack")
        // {
        //     int childrenNumber = _Node.Parent.Children.Count;

        //     Rectangle paddedParentRect = _Node.Parent.Meristem.Rect.Shrink(
        //         new Vector2(padding.X + padding.Width, padding.Y + padding.Height)
        //     );
        //     float height =
        //         (paddedParentRect.Height - spacing * (childrenNumber - 1)) / childrenNumber;

        //     Rect = new Rectangle(
        //         padding.X,
        //         padding.Y + _Node.Order * (spacing + height),
        //         paddedParentRect.Width,
        //         height
        //     );
        //     WorldRect = Rect.Move(paddedParentRect.Position); // new Rectangle(paddedParentRect.X + padding.X, paddedParentRect.Y + padding.Y + _Node.Order * height + spacing * _Node.Order, paddedParentRect.Width, height);
        // }

        // if (_Node.Parent?.ID == "hstack")
        // {
        //     int childrenNumber = _Node.Parent.Children.Count;

        //     Rectangle parentRect = _Node.Parent.Meristem.Rect.Shrink(
        //         new Vector2(padding.X + padding.Width, padding.Y + padding.Height)
        //     );
        //     float width = (parentRect.Width - spacing * (childrenNumber - 1)) / childrenNumber;

        //     WorldRect = new Rectangle(
        //         parentRect.X + padding.X + _Node.Order * width + spacing * _Node.Order,
        //         parentRect.Y + padding.Y,
        //         width,
        //         parentRect.Height
        //     );
        // }
    }

    public void DrawDebug()
    {
        Raylib.DrawRectangleLinesEx(WorldRect, 1, Color.Red);

        int fontSize = 8;

        Vector2 textSize = Raylib.MeasureTextEx(Garden.Font, ID, fontSize, 0.0f);
        Raylib.DrawTextEx(
            Garden.Font,
            ID,
            WorldRect.Center() - textSize / 2,
            fontSize,
            5,
            Color.Red
        );
    }
}
