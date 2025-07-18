// using System.Numerics;
// using Raylib_cs;

// namespace BasicKafana;

// public class Frame : Node
// {
//     Image m_Bg;
//     Node m_ButtonRoot;
//     Button m_Button;
//     Image m_ButtonImg;

//     public Frame(string _ID, Size _Size = new Size())
//         : base(_ID, _Size)
//     {
//         m_Bg = new Image(
//             "bg",
//             new ImageInfo
//             {
//                 Texture = Raylib.LoadTexture("Resources/Textures/frame_smooth_rect.png"),
//                 Patch = new LRTB(16),
//             }
//         );
//         LinkChild(m_Bg);

//         m_ButtonRoot = new Node(
//             "btnRoot",
//             new Size
//             {
//                 AxisX = SizeType.END,
//                 Width = 36,
//                 AxisY = SizeType.START,
//                 Height = 36,
//             }
//         );

//         LinkChild(m_ButtonRoot);

//         m_ButtonImg = new Image(
//             "btnImg",
//             new ImageInfo
//             {
//                 Patch = new LRTB(16),
//                 Texture = Raylib.LoadTexture("Resources/Textures/button_cross.png"),
//             }
//         );

//         m_ButtonRoot.LinkChild(m_ButtonImg);
//     }
// }
