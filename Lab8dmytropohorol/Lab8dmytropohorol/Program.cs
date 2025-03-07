using System;
using System.IO;
using System.Text;

namespace BuildingSecuritySystem
{
	// abstract base classes for sensors and devices
	public abstract class Sensor
	{
		public string SensorName { get; set; }
		public bool IsTriggered { get; protected set; }

		public abstract void CheckSensor();

		public override string ToString()
		{
			return $"Sensor: {SensorName}, Triggered: {IsTriggered}";
		}
	}

	public abstract class Device
	{
		public string DeviceName { get; set; }
		public bool IsEnabled { get; protected set; }

		public abstract void Activate();
		public abstract void Deactivate();

		public override string ToString()
		{
			return $"Device: {DeviceName}, Enabled: {IsEnabled}";
		}
	}

	// concrete sensors classes
	public class TemperatureSensor : Sensor
	{
		public double TemperatureThreshold { get; set; }

		public TemperatureSensor(string name, double threshold)
		{
			SensorName = name;
			TemperatureThreshold = threshold;
			IsTriggered = false;
		}

		public override void CheckSensor()
		{
			// Dummy check – in real scenario you'd measure temperature. //TODO
			double currentTemperature = new Random().NextDouble() * 100;
			if (currentTemperature >= TemperatureThreshold)
			{
				IsTriggered = true;
			}
		}
	}

	public class LightSensor : Sensor
	{
		public int LightLevelThreshold { get; set; }

		public LightSensor(string name, int threshold)
		{
			SensorName = name;
			LightLevelThreshold = threshold;
			IsTriggered = false;
		}

		public override void CheckSensor()
		{
			// Dummy check – e.g., measure brightness 0–100 //TODO
			int currentLight = new Random().Next(0, 101);
			if (currentLight >= LightLevelThreshold)
			{
				IsTriggered = true;
			}
		}
	}

	public class MovementSensor : Sensor
	{
		public MovementSensor(string name)
		{
			SensorName = name;
			IsTriggered = false;
		}

		public override void CheckSensor()
		{
			// Dummy check – random chance of triggering //TODO
			IsTriggered = (new Random().Next(0, 10) > 6);
		}
	}

	// concrete device classes
	public class Camera : Device
	{
		public Camera(string name)
		{
			DeviceName = name;
			IsEnabled = false;
		}

		public override void Activate() { IsEnabled = true; }

		public override void Deactivate() { IsEnabled = false; }
	}

	public class Vent : Device
	{
		public Vent(string name)
		{
			DeviceName = name;
			IsEnabled = false;
		}

		public override void Activate() { IsEnabled = true; }

		public override void Deactivate() { IsEnabled = false; }
	}

	public class Lamp : Device
	{
		public Lamp(string name)
		{
			DeviceName = name;
			IsEnabled = false;
		}

		public override void Activate() { IsEnabled = true; }

		public override void Deactivate() { IsEnabled = false; }
	}

	public class Room
	{
		private const int MaxSensors = 5;
		private const int MaxDevices = 5;

		private Sensor[] _sensors;
		private Device[] _devices;
		private int _sensorCount;
		private int _deviceCount;

		public Room()
		{
			_sensors = new Sensor[MaxSensors];
			_devices = new Device[MaxDevices];
			_sensorCount = 0;
			_deviceCount = 0;
		}

		public static Room operator +(Room r, Sensor s)
		{
			r.AddSensor(s);
			return r;
		}

		public static Room operator -(Room r, Sensor s)
		{
			r.RemoveSensor(s);
			return r;
		}

		public static Room operator +(Room r, Device s)
		{
			r.AddDevice(s);
			return r;
		}

		public static Room operator -(Room r, Device s)
		{
			r.RemoveDevice(s);
			return r;
		}

		public void AddSensor(Sensor sensor)
		{
			if (_sensorCount < MaxSensors && sensor != null)
			{
				_sensors[_sensorCount++] = sensor;
			}
		}

		public void RemoveSensor(Sensor sensor)
		{
			if (sensor == null) return;
			for (int i = 0; i < _sensorCount; i++)
			{
				if (_sensors[i] == sensor)
				{
					// Shift left
					for (int j = i; j < _sensorCount - 1; j++)
					{
						_sensors[j] = _sensors[j + 1];
					}
					_sensors[_sensorCount - 1] = null;
					_sensorCount--;
					break;
				}
			}
		}

		public void AddDevice(Device device)
		{
			if (_deviceCount < MaxDevices && device != null)
			{
				_devices[_deviceCount++] = device;
			}
		}

		public void RemoveDevice(Device device)
		{
			if (device == null) return;
			for (int i = 0; i < _deviceCount; i++)
			{
				if (_devices[i] == device)
				{
					// Shift left
					for (int j = i; j < _deviceCount - 1; j++)
					{
						_devices[j] = _devices[j + 1];
					}
					_devices[_deviceCount - 1] = null;
					_deviceCount--;
					break;
				}
			}
		}

		public void CheckAllSensors()
		{
			for (int i = 0; i < _sensorCount; i++)
			{
				_sensors[i].CheckSensor();
			}
		}

		public override string ToString()
		{
			var sb = new StringBuilder();
			sb.AppendLine("Room info:");
			sb.AppendLine(" Sensors:");
			for (int i = 0; i < _sensorCount; i++)
			{
				sb.AppendLine("  " + _sensors[i]);
			}
			sb.AppendLine(" Devices:");
			for (int i = 0; i < _deviceCount; i++)
			{
				sb.AppendLine("  " + _devices[i]);
			}
			return sb.ToString();
		}
	}

	public class Floor
	{
		public int FloorNumber { get; set; }
		public Floor(int floorNum) { FloorNumber = floorNum; }

