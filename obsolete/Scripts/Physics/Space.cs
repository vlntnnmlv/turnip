using System.Numerics;

namespace Turnip;

public struct PhysicsRules
{
    public float Elasticity;
    public float Gravity;
    public float Friction;
    public float Epsilon;
}

public class Space(PhysicsRules? _Rules = null)
{
    private PhysicsRules m_Rules = _Rules.HasValue ? _Rules.Value : GetDefaultPhysicsRules();
    private readonly List<Collision> m_Collisions = [];

    private static PhysicsRules GetDefaultPhysicsRules()
    {
        return new PhysicsRules
        {
            Elasticity = 0.9f,
            Gravity = 9.81f,
            Friction = 0.1f,
            Epsilon = 0.01f,
        };
    }

    public Box[] Simulate(Box[] _State, float _DeltaTime)
    {
        Box[] newState = new Box[_State.Length];
        Array.Copy(_State, 0, newState, 0, _State.Length);

        // reset collisions
        m_Collisions.Clear();

        // save collisions and apply forces on impact
        {
            for (int i = 0; i < newState.Length; i++)
            {
                Box oneBox = newState[i];

                for (int j = i + 1; j < newState.Length; j++)
                {
                    Box otherBox = newState[j];

                    if (oneBox.IsStatic && otherBox.IsStatic)
                        continue;

                    Collision collision = Collision.SAT(oneBox.Collider, otherBox.Collider, i, j);
                    if (!collision.IsValid)
                        continue;

                    m_Collisions.Add(collision);
                    newState[i].HadCollisionsThisFrame = true;

                    {
                        float relativeVelocity = Vector2.Dot(
                            newState[i].Velocity - newState[j].Velocity,
                            collision.MTVDirection.Reversed(collision.MTVDirection.X != 0)
                        ); //Normal
                        float oneOverMassI = newState[i].IsStatic ? 0 : (1 / newState[i].Mass);
                        float oneOverMassJ = newState[j].IsStatic ? 0 : (1 / newState[j].Mass);

                        float impluseMagnitude =
                            -(1 + m_Rules.Elasticity)
                            * relativeVelocity
                            / (oneOverMassI + oneOverMassJ);
                        Vector2 impulse = impluseMagnitude * collision.MTVDirection;

                        Vector2 forceI =
                            j == 0 ? impulse.Normalized() * 500000 : impulse / _DeltaTime;
                        Vector2 forceJ =
                            i == 0 ? -impulse.Normalized() * 500000 : -impulse / _DeltaTime;

                        if (!newState[i].IsStatic)
                            newState[i].NetForce += forceI;

                        if (!newState[j].IsStatic)
                            newState[j].NetForce += forceJ;
                    }
                }
            }
        }

        // update kinematics
        for (uint i = 0; i < newState.Length; i++)
        {
            if (newState[i].IsStatic)
                continue;

            float frictionMagnitude = newState[i].Mass * m_Rules.Gravity * m_Rules.Friction;

            if (newState[i].Velocity.Magnitude() > m_Rules.Epsilon)
            {
                newState[i].NetForce += frictionMagnitude * -newState[i].Direction;
            }

            newState[i].Acceleration = newState[i].NetForce / newState[i].Mass;

            if (newState[i].Acceleration.Magnitude() < m_Rules.Epsilon)
                newState[i].Acceleration = Vector2.Zero;

            newState[i].Velocity += newState[i].Acceleration * _DeltaTime;

            if (newState[i].Velocity.Magnitude() < m_Rules.Epsilon)
                newState[i].Velocity = Vector2.Zero;

            newState[i].Position += newState[i].Velocity * _DeltaTime;
        }

        // resolve collisions
        foreach (Collision collision in m_Collisions)
        {
            float velocityMagnitudeI = newState[collision.IndexOne].Velocity.Magnitude();
            float velocityMagnitudeJ = newState[collision.IndexOther].Velocity.Magnitude();
            float velocityMagnitudeSumm = velocityMagnitudeI + velocityMagnitudeJ;

            float ratio;
            if (newState[collision.IndexOne].IsStatic)
                ratio = 1;
            else if (newState[collision.IndexOther].IsStatic)
                ratio = 0;
            else
                ratio =
                    velocityMagnitudeSumm != 0 && collision.IndexOne != 0
                        ? velocityMagnitudeI / velocityMagnitudeSumm
                        : 0.5f;

            newState[collision.IndexOne].Position +=
                collision.MTVMagnitude * collision.MTVDirection * (1 - ratio);
            newState[collision.IndexOther].Position -=
                collision.MTVMagnitude * collision.MTVDirection * ratio;

            newState[collision.IndexOne].HP -= 20 * (1 - ratio);
            newState[collision.IndexOther].HP -= 20 * ratio;
        }

        // reset forces
        for (uint i = 0; i < newState.Length; i++)
            newState[i].NetForce = Vector2.Zero;

        return newState;
    }

    public Collision[] GetCollisionsThisFrame()
    {
        return [.. m_Collisions];
    }
}
