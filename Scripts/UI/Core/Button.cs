using System.Numerics;
using Raylib_cs;

namespace Turnip;

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

    public Action Action
    {
        set => m_Info.Action = value;
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
        m_Image.IgnoreEvents = true;
    }

    public override void OnHoverEnter()
    {
        m_Image.Info = m_Info.Hovered;
    }

    public override void OnHoverExit()
    {
        m_Image.Info = m_Info.Normal;
    }

    public override void OnPress()
    {
        if (IsHovered)
            m_Image.Info = m_Info.Pressed;
    }

    public override void OnRelease()
    {
        m_Image.Info = m_Info.Normal;

        if (IsHovered)
            m_Info.Action?.Invoke();
    }
}
