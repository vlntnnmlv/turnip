using System.Numerics;

namespace BasicKafana;

public enum MouseEventType
{
    PRESSED = 0,
    RELEASED = 1,
    MOVED = 2,
    DRAGGED = 3,
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
    public static event Action<MouseEvent>? OnMouseEvent;

    private static Vector2 m_MousePreviousPosition;
    private static bool m_WasMouseDown;

    public static void ProcessMouse(Vector2 _Position, bool _Pressed, bool _Released)
    {
        bool isMoving = _Position != m_MousePreviousPosition;
        bool wasPressed = m_WasMouseDown;
        bool isPressed = _Pressed || (wasPressed && !_Released);
        bool isDragging = isMoving && isPressed;

        if (_Pressed)
        {
            OnMouseEvent?.Invoke(new MouseEvent(MouseEventType.PRESSED, _Position));
        }

        if (_Released)
        {
            OnMouseEvent?.Invoke(new MouseEvent(MouseEventType.RELEASED, _Position));
        }

        if (isDragging)
        {
            OnMouseEvent?.Invoke(new MouseEvent(MouseEventType.DRAGGED, _Position));
        }
        else if (isMoving)
        {
            OnMouseEvent?.Invoke(new MouseEvent(MouseEventType.MOVED, _Position));
        }

        m_MousePreviousPosition = _Position;
        m_WasMouseDown = isPressed;
    }
}
