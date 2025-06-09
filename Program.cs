using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

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

    // TODO:
    // 1. Handle user input → compute desired velocities / forces
    // 2. Apply forces → update velocity from acceleration (F = ma)
    // 3. Integrate velocity → predict new positions (without committing)
    // 4. Detect collisions (between predicted positions)
    // 5. Resolve collisions → adjust velocities & positions
    // 6. Finalize position → apply resolved positions
    static State GetNextState(State _State, float _DeltaTime)
    {
        State newState = _State;

        // reset physics
        for (uint i = 0; i < newState.Boxes.Length; i++)
        {
            newState.Boxes[i].NetForce = Vector2.Zero;
            newState.Boxes[i].HadCollisionsThisFrame = false;
        }

        // resolve froces and collisions
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