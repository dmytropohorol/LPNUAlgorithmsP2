using System.Collections;
using System.Text;

namespace BuildingSecuritySystem
{
	public interface ISensorInfo
	{
		string GetSensorInfo();
	}

	public interface IDeviceControl
	{
		void TurnOn();
		void TurnOff();
	}

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

	public class TemperatureSensor : Sensor, ISensorInfo
	{
		public double TemperatureThreshold { get; set; }
		public double TemperatureThreshold2 { get; set; }

		public TemperatureSensor(string name, double threshold)
		{
			SensorName = name;
			TemperatureThreshold = threshold;
			IsTriggered = false;
		}

		public TemperatureSensor(string name, double threshold1, double threshold2)
		{
			SensorName = name;
			TemperatureThreshold = threshold1;
			TemperatureThreshold2 = threshold2;
			IsTriggered = false;
		}

		public override void CheckSensor()
		{
			double currentTemperature = new Random().NextDouble() * 100;
			if (currentTemperature >= TemperatureThreshold)
			{
				IsTriggered = true;
			}
			if (TemperatureThreshold2 > TemperatureThreshold && currentTemperature >= TemperatureThreshold2)
			{
				IsTriggered = true;
			}
		}

		public string GetSensorInfo()
		{
			return $"TemperatureSensor: {SensorName}, T1={TemperatureThreshold}, T2={TemperatureThreshold2}";
		}
	}

