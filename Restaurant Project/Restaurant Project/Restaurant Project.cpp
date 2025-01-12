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

int main()
{
	char role[8];
	std::cout << "Select a role: ";
	std::cin >> role;
	while (compareStrings(role, "Manager")==0 && compareStrings(role, "Waiter")==0)
	{
		std::cout << "This role was invalid! Please select another one: ";
		std::cin >> role;
	}

	struct Student {
		char name[45];
		int age;
		int heigth;
	};

	std::cout << getStringLenght(role);
}