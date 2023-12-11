#include "Order.hpp"
#include "Functions.hpp"
#include "FunctionsForClasses.hpp"


int main(int argc, char* argv[]) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::string tabulation = "\t\t\t\t\t\t\t\t";

	std::string menu = tabulation + "Выберите пункт меню: \n\n"
						+ tabulation + "1. Авторизоваться\n"
						+ tabulation + "2. Зарегистрироваться\n"
						+ tabulation + "0. Выйти\n";
	for(;;) {
		std::cout << menu;
		int menuChoice = stringToNum(menu, 85, 0);

		switch (menuChoice) {
			case MENU::AUTHORIZE: {
				authorizeUser();
				break;
			}
			case MENU::REGISTER: {
				registerUser();
				break;
			}
			case STATE::EXIT: { 
				system("cls");

				std::vector<Person*> usersVector;

				if (!readUsersDataFromFiles(usersVector)) { return STATE::EXIT; }

				if (menuChoice = writeUsersDataIntoFiles(usersVector); menuChoice) {
					return STATE::EXIT;
				}
				break;
			}
			default: { errorInput(85, 0); }
		}
	}

	return STATE::EXIT;
}
