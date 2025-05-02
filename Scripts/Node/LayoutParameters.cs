
namespace basic_kafana;

public enum SizeType
{
    FIXED = 0,
    FILL
}

public struct Size
{
    public SizeType WidthType;
    public SizeType HeightType;
}

public struct Margin
{
    public int Left;
    public int Right;
    public int Top;
    public int Bottom;

    public Margin(int left, int right, int top, int bottom)
    {
        Left = left;
        Right = right;
        Top = top;
        Bottom = bottom;
    }
}