namespace BasicKafana;

public class ANode<T>
    where T : ANode<T>
{
    public ANode(string _ID)
    {
        ID = _ID;
        UID = UIDCount;
        UIDCount++;
    }

    public static long UIDCount = 0;

    [Serialize]
    public string ID { get; }

    [Serialize]
    public long UID { get; }

    [Serialize]
    public List<T> Children { get; } = new();

    T? m_Parent;

    public T? Parent
    {
        get => m_Parent;
        set
        {
            m_Parent = value;
            m_Parent?.LinkChild((T)this);
        }
    }

    public int Order => Parent?.Children.IndexOf((T)this) ?? 0;

    public void Remove()
    {
        Children.Clear();
        Parent?.Children.Remove((T)this);
        Parent = null;
    }

    public void LinkChild(T _INode)
    {
        Children.Add(_INode);
    }

    public void SetParent(T? _INode)
    {
        Parent = _INode;
    }

    public void Traverse(Action<T>? _Action) // breadth first search
    {
        if (_Action == null || this is not T)
            return;

        Queue<T> q = new();

        q.Enqueue((T)this);

        while (q.Count > 0)
        {
            T current = q.Dequeue();
            _Action?.Invoke(current);

            foreach (ANode<T> child in Children)
                child.Traverse(_Action);
        }
    }
}
