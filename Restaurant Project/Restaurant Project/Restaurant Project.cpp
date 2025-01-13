#include <iostream>
#include <fstream>
#include <string>

using namespace std;

static void displayUserOptions(string role) {
	if (role=="Waiter")
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
	else if (role=="Manager")
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
	if (role!="Manager" && (option < 1 || option>15)) {
		return 0;
	}
	else if (role!="Waiter" && (option < 1 || option>8)) {
		return 0;
	}
	return 1;
}

static int printMenu() {
	std::ifstream MyFile("Menu.txt");
	if (!MyFile.is_open()) return 0;
	std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
	while (!MyFile.eof()) {
		string dish;
		getline(MyFile, dish);
		int separatorIndex = dish.find('|');
		string dishName = dish.substr(0, separatorIndex - 1);
		std::cout << dish << std::endl;
	}
	std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
	return 1;
}

static int makeAnOrder(string order) {
	
}

static int addProductToStorage(const char* product, int amount) {
}

int main()
{
	string role;
	cout << "Select a role: ";
	cin >> role;

	while (role!="Manager" && role!="Waiter")
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
			makeAnOrder(order);
			break;
		}
		if (option == 7 && role=="Manager") {

		}
		if (option == 8 && role=="Manager") {
			char product[100]{};
			std::cout << "Please enter the name of the product: " << std::endl;
			std::cin >> product;
			int amount;
			std::cout << "Please enter the amount: " << std::endl;
			std::cin >> amount;
			addProductToStorage(product, amount);
		}
	}
}