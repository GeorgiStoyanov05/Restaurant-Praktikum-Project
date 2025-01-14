#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool isLeapYear(int year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int getDaysInMonth(int month, int year) {
	switch (month) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:
		return 31;
	case 4: case 6: case 9: case 11:
		return 30;
	case 2:
		return isLeapYear(year) ? 29 : 28;
	default:
		return 0;
	}
}

void incrementDate(int& day, int& month, int& year) {
	int daysInMonth = getDaysInMonth(month, year);

	if (day < daysInMonth) {
		day++;
	}
	else {
		day = 1;
		if (month < 12) {
			month++;
		}
		else {
			month = 1;
			year++;
		}
	}
}

static int checkIfDishExists(string name) {
	ifstream file("Menu.txt");
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string dish;
		getline(file, dish);
		int separatorIndex = dish.find('|');
		dish = dish.substr(0, separatorIndex);
		if (dish == name) {
			return 1;
		}
	}
	return 0;
}

string increaseDayByOne(string date) {
	int month = stoi(date.substr(0, 2));
	int day = stoi(date.substr(3, 2));
	int year = stoi(date.substr(6, 2));
	incrementDate(day, month, year);
	string incrementedDate = to_string(month) + '/' + to_string(day) + '/' + to_string(year);
	return incrementedDate;
}

static void displayUserOptions(string role) {
	if (role == "Waiter")
	{
		std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "1) See the menu" << std::endl;
		std::cout << "2) Order something from the menu" << std::endl;
		std::cout << "3) Cancel an order" << std::endl;
		std::cout << "4) See all previous orders" << std::endl;
		std::cout << "5) See all previous orders, sorted in alphabetical order, as well as the number of orders of each dish" << std::endl;
		std::cout << "6) See today's turnover" << std::endl;
		std::cout << "7) End your shift" << std::endl;
		std::cout << "8) Show all options again" << std::endl;
		std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
	}
	else if (role == "Manager")
	{
		std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "1) See the menu" << std::endl;
		std::cout << "2) Order something from the menu" << std::endl;
		std::cout << "3) Cancel an order" << std::endl;
		std::cout << "4) See all previous orders" << std::endl;
		std::cout << "5) See all previous orders, sorted in alphabetical order, as well as the number of orders of each dish" << std::endl;
		std::cout << "6) See the quantity for all the remaining products" << std::endl;
		std::cout << "7) Remove a product from the storage" << std::endl;
		std::cout << "8) Add a product to the storage" << std::endl;
		std::cout << "9) See today's turnover" << std::endl;
		std::cout << "10) Generate today's report" << std::endl;
		std::cout << "11) See all the turnovers from a certain day to today" << std::endl;
		std::cout << "12) Add a dish to the Menu" << std::endl;
		std::cout << "13) Remove a dish from the menu" << std::endl;
		std::cout << "14) End your day" << std::endl;
		std::cout << "15) Show all options again" << std::endl;
		std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
	}
}

static bool validateOption(int option, string role) {
	bool nz = role != "Manager";
	if (role != "Manager" && role != "Waiter") {
		return 0;
	}
	else if (role == "Waiter" && (option < 1 || option>8)) {
		return 0;
	}
	else if (role == "Manager" && (option < 1 || option>15)) {
		return 0;
	}
	return 1;
}

static int printMenu() {
	std::ifstream file("Menu.txt");
	if (!file.is_open()) return 0;
	std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
	while (!file.eof()) {
		string dish;
		if (dish == "") {
			continue;
		}
		getline(file, dish);
		int separatorIndex = dish.find('|');
		string dishName = dish.substr(0, separatorIndex - 1);
		std::cout << dish << std::endl;
	}
	std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
	return 1;
}
/*
static int makeAnOrder(string order) {
	ifstream file("Menu.txt");
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string dish;
		getline(file, dish);
		int separationNameIndex = dish.find('|');
		int separationIngredientsIndex = dish.find('?');
		int separationPriceIndex = dish.find('`');
		string dishName = dish.substr(0, separationNameIndex);
		double price = stod(dish.substr(separationNameIndex + 1, (separationPriceIndex - separationNameIndex)));
		string ingredients=dish.substr(separationPriceIndex+1, (separationIngredientsIndex-separationPriceIndex-1));
		string amounts=dish.substr(separationIngredientsIndex+1, (dish.length() - (separationIngredientsIndex+1)));

	}
}
*/

