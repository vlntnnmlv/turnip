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

    public Stack(string _ID, Rectangle _Rect, StackType _StackType)
        : base(_ID, _Rect)
    {
        m_StackType = _StackType;
    }
}
