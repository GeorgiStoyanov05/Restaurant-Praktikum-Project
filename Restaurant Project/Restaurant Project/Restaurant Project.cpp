#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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
*/

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
		if (option == 4) {
			showAllPreviousOrders();
			break;
		}
		if (option == 5) {
			showSortedOrders();
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
		if (option == 6 && role == "Manager") {
			showRemainingProducts();
		}
		if (option == 9 && role == "Manager") {
			//showTodaysTurnover();
		}
		if (option == 12 && role == "Manager") {
		}
		if ((option == 7 && role == "Waiter") || (option == 14 && role == "Manager")) {
			displayUserOptions(role);
		}
	}
}