static int addProductToStorage(string product, int amount) {
	bool isFound = 0;
	ifstream file("Storage.txt");
	ofstream temp("temp.txt");
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string productData;
		getline(file, productData);
		if (productData == "") {
			continue;
		}
		int separatorIndex = productData.find('|');
		string productName = productData.substr(0, separatorIndex);
		double prodRemainingAmount = stod(productData.substr(separatorIndex + 1, (productData.length() - (separatorIndex + 1))));
		if (productName == product) {
			isFound = 1;
			prodRemainingAmount += amount;
			productData = productName + '|' + to_string(prodRemainingAmount);
			temp << productData << endl;
			continue;
		}
		temp << productData << endl;
	}
	if (isFound == 0) {
		string newProductData = product + '|' + to_string(amount);
		temp << newProductData << endl;
	}
	file.close();
	temp.close();
	remove("Storage.txt");
	int result = rename("temp.txt", "Storage.txt");
	return 1;
}

static int deleteProductToStorage(string product, int amount) {
	bool isFound = 0;
	ifstream file("Storage.txt");
	ofstream temp("temp.txt");
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string productData;
		getline(file, productData);
		if (productData == "") {
			continue;
		}
		int separatorIndex = productData.find('|');
		string productName = productData.substr(0, separatorIndex);
		double prodRemainingAmount = stod(productData.substr(separatorIndex + 1, (productData.length() - (separatorIndex + 1))));
		if (productName == product) {
			isFound = 1;
			prodRemainingAmount -= amount;
			if (prodRemainingAmount <= 0) {
				continue;
			}
			productData = productName + '|' + to_string(prodRemainingAmount);
			temp << productData << endl;
			continue;
		}
		temp << productData << endl;
	}
	if (isFound == 0) {
		return 0;
	}
	file.close();
	temp.close();
	remove("Storage.txt");
	int result = rename("temp.txt", "Storage.txt");
	return 1;
}

static int showRemainingProducts() {
	ifstream file("Storage.txt");
	if (!file.is_open()) return 0;
	std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
	while (!file.eof()) {
		string prodData;
		if (prodData == "") {
			continue;
		}
		getline(file, prodData);
		int nameSeparatorIndex = prodData.find('|');
		string name = prodData.substr(0, nameSeparatorIndex);
		double amount = stod(prodData.substr(nameSeparatorIndex + 1, prodData.length() - 1));
		cout << name << " -> " << amount << endl;
	}
	std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
}

static int showAllPreviousOrders() {
	ifstream file("Orders.txt");
	if (!file.is_open()) return 0;
	cout << "" << endl;
	while (!file.eof()) {
		string dish;
		getline(file, dish);
		if (dish == "") {
			continue;
		}
		cout << dish << endl;
	}
	cout << "" << endl;
}

static int showSortedOrders() {
	vector<string> dishes;
	vector<int> numOfOrders;
	ifstream file("Orders.txt");
	if (!file.is_open()) return 0;
	cout << "" << endl;
	while (!file.eof()) {
		string dish;
		getline(file, dish);
		if (dish == "") {
			continue;
		}
		bool isAlreadyAdded = false;
		for (int i = 0; i < dishes.size(); i++) {
			if (dishes[i] == dish) {
				isAlreadyAdded = true;
				numOfOrders[i]++;
				break;
			}
		}
		if (isAlreadyAdded == false) {
			dishes.push_back(dish);
			numOfOrders.push_back(1);
		}
	}
	for (int i = 0; i < dishes.size() - 1; ++i) {
		for (int j = 0; j < dishes.size() - i - 1; ++j) {
			if (dishes[j] > dishes[j + 1]) {
				// Swap dishes[j] and dishes[j + 1]
				string temp = dishes[j];
				dishes[j] = dishes[j + 1];
				dishes[j + 1] = temp;

				int tempForNum = numOfOrders[i];
				numOfOrders[i] = numOfOrders[i + 1];
				numOfOrders[i + 1] = tempForNum;
			}
		}
	}

	for (int i = 0; i < dishes.size(); i++) {
		cout << dishes[i] << " -> " << numOfOrders[i] << endl;
	}
	return 1;
}
/*
static int showTodaysTurnover() {
	ifstream file("Turnovers.txt");
	if (!file.is_open()) return 0;

	while (!file.eof()) {
		string turnover;
		if (turnover == "") {
			continue;
		}
		int separationIndex = turnover.find('|');
		//string date = turnover.substr(0, separataionIndex+1);
		double totalProfit = stod(turnover.substr(separationIndex + 1, turnover.length() - 1));
	}
}

static int finishTurnoverForToday() {
	ifstream file("Turnovers.txt");
	int fileLinesCounts = 0;
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string temp;
		getline(file, temp);
		if (temp == "") continue;
		fileLinesCounts++;
	}
	string date;
	if (fileLinesCounts == 0) {
		file.close();
		time_t timestamp = time(NULL);
		struct tm datetime = *localtime(&timestamp);
		char output[50];

		strftime(output, 50, "%m/%d/%y", &datetime);
		date = output;
		date = increaseDayByOne(date) + "|0.00";
		return 1;
	}
	file.ignore(fileLinesCounts - 1);
	getline(file, date);
	file.close();
	date = date.substr(0, 8);
	date = increaseDayByOne(date)+"|0.00";
	ofstream writeFile("Turnovers.txt");
	writeFile.seekp(fileLinesCounts);
	writeFile << date<<endl;
	writeFile.close();
	return 1;
}
*/

