using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public static class Vector2Extensions
{
    public static Vector2 Normalized(this Vector2 _Vector)
    {
        return _Vector == Vector2.Zero ? Vector2.Zero : Vector2.Normalize(_Vector);
    }

    public static float Magnitude(this Vector2 _Vector)
    {
        return Vector2.Distance(Vector2.Zero, _Vector);
    }

    public static Vector2 Reversed(this Vector2 _Vector, bool _X)
    {
        Vector2 reverseVector = new Vector2(_X ? -1 : 1, _X ? 1 : -1);
        return _Vector * reverseVector;
    }
}

public static class RectangleExtensions
{
    public static Vector2 Center(this Rectangle _Rectangle)
    {
       return new Vector2(_Rectangle.X + _Rectangle.Width / 2, _Rectangle.Y + _Rectangle.Height / 2); 
    }
}

struct Box
{
    public string ID;
    public Vector2 Size;
    public Vector2 Position;
    public Vector2 Velocity;
    public Vector2 Acceleration;
    public Vector2 NetForce;
    public bool IsStatic;
    public bool HadCollisionsThisFrame;
    public float Mass;

    public Vector2 Direction => Velocity.Normalized();
    public Rectangle Rectangle => new Rectangle(Position, Size);
};

struct State
{
    public Box[] Boxes;
};

class Program
{
    const int WINDOW_WIDTH = 1080;
    const int WINDOW_HEIGHT = 720;
    const string WINDOW_TITLE = "Basic Kafana";

    const int NUM_BOXES = 15;
    const int PLAYER_SPEED = 300;

    const float PHYSICS_EPSILON = 0.01f;
    const float PHYSICS_GRAVITY = 9.81f;
    const float PHYSICS_ELASTICITY = 0.75f;
    const float PHYSICS_FRICTION_COEFFICIENT = 0.4f;
    const bool PHYSICS_ENABLE_DYNAMIC_PUSHING = false;

    static Font m_DefaultFont = Raylib.GetFontDefault();
    static State m_State;

    // SPV = shortest separation vector
    // the resulted vector must be applied to the rect "a"
    static Vector2 GetSPV(Rectangle a, Rectangle b)
    {
        float dx1 = b.X + b.Width - a.X; // from left
        float dx2 = a.X + a.Width - b.X; // from right
        float dy1 = b.Y + b.Height - a.Y; // from top
        float dy2 = a.Y + a.Height - b.Y; // from bottom

        float spvX = dx1 < dx2 ? dx1 : -dx2;
        float spvY = dy1 < dy2 ? dy1 : -dy2;

        // Choose the axis with the smallest penetration
        if (Math.Abs(spvX) < Math.Abs(spvY))
            return new Vector2(spvX, 0);
        else
            return new Vector2(0, spvY);
    }

    static State CreateState(int _NumBoxes)
    {
        State state = new State { };

        state.Boxes = new Box[_NumBoxes + 5];

        // player controled box
        state.Boxes[0] = new Box() { ID = "Player", Position = new Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Size = Vector2.One * 25, Mass = 1000 };

        // walls
        state.Boxes[1] = new Box() { ID = "WestWall", Position = new Vector2(-10, 0), Size = new Vector2(12, WINDOW_HEIGHT), IsStatic = true };
        state.Boxes[2] = new Box() { ID = "NorthWall", Position = new Vector2(0, -10), Size = new Vector2(WINDOW_WIDTH, 12), IsStatic = true };
        state.Boxes[3] = new Box() { ID = "EastWall", Position = new Vector2(WINDOW_WIDTH - 2, 0), Size = new Vector2(8, WINDOW_HEIGHT), IsStatic = true };
        state.Boxes[4] = new Box() { ID = "SouthWall", Position = new Vector2(0, WINDOW_HEIGHT - 2), Size = new Vector2(WINDOW_WIDTH, 12), IsStatic = true };

        for (uint i = 5; i < state.Boxes.Length; i++)
        {
            state.Boxes[i].ID = $"Box{i}";
            state.Boxes[i].Size = new Vector2(Raylib.GetRandomValue(30, 60), Raylib.GetRandomValue(30, 60));
            state.Boxes[i].Position = new Vector2(
                    Raylib.GetRandomValue(0, WINDOW_WIDTH - (int)state.Boxes[i].Size.X),
                    Raylib.GetRandomValue(0, WINDOW_HEIGHT - (int)state.Boxes[i].Size.Y)
                );
            state.Boxes[i].IsStatic = Raylib.GetRandomValue(0, 100) < 20;
            state.Boxes[i].Mass = state.Boxes[i].Size.X * state.Boxes[i].Size.Y;
        }

        return state;
    }


