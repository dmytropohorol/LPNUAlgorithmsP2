using System.Text;

namespace BuildingSecuritySystem
{
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

	public class TemperatureSensor : Sensor
	{
		public double TemperatureThreshold { get; set; }
		public double TemperatureThreshold2 { get; set; }
		public double currentTemperature { get; set; }

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
			currentTemperature = new Random().NextDouble() * 100;
		}

		public override void CheckSensor()
		{
			if (currentTemperature >= TemperatureThreshold)
			{
				IsTriggered = true;
			}
			if (TemperatureThreshold2 > TemperatureThreshold && currentTemperature >= TemperatureThreshold2)
			{
				IsTriggered = true;
			}
		}

		public void ForceTemperature(double forcedValue)
		{
			currentTemperature = forcedValue;
			IsTriggered = false; // Will re-check in CheckSensor()
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
			IsTriggered = (new Random().Next(0, 10) > 6);
		}
	}

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
		private const int MaxSensors = 20;
		private const int MaxDevices = 20;

		private Sensor[] _sensors;
		private Device[] _devices;
		private int _sensorCount;
		private int _deviceCount;

		public string Name { get; set; }
		public double Width { get; set; }
		public double Length { get; set; }
		public int WindowsCount { get; set; }
		public int DoorsCount { get; set; }
		public bool IsCorridor { get; set; }

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

		public void Simulate(string buildingName, int floorNumber, bool randomize = false)
		{
			CheckAllSensors();
			for (int i = 0; i < _sensorCount; i++)
			{
				if (_sensors[i] is TemperatureSensor tempSens && tempSens.IsTriggered)
				{
					double currentT = tempSens.currentTemperature;
					if (currentT >= tempSens.TemperatureThreshold && currentT < tempSens.TemperatureThreshold2)
					{
						SecurityPanel.LogEvent($"Temperature above T1 in Bldg '{buildingName}', Floor #{floorNumber}, Room '{Name}'. Lamp ON briefly, camera snapshot...");
						Device lamp = FindDevice<Lamp>();
						bool wasLampOn = (lamp != null && lamp.IsEnabled);
						if (lamp != null) lamp.Activate();

						Device camera = FindDevice<Camera>();
						if (camera != null) camera.Activate();
						SecurityPanel.LogEvent($"Snapshot taken in Bldg '{buildingName}', Floor #{floorNumber}, Room '{Name}'.");

						if (!wasLampOn && lamp != null) lamp.Deactivate();
						if (camera != null) camera.Deactivate();
					}
					else if (tempSens.TemperatureThreshold2 > tempSens.TemperatureThreshold &&
							 currentT >= tempSens.TemperatureThreshold2)
					{
						SecurityPanel.LogEvent($"Temperature above T2 in Bldg '{buildingName}', Floor #{floorNumber}, Room '{Name}'. Lamp OFF, water ON briefly.");
						Device lamp = FindDevice<Lamp>();
						if (lamp != null) lamp.Deactivate();
						Device vent = FindDevice<Vent>();
						if (vent != null) vent.Activate();
						SecurityPanel.LogEvent($"Water flow started in Bldg '{buildingName}', Floor #{floorNumber}, Room '{Name}'.");
						if (vent != null) vent.Deactivate();
					}
				}
				else if (_sensors[i] is MovementSensor moveSens && moveSens.IsTriggered)
				{
					Device lamp = FindDevice<Lamp>();
					if (lamp != null && !lamp.IsEnabled)
					{
						lamp.Activate();
						SecurityPanel.LogEvent($"Movement detected in Bldg '{buildingName}', Floor #{floorNumber}, Room '{Name}'; lamp ON briefly.");
						lamp.Deactivate();
						SecurityPanel.LogEvent($"Lamp OFF again in Bldg '{buildingName}', Floor #{floorNumber}, Room '{Name}'.");
					}
				}
				else if (_sensors[i] is LightSensor lightSens && lightSens.IsTriggered)
				{
					Device camera = FindDevice<Camera>();
					if (camera != null && !camera.IsEnabled)
					{
						camera.Activate();
						SecurityPanel.LogEvent($"Light sensor triggered in Bldg '{buildingName}', Floor #{floorNumber}, Room '{Name}'; camera on, snapshot taken.");
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

		public override string ToString()
		{
			var sb = new StringBuilder();
			sb.AppendLine($"Room '{Name}' => Width={Width}, Length={Length}, Windows={WindowsCount}, Doors={DoorsCount}, Corridor={IsCorridor}");
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

		public void ForceTemperature(double newT)
		{
			for (int i = 0; i < _sensorCount; i++)
			{
				if (_sensors[i] is TemperatureSensor t)
				{
					t.ForceTemperature(newT);
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
		private static int _buildingCounter;
		public static int BuildingCounter => _buildingCounter;

		public int FloorsCount
		{
			get { return _floorsCount; }
			set { if (value >= 0) _floorsCount = value; }
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
			_buildingCounter++;
		}

		public Building(string name, int floors)
		{
			Name = name;
			_floorsCount = floors >= 0 ? floors : 0;
			_buildingCounter++;
		}

		public Building(Building other)
		{
			if (other == null) throw new ArgumentNullException(nameof(other));
			this.Name = other.Name;
			this._floorsCount = other._floorsCount;
			_buildingCounter++;
		}

		public void Initialize()
		{
			Name = "DefaultBuilding";
			FloorsCount = 1;
		}
		public void Initialize(string name, int floors)
		{
			if (!string.IsNullOrWhiteSpace(name)) Name = name;
			if (floors >= 0) FloorsCount = floors;
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
		}

		public void OutputToConsole()
		{
			Console.WriteLine(ToString());
		}

		// simplified file I/O (no building area)
		public void WriteToFile(string filePath)
		{
			using (StreamWriter sw = new StreamWriter(filePath, false))
			{
				sw.WriteLine(Name);
				sw.WriteLine(_floorsCount);
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
			}
		}

		public static void ShowBuildingCount()
		{
			Console.WriteLine($"Total Building instances created: {_buildingCounter}");
		}

		public override string ToString()
		{
			return $"Building '{Name}' => Floors: {_floorsCount}";
		}

		public void InputFloorsAndRoomsFromConsole()
		{
			Floors.Clear();
			for (int i = 1; i <= FloorsCount; i++)
			{
				Console.WriteLine($"\n--- Enter info for Floor #{i} ---");
				double floorHeight = 3.0 + i;
				Console.Write($"Enter Floor {i} Height (default {floorHeight}): ");
				string tmp = Console.ReadLine();
				if (double.TryParse(tmp, out double userHeight) && userHeight > 0)
				{
					floorHeight = userHeight;
				}

				Floor floor = new Floor(i, floorHeight);

				int roomCount;
				while (true)
				{
					Console.Write($"How many rooms on Floor #{i}?: ");
					if (int.TryParse(Console.ReadLine(), out roomCount) && roomCount > 0)
					{
						break;
					}
					Console.WriteLine("Invalid room count. Try again.");
				}

				for (int r = 1; r <= roomCount; r++)
				{
					Room room = new Room();
					room.Name = $"Floor{i}_Room{r}";

					Console.WriteLine($"  >> Enter data for Room {r}:");
					Console.Write($"     Room name (default '{room.Name}'): ");
					string rmName = Console.ReadLine();
					if (!string.IsNullOrWhiteSpace(rmName)) room.Name = rmName;

					while (true)
					{
						Console.Write("     Width (>=1): ");
						if (double.TryParse(Console.ReadLine(), out double w) && w >= 1)
						{
							room.Width = w;
							break;
						}
						Console.WriteLine("     Invalid width. Try again.");
					}

					while (true)
					{
						Console.Write("     Length (>=1): ");
						if (double.TryParse(Console.ReadLine(), out double l) && l >= 1)
						{
							room.Length = l;
							break;
						}
						Console.WriteLine("     Invalid length. Try again.");
					}

					while (true)
					{
						Console.Write("     WindowsCount (>=0): ");
						if (int.TryParse(Console.ReadLine(), out int wc) && wc >= 0)
						{
							room.WindowsCount = wc;
							break;
						}
						Console.WriteLine("     Invalid windows count. Try again.");
					}

					while (true)
					{
						Console.Write("     DoorsCount (>=0): ");
						if (int.TryParse(Console.ReadLine(), out int dc) && dc >= 0)
						{
							room.DoorsCount = dc;
							break;
						}
						Console.WriteLine("     Invalid doors count. Try again.");
					}

					Console.Write("     Is corridor? (true/false) ");
					string isCorrStr = Console.ReadLine();
					if (!string.IsNullOrEmpty(isCorrStr))
					{
						bool.TryParse(isCorrStr, out bool corr);
						room.IsCorridor = corr;
					}

					room.AutoInstallEquipment();
					floor.AddRoom(room);
				}

				Floors.Add(floor);
			}
		}

		// Let user choose which floor/room to start a fire
		public void StartFire(int floorNumber, int roomNumber, bool severe = false)
		{
			if (floorNumber < 1 || floorNumber > Floors.Count)
			{
				Console.WriteLine("Invalid floor number!");
				return;
			}
			var floor = Floors[floorNumber - 1];
			if (roomNumber < 1 || roomNumber > floor.Rooms.Count)
			{
				Console.WriteLine("Invalid room number!");
				return;
			}
			var room = floor.Rooms[roomNumber - 1];
			double fireTemp = severe ? 65.0 : 35.0; // T2-like or T1-like
			room.ForceTemperature(fireTemp);
			Console.WriteLine($"Started {(severe ? "severe" : "mild")} fire in Floor#{floorNumber}, Room '{room.Name}'.");
		}

		public void SimulateAllRooms()
		{
			foreach (var fl in Floors)
			{
				foreach (var rm in fl.Rooms)
				{
					rm.Simulate(Name, fl.FloorNumber);
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
		public static void Main(string[] args)
		{
			Console.WriteLine("=== DEMO OF BUILDING SECURITY SYSTEM ===");

			Building b1 = new Building();
			b1.OutputToConsole();

			Building b2 = new Building("Office Center", 5);
			b2.OutputToConsole();

			Building b3 = new Building(b2);
			b3.Name = "Office Clone";
			b3.OutputToConsole();

			b1.Initialize();
			b1.OutputToConsole();
			b1.Initialize("Warehouse", 2);
			b1.OutputToConsole();

			// Basic building console input (removes area)
			b1.InputFromConsole();
			b1.OutputToConsole();

			// File IO (no area)
			string testFile = "building_info.txt";
			b1.WriteToFile(testFile);
			Console.WriteLine("Wrote b1 to file.");

			b2.ReadFromFile(testFile);
			Console.WriteLine("Read file into b2:");
			b2.OutputToConsole();

			// Room example
			Room r1 = new Room { Name = "BigRoom", Width = 5, Length = 6 };
			r1 = r1 + new TemperatureSensor("TempSensor1", 30.0, 60.0);
			r1 = r1 + new MovementSensor("MoveSensor1");
			r1.AddDevice(new Camera("Cam1"));
			r1.AddDevice(new Lamp("Lamp1"));
			r1.CheckAllSensors();
			Console.WriteLine(r1);

			Building.ShowBuildingCount();

			// Creating floors/rooms from console
			Console.WriteLine("\n=== Build custom building with floors/rooms from console! ===");
			Building custom = new Building();
			custom.InputFromConsole();
			custom.InputFloorsAndRoomsFromConsole();
			SecurityPanel.LogEvent($"=== RANDOM FIRE GENERATION ===");
			custom.SimulateAllRooms();

			foreach (var fl in custom.Floors)
			{
				foreach (var rm in fl.Rooms)
				{
					rm.ForceTemperature(20.0);
				}
			}

			// Demonstrate choosing where a fire starts
			Console.WriteLine("\n=== Manually Start Fire in a Specific Floor/Room ===");
			Console.Write("Choose Floor # to start fire: ");
			int userFloor = int.Parse(Console.ReadLine() ?? "1");
			Console.Write("Choose Room # on that floor: ");
			int userRoom = int.Parse(Console.ReadLine() ?? "1");
			Console.Write("Mild or severe fire? (mild/severe): ");
			string severity = Console.ReadLine();
			bool severeFire = (severity?.ToLower() == "severe");
			custom.StartFire(userFloor, userRoom, severeFire);
			SecurityPanel.LogEvent($"=== USER SPECIFIED FIRE GENERATION ===");
			custom.SimulateAllRooms();

			Console.WriteLine("Check 'SecurityLog.txt' for logs of triggered events.");
			Console.WriteLine("\nPress any key to exit...");
			Console.ReadKey();
		}
	}
}
