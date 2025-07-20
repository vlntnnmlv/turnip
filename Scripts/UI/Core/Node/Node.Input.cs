namespace Turnip;

public partial class Node
{
    public bool IgnoreEvents { get; set; }

    public void ProcessMouseEvent(MouseEvent _MouseEvent)
    {
        if (!IgnoreEvents)
            OnMouseEvent(_MouseEvent);

        foreach (Node child in Children)
        {
            child.ProcessMouseEvent(_MouseEvent);
        }
    }

    static List<Node> m_ToRemove = new();

    public static void ScheduleToRemove(Node _Node)
    {
        m_ToRemove.Add(_Node);
    }

    public static void RemoveScheduled()
    {
        foreach (Node node in m_ToRemove)
        {
            node.Remove();
        }

        m_ToRemove.Clear();
    }

    public void OnMouseEvent(MouseEvent _MouseEvent)
    {
        if (WorldRect.Contains(_MouseEvent.Position) && !IsHovered)
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

    public virtual void OnHoverEnter() { }

    public virtual void OnHoverExit() { }

    public virtual void OnPress() { }

    public virtual void OnRelease() { }

    public virtual void OnMove() { }

    public virtual void OnDrag() { }
}
