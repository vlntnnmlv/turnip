using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public class Button : Node
{
    Action m_Action;

    public Button(
        string _ID,
        Node? _Parent,
        Action _Action,
        Size _Size = new Size(),
        Color? _Color = null
    )
        : base(_ID, _Parent, _Size, _Color)
    {
        m_Action = _Action;
    }

    public override void OnClick()
    {
        m_Action?.Invoke();
    }
}
