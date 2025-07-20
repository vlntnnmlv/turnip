using System.Numerics;
using System.Runtime.Serialization;
using Raylib_cs;

namespace Turnip;

public partial class Node : ANode<Node>
{
    public Node(string _ID, Node? _Parent = null, Size _Size = new Size(), Color? _Color = null)
        : base(_ID)
    {
        SetParent(_Parent);

        Size = _Size;
        Color = _Color ?? Color.White;
    }

    [Serialize]
    public Color Color = Color.White;

    public Rectangle Rect;
    public Rectangle RealRect;
    public Rectangle WorldRect;

    public bool IsHovered => Hovered == this;
    public bool IsFocused => Focused == this;

    public static Node? Focused { get; private set; }

    static Node? m_Hovered;
    public static Node? Hovered
    {
        get => m_Hovered;
        private set
        {
            if (m_Hovered?.UID == value?.UID)
                return;

            m_Hovered?.OnHoverExit();
            m_Hovered = value;
            m_Hovered?.OnHoverEnter();
        }
    }

    public virtual void Update(float _DeltaTime) { }

    void PlaceInWorld()
    {
        WorldRect = Parent == null ? RealRect : RealRect.Move(Parent.WorldRect.Position);
    }

    public virtual void Draw() { }

    public void DrawDebug()
    {
        Raylib.DrawRectangleLinesEx(WorldRect, 1, Color.Red);

        int fontSize = 8;

        Vector2 textSize = Raylib.MeasureTextEx(God.Font, ID, fontSize, 0.0f);

        if (IsHovered)
            Raylib.DrawCircle((int)WorldRect.X + 6, (int)WorldRect.Y + 6, 4, Color.Red);

        if (IsFocused)
            Raylib.DrawCircle((int)WorldRect.X + 12, (int)WorldRect.Y + 6, 4, Color.Blue);

        Raylib.DrawTextEx(
            God.Font,
            ID,
            WorldRect.Position + new Vector2(5, textSize.Y),
            fontSize,
            5,
            Color.Red
        );
    }
}
