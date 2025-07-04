namespace BasicKafana;

public class Node<T>(string _ID)
{
    public string ID { get; } = _ID;
    public T? Data { get; }

    public List<Node<T>> Children { get; } = [];
    public Node<T>? Parent { get; set; }

    public override string ToString()
    {
        return $"[{ID}] {Data}";
    }

    public Node(string _ID, params object[] _Arguments) : this(_ID)
    {
        Type dataType = typeof(T);

        Data = dataType.IsPrimitive ? (T)_Arguments[0] : (T?)Activator.CreateInstance(dataType, _Arguments);
    }

    public void LinkChild(Node<T> _Node)
    {
        Children.Add(_Node);
        _Node.SetParent(this);
    }

    public void SetParent(Node<T> _Node)
    {
        Parent = _Node;
    }

    public void Traverse(Action<Node<T>>? _Action)
    {
        if (_Action == null)
            return;

        Queue<Node<T>> q = new();

        q.Enqueue(this);

        while (q.Count > 0)
        {
            Node<T> current = q.Dequeue();
            _Action?.Invoke(current);

            foreach (Node<T> child in Children)
                child.Traverse(_Action);
        }
    }
}