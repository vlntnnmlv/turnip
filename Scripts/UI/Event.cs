using System.Numerics;
using BasicKafana;

public enum MouseEventType
{
    PRESSED = 0,
    RELEASED = 1,
    MOVED = 2,
    CLICKED = 3,
    DRAGGED = 4,
}

public struct MouseEvent
{
    public MouseEventType Type;
    public Vector2 Position;

    public MouseEvent(MouseEventType _Type, Vector2 _Position)
    {
        Type = _Type;
        Position = _Position;
    }
}

public static class Event
{
    public static event Action<MouseEvent>? MouseEvent;

    public static Vector2 MousePosition => m_MousePosition;
    public static bool IsMouseDown => m_IsDown;

    private static Vector2 m_MousePosition;
    private static Vector2 m_MousePreviousPosition;
    private static Vector2 m_MousePressPosition;
    private static bool m_IsDown;
    private static bool m_IsDragging;
    private static float m_DragThreshold = 5f;

    public static void ProcessMouse(Vector2 _Position, bool _Pressed, bool _Released)
    {
        m_MousePreviousPosition = m_MousePosition;
        m_MousePosition = _Position;

        float moveDistance = (m_MousePosition - m_MousePreviousPosition).Magnitude();
        if (_Pressed)
        {
            m_IsDown = true;
            m_MousePressPosition = _Position;
            m_IsDragging = false;

            MouseEvent?.Invoke(new(MouseEventType.PRESSED, m_MousePosition));
        }

        if (moveDistance > 0)
        {
            MouseEvent?.Invoke(new(MouseEventType.MOVED, m_MousePosition));

            if (m_IsDown && !m_IsDragging && moveDistance > m_DragThreshold)
            {
                m_IsDragging = true;
                MouseEvent?.Invoke(new(MouseEventType.DRAGGED, m_MousePosition));
            }
        }

        if (_Released)
        {
            m_IsDown = false;

            MouseEvent?.Invoke(new(MouseEventType.RELEASED, m_MousePosition));

            if (!m_IsDragging)
                MouseEvent?.Invoke(new(MouseEventType.CLICKED, m_MousePosition));

            m_IsDragging = false;
        }
    }
}
