using System.Numerics;
using System.Runtime.Serialization;
using Raylib_cs;

namespace BasicKafana;

public struct EventInfo
{
    public Vector2 Position;
    public MouseState State;
}

public enum MouseState
{
    FREE = 0,
    PRESSED = 1,
    DRAGGED = 2,
}

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

public class Node : ANode<Node>
{
    public Node(string _ID, Node? _Parent = null, Size _Size = new Size(), Color? _Color = null)
        : base(_ID)
    {
        SetParent(_Parent);

        Size = _Size;
        Color = _Color ?? Color.White;
    }

    [Serialize]
    public Size Size;

    [Serialize]
    public LRTB Padding = new(0);

    [Serialize]
    public Color Color = Color.White;

    public Rectangle Rect;
    public Rectangle RealRect;
    public Rectangle WorldRect;

    public bool IsHovered => Hovered == this;
    public bool IsFocused => Focused == this;

    public static Node? Focused { get; private set; }
    public static Node? Hovered { get; private set; }

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
                case SizeType.ABSOLUTE:
                    child.Rect.Width = size.Width ?? 0;
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
                case SizeType.ABSOLUTE:
                    child.Rect.Height = size.Height ?? 0;
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
                case SizeType.ABSOLUTE:
                    child.Rect.X = size.X ?? 0;
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
                case SizeType.ABSOLUTE:
                    child.Rect.Y = size.Y ?? 0;
                    break;
            }
        }
    }

    public void PlaceInWorld()
    {
        // WorldRect = Parent == null ? Rect : Rect.Move(Parent.WorldRect.Position);
        WorldRect = Parent == null ? RealRect : RealRect.Move(Parent.WorldRect.Position);
    }

    public void ProcessMouseEvent(MouseEvent _MouseEvent)
    {
        OnMouseEvent(_MouseEvent);

        foreach (Node child in Children)
        {
            child.ProcessMouseEvent(_MouseEvent);
        }
    }

    public void OnMouseEvent(MouseEvent _MouseEvent)
    {
        if (WorldRect.Contains(_MouseEvent.Position))
            Hovered = this;

        if (IsHovered && _MouseEvent.Type == MouseEventType.CLICKED)
        {
            OnClick();
            Focused = this;
        }
    }

    public virtual void OnClick() { }

    public virtual void Update(float _DeltaTime) { }

    public virtual void Draw() { }

    public void DrawDebug()
    {
        Raylib.DrawRectangleLinesEx(WorldRect, 1, Color.Red);

        int fontSize = 8;

        Vector2 textSize = Raylib.MeasureTextEx(God.Font, ID, fontSize, 0.0f);

        if (IsHovered)
            Raylib.DrawCircle((int)WorldRect.X + 6, (int)WorldRect.Y + 6, 4, Color.Red);

        if (IsFocused)
            Raylib.DrawCircle((int)WorldRect.X + 12, (int)WorldRect.Y + 6, 4, Color.Blue);

        Raylib.DrawTextEx(
            God.Font,
            ID,
            WorldRect.Position + new Vector2(5, textSize.Y),
            fontSize,
            5,
            Color.Red
        );
    }
}
