#include <iostream>

static int getStringLenght(const char* str)
{
	int length = 0;
	while (str[length] != '\0') {
		length++;
	}
	return length;
}

static bool compareStrings(const char* str1, const char* str2) {
	int strLenght1 = getStringLenght(str1);
	int strLenght2 = getStringLenght(str2);
	bool areEqual = 1;
	if (strLenght1 != strLenght2) {
		return 0;
	}
	else {
		for (int i = 0; i < strLenght1; i++) {
			if (str1[i] != str2[i])
			{
				areEqual = 0;
			}
		}
	}
	return areEqual;
}

static void displayUserOptions(const char* role) {
	if (compareStrings(role, "Waiter"))
	{
		std::cout << "1) See the menu" << std::endl;
		std::cout << "2) Order something from the menu" << std::endl;
		std::cout << "3) Cancel an order" << std::endl;
		std::cout << "4) See all previous orders" << std::endl;
		std::cout << "5) See all previous orders, sorted in alphabetical order, as well as the number of orders of each dish" << std::endl;
		std::cout << "6) See today's turnover" << std::endl;
		std::cout << "7) End your shift" << std::endl;
		std::cout << "8) Show all options again" << std::endl;
	}
	else if (compareStrings(role, "Manager"))
	{
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
	}
}

static bool validateOption(int option, const char* role) {
	if (compareStrings(role, "Manager") && (option < 1 || option>15)) {
		return 0;
	}
	else if (compareStrings(role, "Waiter") && (option < 1 || option>8)) {
		return 0;
	}
	return 1;
}

int main()
{
	char role[8];
	std::cout << "Select a role: ";
	std::cin >> role;

	while (compareStrings(role, "Manager") == 0 && compareStrings(role, "Waiter") == 0)
	{
		std::cout << "This role was invalid! Please select another one (Waiter or Manager): ";
		std::cin >> role;
	}

	displayUserOptions(role);
	int option;
	std::cin >> option;
	while (validateOption(option, role)) {
		std::cout << option<<std::endl;
		std::cin >> option;
	}

	struct Student {
		char name[45];
		int age;
		int heigth;
	};
}