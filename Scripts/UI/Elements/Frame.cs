using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public class Frame : Node
{
    Image m_Bg;
    Node m_ButtonRoot;
    Button m_Button;
    Image m_ButtonImg;

    public Frame(string _ID, Size _Size = new Size())
        : base(_ID, _Size)
    {
        m_Bg = new Image(
            "bg",
            new ImageInfo
            {
                Patch = new LRTB(22),
                Texture = Raylib.LoadTexture("Resources/Textures/frame@2x.png"),
            }
        );
        LinkChild(m_Bg);

        m_ButtonRoot = new Node(
            "btnRoot",
            new Size
            {
                AxisX = SizeType.END,
                Width = 16,
                AxisY = SizeType.START,
                Height = 16,
            }
        );

        LinkChild(m_ButtonRoot);

        m_ButtonImg = new Image(
            "btnImg",
            new ImageInfo
            {
                Patch = new LRTB(6, 6, 4, 4),
                Texture = Raylib.LoadTexture("Resources/Textures/cross.png"),
            }
        );

        m_ButtonRoot.LinkChild(m_ButtonImg);
    }
}
