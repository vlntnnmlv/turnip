using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public struct ButtonInfo
{
    public ImageInfo Normal;
    public ImageInfo Hovered;
    public ImageInfo Pressed;
    public Action? Action;
}

public class Button : Node
{
    ButtonInfo m_Info;
    Image m_Image;

    public Button(
        string _ID,
        Node? _Parent,
        Action _Action,
        Size _Size = new Size(),
        Color? _Color = null
    )
        : base(_ID, _Parent, _Size, _Color)
    {
        // m_Action = _Action;
    }

    public Button(
        string _ID,
        Node? _Parent,
        ButtonInfo _ButtonInfo,
        Size _Size = new Size(),
        Color? _Color = null
    )
        : base(_ID, _Parent, _Size, _Color)
    {
        m_Info = _ButtonInfo;

        m_Image = new Image("img", this, _ButtonInfo.Normal, _Color: _Color);
    }

    public override void OnHover()
    {
        // m_Image.Info = m_Info.Hovered;
    }

    public override void OnClick()
    {
        m_Info.Action?.Invoke();
    }

    public override void OnPress()
    {
        // Console.WriteLine("Pressed");
        m_Image.Info = m_Info.Pressed;
    }

    public override void OnRelease()
    {
        // Console.WriteLine("Released");
        m_Image.Info = m_Info.Normal;
    }
}
