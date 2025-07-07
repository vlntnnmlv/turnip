using Raylib_cs;

namespace BasicKafana;

public struct LRTB
{
    public int Left;
    public int Right;
    public int Top;
    public int Bottom;

    public LRTB(int _V)
    {
        Left = _V;
        Right = _V;
        Top = _V;
        Bottom = _V;
    }

    public LRTB(int _Left, int _Right, int _Top, int _Bottom)
    {
        Left = _Left;
        Right = _Right;
        Top = _Top;
        Bottom = _Bottom;
    }

    public LRTB(Rectangle _Rect)
    {
        Left = (int)_Rect.X;
        Right = (int)_Rect.Y;
        Top = (int)_Rect.Width;
        Bottom = (int)_Rect.Height;
    }
}
