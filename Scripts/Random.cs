using System;

namespace basic_kafana;

public static class RandomManager
{
    static Random _random;

    public static void Init()
    {
        _random = new Random();
    }

    public static int RandInt(int low, int high)
    {
        return _random.Next(low, high);
    }
}