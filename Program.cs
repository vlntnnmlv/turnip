#define DEBUG_PHYSICS

using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public static class Globals
{
    public const int WINDOW_WIDTH = 1080;
    public const int WINDOW_HEIGHT = 720;
}

class Program
{
    const int WINDOW_WIDTH = 1080;
    const int WINDOW_HEIGHT = 720;
    const string WINDOW_TITLE = "Basic Kafana";

    const int NUM_BOXES = 1;
    const int PLAYER_SPEED = 300;

    const float PHYSICS_EPSILON = 0.01f;
    const float PHYSICS_GRAVITY = 9.81f;
    const float PHYSICS_ELASTICITY = 0.9f;
    const float PHYSICS_FRICTION_COEFFICIENT = 0.1f;
    const bool PHYSICS_ENABLE_PLAYER_DYNAMIC_PUSHING = false;

    static State m_State;
    static List<Collision> m_Collisions = new();

    static State CreateState(int _NumBoxes)
    {
        State state = new State { };

        state.Boxes = new Box[_NumBoxes + 5];

        // player controled box
        state.Boxes[0] = new Box() { Index = 0, ID = "Player", Position = new Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Size = Vector2.One * 50, Mass = 75 };

        // walls
        state.Boxes[1] = new Box() { Index = 1, ID = "WestWall", Position = new Vector2(-10, 0), Size = new Vector2(12, WINDOW_HEIGHT), IsStatic = true };
        state.Boxes[2] = new Box() { Index = 2, ID = "NorthWall", Position = new Vector2(0, -10), Size = new Vector2(WINDOW_WIDTH, 12), IsStatic = true };
        state.Boxes[3] = new Box() { Index = 3, ID = "EastWall", Position = new Vector2(WINDOW_WIDTH - 2, 0), Size = new Vector2(8, WINDOW_HEIGHT), IsStatic = true };
        state.Boxes[4] = new Box() { Index = 4, ID = "SouthWall", Position = new Vector2(0, WINDOW_HEIGHT - 2), Size = new Vector2(WINDOW_WIDTH, 12), IsStatic = true };

        for (uint i = 5; i < state.Boxes.Length; i++)
        {
            state.Boxes[i].Index = i;
            state.Boxes[i].ID = $"Box{i}";
            state.Boxes[i].Size = new Vector2(75, 75); //new Vector2(Raylib.GetRandomValue(69, 169), Raylib.GetRandomValue(69, 169));
            state.Boxes[i].Position = new Vector2(
                    Raylib.GetRandomValue(0, WINDOW_WIDTH - (int)state.Boxes[i].Size.X),
                    Raylib.GetRandomValue(0, WINDOW_HEIGHT - (int)state.Boxes[i].Size.Y)
                );
            state.Boxes[i].IsStatic = false; //Raylib.GetRandomValue(0, 100) < 20;
            state.Boxes[i].Mass = (float)Math.Sqrt(state.Boxes[i].Size.X * state.Boxes[i].Size.Y);
        }

        return state;
    }

    static State GetNextState(State _State, float _DeltaTime)
    {
        State newState = _State;

        // update from user input
        {
            // movement
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

                Vector2 newPosition = newState.Boxes[0].Position + direction.Normalized() * PLAYER_SPEED * _DeltaTime;

                newState.Boxes[0].Position = newPosition;

                if (direction != Vector2.Zero)
                    newState.Boxes[0].Velocity = Vector2.Zero;
            }

            // kick force
            {
                Vector2 mousePosition = Raylib.GetMousePosition();

                newState.ManualControl = mousePosition - newState.Boxes[0].Rectangle.Center();

                for (uint i = 1; i < _State.Boxes.Length; i++)
                {
                    if (_State.Boxes[i].Rectangle.Contains(mousePosition) && Raylib.IsKeyPressed(KeyboardKey.Space))
                    {
                        _State.Boxes[i].NetForce += newState.ManualControl.Normalized() * 5000 / _DeltaTime;
                    }
                }
            }
        }

        // save collisions and apply forces on impact
        {
            for (uint i = 0; i < newState.Boxes.Length; i++)
            {
                Box oneBox = newState.Boxes[i];

                for (uint j = i + 1; j < newState.Boxes.Length; j++)
                {
                    Box otherBox = newState.Boxes[j];

                    if (oneBox.IsStatic && otherBox.IsStatic)
                        continue;

                    Collision collision = new Collision(oneBox, otherBox);
                    if (!collision.IsValid)
                        continue;

                    m_Collisions.Add(collision);
                    newState.Boxes[i].HadCollisionsThisFrame = true;

                    {
                        float relativeVelocity = Vector2.Dot(newState.Boxes[i].Velocity - newState.Boxes[j].Velocity, collision.Normal);
                        float oneOverMassI = newState.Boxes[i].IsStatic ? 0 : (1 / newState.Boxes[i].Mass);
                        float oneOverMassJ = newState.Boxes[j].IsStatic ? 0 : (1 / newState.Boxes[j].Mass);

                        float impluseMagnitude = -(1 + PHYSICS_ELASTICITY) * relativeVelocity / (oneOverMassI + oneOverMassJ);
                        Vector2 impulse = impluseMagnitude * collision.Normal;

                        Vector2 forceI = j == 0 ? impulse.Normalized() * 500000 : impulse / _DeltaTime;
                        Vector2 forceJ = i == 0 ? -impulse.Normalized() * 500000 : -impulse / _DeltaTime;

                        if (!newState.Boxes[i].IsStatic)
                            newState.Boxes[i].NetForce += forceI;

                        if (!newState.Boxes[j].IsStatic)
                            newState.Boxes[j].NetForce += forceJ;
                    }
                }
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

        // resolve collisions
        foreach (Collision collision in m_Collisions)
        {
            float velocityMagnitudeI = collision.One.Velocity.Magnitude();
            float velocityMagnitudeJ = collision.Other.Velocity.Magnitude();
            float velocityMagnitudeSumm = velocityMagnitudeI + velocityMagnitudeJ;

            float ratio;
            if (collision.One.IsStatic)
                ratio = 1;
            else if (collision.Other.IsStatic)
                ratio = 0;
            else
                ratio = velocityMagnitudeSumm != 0 && collision.One.Index != 0
                    ? velocityMagnitudeI / velocityMagnitudeSumm
                    : 0.5f;

            newState.Boxes[collision.One.Index].Position += collision.SPV * (1 - ratio);
            newState.Boxes[collision.Other.Index].Position -= collision.SPV * ratio;

            newState.Boxes[collision.One.Index].HP -= 20 * (1 - ratio);
            newState.Boxes[collision.Other.Index].HP -= 20 * ratio;
        }

        // reset physics
        m_Collisions.Clear();
        for (uint i = 0; i < newState.Boxes.Length; i++)
            newState.Boxes[i].NetForce = Vector2.Zero;

        return newState;
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

            Renderer.DrawState(m_State);

            Raylib.EndDrawing();

            m_State = GetNextState(m_State, Raylib.GetFrameTime());
        }

        Raylib.CloseWindow();
    }
}