#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

double dailyTurnover;

static bool isLeapYear(int year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int  checkDateInput(int day, int month, int year) {

	if (year < 1 || year > 9999) {
		return 0;
	}

	if (month < 1 || month > 12) {
		return 0;
	}

	int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (month == 2 && isLeapYear(year)) {
		daysInMonth[1] = 29;
	}

	if (day < 1 || day > daysInMonth[month - 1]) {
		return 0;
	}

	return 1;
}

static double insertDailyTurnoverOnStart() {
	double turnover = 0.00;
	fstream file("Turnovers.txt");
	if (!file.is_open())return 0;
	while (!file.eof()) {
		string data;
		getline(file, data);
		if (data == "") continue;
		int separatorIndex = data.find('|');
		turnover = stod(data.substr(separatorIndex + 1, data.length() - separatorIndex));
	}
	return turnover;
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

static void incrementDate(int& day, int& month, int& year) {
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

static vector<string> splitStringToStrArr(string str, char separator) {
	vector<string>result;
	int indexOfSeparator = str.find_last_of(separator);
	if (indexOfSeparator == -1) {
		result.push_back(str);
	}
	else {
		while (str.size() != 0) {
			string strSub = str.substr(indexOfSeparator + 1, str.length() - indexOfSeparator);
			result.push_back(strSub);
			if (indexOfSeparator == -1) {
				str.erase(strSub.length());
				break;
			}
			else {
				str.erase(indexOfSeparator);
			}
			indexOfSeparator = str.find(separator);
		}
	}
	return result;
}

static double showTodaysTurnover() {
	ifstream file("Turnovers.txt");
	string turnover;
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string check;
		getline(file, check);
		if (check == "") continue;
		turnover = check;
	}
	file.close();
	int separatorIndex = turnover.find('|');
	double todaysTurnover = stod(turnover.substr(separatorIndex + 1, (turnover.length() - separatorIndex)));
	return todaysTurnover;
}

static int finishTurnoverForToday() {
	fstream file("Turnovers.txt");
	vector<string> turnovers;
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string check;
		getline(file, check);
		if (check == "") continue;
		turnovers.push_back(check);
	}
	file.close();
	string todaysDate = turnovers[turnovers.size() - 1].substr(0, 8);
	double todaysTurnOverAmount = stod(turnovers[turnovers.size() - 1].substr(11, turnovers[turnovers.size() - 1].length() - 8));
	string todaysData = todaysDate + "|" + to_string(dailyTurnover);
	turnovers[turnovers.size() - 1] = todaysData;
	vector<string> dateComponents = splitStringToStrArr(todaysDate, '.');
	vector<int>dateComponentsAsInt;
	for (int i = dateComponents.size() - 1; i >= 0; i--) {
		dateComponentsAsInt.push_back(stoi(dateComponents[i]));
	}
	incrementDate(dateComponentsAsInt[0], dateComponentsAsInt[1], dateComponentsAsInt[2]);
	string tomorrowsData = to_string(dateComponentsAsInt[0]) + '.' + to_string(dateComponentsAsInt[1]) + '.' + to_string(dateComponentsAsInt[2]);
	tomorrowsData += "|0.000000";
	turnovers.push_back(tomorrowsData);
	ofstream newFile("temp.txt");
	if (!newFile.is_open()) return 0;
	for (int i = 0; i < turnovers.size(); i++) {
		newFile << turnovers[i] << endl;
	}
	newFile.close();
	dailyTurnover = 0.00;
	remove("Turnovers.txt");
	int result = rename("temp.txt", "Turnovers.txt");
}

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

static int deleteProductFromStorage(string product, int amount, bool isForOrder) {
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
			if (prodRemainingAmount - amount <= 0 && isForOrder == 0) {
				prodRemainingAmount = 0;
			}
			if (prodRemainingAmount - amount <= 0 && isForOrder == 1) {
				return 0;
			}
			if (prodRemainingAmount - amount > 0) {
				prodRemainingAmount -= amount;
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

static int reduceProductsAmounts(vector<string>products, vector<int> amounts) {
	for (int i = 0; i < products.size(); i++) {
		if (deleteProductFromStorage(products[i], amounts[i], 1) == 0) {
			return 0;
		}
	}
	return 1;
}

static int increaseProductsAmounts(vector<string>products, vector<int> amounts) {
	for (int i = 0; i < products.size(); i++) {
		if (addProductToStorage(products[i], amounts[i]) == 0) {
			return 0;
		}
	}
	return 1;
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

static string increaseDayByOne(string date) {
	int month = stoi(date.substr(0, 2));
	int day = stoi(date.substr(3, 2));
	int year = stoi(date.substr(6, 4));
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
		if (dishName == order) {
			double price = stod(dish.substr(separationNameIndex + 1, (separationPriceIndex - separationNameIndex)));
			string ingredients = dish.substr(separationPriceIndex + 1, (separationIngredientsIndex - separationPriceIndex - 1));
			string amounts = dish.substr(separationIngredientsIndex + 1, (dish.length() - (separationIngredientsIndex + 1)));
			vector<string> ingredientsArr;
			vector<string> amountsStrArr;
			vector<int> amountsArr;
			ingredientsArr = splitStringToStrArr(ingredients, ',');
			amountsStrArr = splitStringToStrArr(amounts, ',');
			for (int i = 0; i < amountsStrArr.size(); i++) {
				amountsArr.push_back(stoi(amountsStrArr[i]));
			}
			if (reduceProductsAmounts(ingredientsArr, amountsArr) == 0) {
				cout << "This dish can not be prepared at the moment";
				return 0;
			}
			fstream ordersFile("Orders.txt", ios::in | ios::out);
			while (!ordersFile.eof()) {
				string temp;
				getline(ordersFile, temp);
			}
			ordersFile.clear();
			dailyTurnover += price;
			ordersFile << dishName << endl;
			ordersFile.close();
			return 1;
		}
	}
	return 0;
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

static int createDish(string name, int price, vector<string> ingredients, vector<int>amounts) {
	string finalDish = name + '|' + to_string(price) + '`';
	for (int i = 0; i < ingredients.size(); i++) {
		finalDish += (ingredients[i] + ',');
	}
	finalDish[finalDish.length() - 1] = '?';
	for (int i = 0; i < amounts.size(); i++) {
		finalDish += (to_string(amounts[i]) + ',');
	}
	finalDish = finalDish.erase(finalDish.length() - 1);
	fstream file("Menu.txt", ios::in | ios::out);
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string temp;
		getline(file, temp);
	}
	file.clear();
	file << finalDish << endl;
	file.close();
	return 1;
}

static int deleteDish(string name) {
	fstream oldFile("Menu.txt");
	ofstream newFile("temp.txt");
	while (!oldFile.is_open()) return 0;
	while (!oldFile.eof()) {
		string dish;
		getline(oldFile, dish);
		if (dish == "") continue;
		int dishNameSeparatorIndex = dish.find('|');
		string dishName = dish.substr(0, dishNameSeparatorIndex);
		if (dishName == name) {
			continue;
		}
		newFile << dish;
	}
	oldFile.close();
	newFile.close();
	remove("Menu.txt");
	int result = rename("temp.txt", "Menu.txt");
	return 1;
}

static int deleteOrder(string name) {
	ifstream file("Menu.txt");
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string dish;
		getline(file, dish);
		int separationNameIndex = dish.find('|');
		int separationIngredientsIndex = dish.find('?');
		int separationPriceIndex = dish.find('`');
		string dishName = dish.substr(0, separationNameIndex);
		if (dishName == name) {
			double price = stod(dish.substr(separationNameIndex + 1, (separationPriceIndex - separationNameIndex)));
			string ingredients = dish.substr(separationPriceIndex + 1, (separationIngredientsIndex - separationPriceIndex - 1));
			string amounts = dish.substr(separationIngredientsIndex + 1, (dish.length() - (separationIngredientsIndex + 1)));
			vector<string> ingredientsArr;
			vector<string> amountsStrArr;
			vector<int> amountsArr;
			ingredientsArr = splitStringToStrArr(ingredients, ',');
			amountsStrArr = splitStringToStrArr(amounts, ',');
			for (int i = 0; i < amountsStrArr.size(); i++) {
				amountsArr.push_back(stoi(amountsStrArr[i]));
			}
			increaseProductsAmounts(ingredientsArr, amountsArr);
			bool isFound = 0;
			ifstream oldOrdersFile("Orders.txt");
			ofstream newOrdersFile("temp.txt");
			while (!oldOrdersFile.eof()) {
				string order;
				getline(oldOrdersFile, order);
				if (order == dishName && isFound == 0) {
					isFound = 1;
					continue;
				}
				newOrdersFile << order << endl;
			}
			dailyTurnover -= price;
			oldOrdersFile.close();
			newOrdersFile.close();
			remove("Orders.txt");
			int result = rename("temp.txt", "Orders.txt");
			return 1;
		}
	}
	return 0;
}

static int checkIfOrderExists(string name) {
	fstream file("Orders.txt", ios::in | ios::out);
	if (!file.is_open()) return 0;
	while (!file.eof()) {
		string orderName;
		getline(file, orderName);
		if (orderName == name) {
			return 1;
		}
	}
	file.close();
	return 0;
}

static int printTurnoverData(string date) {
	ifstream file("Turnovers.txt");
	if (!file.is_open()) return 0;
	cout << endl;
	while (!file.eof()) {
		string turnoverData;
		getline(file, turnoverData);
		int separationIndex = turnoverData.find('|');
		string toDate = turnoverData.substr(0, separationIndex);
		reverse(toDate.begin(), toDate.end());
		reverse(date.begin(), date.end());
		if (toDate>= date) {
			turnoverData = turnoverData.substr(0, turnoverData.length() - 4);
			turnoverData.replace(separationIndex, 1, " -> ");
			turnoverData += "lv.";
			cout << turnoverData << endl;
		}
	}
	cout << endl;
	file.close();
}

int main()
{
	dailyTurnover = insertDailyTurnoverOnStart();
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
			cout << "What would you like to order(instead of ' ' use '_')?: " << endl;
			string order;
			cin >> order;
			if (!checkIfDishExists(order)) {
				cout << "There is no such dish on your menu!" << endl;
			}
			else {
				if (makeAnOrder(order) == 1) {
					cout << "Your order has been successful!" << endl;
				}
			}
			break;
		}
		if (option == 3) {
			cout << "What dish would you like to cancel(instead of ' ' use '_')?: " << endl;
			string dishName;
			cin >> dishName;
			if (checkIfOrderExists(dishName) == 0) {
				cout << "There is no such order!" << endl;
			}
			else {
				deleteOrder(dishName);
				cout << "The order has been cancelled successfully!" << endl;
			}
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
		if ((option == 6 && role == "Waiter") || (option == 9 && role == "Manager")) {
			double turnOver = showTodaysTurnover();
			cout << "Today's turnover is: " << turnOver << "lv." << endl;
			break;
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
			int result = deleteProductFromStorage(product, amount, 0);
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
		if (option == 10 && role == "Manager") {
			finishTurnoverForToday();
			break;
		}
		if (option == 11 && role == "Manager") {
			int day;
			int month;
			int year;
			cout << "Enter a day(1-31): " << endl;
			cin >> day;
			cout << "Enter a month(1-12): " << endl;
			cin >> month;
			cout << "Enter an year: " << endl;
			cin >> year;
			while (checkDateInput(day, month, year) == 0) {
				cout << "Sorry, but this date is invalid, let's try again:" << endl;
				cout << "Enter a day(1-31): " << endl;
				cin >> day;
				cout << "Enter a month(1-12): " << endl;
				cin >> month;
				cout << "Enter an year: " << endl;
				cin >> year;
			}
			string date = to_string(day) + '.' + to_string(month) + '.' + to_string(year);
			printTurnoverData(date);
			break;
		}
		if (option == 12 && role == "Manager") {
			string name;
			double price;
			vector<string> ingredients;
			vector<int> amounts;
			cout << "Enter the dish's name(use '_' instead of ' '): " << endl;
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
				cout << "Enter ingredient " << i + 1 << endl;
				cin >> ingredient;
				cout << "How much " << ingredient << " would you need" << endl;
				cin >> amount;
				ingredients.push_back(ingredient);
				amounts.push_back(amount);
			}
			createDish(name, price, ingredients, amounts);
			break;
		}
		if (option == 13 && role == "Manager") {
			string name;
			cout << "Enter the dish's name(use '_' instead of ' '): " << endl;
			cin >> name;
			int indexOfSpace = name.find('_');
			if (indexOfSpace != -1) {
				name[indexOfSpace] = ' ';
			}
			if (!checkIfDishExists(name)) {
				cout << "There is no dish with that name on the menu!" << endl;
			}
			else {
				deleteDish(name);
			}
			break;
		}
		if ((option == 7 && role == "Waiter") || (option == 14 && role == "Manager")) {
			displayUserOptions(role);
		}
	}
}