namespace Turnip;

public partial class Node
{
    static List<Node> m_ToRemove = new();
    public bool IgnoreEvents { get; set; }

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

    public void ProcessMouseEvent(MouseEvent _MouseEvent)
    {
        if (!IgnoreEvents)
            OnMouseEvent(_MouseEvent);

        foreach (Node child in Children)
        {
            child.ProcessMouseEvent(_MouseEvent);
        }
    }

    public void Remove()
    {
        m_ToRemove.Add(this);
    }

    public static void RemoveScheduled()
    {
        foreach (Node node in m_ToRemove)
        {
            node.RemoveImmediate();
        }

        m_ToRemove.Clear();
    }

    public virtual void OnHoverEnter() { }

    public virtual void OnHoverExit() { }

    public virtual void OnPress() { }

    public virtual void OnRelease() { }

    public virtual void OnMove() { }

    public virtual void OnDrag() { }

    void OnMouseEvent(MouseEvent _MouseEvent)
    {
        if (WorldRect.Contains(_MouseEvent.Position) && !IsHovered && !IgnoreEvents)
        {
            Hovered = this;
        }

        switch (_MouseEvent.Type)
        {
            case MouseEventType.PRESSED:
                OnPress();
                break;
            case MouseEventType.RELEASED:
                OnRelease();
                break;
            case MouseEventType.MOVED:
                OnMove();
                break;
            case MouseEventType.DRAGGED:
                OnDrag();
                break;
        }
    }
}
