namespace BasicKafana;

public class Node(string _ID, Meristem _Meristem)
{
    public string ID { get; } = _ID;
    public Meristem Meristem { get; } = _Meristem;

    public List<Node> Children { get; } = [];
    public Node? Parent { get; set; }

    public override string ToString()
    {
        return $"[{ID}] {Meristem}";
    }

    public void LinkChild(Node _Node)
    {
        Children.Add(_Node);
        _Node.SetParent(this);
    }

    public void SetParent(Node _Node)
    {
        Parent = _Node;
    }

    public void Traverse(Action<Node>? _Action) // breadth firsrt search
    {
        if (_Action == null)
            return;

        Queue<Node> q = new();

        q.Enqueue(this);

        while (q.Count > 0)
        {
            Node current = q.Dequeue();
            _Action?.Invoke(current);

            foreach (Node child in Children)
                child.Traverse(_Action);
        }
    }
}