	public class LightSensor : Sensor, ISensorInfo
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
			int currentLight = new Random().Next(0, 101);
			if (currentLight >= LightLevelThreshold)
			{
				IsTriggered = true;
			}
		}

		public string GetSensorInfo()
		{
			return $"LightSensor: {SensorName}, Threshold={LightLevelThreshold}";
		}
	}

	public class MovementSensor : Sensor, ISensorInfo
	{
		public MovementSensor(string name)
		{
			SensorName = name;
			IsTriggered = false;
		}

		public override void CheckSensor()
		{
			IsTriggered = (new Random().Next(0, 10) > 6);
		}

		public string GetSensorInfo()
		{
			return $"MovementSensor: {SensorName}";
		}
	}

	public class Camera : Device, IDeviceControl
	{
		public Camera(string name)
		{
			DeviceName = name;
			IsEnabled = false;
		}

		public override void Activate() { IsEnabled = true; }
		public override void Deactivate() { IsEnabled = false; }

		public void TurnOn() { Activate(); }
		public void TurnOff() { Deactivate(); }
	}

	public class Vent : Device, IDeviceControl
	{
		public Vent(string name)
		{
			DeviceName = name;
			IsEnabled = false;
		}

		public override void Activate() { IsEnabled = true; }
		public override void Deactivate() { IsEnabled = false; }

		public void TurnOn() { Activate(); }
		public void TurnOff() { Deactivate(); }
	}

	public class Lamp : Device, IDeviceControl
	{
		public Lamp(string name)
		{
			DeviceName = name;
			IsEnabled = false;
		}

		public override void Activate() { IsEnabled = true; }
		public override void Deactivate() { IsEnabled = false; }

		public void TurnOn() { Activate(); }
		public void TurnOff() { Deactivate(); }
	}

	public sealed class SmokeTemperatureSensor : TemperatureSensor
	{
		public SmokeTemperatureSensor(string name, double threshold1, double threshold2)
			: base(name, threshold1, threshold2) { }

		public override void CheckSensor()
		{
			base.CheckSensor();
			if (IsTriggered)
			{
				SecurityPanel.LogEvent("SmokeTemperatureSensor triggered extra safety procedure");
			}
		}

		public void ExtraSmokeMethod()
		{
			SecurityPanel.LogEvent("SmokeTemperatureSensor performed specialized check");
		}
	}

	public sealed class SmartLamp : Lamp
	{
		public SmartLamp(string name) : base(name) { }

		public void Dim(int level)
		{
			SecurityPanel.LogEvent($"SmartLamp {DeviceName} dimmed to level {level}");
		}
	}

	public class Room
	{
		private const int MaxSensors = 20;
		private const int MaxDevices = 20;

		private Sensor[] _sensors;
		private Device[] _devices;
		private int _sensorCount;
		private int _deviceCount;

		public double Width { get; set; }
		public double Length { get; set; }
		public int WindowsCount { get; set; }
		public int DoorsCount { get; set; }
		public double VolumeUsage { get; set; }
		public bool IsCorridor { get; set; }
		public string RoomName { get; set; }

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

		// Consider "adding two rooms" to produce a new Room whose area is the sum, etc.
		public static Room operator +(Room a, Room b)
		{
			Room combined = new Room();
			combined.Width = a.Width + b.Width;
			combined.Length = Math.Max(a.Length, b.Length);
			combined.RoomName = a.RoomName + "_" + b.RoomName;
			return combined;
		}

		public static bool operator >(Room a, Room b)
		{
			return a.GetArea() > b.GetArea();
		}
		public static bool operator <(Room a, Room b)
		{
			return a.GetArea() < b.GetArea();
		}
		public static bool operator >=(Room a, Room b)
		{
			return a.GetArea() >= b.GetArea();
		}
		public static bool operator <=(Room a, Room b)
		{
			return a.GetArea() <= b.GetArea();
		}
		public static bool operator ==(Room a, Room b)
		{
			// Handle nulls carefully
			if (ReferenceEquals(a, b)) return true;
			if (a is null || b is null) return false;
			return a.GetArea() == b.GetArea();
		}
		public static bool operator !=(Room a, Room b)
		{
			return !(a == b);
		}

		public override bool Equals(object obj)
		{
			if (obj is Room other)
			{
				return this.GetArea() == other.GetArea();
			}
			return false;
		}
		public override int GetHashCode()
		{
			return GetArea().GetHashCode();
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
			sb.AppendLine($"Room: {RoomName}");
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

		public double GetArea()
		{
			return Width * Length;
		}

		public void AutoInstallEquipment()
		{
			double area = GetArea();
			int neededTempSensors = (int)Math.Ceiling(area / 9.0);
			int neededVents = (int)Math.Ceiling(area / 4.0);
			int neededCamerasByArea = (int)Math.Ceiling(area / 16.0);
			int neededCamerasByDoors = DoorsCount < 1 ? 1 : DoorsCount;
			int totalCameras = Math.Max(neededCamerasByArea, neededCamerasByDoors);

			if (!IsCorridor)
			{
				for (int i = 0; i < neededTempSensors; i++)
				{
					AddSensor(new TemperatureSensor("TempSensor_" + (i + 1), 30, 60));
				}
				for (int i = 0; i < neededVents; i++)
				{
					AddDevice(new Vent("Vent_" + (i + 1)));
				}
				for (int i = 0; i < totalCameras; i++)
				{
					Camera cam = new Camera("Camera_" + (i + 1));
					AddDevice(cam);
					AddSensor(new LightSensor("LightSensor_for_" + cam.DeviceName, 50));
				}
				for (int i = 0; i < DoorsCount; i++)
				{
					AddSensor(new MovementSensor("MoveSensor_door_" + (i + 1)));
				}
			}
			else
			{
				int neededTempCorrSensors = (int)Math.Ceiling(area / 9.0);
				int neededCorrVents = (int)Math.Ceiling(area / 4.0);
				for (int i = 0; i < neededTempCorrSensors; i++)
				{
					AddSensor(new TemperatureSensor("Corr_Temp_" + (i + 1), 30, 60));
				}
				for (int i = 0; i < neededCorrVents; i++)
				{
					AddDevice(new Vent("Corr_Vent_" + (i + 1)));
				}
				for (int i = 0; i < DoorsCount; i++)
				{
					Camera cam = new Camera("Corr_Camera_Door_" + (i + 1));
					AddDevice(cam);
					AddSensor(new LightSensor("Corr_LightSensor_Door_" + (i + 1), 50));
					AddSensor(new MovementSensor("Corr_MoveSensor_Door_" + (i + 1)));
				}
			}
		}

		public void Simulate()
		{
			CheckAllSensors();
			for (int i = 0; i < _sensorCount; i++)
			{
				if (_sensors[i] is TemperatureSensor tempSens && tempSens.IsTriggered)
				{
					double currentT = new Random().NextDouble() * 100;
					if (currentT >= tempSens.TemperatureThreshold && currentT < tempSens.TemperatureThreshold2)
					{
						SecurityPanel.LogEvent("Temperature above T1 in room. Lamp ON briefly, camera snapshot.");
						Device lamp = FindDevice<Lamp>();
						bool wasLampOn = (lamp != null && lamp.IsEnabled);
						if (lamp != null) lamp.Activate();
						Device camera = FindDevice<Camera>();
						if (camera != null) camera.Activate();
						SecurityPanel.LogEvent("Snapshot taken.");
						if (!wasLampOn && lamp != null) lamp.Deactivate();
						if (camera != null) camera.Deactivate();
					}
					else if (tempSens.TemperatureThreshold2 > tempSens.TemperatureThreshold &&
							 currentT >= tempSens.TemperatureThreshold2)
					{
						SecurityPanel.LogEvent("Temperature above T2 in room. Lamp OFF, water ON briefly.");
						Device lamp = FindDevice<Lamp>();
						if (lamp != null) lamp.Deactivate();
						Device vent = FindDevice<Vent>();
						if (vent != null) vent.Activate();
						SecurityPanel.LogEvent("Water flow started.");
						if (vent != null) vent.Deactivate();
					}
				}
				else if (_sensors[i] is MovementSensor moveSens && moveSens.IsTriggered)
				{
					Device lamp = FindDevice<Lamp>();
					if (lamp != null && !lamp.IsEnabled)
					{
						lamp.Activate();
						SecurityPanel.LogEvent("Movement detected; lamp turned ON briefly.");
						lamp.Deactivate();
						SecurityPanel.LogEvent("Lamp turned OFF again.");
					}
				}
				else if (_sensors[i] is LightSensor lightSens && lightSens.IsTriggered)
				{
					Device camera = FindDevice<Camera>();
					if (camera != null && !camera.IsEnabled)
					{
						camera.Activate();
						SecurityPanel.LogEvent("Light sensor triggered; camera on, snapshot taken.");
						camera.Deactivate();
					}
				}
			}
		}

		private T FindDevice<T>() where T : Device
		{
			for (int i = 0; i < _deviceCount; i++)
			{
				if (_devices[i] is T dev) return dev;
			}
			return null;
		}
	}

	public class RoomCollection
	{
		private ArrayList _rooms = new ArrayList();             // numeric index
		private Dictionary<string, Room> _roomsByName = new();  // associative index

		public void Add(Room room)
		{
			if (room != null)
			{
				_rooms.Add(room);
				if (!string.IsNullOrEmpty(room.RoomName))
				{
					_roomsByName[room.RoomName] = room;
				}
			}
		}

		public int Count => _rooms.Count;

		// Numeric indexer
		public Room this[int index]
		{
			get
			{
				if (index < 0 || index >= _rooms.Count) return null;
				return (Room)_rooms[index];
			}
			set
			{
				if (index >= 0 && index < _rooms.Count)
				{
					_rooms[index] = value;
					if (!string.IsNullOrEmpty(value.RoomName))
					{
						_roomsByName[value.RoomName] = value;
					}
				}
			}
		}

		// Associative indexer
		public Room this[string roomName]
		{
			get
			{
				if (_roomsByName.ContainsKey(roomName))
				{
					return _roomsByName[roomName];
				}
				return null;
			}
			set
			{
				_roomsByName[roomName] = value;
				bool found = false;
				for (int i = 0; i < _rooms.Count; i++)
				{
					if (_rooms[i] is Room r && r.RoomName == roomName)
					{
						_rooms[i] = value;
						found = true;
						break;
					}
				}
				if (!found)
				{
					_rooms.Add(value);
				}
			}
		}
	}

	public class Floor
	{
		public int FloorNumber { get; set; }
		public double FloorHeight { get; set; }
		public List<Room> Rooms { get; } = new List<Room>();

		public Floor(int floorNum)
		{
			FloorNumber = floorNum;
		}

		public Floor(int floorNum, double height)
		{
			FloorNumber = floorNum;
			FloorHeight = height;
		}

		public void AddRoom(Room r)
		{
			Rooms.Add(r);
		}

		public override string ToString()
		{
			return $"Floor #{FloorNumber}, Height={FloorHeight}";
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

		public List<Floor> Floors { get; } = new List<Floor>();

		static Building()
		{
			_buildingCounter = 0;
		}

		public Building()
		{
			Name = "NoName";
			_floorsCount = 0;
			_area = 0.0;
			_buildingCounter++;
		}

		public Building(string name, int floors, double area)
		{
			Name = name;
			_floorsCount = floors >= 0 ? floors : 0;
			_area = area >= 0 ? area : 0.0;
			_buildingCounter++;
		}

		public Building(Building other)
		{
			if (other == null) throw new ArgumentNullException(nameof(other));
			this.Name = other.Name;
			this._floorsCount = other._floorsCount;
			this._area = other._area;
			_buildingCounter++;
		}

		public void Initialize()
		{
			Name = "DefaultBuilding";
			FloorsCount = 1;
			Area = 100.0;
		}
		public void Initialize(string name, int floors, double area)
		{
			if (!string.IsNullOrWhiteSpace(name)) Name = name;
			if (floors >= 0) FloorsCount = floors;
			if (area >= 0) Area = area;
		}

		public void InputFromConsole()
		{
			Console.Write("Enter Building Name: ");
			string tmpName = Console.ReadLine();
			if (!string.IsNullOrWhiteSpace(tmpName))
			{
				Name = tmpName;
			}
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
			Console.WriteLine(ToString());
		}

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

		public void AutoCreateFloorsAndRooms()
		{
			for (int i = 1; i <= FloorsCount; i++)
			{
				Floor f = new Floor(i, 3.0 + i);
				int roomsCount = new Random().Next(1, 4);
				for (int r = 1; r <= roomsCount; r++)
				{
					Room room = new Room();
					room.RoomName = $"R_{i}_{r}";
					room.Width = new Random().Next(3, 10);
					room.Length = new Random().Next(3, 10);
					room.DoorsCount = new Random().Next(1, 3);
					room.WindowsCount = new Random().Next(0, 4);
					room.VolumeUsage = new Random().Next(0, 100);
					room.IsCorridor = (r == roomsCount && roomsCount > 2);
					room.AutoInstallEquipment();
					f.AddRoom(room);
				}
				Floors.Add(f);
			}
		}

		public void SimulateAllRooms()
		{
			foreach (var fl in Floors)
			{
				foreach (var rm in fl.Rooms)
				{
					rm.Simulate();
				}
			}
		}
	}

	public static class SecurityPanel
	{
		public static void LogEvent(string info)
		{
			string timestamp = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
			File.AppendAllText("SecurityLog.txt", $"[{timestamp}] {info}{Environment.NewLine}");
		}
	}

	public class Pult
	{
		private static void PolymorphicSensorTest(Sensor sensor)
		{
			sensor.CheckSensor();
			if (sensor is SmokeTemperatureSensor sts)
			{
				sts.ExtraSmokeMethod();
			}
			Console.WriteLine(sensor);
		}

		private static void PolymorphicDeviceTest(Device device)
		{
			device.Activate();
			if (device is SmartLamp lamp)
			{
				lamp.Dim(50);
			}
			device.Deactivate();
			Console.WriteLine(device);
		}

		public static void Main(string[] args)
		{
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
			b1.Initialize();
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

			// Re-use b2 to read from that file
			b2.ReadFromFile(testFile);
			Console.WriteLine("Read file into b2:");
			b2.OutputToConsole();

			// 7) Demonstrate operator overloading on Room
			Room r1 = new Room();
			r1.RoomName = "ExampleRoom";
			r1.Width = 5;
			r1.Length = 6;
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

			// 8) Demonstrate static field and method usage
			Building.ShowBuildingCount();
			Building bigBuilding = new Building("Big Complex", 3, 9999); // how many building objects were created
			bigBuilding.AutoCreateFloorsAndRooms();
			bigBuilding.SimulateAllRooms();
			Console.WriteLine("Simulation complete. Check SecurityLog.txt for events.");

			// 9) Demonstrate interfaces and sealed
			Console.WriteLine("\n=== Interfaces and sealed ===");
			Sensor sealedSmokeSensor = new SmokeTemperatureSensor("SmokeSensorX", 25, 50);
			PolymorphicSensorTest(sealedSmokeSensor);

			Device sealedSmartLamp = new SmartLamp("SmartLampX");
			PolymorphicDeviceTest(sealedSmartLamp);

			// 10) Demonstrate collection
			Console.WriteLine("\n=== Collection ===");
			RoomCollection collection = new RoomCollection();

			Room roomA = new Room() { RoomName = "A", Width = 4, Length = 4 };
			Room roomB = new Room() { RoomName = "B", Width = 5, Length = 5 };
			Room roomC = new Room() { RoomName = "C", Width = 3, Length = 10 };

			collection.Add(roomA);
			collection.Add(roomB);
			collection.Add(roomC);

			Console.WriteLine("Rooms in collection by numeric index:");
			for (int i = 0; i < collection.Count; i++)
			{
				Console.WriteLine($" Index {i}: {collection[i].RoomName}, area = {collection[i].GetArea()}");
			}

			Console.WriteLine("Rooms in collection by associative index:");
			Console.WriteLine($" 'A': area={collection["A"].GetArea()}");
			Console.WriteLine($" 'B': area={collection["B"].GetArea()}");
			Console.WriteLine($" 'C': area={collection["C"].GetArea()}");

			// 11) Demonstrate arithmetic and logical operators
			Console.WriteLine("\n=== Arithmetic and logical operators on rooms ===");
			Room sumRoom = roomA + roomB;
			Console.WriteLine($"SumRoom (A+B) => width={sumRoom.Width}, length={sumRoom.Length}, name={sumRoom.RoomName}");
			Console.WriteLine($"sumRoom area={sumRoom.GetArea()} (should be area of width=4+5=9 x length=max(4,5)=5 => 45)");

			bool isRoomALarger = (roomA > roomC);
			Console.WriteLine($"Is Room A bigger than Room C? {isRoomALarger} (area A={roomA.GetArea()}, area C={roomC.GetArea()})");

			bool areTheyEqual = (roomB == roomC);
			Console.WriteLine($"Is Room B area equal to Room C area? {areTheyEqual} (B={roomB.GetArea()}, C={roomC.GetArea()})");

			Console.WriteLine("\nPress any key to exit...");
			Console.ReadKey();
		}
	}
}
