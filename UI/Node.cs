using System.Numerics;
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
    public SizeType AxisX;
    public SizeType AxisY;
    public float? X;
    public float? Y;
    public float? Width;
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

    public bool IsHovered;
    public bool IsFocused => Focused == this;

    public Action<Node>? OnClick;

    public static Node? Focused { get; private set; }

    public static MouseState mState;
    public static Vector2 mPos;
    public static Vector2 mDelta;
    static bool mPressed;

    public static void CheckMouse()
    {
        mDelta = Raylib.GetMousePosition() - mPos;
        mPos = Raylib.GetMousePosition();

        bool pressed = Raylib.IsMouseButtonDown(MouseButton.Left);

        if (pressed)
        {
            if (mDelta != Vector2.Zero && mPressed)
                mState = MouseState.DRAGGED;
            else
                mState = MouseState.PRESSED;

            mPressed = pressed;
        }
        else
        {
            mState = MouseState.FREE;
        }
    }

    public Action? CheckEvent(Vector2 _Position, bool _IsPressed)
    {
        IsHovered = WorldRect.Contains(_Position);

        bool wasFocused = IsFocused;
        if (_IsPressed && IsHovered)
            Focused = this;

        if (!wasFocused && IsFocused)
        {
            return OnClicked;
        }

        return null;
    }

    public virtual void OnClicked() { }

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
        WorldRect = Parent == null ? Rect : Rect.Move(Parent.WorldRect.Position);
    }

    public virtual void Draw() { }

    public void DrawDebug()
    {
        Raylib.DrawRectangleLinesEx(WorldRect, 1, Color.Red);

        int fontSize = 8;

        Vector2 textSize = Raylib.MeasureTextEx(Garden.Font, ID, fontSize, 0.0f);

        if (IsHovered)
            Raylib.DrawCircle((int)WorldRect.X + 6, (int)WorldRect.Y + 6, 4, Color.Red);

        if (IsFocused)
            Raylib.DrawCircle((int)WorldRect.X + 12, (int)WorldRect.Y + 6, 4, Color.Blue);

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