    static State GetNextState(State _State, float _DeltaTime)
    {
        State newState = _State;

        // reset net force
        for (uint i = 0; i < newState.Boxes.Length; i++)
        {
            newState.Boxes[i].NetForce = Vector2.Zero;
            newState.Boxes[i].HadCollisionsThisFrame = false;
        }

        // update boxes collisions
        {
            for (uint i = 0; i < newState.Boxes.Length; i++)
            {
                Box oneBox = newState.Boxes[i];

                for (uint j = i + 1; j < newState.Boxes.Length; j++)
                {
                    Box otherBox = newState.Boxes[j];

                    if (oneBox.IsStatic && otherBox.IsStatic)
                        continue;

                    if (!Raylib.CheckCollisionRecs(oneBox.Rectangle, otherBox.Rectangle))
                        continue;

                    newState.Boxes[i].HadCollisionsThisFrame = true;

                    Vector2 spv = GetSPV(oneBox.Rectangle, otherBox.Rectangle);

                    // resolve collision
                    {
                        float velocityMagnitudeI = newState.Boxes[i].Velocity.Magnitude();
                        float velocityMagnitudeJ = newState.Boxes[j].Velocity.Magnitude();
                        float velocityMagnitudeSumm = velocityMagnitudeI + velocityMagnitudeJ;

                        float ratio;
                        if (newState.Boxes[i].IsStatic)
                            ratio = 1;
                        else if (newState.Boxes[j].IsStatic)
                            ratio = 0;
                        else
                            ratio = velocityMagnitudeSumm != 0
                                ? velocityMagnitudeI / velocityMagnitudeSumm
                                : 0.5f;

                        newState.Boxes[i].Position += spv * (1 - ratio);
                        newState.Boxes[j].Position -= spv * ratio;
                    }

                    // apply forces on impact
                    if (i != 0 && j != 0 || PHYSICS_ENABLE_DYNAMIC_PUSHING)
                    {
                        Vector2 collisionNormal = -spv.Normalized();
                        float relativeVelocity = Vector2.Dot(newState.Boxes[i].Velocity - newState.Boxes[j].Velocity, collisionNormal);
                        float oneOverMassI = newState.Boxes[i].IsStatic ? 0 : (1 / newState.Boxes[i].Mass);
                        float oneOverMassJ = newState.Boxes[j].IsStatic ? 0 : (1 / newState.Boxes[j].Mass);

                        float impluseMagnitude = -(1 + PHYSICS_ELASTICITY) * relativeVelocity / (oneOverMassI + oneOverMassJ);
                        Vector2 impulse = impluseMagnitude * collisionNormal;

                        Vector2 forceI = impulse / _DeltaTime;
                        Vector2 forceJ = -impulse / _DeltaTime;

                        if (!newState.Boxes[i].IsStatic)
                            newState.Boxes[i].NetForce += forceI;

                        if (!newState.Boxes[j].IsStatic)
                            newState.Boxes[j].NetForce += forceJ;
                    }
                }

                if (!newState.Boxes[i].HadCollisionsThisFrame)
                {
                    // TODO: Something
                }
            }

            // update from user input
            {
                Vector2 direction = Vector2.Zero;

                if (Raylib.IsKeyDown(KeyboardKey.W))
                    direction += new Vector2(0, -1);
                if (Raylib.IsKeyDown(KeyboardKey.A))
                    direction += new Vector2(-1, 0);
                if (Raylib.IsKeyDown(KeyboardKey.S))
                    direction += new Vector2(0, 1);
                if (Raylib.IsKeyDown(KeyboardKey.D))
                    direction += new Vector2(1, 0);

                // newState.Boxes[0].NetForce += direction * 100000;

                Vector2 newPosition = newState.Boxes[0].Position + direction.Normalized() * PLAYER_SPEED * _DeltaTime;
                Vector2 newVelocity = (newPosition - newState.Boxes[0].Position) / _DeltaTime;

                newState.Boxes[0].Velocity = newVelocity;
                newState.Boxes[0].Position = newPosition;
            }
        }

        // update kinematics
        for (uint i = 0; i < newState.Boxes.Length; i++)
        {
            if (newState.Boxes[i].IsStatic)
                continue;

            float frictionMagnitude = newState.Boxes[i].Mass * PHYSICS_GRAVITY * PHYSICS_FRICTION_COEFFICIENT;

            if (newState.Boxes[i].Velocity.Magnitude() > PHYSICS_EPSILON)
            {
                newState.Boxes[i].NetForce += frictionMagnitude * -newState.Boxes[i].Direction;
            }

            newState.Boxes[i].Acceleration = newState.Boxes[i].NetForce / newState.Boxes[i].Mass;

            if (newState.Boxes[i].Acceleration.Magnitude() < PHYSICS_EPSILON)
                newState.Boxes[i].Acceleration = Vector2.Zero;

            newState.Boxes[i].Velocity += newState.Boxes[i].Acceleration * _DeltaTime;

            if (newState.Boxes[i].Velocity.Magnitude() < PHYSICS_EPSILON)
                newState.Boxes[i].Velocity = Vector2.Zero;

            newState.Boxes[i].Position += newState.Boxes[i].Velocity * _DeltaTime;
        }

        return newState;
    }

