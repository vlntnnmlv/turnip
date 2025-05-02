using System.Collections.Generic;
using Microsoft.Xna.Framework;

using basic_kafana;

public class CharacterManager
{
    List<Character> _characters = new();
    Node _visitorSpawn;
    List<ITarget> _visitorTargets;
    double _lastVisitorSpawnedTime;
    double _visitorSpawnPeriod = 2.0;

    public Character ManualCharacter { get; private set; }

    public CharacterManager(Kafana game)
    {
        _lastVisitorSpawnedTime = 0.0;
        _visitorSpawn = new Node("spawn", Node.Root, AssetManager.GetTexture("point"), new Vector2(game.Width + 100, game.Height / 2 + 300), Vector2.One * 16);

        ITarget entranceTarget = new Node("entranceTarget", Node.Root, AssetManager.GetTexture("point"), new Vector2(5 * game.Width / 8, game.Height / 2), Vector2.One * 8);
        ITarget desk = new Desk("desk", AssetManager.GetTexture("desk"), new Vector2(5 * game.Width / 8, game.Height / 4 - 32), new Vector2(128, 64));
        _visitorTargets = new List<ITarget>() { entranceTarget, desk };
    }

    public void CreateManualCharacter(Vector2 position)
    {
        var directionControllerManual = new MovementControllerManual();
        ManualCharacter = new Character(
                    "manualCharacter",
                    Node.Root,
                    GetRandomCharacterInfo(),
                    AssetManager.GetTexture("player"),
                    position,
                    directionControllerManual,
                    150
                );
        directionControllerManual.Init(ManualCharacter);
    }

    public void CreateVisitorCharacter()
    {
        var movementControllerPath = new MovementControllerPath();
        Character visitorCharacter = new Character(
                    $"character_{_characters.Count}",
                    Node.Root,
                    GetRandomCharacterInfo(),
                    AssetManager.GetTexture("visitor"),
                    _visitorSpawn.Position,
                    movementControllerPath,
                    200
                );
        movementControllerPath.Init(visitorCharacter, visitorCharacter.Size.X);
        movementControllerPath.OnAgentReachedTarget += OnVisitorReachedTarget;

        foreach (ITarget target in _visitorTargets)
        {
            movementControllerPath.AddTarget(target);
        }

        _characters.Add(visitorCharacter);
    }

    void OnVisitorReachedTarget(IAgent agent, ITarget target)
    {
        if (target is Desk desk)
        {
            desk.AddVisitorToQueue(agent as Character);
        }
    }

    public void Update(GameTime gameTime)
    {
        if (gameTime.TotalGameTime.Seconds - _lastVisitorSpawnedTime > _visitorSpawnPeriod
            && _characters.Count < 5)
        {
            _lastVisitorSpawnedTime = gameTime.TotalGameTime.Seconds;
            CreateVisitorCharacter();
        }
    }

    public CharacterInfo GetRandomCharacterInfo()
    {
        return new CharacterInfo()
        {
            Name = "Billy Bob",
            Surname = "Turnton",
            Age = RandomManager.RandInt(6, 121),
            Weight = RandomManager.RandInt(25, 140)
        };
    }
}