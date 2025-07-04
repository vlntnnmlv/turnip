using Raylib_cs;

namespace BasicKafana;

public static class Garden
{
    public static bool DebugPhysics { get; set; }
    public static bool DebugUI { get; set; }// = true;
    public static Font Font { get; } = Raylib.GetFontDefault();
}