    static void DrawLabel(string _Text, Vector2 _Position, Color _Color, int _FontSize = 16)
    {
        int textWidth = Raylib.MeasureText(_Text, _FontSize);
        Raylib.DrawTextEx(m_DefaultFont, _Text, _Position - new Vector2(textWidth / 2, 0), _FontSize, 5, _Color);
    }

    static void DrawArrow(Vector2 _Start, Vector2 _End, Color _Color, int _Thickness = 2)
    {
        Raylib.DrawLineEx(_Start, _End, _Thickness, _Color);
    }

    static void DrawBox(Box _Box, Color? _Color = null)
    {
        Color color = _Color.HasValue
            ? _Color.Value
            : _Box.IsStatic
                ? Color.Red
                : Color.Blue;

        if (_Box.IsStatic)
            Raylib.DrawRectangle((int)_Box.Rectangle.X, (int)_Box.Rectangle.Y, (int)_Box.Rectangle.Width, (int)_Box.Rectangle.Height, color);
        else
            Raylib.DrawRectangleLinesEx(_Box.Rectangle, 6, color);

        Vector2 center = _Box.Rectangle.Center();
        if (_Box.Velocity != Vector2.Zero)
        {
            DrawArrow(center, center + _Box.Velocity, Color.DarkPurple, 5);
            DrawLabel("Velocity", center + _Box.Velocity, Color.DarkPurple);
        }

        if (_Box.Acceleration != Vector2.Zero)
        {
            DrawArrow(center, center + _Box.Acceleration, Color.Green, 3);
            DrawLabel("Acceleration", center + _Box.Acceleration, Color.Green);
        }

        if (_Box.NetForce != Vector2.Zero)
        {
            DrawArrow(center, center + _Box.NetForce, Color.Black, 1);
            DrawLabel("NetForce", center + _Box.NetForce, Color.Black);
        }
    }

    static void DrawState(State _State)
    {
        for (uint i = 0; i < _State.Boxes.Length; i++)
        {
            DrawBox(_State.Boxes[i], i == 0 ? Color.Green : null);
        }
    }

    // TODO: Figure our wth is this?
    // STAThread is required if you deploy using NativeAOT on Windows - See https://github.com/raylib-cs/raylib-cs/issues/301
    [STAThread]
    public static void Main()
    {
        Raylib.InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

        Random random = new Random();
        random.Next();
        Raylib.SetRandomSeed((uint)random.Next());

        m_State = CreateState(NUM_BOXES);

        while (!Raylib.WindowShouldClose())
        {

            if (Raylib.IsKeyPressed(KeyboardKey.R))
                m_State = CreateState(NUM_BOXES);

            Raylib.BeginDrawing();
            Raylib.ClearBackground(Color.RayWhite);

            DrawState(m_State);

            Raylib.EndDrawing();

            m_State = GetNextState(m_State, Raylib.GetFrameTime());
        }

        Raylib.CloseWindow();
    }
}