using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public class Button : Node
{
    Action m_Action;

    public Button(string _ID, Action _Action, Size _Size = new Size())
        : base(_ID, _Size)
    {
        m_Action = _Action;
    }

    public override void OnClick()
    {
        m_Action?.Invoke();
    }
}
