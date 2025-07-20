using System.Globalization;
using System.Numerics;
using Raylib_cs;

namespace Turnip;

public class Stack : Node
{
    public enum ContentType
    {
        START = 0,
        END = 1,
        CENTER = 2,
    }

    public enum StackType
    {
        VERTICAL = 0,
        HORIZONTAL = 1,
    }

    [Serialize]
    StackType m_StackType;

    [Serialize]
    ContentType m_ContentType;

    [Serialize]
    public float Spacing { get; set; }

    public Stack(
        string _ID,
        Node? _Parent,
        StackType _StackType,
        ContentType _ContentType,
        Size _Size = new Size(),
        Color? _Color = null
    )
        : base(_ID, _Parent, _Size, _Color)
    {
        m_StackType = _StackType;
        m_ContentType = _ContentType;
    }

    public override void Measure()
    {
        switch (m_StackType)
        {
            case StackType.VERTICAL:
                MeasureVertical();
                break;
            case StackType.HORIZONTAL:
                MeasureHorizontal();
                break;
        }
    }

    void MeasureVertical()
    {
        int childrenCount = Children.Count;
        int childrenCountFill = Children.Count(_C => _C.Size.AxisY == SizeType.FILL);
        float freeSpace =
            Rect.Height
            - (
                Children
                    .Where(_C => _C.Size.AxisY != SizeType.FILL)
                    .Select(_C => _C.Size.Height)
                    .Sum() ?? 0.0f
            )
            - Spacing * (childrenCount - 1)
            - Padding.Top
            - Padding.Bottom;

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
                    child.Rect.Height = freeSpace / childrenCountFill;
                    break;
                case SizeType.START:
                case SizeType.END:
                case SizeType.CENTER:
                    child.Rect.Height = size.Height ?? 0.0f;
                    break;
            }
        }
    }

    void MeasureHorizontal()
    {
        int childrenCount = Children.Count;
        int childrenCountFill = Children.Count(_C => _C.Size.AxisX == SizeType.FILL);
        float freeSpace =
            Rect.Width
            - (
                Children
                    .Where(_C => _C.Size.AxisX != SizeType.FILL)
                    .Select(_C => _C.Size.Width)
                    .Sum() ?? 0.0f
            )
            - Spacing * (childrenCount - 1)
            - Padding.Left
            - Padding.Right;

        foreach (Node child in Children)
        {
            Size size = child.Size;
            switch (size.AxisX)
            {
                case SizeType.FILL:
                    child.Rect.Width = freeSpace / childrenCountFill;
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

    public override void Arrange()
    {
        switch (m_StackType)
        {
            case StackType.VERTICAL:
                ArrangeVertical();
                break;
            case StackType.HORIZONTAL:
                ArrangeHorizontal();
                break;
        }
    }

    float GetContentX()
    {
        float contentWidth = Children.Any(_C => _C.Size.AxisX == SizeType.FILL)
            ? Rect.Width - Padding.Left - Padding.Right
            : (Children.Select(_C => _C.Size.Width).Sum() + (Children.Count - 1) * Spacing ?? 0);

        switch (m_ContentType)
        {
            case ContentType.START:
                return Padding.Left;
            case ContentType.END:
                return Rect.Width - contentWidth;
            case ContentType.CENTER:
                return (Rect.Width - contentWidth) / 2.0f;
            default:
                return 0;
        }
    }

    float GetContentY()
    {
        float contentHeight = Children.Any(_C => _C.Size.AxisY == SizeType.FILL)
            ? Rect.Height - Padding.Top - Padding.Bottom
            : (Children.Select(_C => _C.Size.Height).Sum() + (Children.Count - 1) * Spacing ?? 0);

        switch (m_ContentType)
        {
            case ContentType.START:
                return Padding.Top;
            case ContentType.END:
                return Rect.Height - contentHeight;
            case ContentType.CENTER:
                return (Rect.Height - contentHeight) / 2.0f;
            default:
                return 0;
        }
    }

    void ArrangeVertical()
    {
        float contentHeight = Children.Any(_C => _C.Size.AxisY == SizeType.FILL)
            ? Rect.Height - Padding.Top - Padding.Bottom
            : (Children.Select(_C => _C.Size.Height).Sum() ?? 0);

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
                    child.Rect.X = Rect.Width - child.Rect.Width - Padding.Right;
                    break;
                case SizeType.CENTER:
                    child.Rect.X = Rect.Center().X - child.Rect.Width / 2.0f;
                    break;
            }

            switch (size.AxisY)
            {
                case SizeType.FILL:
                case SizeType.START:
                case SizeType.END:
                case SizeType.CENTER:
                    child.Rect.Y =
                        GetContentY()
                        + Children
                            .Where(_C => _C.Order < child.Order)
                            .Select(_C => _C.Rect.Height)
                            .Sum()
                        + child.Order * Spacing;
                    break;
            }
        }
    }

    void ArrangeHorizontal()
    {
        float contentWidth = Children.Any(_C => _C.Size.AxisX == SizeType.FILL)
            ? Rect.Width - Padding.Left - Padding.Right
            : (Children.Select(_C => _C.Size.Width).Sum() ?? 0);

        foreach (Node child in Children)
        {
            Size size = child.Size;
            switch (size.AxisY)
            {
                case SizeType.FILL:
                case SizeType.START:
                    child.Rect.Y = Padding.Top;
                    break;
                case SizeType.END:
                    child.Rect.Y = Rect.Height - child.Rect.Height - Padding.Bottom;
                    break;
                case SizeType.CENTER:
                    child.Rect.Y = Rect.Center().Y - child.Rect.Height / 2.0f;
                    break;
            }

            switch (size.AxisX)
            {
                case SizeType.FILL:
                case SizeType.START:
                case SizeType.END:
                case SizeType.CENTER:
                    child.Rect.X =
                        GetContentX()
                        + Children
                            .Where(_C => _C.Order < child.Order)
                            .Select(_C => _C.Rect.Width)
                            .Sum()
                        + child.Order * Spacing;
                    break;
            }
        }
    }
}
