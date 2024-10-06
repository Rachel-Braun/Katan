#include "Functions.h"
#include <iostream>

Functions::Functions()
{
	//resource
	functionsMap["Resource"] = [](World& world, Command command) 
	{
		int amount= stoi(command.arguments[0]);
		string resource = command.arguments[1];
		int x = stoi(command.arguments[2])-1;
		int y = stoi(command.arguments[3])-1;

		world.initialResource(x, y, amount, resource);
	};

	//resources
	functionsMap["Resources"] = [](World& world, Command command)
	{
		int x = stoi(command.arguments[4]) - 1;
		int y = stoi(command.arguments[5]) - 1;
		vector<int>resources(4);
		for (int i = 0; i < 4; i++)
			resources[i] = stoi(command.arguments[i]);
		world.initialResources(x, y, resources);
	};

	//SelectedResource
	functionsMap["SelectedResource"] = [](World& world, Command command)
	{
		cout << world.selectedResource() << endl;
	};

	//People
	functionsMap["People"] = [](World& world, Command command)
	{
		int amount = stoi(command.arguments[0]);
		int x = stoi(command.arguments[1])-1;
		int y = stoi(command.arguments[2])-1;
		world.itemCount["People"] += amount;
		if (!world.empty(x, y))
			world.cells[y][x].objectPtr->addResource("People", amount);
			
		world.cells[y][x].itemPtr = new Object("People");
		
		world.board[y / Tile::size[0]][x / Tile::size[1]].peopleCount += amount;
	};

	//Select
	functionsMap["Select"] = [](World& world, Command command)
	{
		world.selectedX = stoi(command.arguments[0])-1;
		world.selectedY = stoi(command.arguments[1])-1;
	};

	//SelectedCategory
	functionsMap["SelectedCategory"] = [](World& world, Command command)
	{
		cout << "SelectedCategory " << world.getCategoryByLocation(world.selectedX, world.selectedY)<<endl;
	};

	//SelectedComplete
	functionsMap["SelectedComplete"] = [](World& world, Command command)
	{
		cout << "SelectedComplete " << world.getIsComplete(world.selectedX, world.selectedY) << endl;
	};

	//work
	functionsMap["Work"] = [=](World& world, Command command)
	{
		if (find(waits.begin(), waits.end(), command) != waits.end()) //exist in queue
		{
			int x = (stoi(command.arguments[1]) - 1) / Tile::size[1];
			int y = (stoi(command.arguments[2]) - 1) / Tile::size[0];
			int time = stoi(command.arguments[0]);
			Tile& tile = world.board[y][x];
			tile.amount = 0; // כשיהיו נתונים בקובץ צריך להוריד בהתאמה
			world.amountsOfResource[tile.resource] = 0; //כנ"ל
		}
		else
		{
			command.arguments.insert(command.arguments.begin(), to_string(INT_MAX)); //הוספת אלמנט זמן
			waits.emplace_back(command);
		}
	};

	//wait
	functionsMap["Wait"] = [=](World& world, Command command)
	{
		int time = stoi(command.arguments[0]), timeOfCommand;
		bool isTerminated;
		for (Command& commandPtr : waits)
		{
			timeOfCommand = stoi(commandPtr.arguments[0]);
			isTerminated = timeOfCommand <= time;
			commandPtr.arguments[0]= to_string(min(time, timeOfCommand));
			functionsMap[commandPtr.name](world, commandPtr);
			if (isTerminated)
				waits.erase(find(waits.begin(), waits.end(), commandPtr));
			else
				commandPtr.arguments[0] = to_string(timeOfCommand - time);
		}
		while (time--);
	};

	//rain
	functionsMap["Rain"] = [=](World& world, Command command)
	{
		if (find(waits.begin(), waits.end(), command) == waits.end()) 
			waits.emplace_back(command);
		else //exist in queue
		{
			int time = stoi(command.arguments[0]);
			world.growResources(time);
		}
	};

	//Build
	//if is start-we have INT_MAX time to build
	//but if is input - we have only the time of wait
	functionsMap["Build"] = [=](World& world, Command command)
	{
		auto it = find(waits.begin(), waits.end(), command);

		if (isStart || it != waits.end())  //if is start or the command is waiting
		{
			string itemName = command.arguments[0 + !isStart];
			int x = stoi(command.arguments[1 + !isStart]) - 1;
			int y = stoi(command.arguments[2 + !isStart]) - 1;
			world.build(itemName, x, y, isStart, isStart?INT_MAX:stoi(command.arguments[0]));
		}
		else
		{
			string itemName = command.arguments[0];
			useJSON jsonData;
			int constructionTime = jsonData.getTimeOf(itemName);
			command.arguments.insert(command.arguments.begin(), to_string(constructionTime)); //הוספת אלמנט זמן
			waits.emplace_back(command);
		}
	};

	//Deposit
	functionsMap["Deposit"] = [](World& world, Command command)
	{
	};

	//Move
	functionsMap["Move"] = [](World& world, Command command)
	{
		
	};

	//TakeResources
	functionsMap["TakeResources"] = [=](World& world, Command command)
	{
		int x = stoi(command.arguments[0]) - 1;
		int y = stoi(command.arguments[1]) - 1;
		world.takeResources(x, y);
	};

	//VillageCount
	functionsMap["VillageCount"] = [](World& world, Command command)
	{
		cout << "VillageCount " << world.itemCount["Village"] << endl;
	};

	//CityCount
	functionsMap["CityCount"] = [](World& world, Command command)
	{
		cout << "CityCount " << world.itemCount["City"] << endl;
	};

	//RoadCount
	functionsMap["RoadCount"] = [](World& world, Command command)
	{
		cout << "RoadCount " << world.itemCount["Road"] << endl;
	};

	//PeopleCount
	functionsMap["PeopleCount"] = [](World& world, Command command)
	{
		cout << "PeopleCount " << world.itemCount["People"] << endl;
	};

	//HelicopterCount
	functionsMap["HelicopterCount"] = [](World& world, Command command)
	{
		cout << "HelicopterCount " << world.itemCount["Helicopter"] << endl;
	};

	//TruckCount
	functionsMap["TruckCount"] = [](World& world, Command command)
	{
		cout << "TruckCount " << world.itemCount["Truck"] << endl;
	};

	//CarCount
	functionsMap["CarCount"] = [](World& world, Command command)
	{
		cout << "CarCount " << world.itemCount["Car"] << endl;
	};

	//MakeEmpty
	functionsMap["MakeEmpty"] = [](World& world, Command command)
	{
		int x = stoi(command.arguments[0])-1;
		int y = stoi(command.arguments[1])-1;
		if (!world.empty(x, y))
			world.resetResources(x, y);
	};

	//SelectedPeople
	functionsMap["SelectedPeople"] = [](World& world, Command command)
	{
		cout << "SelectedPeople " << world.cells[world.selectedY][world.selectedX]
			.objectPtr->amountsOfResource["People"] << endl;
		//world.board[(world.selectedY / Tile::size[0])][(world.selectedX / Tile::size[1])].peopleCount << endl;
	};
	
	//SelectedCar
	functionsMap["SelectedCar"] = [](World& world, Command command)
	{
		cout << "SelectedCar " << world.cells[world.selectedY][world.selectedX]
			.objectPtr->amountsOfResource["Car"] << endl;
	};

	//SelectedTruck
	functionsMap["SelectedTruck"] = [](World& world, Command command)
	{
		cout << "SelectedTruck " << world.cells[world.selectedY][world.selectedX]
			.objectPtr->amountsOfResource["Truck"] << endl;
	};

	//SelectedCoordinates
	functionsMap["SelectedCoordinates"] = [](World& world, Command command)
	{
		cout << "SelectedCoordinates " << world.selectedY << " " << world.selectedX;
	};

	//Manufacture
	functionsMap["Manufacture"] = [=](World& world, Command command)
	{
		string category = command.arguments[0];
		int x = stoi(command.arguments[1])-1;
		int y = stoi(command.arguments[2])-1;
		world.manufacture(category, x, y, isStart);
	};
}
