// ========== [1] Abstract Products ==========

public interface IEdibleGift
{
	string GetName();
}

public interface IUnedibleGift
{
	string GetName();
}

// ========== [2] Concrete Products ==========

public class Candy : IEdibleGift
{
	public string GetName() => "Sweet Candy";
}

public class Toy : IUnedibleGift
{
	public string GetName() => "Toy";
}

public class BitterPill : IEdibleGift
{
	public string GetName() => "Bitter Pill";
}

public class Rizhky : IUnedibleGift
{
	public string GetName() => "Birch Rods";
}

// ========== [3] Abstract Factory ==========

public interface IGiftFactory
{
	IEdibleGift CreateEdibleGift();
	IUnedibleGift CreateUnedibleGift();
}

// ========== [4] Concrete Factories ==========

public class GoodGiftFactory : IGiftFactory
{
	public IEdibleGift CreateEdibleGift()
	{
		return new Candy();
	}

	public IUnedibleGift CreateUnedibleGift()
	{
		return new Toy();
	}
}

public class BadGiftFactory : IGiftFactory
{
	public IEdibleGift CreateEdibleGift()
	{
		return new BitterPill();
	}

	public IUnedibleGift CreateUnedibleGift()
	{
		return new Rizhky();
	}
}

// ========== [5] Child class ==========

public class Child
{
	public int GoodDeeds { get; }
	public int BadDeeds { get; }

	public Child(int goodDeeds, int badDeeds)
	{
		GoodDeeds = goodDeeds;
		BadDeeds = badDeeds;
	}
}

// ========== [6] Iterator Interfaces & Implementations ==========

public interface IChildIterator
{
	bool MoveNext();
	Child Current { get; }
	void Reset();
}

public class ChildrenIterator : IChildIterator
{
	private List<Child> _children;
	private int _position = -1;

	public ChildrenIterator(List<Child> children)
	{
		_children = children;
	}

	public bool MoveNext()
	{
		_position++;
		return (_position < _children.Count);
	}

	public Child Current
	{
		get
		{
			if (_position < 0 || _position >= _children.Count)
				throw new InvalidOperationException();
			return _children[_position];
		}
	}

	public void Reset()
	{
		_position = -1;
	}
}

// ========== [7] Children Collection ==========

public class ChildrenCollection
{
	private List<Child> _children = new List<Child>();

	public void AddChild(Child child)
	{
		_children.Add(child);
	}

	public IChildIterator CreateIterator()
	{
		return new ChildrenIterator(_children);
	}
}

// ========== [8] Singleton: SaintNicholas ==========

public sealed class SaintNicholas
{
	private static SaintNicholas _instance = null;
	private static readonly object _lock = new object();

	private ChildrenCollection _children = new ChildrenCollection();

	private SaintNicholas() { }

	public static SaintNicholas Instance
	{
		get
		{
			lock (_lock)
			{
				if (_instance == null)
					_instance = new SaintNicholas();
			}
			return _instance;
		}
	}

	public void AddChild(Child child)
	{
		_children.AddChild(child);
	}

	public void DistributeGifts()
	{
		IChildIterator iterator = _children.CreateIterator();

		while (iterator.MoveNext())
		{
			Child currentChild = iterator.Current;

			IGiftFactory factory;
			if (currentChild.GoodDeeds >= currentChild.BadDeeds)
			{
				factory = new GoodGiftFactory();
			}
			else
			{
				factory = new BadGiftFactory();
			}

			var edibleGift = factory.CreateEdibleGift();
			var unedibleGift = factory.CreateUnedibleGift();

			Console.WriteLine($"Child (Good={currentChild.GoodDeeds}, Bad={currentChild.BadDeeds}) receives:");
			Console.WriteLine($"  1) {edibleGift.GetName()}");
			Console.WriteLine($"  2) {unedibleGift.GetName()}");
			Console.WriteLine("--------------------------------");
		}
	}
}

// ========== [9] Main program to demonstrate pattern usage ==========

public class Program
{
	public static void Main(string[] args)
	{
		SaintNicholas stNicholas = SaintNicholas.Instance;

		// Add children with various good/bad deed counts
		stNicholas.AddChild(new Child(5, 2));  // more good deeds
		stNicholas.AddChild(new Child(1, 3));  // more bad deeds
		stNicholas.AddChild(new Child(4, 4));  // equal, but >= means good gifts
		stNicholas.AddChild(new Child(0, 10)); // only bad deeds

		stNicholas.DistributeGifts();

		Console.WriteLine("\nPress any key to exit...");
		Console.ReadKey();
	}
}
