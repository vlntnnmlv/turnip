public class Animation
{
    static List<Animation> m_Animations = new();
    static List<Animation> m_ToDelete = new();

    static float m_Time;

    float Duration;
    float StartTime;

    Action<float>? Action;
    Action? OnFinish;

    public Animation(float _Duration, Action<float>? _Update, Action? _OnFinish)
    {
        Duration = _Duration;
        Action = _Update;
        OnFinish = _OnFinish;
    }

    public void Start()
    {
        StartTime = m_Time;
        m_Animations.Add(this);
    }

    public void Stop()
    {
        OnFinish?.Invoke();
    }

    public static void Update(float _DeltaTime)
    {
        m_Time += _DeltaTime;

        m_ToDelete.Clear();

        foreach (Animation animation in m_Animations)
        {
            animation.Action?.Invoke((m_Time - animation.StartTime) / animation.Duration);

            if (animation.Duration < m_Time)
            {
                animation.Stop();
                m_ToDelete.Add(animation);
            }
        }

        foreach (Animation animationToDelete in m_ToDelete)
        {
            m_Animations.Remove(animationToDelete);
        }
    }
}
