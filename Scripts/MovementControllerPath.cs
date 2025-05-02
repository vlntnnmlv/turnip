using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;

namespace basic_kafana;

public class MovementControllerPath : IMovementController
{
    public Vector2 Direction { get; private set; }
    public event Action<IAgent, ITarget> OnAgentReachedTarget;

    private float _rotation;
    private float _stopDistance;

    private IAgent _agent;
    private Queue<ITarget> _targets = new();

    #nullable enable
    private ITarget? _currentTarget = null;
    #nullable disable

    public void Init(IAgent agent, float stopDistance = 16.0f)
    {
        _agent = agent;
        _stopDistance = stopDistance;
    }

    public void AddTarget(ITarget target)
    {
        _targets.Enqueue(target);
    }

    public Vector2 GetDirection()
    {
        if (_currentTarget == null)
        {
            if (_targets.Count == 0)
                return Vector2.Zero;

            _currentTarget = _targets.Dequeue();
        }

        Vector2 distance = _currentTarget.TargetPosition - _agent.Position;
        Vector2 realDistance = distance.Length() < _stopDistance ? Vector2.Zero : distance;
        if (realDistance == Vector2.Zero)
        {
            OnAgentReachedTarget.Invoke(_agent, _currentTarget);
            _currentTarget = null;
        }
        else
            realDistance.Normalize();

        Direction = realDistance;
        return Direction;
    }

    public float GetRotation()
    {
        if (Direction.Length() != 0)
        {
            // texture is oriented upwards, so up direction is no rotation
            _rotation = MathF.Atan2(Direction.Y , Direction.X) + MathF.PI / 2;
        }

        return _rotation;
    }
}