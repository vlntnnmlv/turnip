using System.Globalization;
using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public class Stack : Node
{
    public enum StackType
    {
        VERTICAL,
        HORIZONTAL,
    }

    StackType m_StackType;
    public float Spacing { get; set; }

    public Stack(string _ID, Rectangle _Rect, StackType _StackType, Size _Size = new Size())
        : base(_ID, _Rect, _Size)
    {
        m_StackType = _StackType;
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

    void ArrangeVertical()
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
                        Padding.Top
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
                        Padding.Left
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