static int createDish(string name, int price, vector<string> ingredients, vector<int>amounts) {
	string finalDish = name + '|' + to_string(price) + '`';
	for (int i = 0; i < ingredients.size(); i++) {
		finalDish += (ingredients[i] + ',');
	}
	finalDish[finalDish.length() - 1] = '?';
	for (int i = 0; i < amounts.size(); i++) {
		finalDish += (to_string(amounts[i]) + ' ');
	}
	finalDish = finalDish.erase(finalDish.length() - 1);
	fstream file("Menu.txt", ios::in | ios::out);
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string temp;
		getline(file, temp);
	}
	file.clear();
	file << finalDish<<endl;
	file.close();
	return 1;
}
int main()
{
	string role;
	cout << "Select a role: ";
	cin >> role;

	while (role != "Manager" && role != "Waiter")
	{
		std::cout << "This role was invalid! Please select another one (Waiter or Manager): ";
		std::cin >> role;
	}

	displayUserOptions(role);
	int option;
	std::cin >> option;
	while (validateOption(option, role)) {
		if (option == 1) {
			printMenu();
			break;
		}
		if (option == 2) {
			std::cout << "What would you like to order?: " << std::endl;
			string order;
			std::cin >> order;
			//makeAnOrder(order);
			break;
		}
		if (option == 3) {
			//TODO...
			break;
		}
		if (option == 4) {
			showAllPreviousOrders();
			break;
		}
		if (option == 5) {
			showSortedOrders();
			break;
		}
		if (option == 6 && role == "Waiter") {
			//TODO...
		}
		if (option == 6 && role == "Manager") {
			showRemainingProducts();
		}
		if (option == 7 && role == "Manager") {
			string product;
			std::cout << "Please enter the name of the product: " << std::endl;
			std::cin >> product;
			int amount;
			std::cout << "Please enter the amount: " << std::endl;
			std::cin >> amount;
			int result = deleteProductToStorage(product, amount);
			if (result == 0) {
				cout << "There was an error with removing the selected product!";
			}
			else {
				cout << "Product has been removed successfully!" << endl;
			}
			break;
		}
		if (option == 8 && role == "Manager") {
			string product;
			std::cout << "Please enter the name of the product: " << std::endl;
			std::cin >> product;
			int amount;
			std::cout << "Please enter the amount: " << std::endl;
			std::cin >> amount;
			int result = addProductToStorage(product, amount);
			if (result == 0) {
				cout << "There was an error with adding the selected product!";
			}
			else {
				cout << "Product has been added successfully!" << endl;
			}
			break;
		}
		if (option == 9 && role == "Manager") {
			//showTodaysTurnover();
		}
		if (option == 10 && role == "Manager") {
			//finishTurnoverForToday();
		}
		if (option == 12 && role == "Manager") {
			string name;
			double price;
			vector<string> ingredients;
			vector<int> amounts;
			cout << "Enter dish's name(use '_' instead of ' '): " << endl;
			cin >> name;
			int indexOfSpace = name.find('_');
			if (indexOfSpace != -1) {
				name[indexOfSpace] = ' ';
			}
			while (checkIfDishExists(name)) {
				cout << "Dish with that name already exists! Enter a new dish: " << endl;
				cin >> name;
				int indexOfSpace = name.find('_');
				if (indexOfSpace != -1) {
					name[indexOfSpace] = ' ';
				}
			}
			cout << "How much will that dish cost: " << endl;
			cin >> price;
			cout << "How many ingredients will this dish have?" << endl;
			int ingredientsCount;
			cin >> ingredientsCount;
			for (int i = 0; i < ingredientsCount; i++) {
				string ingredient;
				int amount;
				cout << "Enter ingredient " << i+1 << endl;
				cin >> ingredient;
				cout << "How much " << ingredient << " would you need"<<endl;
				cin >> amount;
				ingredients.push_back(ingredient);
				amounts.push_back(amount);
			}
			createDish(name, price, ingredients, amounts);
			break;
		}
		if ((option == 7 && role == "Waiter") || (option == 14 && role == "Manager")) {
			displayUserOptions(role);
		}
	}
}