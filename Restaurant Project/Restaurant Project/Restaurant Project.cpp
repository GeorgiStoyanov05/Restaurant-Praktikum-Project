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
	if (role != "Manager" && (option < 1 || option>15)) {
		return 0;
	}
	else if (role != "Waiter" && (option < 1 || option>8)) {
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
		if (product == "") {
			break;
		}
		int separatorIndex = productData.find('|');
		string productName = productData.substr(0, separatorIndex);
		double prodRemainingAmount = stod(productData.substr(separatorIndex + 1, (productData.length() - (separatorIndex + 1))));
		if (productName == product) {
			isFound = 1;
			prodRemainingAmount += amount;
			productData = productName + '|' + to_string(prodRemainingAmount);
			temp << productData<<endl;
			continue;
		}
			temp << productData<<endl;
	}
	if (isFound == 0) {
		string newProductData = product + '|'+to_string(amount);
		temp << newProductData<<endl;
	}
	file.close();
	temp.close();
	remove("Storage.txt");
	int result = rename("temp.txt", "Storage.txt");
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
		if (option == 7 && role == "Manager") {

		}
		if (option == 8 && role == "Manager") {
			string product;
			std::cout << "Please enter the name of the product: " << std::endl;
			std::cin >> product;
			int amount;
			std::cout << "Please enter the amount: " << std::endl;
			std::cin >> amount;
			addProductToStorage(product, amount);
			break;
		}
	}
}