		public override string ToString()
		{
			return $"Floor #{FloorNumber}";
		}
	}

	public class Building
	{
		public string Name;
		private int _floorsCount;
		protected double _area;

		private static int _buildingCounter;
		public static int BuildingCounter => _buildingCounter;

		public int FloorsCount
		{
			get { return _floorsCount; }
			set { if (value >= 0) _floorsCount = value; }
		}

		public double Area
		{
			get { return _area; }
			set { if (value >= 0) _area = value; }
		}

		// Static constructor
		static Building()
		{
			_buildingCounter = 0;
		}

		// Default constructor
		public Building()
		{
			Name = "NoName";
			_floorsCount = 0;
			_area = 0.0;
			_buildingCounter++;
		}

		// Param constructor
		public Building(string name, int floors, double area)
		{
			Name = name;
			_floorsCount = floors >= 0 ? floors : 0;
			_area = area >= 0 ? area : 0.0;
			_buildingCounter++;
		}

		// Copy constructor
		public Building(Building other)
		{
			if (other == null) throw new ArgumentNullException(nameof(other));
			this.Name = other.Name;
			this._floorsCount = other._floorsCount;
			this._area = other._area;
			_buildingCounter++;
		}

		// Overload methods
		public void Initialize()
		{
			// Sets some defaults
			Name = "DefaultBuilding";
			FloorsCount = 1;
			Area = 100.0;
		}
		public void Initialize(string name, int floors, double area)
		{
			// Overload that uses arguments
			if (!string.IsNullOrWhiteSpace(name)) Name = name;
			if (floors >= 0) FloorsCount = floors;
			if (area >= 0) Area = area;
		}

		// Console I/O methods
		public void InputFromConsole()
		{
			// Validate name
			Console.Write("Enter Building Name: ");
			string tmpName = Console.ReadLine();
			if (!string.IsNullOrWhiteSpace(tmpName))
			{
				Name = tmpName;
			}

			// Validate floors
			while (true)
			{
				Console.Write("Enter number of floors (>=0): ");
				if (int.TryParse(Console.ReadLine(), out int tmpFloors) && tmpFloors >= 0)
				{
					FloorsCount = tmpFloors;
					break;
				}
				Console.WriteLine("Invalid floors count. Please try again.");
			}

			// Validate area
			while (true)
			{
				Console.Write("Enter total building area (>=0): ");
				if (double.TryParse(Console.ReadLine(), out double tmpArea) && tmpArea >= 0)
				{
					Area = tmpArea;
					break;
				}
				Console.WriteLine("Invalid area. Please try again.");
			}
		}

		public void OutputToConsole()
		{
			Console.WriteLine(this.ToString());
		}

		// File I/O methods
		public void WriteToFile(string filePath)
		{
			using (StreamWriter sw = new StreamWriter(filePath, false))
			{
				sw.WriteLine(Name);
				sw.WriteLine(_floorsCount);
				sw.WriteLine(_area);
			}
		}

		public void ReadFromFile(string filePath)
		{
			if (!File.Exists(filePath)) return;

			using (StreamReader sr = new StreamReader(filePath))
			{
				string line = sr.ReadLine();
				if (!string.IsNullOrWhiteSpace(line)) Name = line;

				line = sr.ReadLine();
				if (int.TryParse(line, out int floors) && floors >= 0)
					_floorsCount = floors;

				line = sr.ReadLine();
				if (double.TryParse(line, out double area) && area >= 0)
					_area = area;
			}
		}

		public static void ShowBuildingCount()
		{
			Console.WriteLine($"Total Building instances created: {_buildingCounter}");
		}

		public override string ToString()
		{
			return $"Building '{Name}' => Floors: {_floorsCount}, Area: {_area} m^2";
		}
	}

	public class Pult
	{
		public static void Main(string[] args)
		{
			Console.WriteLine("=== DEMO OF BUILDING SECURITY SYSTEM CLASSES ===");

			// 1) Demonstrate default constructor
			Building b1 = new Building();
			b1.OutputToConsole();

			// 2) Demonstrate param constructor
			Building b2 = new Building("Office Center", 5, 2500.5);
			b2.OutputToConsole();

			// 3) Demonstrate copy constructor
			Building b3 = new Building(b2);
			b3.Name = "Office Clone";
			b3.OutputToConsole();

			// 4) Demonstrate overloaded Initialize() methods
			b1.Initialize();  // sets default
			b1.OutputToConsole();
			b1.Initialize("Warehouse", 2, 500.0);
			b1.OutputToConsole();

			// 5) Demonstrate console input with validation
			b1.InputFromConsole();
			b1.OutputToConsole();

			// 6) Demonstrate file I/O
			string testFile = "building_info.txt";
			b1.WriteToFile(testFile);
			Console.WriteLine("Wrote b1 to file.");

			// We'll re-use b2 to read from that file
			b2.ReadFromFile(testFile);
			Console.WriteLine("Read file into b1:");
			b2.OutputToConsole();

			// 7) Demonstrate operator overloading on Room
			Room r1 = new Room();
			TemperatureSensor ts = new TemperatureSensor("TempSensor1", 30.0);
			MovementSensor ms = new MovementSensor("MoveSensor1");

			r1 = r1 + ts;
			r1 = r1 + ms;
			// Remove a sensor:
			// r1 = r1 - ms;

			// Add some devices
			r1.AddDevice(new Camera("Cam1"));
			r1.AddDevice(new Lamp("Lamp1"));

			// Check all sensors
			r1.CheckAllSensors();
			Console.WriteLine(r1);

			// 8) Demonstrate static field & method usage
			Building.ShowBuildingCount(); // how many building objects were created

			Console.WriteLine("\nPress any key to exit...");
			Console.ReadKey();
		}
	}
}
