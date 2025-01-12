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
	std::cin >> role;

	std::cout << role << std::endl;

	struct Student {
		char name[45];
		int age;
		int heigth;
	};

	std::cout << getStringLenght(role);
}