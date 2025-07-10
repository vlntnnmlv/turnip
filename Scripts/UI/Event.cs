using System.Numerics;
using BasicKafana;

public enum MouseState
{
    UP = 0,
    DOWN = 1,
}

public static class Event
{
    public static event Action<Vector2>? MousePressed;
    public static event Action<Vector2>? MouseReleased;
    public static event Action<Vector2>? MouseDragged;

    public static MouseState MouseState;

    public static Vector2 MousePosition;

    public static void Process(Vector2 _MousePosition, bool _MousePressed, bool _MouseReleased)
    {
        if (MouseState == MouseState.UP && _MousePressed)
            MousePressed?.Invoke(_MousePosition);

        if (MouseState == MouseState.DOWN && _MouseReleased)
            MouseReleased?.Invoke(_MousePosition);

        if (
            _MousePressed
            && MouseState == MouseState.DOWN
            && (MousePosition - _MousePosition).Magnitude() > 5
        )
        {
            MouseDragged?.Invoke(_MousePosition);
        }

        if (_MousePressed)
        {
            MouseState = MouseState.DOWN;
        }
        else if (_MouseReleased)
        {
            MouseState = MouseState.UP;
        }

        MousePosition = _MousePosition;
    }
}
