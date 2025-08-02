using System.Numerics;
using Raylib_cs;

namespace Turnip;

public class Switch : Node
{
    Action<bool> m_OnSwitch;
    bool m_Value;
    ImageInfo m_ImageInfoOn;
    ImageInfo m_ImageInfoOff;
    Image m_Image;

    public Switch(
        string _ID,
        Node? _Parent,
        Action<bool> _OnSwitch,
        string _Text = "",
        Size _Size = new Size(),
        Color? _Color = null
    )
        : base(_ID, _Parent, _Size, _Color)
    {
        m_OnSwitch = _OnSwitch;

        m_ImageInfoOff = new ImageInfo
        {
            Texture = Resources.LoadTexture("switch_off"),
            Patch = new LRTB(2),
        };
        m_ImageInfoOn = new ImageInfo
        {
            Texture = Resources.LoadTexture("switch_on"),
            Patch = new LRTB(2),
        };

        m_Image = new Image("switchImg", this, m_ImageInfoOff);
    }

    public override void OnPress() { }

    public override void OnRelease()
    {
        if (!IsHovered)
            return;

        m_Value = !m_Value;

        m_Image.Info = m_Value ? m_ImageInfoOn : m_ImageInfoOff;
        m_OnSwitch?.Invoke(m_Value);
    }
}
