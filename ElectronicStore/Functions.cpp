#include "Order.hpp"
#include "Accessory.hpp"
#include "Gadget.hpp"
#include "Functions.hpp"
#include "FunctionsForClasses.hpp"
#include "Person.hpp"


static gadget::Gadget* makeGadget(product::Product* product) { return dynamic_cast<gadget::Gadget*>(product); }

static accessory::Accessory* makeAccessory(product::Product* product) { return dynamic_cast<accessory::Accessory*>(product); }

template <typename T>
std::string to_string_with_precision(const T value, const int n = STATE::DEFAULT_DIGITS_AFTER_POINT) {
	std::ostringstream out;
	out.precision(n);
	out << std::fixed <<  std::setprecision(STATE::FIXED_DIGITS_AFTER_POINT) << value;
	return std::move(out).str();
}

template<typename T>
int personalCabinet(T& person) {
	system("cls");
	std::string menu = "\t\t\t\t\t\t\t\t�������� �����:\n"
					   "\t\t\t\t\t\t\t\t1. ����������� ����������\n"
					   "\t\t\t\t\t\t\t\t2. �������� ����������\n"
					   "\t\t\t\t\t\t\t\t3. ������� �������\n"
					   "\t\t\t\t\t\t\t\t0. �����";

	for (;;) {
		std::cout << menu;
		int menuChoice = stringToNum(menu, 80, PERS_CAB::PERSONAL_CABINET_Y_COORD);

		switch (menuChoice) {
			case PERS_CAB::SHOW_INFO: {
				if (auto client = dynamic_cast<client::Client*>(person); client) {
					client->showPersonalInfo();
				}
				else {
					dynamic_cast<employee::Employee*>(person)->showPersonalInfo();
				}
				std::cout << "\t\t\t\t\t\t\t\t";
				system("pause");
				system("cls");
				break;
			}
			case PERS_CAB::CHANGE_INFO: {
				if (auto client = dynamic_cast<client::Client*>(person); client) {
					client->changePersonalInfo();
				}
				else {
					dynamic_cast<employee::Employee*>(person)->changePersonalInfo();
				}
				break;
			}
			case PERS_CAB::DELETE_ACCOUNT: {
				system("cls");
				std::cout << "\t\t\t\t\t\t\t\t�� ������������� ������ ������� ���� �������? (�): ";

				std::string choice;
				getline(std::cin, choice);

				for (auto& letter : choice) { letter = tolower(letter); }

				if (choice != "�" && choice != "��") {
					system("cls");
					return 0;
				}

				// get all clients to delete one of them and rewrite
				std::vector<Person*> usersVector;
				readUsersDataFromFiles(usersVector);

				auto clientAccount = find_if(usersVector.begin(), usersVector.end(), [&person](Person* person_) {
					return dynamic_cast<Person*>(person)->getId() == person_->getId();
				});
				usersVector.erase(clientAccount);
				writeUsersDataIntoFiles(usersVector);


				// rewrite orders
				std::vector<order::Order*> ordersVector;
				readOrders(ordersVector);
				
				ordersVector.erase(std::remove_if(ordersVector.begin(), ordersVector.end(),
					[&person](order::Order* order) {
						return dynamic_cast<Person*>(person)->getId() == order->getClientID();
					}), ordersVector.end());
				
				std::ofstream out("Orders.txt");
				out.close();

				for (const auto& order : ordersVector) {
					order->writeIntoFile();
				}

				delete person;
				person = nullptr;

				std::cout << "\t\t\t\t\t\t\t\t������� ������� ������!\n\n\t\t\t\t\t\t\t\t";
				system("pause");
				system("cls");
				return 1;
			}
			case STATE::EXIT : { 
				system("cls");
				return 0;
			}
			default: { errorInput(80, FIND_PRODUCTS::FIND_PRODUCTS_Y_COORD); }
		}
	}
}



std::vector<product::Product*> readProducts() {
	std::ifstream inGadget("Gadgets.txt", std::ios::in);
	std::ifstream inAccessory("Accessories.txt", std::ios::in);

	std::vector<product::Product*> productsVector;

	// check if sm-ng wrong while opening files
	if (!inAccessory.is_open() || !inGadget.is_open()) {
		std::cout << "���������� ������� ���� ��� ���������� ������!\n"
			"������ ��������? (�): ";

		std::string choice;
		getline(std::cin, choice);

		for (auto& letter : choice) { letter = tolower(letter); }

		if (choice == "�" || choice == "��") {
			system("cls");
			return productsVector;
		}
	}

	// read data from file and add it into productsVector
	for (;!inGadget.eof();) {
		gadget::Gadget gadget;
		inGadget >> gadget;
		if (!gadget.getId()) { break; }
		productsVector.push_back(new gadget::Gadget(gadget));
	}

	// same as above
	for (;!inAccessory.eof();) {
		accessory::Accessory accessory;
		inAccessory >> accessory;
		if (!accessory.getId()) { break; }
		productsVector.push_back(new accessory::Accessory(accessory));
	}

	return productsVector;
}

int readUsersDataFromFiles(std::vector<Person*>& usersVector) {
	std::ifstream getClient("Clients.txt");
	std::ifstream getEmployee("Employees.txt");

	if (!getClient.is_open() || !getEmployee.is_open()) {
		std::cout << "���������� ������� ���� ��� ���������� ������!\n"
			"������ ��������? (�): ";
		std::string choice;
		getline(std::cin, choice);

		for (auto& letter : choice) { letter = tolower(letter); }

		if (choice == "�" || choice == "��") {
			system("cls");
			return STATE::EXIT;
		}
	}

	for (;!getClient.eof();) {

		client::Client client;

		getClient >> client;
		if (!client.getId()) { break; }

		usersVector.push_back(new client::Client(client));
	}
	getClient.close();

	for (;!getEmployee.eof();) {

		employee::Employee employee;
		
		getEmployee >> employee;
		if (!employee.getId()) { break; }

		usersVector.push_back(new employee::Employee(employee));
	}
	getEmployee.close();

	return STATE::STABLE;
}

int writeUsersDataIntoFiles(const std::vector<Person*>& usersVector) {
	std::ofstream output;

	// clear all txt files
	output.open("Clients.txt");
	output.close();

	output.open("Employees.txt");
	output.close();

	for (const auto& user : usersVector) {
		if (auto client = dynamic_cast<client::Client*>(user); client) {
			if (!client->writeIntoFile()) { return STATE::EXIT; }
			continue;
		}
		if (auto employee = dynamic_cast<employee::Employee*>(user); employee) {
			if (!employee->writeIntoFile()) { return STATE::EXIT; }
			continue;
		}
	}
	return STATE::STABLE;
}


std::string encrypt(const std::string& plaintext, const std::string& key) {
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(ctx);

	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr);

	std::string ciphertext;
	ciphertext.resize(plaintext.size() + AES_BLOCK_SIZE, '\0');

	int len = 0;
	EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(&ciphertext[0]), &len,
		reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());

	int ciphertextLen = len;

	EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&ciphertext[len]), &len);
	ciphertextLen += len;

	EVP_CIPHER_CTX_free(ctx);

	return ciphertext.substr(0, ciphertextLen);
}

std::string decrypt(const std::string& ciphertext, const std::string& key) {
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(ctx);

	EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr);

	std::string plaintext;
	plaintext.resize(ciphertext.size(), '\0');

	int len = 0;
	EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]), &len,
		reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());

	int plaintextLen = len;

	EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&plaintext[len]), &len);
	plaintextLen += len;

	EVP_CIPHER_CTX_free(ctx);

	return plaintext.substr(0, plaintextLen);
}






//std::string encrypt(std::string plain_text, std::string key, std::string iv) {
//	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
//	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.c_str(), (unsigned char*)iv.c_str());
//
//	int block_size = EVP_CIPHER_CTX_block_size(ctx);
//	int cipher_text_len = plain_text.length() + block_size;
//	unsigned char* cipher_text = new unsigned char[cipher_text_len];
//	int len;
//
//	EVP_EncryptUpdate(ctx, cipher_text, &len, (unsigned char*)plain_text.c_str(), plain_text.length());
//
//	int final_len;
//
//	EVP_EncryptFinal_ex(ctx, cipher_text + len, &final_len);
//
//	std::string result((char*)cipher_text, len + final_len);
//	delete[] cipher_text;
//
//	EVP_CIPHER_CTX_free(ctx);
//
//	return result;
//}
//
//std::string decrypt(std::string cipher_text, std::string key, std::string iv) {
//	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
//	EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.c_str(), (unsigned char*)iv.c_str());
//
//	int block_size = EVP_CIPHER_CTX_block_size(ctx);
//	int plain_text_len = cipher_text.length() + block_size;
//	unsigned char* plain_text = new unsigned char[plain_text_len];
//	int len;
//
//	EVP_DecryptUpdate(ctx, plain_text, &len, (unsigned char*)cipher_text.c_str(), cipher_text.length());
//
//	int final_len;
//
//	EVP_DecryptFinal_ex(ctx, plain_text + len, &final_len);
//
//	std::string result((char*)plain_text, len + final_len);
//	delete[] plain_text;
//
//	EVP_CIPHER_CTX_free(ctx);
//
//	return result;
//}



float stringToNum(const std::string& text, const short& xCoord, const short& yCoord) {
	std::string str;
	for (;;) {
		gotoxy(xCoord, yCoord);
		getline(std::cin, str);
		try {
			for (int i = 0; i < str.size(); ++i) {
				if ((str.at(i) < '0' - 1 || str.at(i) > '9') && str.at(i) != '.') { std::stof("brute call except"); }
			}
			return(stof(str));
		}
		catch (std::invalid_argument) {
			errorInput(xCoord, yCoord);
			std::cout << text;
		}
	}
}

std::string hashEncrypt(const unsigned char* str) {
	const unsigned char* data = str;
	size_t length = sizeof(data);

	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(data, length, hash);

	char mdString[SHA_DIGEST_LENGTH];

	for (int i = 0; i < (SHA_DIGEST_LENGTH / 2) - 1; i++) {
		snprintf(&mdString[i * 2], sizeof(&mdString[i * 2]), "%02x", (unsigned int)hash[i]);
	}
	return mdString;
}

std::pair<std::string, std::string> reg() {
	system("cls");

	std::string tabulation = "\t\t\t\t\t\t\t\t";

	std::string name = correctUserName();

	std::string password, passwordConfirmation;
	bool equals = false;
	for (;!equals;) {
		do {
			std::cout << "\n" + tabulation + "������� ������: ";
			password = enteringPassword();
		} while (!correctPassword(password));

		std::cout << "\n" + tabulation + "����������� ������: ";
		passwordConfirmation = enteringPassword();

		if (password == passwordConfirmation) { equals = true; }
		else { std::cout << "\n\n" + tabulation + "������ �� ���������!" << std::endl; }
	}

	const char* cPassword = password.c_str();

	return { name, cPassword };
}

std::string correctUserName() {
	std::string tabulation = "\t\t\t\t\t\t\t\t";

	std::string name, enterName = tabulation + "������� ���� ���: ";

	std::cout << enterName;
	getline(std::cin, name);

	while (name.size() < CORRECT_USERNAME_LEN::MIN_USERNAME_LEN) {
		gotoxy(82, REG_MENU_COORD::USER_NAME_Y_COORD);

		SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED); // change text color
		std::cout << "��� ������� ��������!";
		SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY);

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear
		
		system("cls");
		std::cout << enterName;
		getline(std::cin, name);
	}

	while (name.size() > CORRECT_USERNAME_LEN::MAX_USERNAME_LEN) {
		gotoxy(82, REG_MENU_COORD::USER_NAME_Y_COORD);

		SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED); // change text color
		std::cout << "��� ������� ������!";
		SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY);

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear

		system("cls");
		std::cout << enterName;
		getline(std::cin, name);
	}

	std::string notAllowedSymbs = "0123456789*-+!#$%^&()_=?><`~,.�;:@\'\"\\/";
	for (auto symb : name) {
		if (std::find(notAllowedSymbs.cbegin(), notAllowedSymbs.cend(), symb) != notAllowedSymbs.cend()) {
			gotoxy(82, REG_MENU_COORD::USER_NAME_Y_COORD);

			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED); // change text color
			std::cout << "�� ����� ������������ ������!";
			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY);

			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear

			system("cls");

			name = correctUserName();
		}
	}
	return name;
}

std::string enteringPassword() {
	char ch{};
	int j = 0;
	char asciiChar[50]{};

	while (ch != 13) {
		ch = _getch();
		if (ch == 13) {
			break;
		}
		else if (ch == 8 && j > 0) {
			std::cout << "\b\b ";
			asciiChar[--j] = '\0';
		}
		else if (j >= 0) {
			//std::cout << "\b" << ch;
			//Sleep(100);
			std::cout << "\b* ";
			int asciiVal = ch;
			asciiChar[j++] = static_cast<char>(asciiVal); // get symb from its code
		}
	}

	return std::string(asciiChar);
}

bool correctPassword(const std::string& password) {
	std::string tabulation = "\t\t\t\t\t\t\t\t";

	if (password.size() < CORRECT_PASSWORD_LEN::MIN_PASSWORD_LEN) {
		std::cout << "\n" + tabulation + "������ ������� ��������!" << std::endl;
		return STATE::EXIT;
	}

	if (password.size() > CORRECT_PASSWORD_LEN::MAX_PASSWORD_LEN) {
		std::cout << "\n" + tabulation + "������ ������� ������!" << std::endl;
		return STATE::EXIT;
	}

	std::string notAllowedSymbs = "*-+!#$%^&()_=?><`~,.�;@\'\"\\/ ";
	for (auto symb : password) {
		if (std::find(notAllowedSymbs.cbegin(), notAllowedSymbs.cend(), symb) != notAllowedSymbs.cend()) {
			std::cout << "\n" + tabulation + "�� ����� ������������ ������!" << std::endl;
			return STATE::EXIT;
		}
	}
	return STATE::STABLE;
}

bool correctEmail(const std::string& email) {
	const char atSymbol = '@';

	if (auto iter = std::find(email.begin(), email.end(), atSymbol); iter == email.end() || email.at(0) == atSymbol) {
		std::cout << "\n\t\t\t\t\t\t\t\t�������� ������ �����!" << std::endl;
		return STATE::EXIT;
	};

	std::string notAllowedSymbs = " *-+!#$%^&()_=?><`~,�;\'\"\\/";
	for (auto symb : email) {
		if (std::find(notAllowedSymbs.cbegin(), notAllowedSymbs.cend(), symb) != notAllowedSymbs.cend()) {
			std::cout << "\n\t\t\t\t\t\t\t\t�� ����� ������������ ������!" << std::endl;
			return STATE::EXIT;
		}
	}
	return STATE::STABLE;
}

void gotoxy(const short& xCoord, const short& yCoord) {
	SetConsoleCursorPosition(hStdOut, { xCoord, yCoord });
}

void errorInput(const short& xCoord, const short& yCoord) {
	gotoxy(xCoord, yCoord);

	SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED); // change text color
	std::cout << "�������� ����!";
	SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY);
	
	std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear
	
	system("cls");

	/*gotoxy(xCoord, yCoord);

	std::cout << std::setfill(' ') << std::setw(ERROR_INPUT::WHITESPACES_LEN) << "" << std::setfill(' ');*/
}



static void redirectToMenu(std::vector<Person*>& usersVector, const int& id) {
	system("cls");

	auto person = std::find_if(usersVector.begin(), usersVector.end(),
		[&id](const Person* person) {
			return person->getId() == id;
		});

	if (auto client = dynamic_cast<client::Client*>(*person); client) {
		clientMenu(client);
		return;
	}
	if (auto employee = dynamic_cast<employee::Employee*>(*person); employee) {
		employeeMenu(employee);
		return;
	}
}




void authorizeUser() {
	system("cls");

	std::vector<Person*> usersVector;

	if (!readUsersDataFromFiles(usersVector)) { return; }

	std::string tabulation = "\t\t\t\t\t\t\t\t";
	
	std::string text = tabulation + "������� ���� ID: ";

	std::cout << text;
	int id = stringToNum(text, 81, AUTH_MENU_COORD::AUTHORIZE_MENU_Y_COORD);

	// iterator from usersVector with correct id 
	auto user = std::find_if(usersVector.begin(), usersVector.end(),
		[&id](Person*& user) { // lambda func
			return user->getId() == id;
		});

	if (user == usersVector.end()) {
		std::cout << "\n" + tabulation + "� ������� ��� ������������ � ����� ID!\n\n" + tabulation;
		system("pause");
		system("cls");
		return;
	}

	std::cout << tabulation + "������� ������: ";
	std::string password = enteringPassword();

	int incorrectInputCounter = 0;

	while (decrypt((*user)->getPassword(), "0x30") != password && incorrectInputCounter != INCORRECT_PASSWORD::ATTEMPTS_COUNT) {
		std::cout << "\n\n" + tabulation + "�������� ������!\n" + tabulation + "� ��� �������� " << INCORRECT_PASSWORD::ATTEMPTS_COUNT - incorrectInputCounter << " �������\n" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // sleep while incorrect message disappeared
		gotoxy(INCORRECT_PASSWORD::INCORRECT_PASSWORD_X_COORD, INCORRECT_PASSWORD::INCORRECT_PASSWORD_Y_COORD);

		// clear password input by entering 27*27 white spaces
		std::cout << std::setfill(' ') << std::setw(ERROR_X_COORD * ERROR_X_COORD) << "" << std::setfill(' ') << std::endl << std::endl;
		gotoxy(INCORRECT_PASSWORD::INCORRECT_PASSWORD_X_COORD, INCORRECT_PASSWORD::INCORRECT_PASSWORD_Y_COORD);

		std::cout << tabulation + "������� ������: ";
		password = enteringPassword();
		++incorrectInputCounter;
	}

	if (incorrectInputCounter == INCORRECT_PASSWORD::ATTEMPTS_COUNT) {
		std::cout << "\n\n" + tabulation + "�� ��������� ���������� ������� �����!\n\n" + tabulation;
		system("pause");
		system("cls");
		return;
	}

	redirectToMenu(usersVector, id);
}


void registerUser() {
	system("cls");

	std::string tabulation = "\t\t\t\t\t\t\t\t";

	std::string roleMenu = tabulation + "�������� ����:\n" +
		tabulation + "1. ������\n" +
		tabulation + "2. ��������\n" +
		tabulation + "0. �����\n";
	std::cout << roleMenu;
	int menuChoice = stringToNum(roleMenu, 79, REG_MENU_COORD::REGISTER_MENU_Y_COORD);

	std::vector<Person*> usersVector;

	if (!readUsersDataFromFiles(usersVector)) { return; }

	switch (menuChoice) {
		case REGISTER::CLIENT: {
			const int id = client::registerClient(usersVector);
			redirectToMenu(usersVector, id);
			break;
		}
		case REGISTER::EMPLOYEE: {
			const int id = employee::registerEmployee(usersVector);
			redirectToMenu(usersVector, id);
			// same
			break;
		}
		case STATE::EXIT: {
			system("cls");
			return;
		}
		default: { errorInput(79, REG_MENU_COORD::REGISTER_MENU_Y_COORD); }
	}
}



std::vector<product::Product*>& findProducts() {

	/*
		it's probably better to read data from file in each func below,
		cause user can choose return back and our vector will be usefull
		and we make more useless actions
	*/

	std::ifstream inGadget("Gadgets.txt", std::ios::in);
	std::ifstream inAccessory("Accessories.txt", std::ios::in);

	std::vector<product::Product*> productsVector = readProducts();

	system("cls");
	std::string menu =  "\t\t\t\t\t\t\t\t����� ������ ��:\n"
						"\t\t\t\t\t\t\t\t1. ����\n"
						"\t\t\t\t\t\t\t\t2. ��������\n"
						"\t\t\t\t\t\t\t\t3. ���� ������� ������� (�� ���������� ���� �� ��������)\n"
						"\t\t\t\t\t\t\t\t4. ID �������, ������������ � �����������\n"
						"\t\t\t\t\t\t\t\t0. �����";
	
	for (;;) {
		std::cout << menu;
		int menuChoice = stringToNum(menu, 81, FIND_PRODUCTS::FIND_PRODUCTS_Y_COORD);

		switch (menuChoice) {
			case FIND_PRODUCTS::FIND_BY_PRICE: {
				findProductsByPrice(productsVector);
				break;
			}
			case FIND_PRODUCTS::FIND_BY_NAME: {
				findProductsByName(productsVector);
				break;
			}
			case FIND_PRODUCTS::FIND_BY_RELEASE_YEAR: {
				findProductsByReleaseYear(productsVector);
				break;
			}
			case FIND_PRODUCTS::FIND_BY_ID: {
				findProductsByID(productsVector);
				break;
			}
			case STATE::EXIT: {
				system("cls");
				return productsVector;
			}
			default: { errorInput(81, FIND_PRODUCTS::FIND_PRODUCTS_Y_COORD); }
		}
	}
}

void findProductsByPrice(const std::vector<product::Product*>& productsVector) {
	system("cls");

	float leftRange = 0.F, rightRange = 0.F;

	for (;;) {
		std::string leftRng = "\t\t\t\t\t\t\t\t������� ������� ��������\n"
			"\t\t\t\t\t\t\t\t��: ";
		std::cout << leftRng;
		leftRange = stringToNum(leftRng, 68, FIND_PRODUCTS::LEFT_RANGE_Y_COORD);

		std::string rightRng = "\t\t\t\t\t\t\t\t��: ";
		std::cout << rightRng;
		rightRange = stringToNum(leftRng + to_string_with_precision(leftRange) + '\n' + rightRng, 68, FIND_PRODUCTS::RIGHT_RANGE_Y_COORD);

		if (leftRange < rightRange) { break; } // if left < right then all is ok otherwise error input

		gotoxy(FIND_PRODUCTS::ERROR_X_COORD, FIND_PRODUCTS::ERROR_Y_COORD);

		SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED);
		std::cout << "\t\t\t\t\t\t\t\t~> ����� ������� �� ����� ���� ������ ������ <~";
		SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY);

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear
		system("cls");
	}

	std::vector<product::Product*> selectedProductsVector;

	for (const auto& product : productsVector) {
		if (auto gadget = makeGadget(product); gadget &&
			gadget->getPrice() >= leftRange && gadget->getPrice() <= rightRange)
		{
			selectedProductsVector.push_back(new gadget::Gadget(*gadget));
			continue;
		}
		if (auto accessory = makeAccessory(product); accessory && 
			accessory->getPrice() >= leftRange && accessory->getPrice() <= rightRange)
		{
			selectedProductsVector.push_back(new accessory::Accessory(*accessory));
			continue;
		}
	}

	if (!selectedProductsVector.size()) {
		std::cout << "\t\t\t\t\t\t\t\t�� ������ ������� ������ �� ������� :(\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear
		
		system("cls");
		return;
	}

	showProductsInfo(selectedProductsVector);
	system("pause");
	system("cls");
}

static char tolowerRus(char symbol) {
	switch (symbol) {
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		case '�': { return '�'; }
		{ default: return '\0'; }
	}
}

std::string makeStrToLower(std::string text) {
	std::string tolowerStr;
	for (int i = 0; i < text.size(); ++i) {
		char symbol = tolowerRus(text.at(i));

		if (symbol == '\0') {
			tolowerStr += tolower(text.at(i));
			continue;
		}
		tolowerStr += symbol;
	}
	return tolowerStr;
}

void findProductsByName(const std::vector<product::Product*>& productsVector) {
	system("cls");


	std::string name = "\t\t\t\t\t\t\t\t������� ��� ��������: ";
	std::cout << name;
	getline(std::cin, name);
	
	name = makeStrToLower(name); // make name lower case to make search not case sensetive

	std::vector<product::Product*> selectedProductsVector;

	for (const auto& product : productsVector) {

		std::string toLowerProductName = makeStrToLower(product->getName());

		if(auto gadget = makeGadget(product); gadget &&
			toLowerProductName == name)
		{
			selectedProductsVector.push_back(new gadget::Gadget(*gadget));
			continue;
		}
		if (auto accessory = makeAccessory(product); accessory &&
			toLowerProductName == name)
		{
			selectedProductsVector.push_back(new accessory::Accessory(*accessory));
			continue;
		}
	}

	if (!selectedProductsVector.size()) {
		std::cout << "\t\t\t\t\t\t\t\t�� ������ ������� ������ �� ������� :(\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear

		system("cls");
		return;
	}

	showProductsInfo(selectedProductsVector);
	system("pause");
	system("cls");
}

void findProductsByReleaseYear(const std::vector<product::Product*>& productsVector) {
	system("cls");

	int year = 0;
	for (;;) {
		std::string yearStr = "\t\t\t\t\t\t\t\t������� ��� �������: ";
		std::cout << yearStr;
		year = stringToNum(yearStr, 86, FIND_PRODUCTS::FIND_BY_RELEASE_YEAR_Y_COORD);

		std::vector<product::Product*> selectedProductsVector;

		int currentYear = std::stoi(std::format("{:%Y}", std::chrono::system_clock::now())); //to get current year
		if (year <= currentYear && year >= STATE::MIN_RELEASE_YEAR ) { break; };

		gotoxy(86, FIND_PRODUCTS::FIND_BY_RELEASE_YEAR_Y_COORD);

		SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED);
		std::cout << "\t\t\t\t\t\t\t\t~> ��� ������ ���� � ��������� [" << STATE::MIN_RELEASE_YEAR << "; " << currentYear << "] <~";
		SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY);

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear
		system("cls");
	}

	std::vector<product::Product*> selectedProductsVector;

	for (const auto& product : productsVector) {
		if (auto gadget = makeGadget(product); gadget &&
			gadget->getYearOfRelease() >= year)
		{
			selectedProductsVector.push_back(new gadget::Gadget(*gadget));
		}
	}

	if (!selectedProductsVector.size()) {
		std::cout << "\t\t\t\t\t\t\t\t�� ������ ������� ������ �� ������� :(\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear

		system("cls");
		return;
	}

	showProductsInfo(selectedProductsVector);
	system("pause");
	system("cls");
}

void findProductsByID(const std::vector<product::Product*>& productsVector) {
	system("cls");

	std::string idStr = "\t\t\t\t\t\t\t\t������� ID �������: ";
	std::cout << idStr;
	int id = stringToNum(idStr, 84, FIND_PRODUCTS::FIND_BY_ID_Y_COORD);

	std::vector<product::Product*> selectedProductsVector;

	for (const auto& product : productsVector) {
		if (auto accessory = makeAccessory(product); accessory &&
			accessory->CompatibleWith() == id)
		{
			selectedProductsVector.push_back(new accessory::Accessory(*accessory));
			continue;
		}
	}

	if (!selectedProductsVector.size()) {
		std::cout << "\t\t\t\t\t\t\t\t�� ������ ������� ������ �� ������� :(\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear

		system("cls");
		return;
	}

	showProductsInfo(selectedProductsVector);
	system("pause");
	system("cls");
}



void showAllProducts() {
	std::ifstream inGadget("Gadgets.txt", std::ios::in);
	std::ifstream inAccessory("Accessories.txt", std::ios::in);

	std::vector<product::Product*> productsVector = readProducts();

	showProductsInfo(productsVector);
	system("pause");
	system("cls");
}

void orderProduct(const client::Client* client) {
	std::ifstream inGadget("Gadgets.txt", std::ios::in);
	std::ifstream inAccessory("Accessories.txt", std::ios::in);

	std::vector<product::Product*> productsVector = readProducts();

	inGadget.close();
	inAccessory.close();

	int choice = 0;
	for (;;) {
		showProductsInfo(productsVector); // this func is not good here cause we take O(N) each time when we choose wrong item

		std::string productChoice = "�������� �������: ";
		
		gotoxy(ORDER_PRODUCT::ORDER_PRO_Y_COORD, ORDER_PRODUCT::ORDER_PRO_Y_COORD);
		std::cout << productChoice << std::setfill(' ') << std::setw(ORDER_PRODUCT::ORDER_PRO_X_COORD) << "" << std::setfill(' ');
	
		choice = stringToNum(productChoice, ORDER_PRODUCT::ORDER_PRO_X_COORD, ORDER_PRODUCT::ORDER_PRO_Y_COORD);
		if (choice > productsVector.size() || choice <= 0) {
			errorInput(ORDER_PRODUCT::ORDER_PRO_X_COORD, ORDER_PRODUCT::ORDER_PRO_Y_COORD);
			continue;
		}
		break;
	}

	if (!productsVector.at(choice - STATE::STABLE)->getCount()) {
		system("cls");
		std::cout << "\n\t\t\t\t\t\t\t\t� ��������� ������� ������ ��� � �������!\n" << std::endl;
		system("pause");
		system("cls");
		return;
	}

	system("cls");
	std::string countStr = "\t\t\t\t\t\t\t\t������� ����������, ������� ������ ����������:";
	std::cout << countStr;

	int count = stringToNum(countStr, 111, SHOW_ORDER::ORDER_COUNT_Y_COORD);

	showOrder(count, productsVector.at(choice - STATE::STABLE));

	if (!confirmOrder()) { return; }

	time_t now = time(0);
	tm* ltm = localtime(&now);
	std::string date = std::to_string(1900 + ltm->tm_year) + '.' + std::to_string(1 + ltm->tm_mon) + '.' + std::to_string(ltm->tm_mday) + '-' + std::to_string(ltm->tm_hour) + ':' + std::to_string(ltm->tm_min);

	order::Order* order = new order::Order(ordersCount() + STATE::STABLE, ORDER_PRODUCT::ORDER_IN_CONFIRMATION, productsVector.at(choice - STATE::STABLE)->getPrice(),
										   productsVector.at(choice - STATE::STABLE)->getName(), date, client->getId(), productsVector.at(choice - STATE::STABLE)->getId());
	order->writeIntoFile();

	std::cout << "\n\n\t\t\t\t\t\t\t\t����� ��������!\n\t\t\t\t\t\t\t\t��������� ������ ����� ������ � ������� \"��������� ��������� ������\"\n\n\t\t\t\t\t\t\t\t";
	system("pause");
	system("cls");
}



bool confirmOrder() {
	std::string choice;
	for (;;) {
		std::cout << "\n\n����������� �����? (�/�): ";
		getline(std::cin, choice);

		for (int i = 0; i < choice.size(); ++i) { choice.at(i) = tolower(choice.at(i)); }
		if (choice == "�") {
			std::cout << "\n\n����� �������!\n" << std::endl;
			system("pause");
			system("cls");
			return false;
		}
		if (choice == "�") {
			return true;
		}

		if (choice != "�" || choice != "�") { std::cout << "\n�������� ����!"; }
	}
}

void showOrder(const int& count, product::Product* product) {
	float price = 0;
	if (auto gadget = makeGadget(product); gadget) {
		price = gadget->getPrice();

		std::cout << "\n��� �����\n" <<
					 "�����: " << gadget->getBrand() << std::endl <<
					 "��������: " << gadget->getName() << std::endl <<
					 "����: " << std::fixed << std::setprecision(2) << price << std::endl <<
					 "����������: " << count;
	}
	else {
		auto accessory = makeAccessory(product);
		price = accessory->getPrice();

		std::cout << "\n��� �����\n" <<
					"������������: " << (accessory->IsWireless() ? "��" : "���") << std::endl <<
					"��������: " << accessory->getName() << std::endl <<
					"����: " << std::fixed << std::setprecision(2) << accessory->getPrice() << std::endl <<
					"����������: " << count;
	}
	std::cout << "\n\n����� � ������: " << std::fixed << std::setprecision(2) << price * count;
}

void readOrders(std::vector<order::Order*>& ordersVector) {
	std::ifstream inOrder("Orders.txt", std::ios::in);

	for (;!inOrder.eof();) {
		order::Order order;
		inOrder >> order;
		if (!order.getId()) { return; }
		ordersVector.push_back(new order::Order(order));
	}
}

int ordersCount() {
	std::ifstream inOrder("Orders.txt", std::ios::in);

	int count = 0;
	for (count; !inOrder.eof(); ++count) {
		std::string temp{};
		getline(inOrder, temp);
	}

	inOrder.close();
	
	return --count;
}

int dateToInt(std::string& date) {
	date.erase(std::remove_if(date.begin(), date.end(),
		[](char symb) {
			return symb == '.' || symb == ':' || symb == '-';
		}), date.end());
	date.at(10) = '\0';
	return stoi(date);
}

void trackOrder(const client::Client* client) {
	std::vector<order::Order*> allOrdersVector, clientsOrdersVector;
	readOrders(allOrdersVector);

	for (auto& order : allOrdersVector) {
		if (order->getClientID() == client->getId()) {
			clientsOrdersVector.push_back(order);
		}
	}

	for (auto& order : allOrdersVector) {
		std::string orderDate = order->getPurchaseDate();
		int date = dateToInt(orderDate);

		time_t now = time(0);
		tm* ltm = localtime(&now);
		int currentDate = (1900 + ltm->tm_year) * pow(10, 6) + (1 + ltm->tm_mon) * pow(10, 4) + (ltm->tm_mday) * pow(10, 2) + ltm->tm_hour;

		if (currentDate - date > 12) { order->setState(true); } // abstract time value which should order pass to be verified
	}

	system("cls");
	std::cout << "\t\t\t\t\t\t\t\t���� ������\n\n"
		<< std::setw(PRODUCTS_INFO::PRODUCT_ID) << std::left << "| ID"
		<< std::setw(PRODUCTS_INFO::PRODUCT_NAME) << std::left << "| ��������"
		<< std::setw(20) << std::left << "| ���� ������(���.)"
		<< std::setw(20) << std::left << "| ���� ������"
						<< std::left << "| ����� �����������" << std::endl
		<< std::setfill('�') << std::setw(105) << "" << std::setfill(' ') << std::endl;

	for (auto& order : clientsOrdersVector) {
		std::cout << std::setw(PRODUCTS_INFO::PRODUCT_ID) << std::left << "| " + std::to_string(order->getId())
				  << std::setw(PRODUCTS_INFO::PRODUCT_NAME) << std::left << "| " + order->getName()
											  << std::setw(2) << std::left << "| " << std::setw(18) << std::left << std::fixed << std::setprecision(2) << order->getPurchaseCount()
											  << std::setw(20) << std::left << "| " + order->getPurchaseDate()
										<< std::left << (order->getState() ? "| ��" : "| ���") << std::endl;
	}

	std::cout << "\n\n\t\t\t\t\t\t\t\t";
	system("pause");
	system("cls");
}




void sortProducts() {
	system("cls");

	std::string menu = "\t\t\t\t\t\t\t\t������������� ��:\n"
					   "\t\t\t\t\t\t\t\t1. ����\n"
					   "\t\t\t\t\t\t\t\t2. ����������\n"
					   "\t\t\t\t\t\t\t\t3. ��������\n"
					   "\t\t\t\t\t\t\t\t0. �����\n";


	for (;;) {
		std::cout << menu;
		int choice = stringToNum(menu, 82, SORT_PRODUCTS::SORT_CHOICE_Y_COORD);

		switch (choice) {
		case FIND_PRODUCTS::FIND_BY_PRICE: {
			sortByPrice();
			break;
		}
		case FIND_PRODUCTS::FIND_BY_NAME: {
			sortByCount();
			break;
		}
		case FIND_PRODUCTS::FIND_BY_RELEASE_YEAR: {
			sortByName();
			break;
		}
		case STATE::EXIT: {
			system("cls");
			return;
		}
		default: { errorInput(82, FIND_PRODUCTS::FIND_PRODUCTS_Y_COORD); }
		}
	}
}



void sortByPrice() {
	std::ifstream inGadget("Gadgets.txt", std::ios::in);
	std::ifstream inAccessory("Accessories.txt", std::ios::in);

	std::vector<product::Product*> productsVector = readProducts();

	sort(productsVector.begin(), productsVector.end(), [](const product::Product* first, const product::Product* second) {
		return first->getPrice() < second->getPrice();
	});

	system("cls");
	showProductsInfo(productsVector);
	system("pause");
	system("cls");
}

void sortByCount() {
	std::ifstream inGadget("Gadgets.txt", std::ios::in);
	std::ifstream inAccessory("Accessories.txt", std::ios::in);

	std::vector<product::Product*> productsVector = readProducts();

	sort(productsVector.begin(), productsVector.end(), [](const product::Product* first, const product::Product* second) {
		return first->getCount() < second->getCount();
	});

	system("cls");
	showProductsInfo(productsVector);
	system("pause");
	system("cls");
}

void sortByName() {
	std::ifstream inGadget("Gadgets.txt", std::ios::in);
	std::ifstream inAccessory("Accessories.txt", std::ios::in);

	std::vector<product::Product*> productsVector = readProducts();

	sort(productsVector.begin(), productsVector.end(), [](const product::Product* first, const product::Product* second) {
		return first->getName() > second->getName();
	});

	system("cls");
	showProductsInfo(productsVector);
	system("pause");
	system("cls");
}



void showProductsInfo(const std::vector<product::Product*>& selectedProductsVector) {
	system("cls");

	std::cout << "\t\t\t\t\t\t\t\t���������� �� ������ �������:\n\n";
	
	std::cout << std::setw(PRODUCTS_INFO::NUM_COLUMN) << std::left << "| �"
			  << std::setw(PRODUCTS_INFO::PRODUCT_TYPE) << std::left << "| ��� ��������"
			  << std::setw(PRODUCTS_INFO::PRODUCT_ID) << std::left << "| ID"
			  << std::setw(PRODUCTS_INFO::PRODUCT_PRICE) << std::left << "| ���� (���.)"
			  << std::setw(PRODUCTS_INFO::PRODUCT_NAME) << std::left << "| ��������"
			  << std::setw(PRODUCTS_INFO::YEAR_PRODUCT_RELEASE) << std::left << "| ��� �������"
			  << std::setw(PRODUCTS_INFO::PRODUCT_BRAND) << std::left << "| �����"
			  << std::setw(PRODUCTS_INFO::IS_WIRELESS) << std::left << "| ������������"
			  << std::setw(PRODUCTS_INFO::COMPATIBLE_WITH) << std::left << "| ��������� � ��������" << std::endl
			  << std::setfill('�') << std::setw(PRODUCTS_INFO::UNDERSCORE_LEN) << "" << std::setfill(' ') << std::endl;

	{
		int count = 1;
		for (const auto& product : selectedProductsVector) {
			if (auto gadget = makeGadget(product); gadget) {
				std::cout << "| " << std::setw(PRODUCTS_INFO::NUM_COLUMN - 2) << std::left << count++;
				gadget->displayDescription("������");
				continue;
			}
			if (auto accessory = makeAccessory(product); accessory) {
				std::cout << "| " << std::setw(PRODUCTS_INFO::NUM_COLUMN - 2) << std::left << count++;
				accessory->displayDescription("���������");
				continue;
			}
		}
	}

	std::cout << '\n' << std::endl;
}



void clientMenu(client::Client* client) {
	std::string menu = "\t\t\t\t\t\t\t\t���� ������������:\n"
					   "\t\t\t\t\t\t\t\t1. ����� ������\n"
					   "\t\t\t\t\t\t\t\t2. ����������� ��� ������\n"
					   "\t\t\t\t\t\t\t\t3. �������� �����\n"
					   "\t\t\t\t\t\t\t\t4. ��������� ��������� ������\n"
					   "\t\t\t\t\t\t\t\t5. ������ �������\n"
					   "\t\t\t\t\t\t\t\t6. ������������� ������\n"
					   "\t\t\t\t\t\t\t\t0. �����\n";
	for (;;) {
		std::cout << menu;
		int menuChoice = stringToNum(menu, 83, CLIENT_MENU::CLIENT_MENU_Y_COORD);

		switch (menuChoice) {
		case CLIENT_MENU::FIND_PRODUCT: {
			findProducts(); // done
			break;
		}
		case CLIENT_MENU::SHOW_PRODUCT_CHARACT: {
			showAllProducts(); // done
			break;
		}
		case CLIENT_MENU::ORDER_PRODUCT: {
			orderProduct(client); // done
			break;
		}
		case CLIENT_MENU::TRACK_ORDER: { // done
			trackOrder(client);
			break;
		}
		case CLIENT_MENU::PERSONAL_CABINET: { //done
			if (personalCabinet(client)) { return; }; // done
			break;
		}
		case CLIENT_MENU::SORT_PRODUCTS: {
			sortProducts(); // done
			break;
		}
		case STATE::EXIT: {
			system("cls");
			return;
		}
		default: { errorInput(83, CLIENT_MENU::CLIENT_MENU_Y_COORD); }
		}
	}
}



void addProduct() {
	system("cls");
	std::string categoryStr = "\t\t\t\t\t\t\t\t�������� ��������� ������:\n"
							"\t\t\t\t\t\t\t\t1. ������\n"
							"\t\t\t\t\t\t\t\t2. ���������\n"
							"\t\t\t\t\t\t\t\t0. �����";
	int category = -1;
	for (;category;) {
		std::cout << categoryStr;
		category = stringToNum(categoryStr, 91, ADD_PRODUCT::ADD_PROD_Y_COORD);

		switch (category) {
			case ADD_PRODUCT::ADD_GADGET: {
				addGadget();
				break;
			}
			case ADD_PRODUCT::ADD_ACCESSORY: {
				category = 0;
				addAccessory();
				break;
			}
			case STATE::EXIT: {
				category = 0;
				system("cls");
				return;
			}
			default: {
				category = 0;
				errorInput(91, ADD_PRODUCT::ADD_PROD_Y_COORD);
			}
		}
	}
}

void addGadget() {
	system("cls");
	std::ifstream inGadget("Gadgets.txt", std::ios::in);

	std::vector<gadget::Gadget*> gadgetsVector;

	// check if sm-ng wrong while opening files
	if (!inGadget.is_open()) {
		std::cout << "\t\t\t\t\t\t\t\t���������� ������� ���� ��� ���������� ������!\n"
					"\t\t\t\t\t\t\t\t������ ��������? (�): ";

		std::string choice;
		getline(std::cin, choice);

		for (auto& letter : choice) { letter = tolower(letter); }

		if (choice == "�" || choice == "��") {
			system("cls");
			return;
		}
	}

	for (;!inGadget.eof();) {
		gadget::Gadget gadget;
		inGadget >> gadget;
		if (!gadget.getId()) { break; }
		gadgetsVector.push_back(new gadget::Gadget(gadget));
	}

	inGadget.close();

	int id = gadgetsVector.size() + 1;

	std::string countStr = "\t\t\t\t\t\t\t\t������� ���������� ������ ������: ";
	std::cout << countStr;

	int count = stringToNum(countStr, 98, 0);
	while (count < 1 || count > 10000) {
		errorInput(98, 0);
		std::cout << countStr;
		count = stringToNum(countStr, 98, 0);
	}

	std::string costStr = "\t\t\t\t\t\t\t\t������� ���� ������ ������: ";
	std::cout << costStr;

	float cost = stringToNum(costStr, 92, 1);
	while (cost < 0.50 || cost > 1000) {
		errorInput(92, 1);
		std::cout << countStr << count << '\n' << costStr;
		cost = stringToNum(countStr + std::to_string(count) +'\n' + costStr, 92, 1);
	}
		
	std::string nameStr = "\t\t\t\t\t\t\t\t������� ��� ������ ������: ";
	std::cout << nameStr;

	std::string name;
	do {
		getline(std::cin, name);

		if (name.size() > 40) {
			gotoxy(91, 2);
			std::cout << "�� ����� ������� ������ ��������!                       ";
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC));
			system("cls");

			std::cout << countStr << count << '\n' << costStr << cost << '\n' << nameStr;
			gotoxy(91, 2);

			name = "";
			break;
		}

		if (!name.size()) {
			gotoxy(91, 2);
			std::cout << "�� ����� ������� �������� ��������!                       ";
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC));
			system("cls");

			std::cout << countStr << count << '\n' << costStr << cost << '\n' << nameStr;
			gotoxy(91, 2);

			name = "";
			break;
		}

		std::string notAllowedSymbs = "0123456789*-+!#$%^&()_=?><`~,.�;@\'\"\\/\n";
		for (auto symb : name) {
			if (std::find(notAllowedSymbs.cbegin(), notAllowedSymbs.cend(), symb) != notAllowedSymbs.cend()) {
				gotoxy(91, 2);
				std::cout << "�� ����� ������������ ������!                       ";
				std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC));
				system("cls");

				std::cout << countStr << count << '\n' << costStr << cost << '\n' << nameStr;
				gotoxy(91, 2);

				name = "";
				break;
			}
		}
	} while (name == "");


	std::string yearOfReleaseStr = "\t\t\t\t\t\t\t\t������� ��� ������� ������: ";
	std::cout << yearOfReleaseStr;

	int yearOfRelease = stringToNum(yearOfReleaseStr, 91, 3);

	time_t now = time(0);
	tm* ltm = localtime(&now);
	while (yearOfRelease < 2000 || yearOfRelease >(1900 + ltm->tm_year)) {
		gotoxy(92, 3);
		std::cout << "�� ����� ������������ ��� (2000; current year]!                         ";
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC));
		system("cls");

		std::cout << countStr << count << '\n' << costStr << cost << '\n' << nameStr << name << '\n' << yearOfReleaseStr;
		gotoxy(92, 3);

		yearOfRelease = stringToNum(yearOfReleaseStr, 93, 3);
	}


	std::string brandStr = "\t\t\t\t\t\t\t\t������� ����� ������ ������: ";
	std::cout << brandStr;

	std::string brand;
	do {
		getline(std::cin, brand);
		std::string notAllowedSymbs = "0123456789*-+!#$%^&()_=?><`~,.�;@\'\"\\/\n ";
		for (auto symb : brand) {
			if (std::find(notAllowedSymbs.cbegin(), notAllowedSymbs.cend(), symb) != notAllowedSymbs.cend()) {
				gotoxy(93, 4);
				std::cout << "�� ����� ������������ ������!                          ";
				std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC));
				system("cls");

				std::cout << countStr << count << '\n' << costStr << cost << '\n' << nameStr << name << '\n' << yearOfReleaseStr << yearOfRelease << '\n' << brandStr;
				gotoxy(93, 4);

				brand = "";
				break;
			}
		}
	} while (brand == "");

	gadgetsVector.push_back(new gadget::Gadget(id, count, cost, name, yearOfRelease, brand));

	std::ofstream out("Gadgets.txt");
	for (const auto& gadget : gadgetsVector) {
		out << gadget << '\n';
	}

	std::cout << "\n\n\t\t\t\t\t\t\t\t����� ��������!\n\n\t\t\t\t\t\t\t\t";
	system("pause");
	system("cls");
}

void addAccessory() {
	system("cls");
	std::ifstream inAccessory("Accessories.txt", std::ios::in);

	std::vector<accessory::Accessory*> accessoriesVector;

	if (!inAccessory.is_open()) {
		std::cout << "\t\t\t\t\t\t\t\t���������� ������� ���� ��� ���������� ������!\n"
			"\t\t\t\t\t\t\t\t������ ��������? (�): ";

		std::string choice;
		getline(std::cin, choice);

		for (auto& letter : choice) { letter = tolower(letter); }

		if (choice == "�" || choice == "��") {
			system("cls");
			return;
		}
	}

	for (;!inAccessory.eof();) {
		accessory::Accessory accessory;
		inAccessory >> accessory;
		if (!accessory.getId()) { break; }
		accessoriesVector.push_back(new accessory::Accessory(accessory));
	}

	inAccessory.close();

	int id = accessoriesVector.size() + 1 + 1000;
	
	std::string countStr = "\t\t\t\t\t\t\t\t������� ���������� ������ ����������: ";
	std::cout << countStr;

	int count = stringToNum(countStr, 102, 0);
	while (count < 1 || count > 10000) {
		errorInput(102, 0);
		std::cout << countStr;
		count = stringToNum(countStr, 102, 0);
	}



	std::string costStr = "\t\t\t\t\t\t\t\t������� ���� ������ ����������: ";
	std::cout << costStr;

	float cost = stringToNum(countStr + std::to_string(count) + '\n' + costStr, 96, 1);
	while (cost < 0.50 || cost > 1000) {
		errorInput(96, 1);
		std::cout << countStr << count << '\n' << costStr;
		cost = stringToNum(countStr + std::to_string(count) + '\n' + costStr, 96, 1);
	}



	std::string nameStr = "\t\t\t\t\t\t\t\t������� ��� ������ ����������: ";
	std::cout << nameStr;

	std::string name;
	do {
		getline(std::cin, name);
		std::string notAllowedSymbs = "0123456789*-+!#$%^&()_=?><`~,.�;@\'\"\\/\n";
		for (auto symb : name) {
			if (std::find(notAllowedSymbs.cbegin(), notAllowedSymbs.cend(), symb) != notAllowedSymbs.cend()) {
				gotoxy(95, 2);
				std::cout << "�� ����� ������������ ������!                       ";
				std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC));
				system("cls");

				std::cout << countStr << count << '\n' << costStr << cost << '\n' << nameStr;
				gotoxy(95, 2);

				name = "";
				break;
			}
		}
	} while (name == "");



	std::string isWirelessStr = "\t\t\t\t\t\t\t\t�������� �� ��������� ������������ (�): ";
	std::cout << isWirelessStr;

	std::string isWireless;
	getline(std::cin, isWireless);

	for (auto& letter : isWireless) {
		letter = tolower(letter);
	}

	bool wireless = false;
	if (isWireless == "�" || isWireless == "��") {
		wireless = true;
	}



	std::string compatibleWithStr = "\t\t\t\t\t\t\t\t� ����� �������� ��������� ����� ��������� (������� 0, ���� �� ��������� �� � �����):";

	std::ifstream inGadget("Gadgets.txt", std::ios::in);

	std::vector<gadget::Gadget*> gadgetsVector;

	for (;!inGadget.eof();) {
		gadget::Gadget gadget;
		inGadget >> gadget;
		if (!gadget.getId()) { break; }
		gadgetsVector.push_back(new gadget::Gadget(gadget));
	}

	inGadget.close();

	std::cout << compatibleWithStr;
	
	std::cout << "\n\n" << std::setw(PRODUCTS_INFO::PRODUCT_TYPE) << std::left << "| ��� ��������"
		<< std::setw(PRODUCTS_INFO::PRODUCT_ID) << std::left << "| ID"
		<< std::setw(PRODUCTS_INFO::PRODUCT_PRICE) << std::left << "| ���� (���.)"
		<< std::setw(PRODUCTS_INFO::PRODUCT_NAME) << std::left << "| ��������"
		<< std::setw(PRODUCTS_INFO::YEAR_PRODUCT_RELEASE) << std::left << "| ��� �������"
		<< std::setw(PRODUCTS_INFO::PRODUCT_BRAND) << std::left << "| �����" << std::endl
		<< std::setfill('�') << std::setw(PRODUCTS_INFO::UNDERSCORE_LEN) << "" << std::setfill(' ') << '\n' << std::endl;


	for (const auto& gadget : gadgetsVector) {
		gadget->displayDescription("������");
	}

	int compatibleWith = stringToNum(compatibleWithStr, 150, 4);

	while (compatibleWith < 0 || compatibleWith > gadgetsVector.size()) {
		errorInput(150, 4);
		std::cout << countStr + std::to_string(count) + '\n' + costStr + std::to_string(cost) + '\n' + nameStr + name + '\n' + isWirelessStr << isWireless << '\n' << compatibleWithStr;

		std::cout << "\n\n" << std::setw(PRODUCTS_INFO::PRODUCT_TYPE) << std::left << "| ��� ��������"
			<< std::setw(PRODUCTS_INFO::PRODUCT_ID) << std::left << "| ID"
			<< std::setw(PRODUCTS_INFO::PRODUCT_PRICE) << std::left << "| ���� (���.)"
			<< std::setw(PRODUCTS_INFO::PRODUCT_NAME) << std::left << "| ��������"
			<< std::setw(PRODUCTS_INFO::YEAR_PRODUCT_RELEASE) << std::left << "| ��� �������"
			<< std::setw(PRODUCTS_INFO::PRODUCT_BRAND) << std::left << "| �����" << std::endl
			<< std::setfill('�') << std::setw(PRODUCTS_INFO::UNDERSCORE_LEN) << "" << std::setfill(' ') << '\n' << std::endl;

		for (const auto& gadget : gadgetsVector) {
			gadget->displayDescription("������");
		}

		compatibleWith = stringToNum(compatibleWithStr, 150, 4);
	}

	accessoriesVector.push_back(new accessory::Accessory(id, count, cost, name, wireless, compatibleWith));

	std::ofstream out("Accessories.txt");
	for (const auto& accessory : accessoriesVector) {
		out << accessory << '\n';
	}

	system("cls");
	std::cout << "\t\t\t\t\t\t\t\t��������� ��������!\n\n\t\t\t\t\t\t\t\t";
	system("pause");
	system("cls");
}

void deleteProduct() {
	system("cls");
	std::vector<product::Product*> productsVector = readProducts();

	std::cout << "\t\t\t\t\t\t\t\t�������� ID ��������:\n" << std::endl;

	std::cout << std::setw(PRODUCTS_INFO::PRODUCT_TYPE) << std::left << "| ��� ��������"
		<< std::setw(PRODUCTS_INFO::PRODUCT_ID) << std::left << "| ID"
		<< std::setw(PRODUCTS_INFO::PRODUCT_PRICE) << std::left << "| ���� (���.)"
		<< std::setw(PRODUCTS_INFO::PRODUCT_NAME) << std::left << "| ��������"
		<< std::setw(PRODUCTS_INFO::YEAR_PRODUCT_RELEASE) << std::left << "| ��� �������"
		<< std::setw(PRODUCTS_INFO::PRODUCT_BRAND) << std::left << "| �����"
		<< std::setw(PRODUCTS_INFO::IS_WIRELESS) << std::left << "| ������������"
		<< std::setw(PRODUCTS_INFO::COMPATIBLE_WITH) << std::left << "| ��������� � ��������" << std::endl
		<< std::setfill('�') << std::setw(PRODUCTS_INFO::UNDERSCORE_LEN) << "" << std::setfill(' ') << std::endl;

	{
		for (const auto& product : productsVector) {
			if (auto gadget = makeGadget(product); gadget) {
				gadget->displayDescription("������");
				continue;
			}
			if (auto accessory = makeAccessory(product); accessory) {
				accessory->displayDescription("���������");
				continue;
			}
		}
	}

	int productID = stringToNum("\t\t\t\t\t\t\t\t�������� ����� ��������: ", 86, 0);

	auto productIter = find_if(productsVector.begin(), productsVector.end(), [&productID](const product::Product* product) {
			return product->getId() == productID;
		});

	if (productIter != productsVector.end()) {
		productsVector.erase(productIter, productIter + 1);
		
		std::ofstream outGadget("Gadgets.txt");
		std::ofstream outAccessory("Accessories.txt");

		for (const auto& product : productsVector) {
			if (auto gadget = dynamic_cast<gadget::Gadget*>(product); gadget) {
				outGadget << gadget << '\n';
				continue;
			}
			outAccessory << dynamic_cast<accessory::Accessory*>(product) << '\n';
		}

		outGadget.close();
		outAccessory.close();

		system("cls");
		std::cout << "\t\t\t\t\t\t\t\t������� ������� ������!\n\n\t\t\t\t\t\t\t\t";
		system("pause");
		system("cls");
		return;
	}

	system("cls");
	std::cout << "\t\t\t\t\t\t\t\t��� �������� � ����� ID!\t���������� ��� ���\n\n\t\t\t\t\t\t\t\t";
	system("pause");

	deleteProduct();
}

void createOrdersReport() {
	std::ifstream in("Orders.txt", std::ios::in);

	std::vector<order::Order*> ordersVector;

	for (;!in.eof();) {
		order::Order order;
		in >> order;
		if (!order.getId()) { break; }
		ordersVector.push_back(new order::Order(order));
	}
	in.close();

	std::ofstream out("ordersReport.txt");

	out << std::setw(11) << std::left << "| ID ������"
		<< std::setw(18) << std::left << "| ��������� ������"
		<< std::setw(12) << std::left << "| ID �������"
		<< std::setw(13) << std::left << "| ID ��������"
		<< std::setw(19) << std::left << "| ���� ������(���.)"
		<< std::setw(23) << std::left << "| ��� ���������� ������"
		<< std::setw(40) << std::left << "| �������� ������" << std::endl
		<< std::setfill('�') << std::setw(123) << "" << std::setfill(' ') << std::endl;

	float totalCost = 0.f;
	for (const auto& order : ordersVector) {
		totalCost += order->getPurchaseCount();

		std::string orderDate = order->getPurchaseDate();
		int date = dateToInt(orderDate);

		time_t now = time(0);
		tm* ltm = localtime(&now);
		int currentDate = (1900 + ltm->tm_year) * pow(10, 6) + (1 + ltm->tm_mon) * pow(10, 4) + (ltm->tm_mday) * pow(10, 2) + ltm->tm_hour;

		if (currentDate - date > 12) { order->setState(true); } // abstract time value which should order pass to be verified

		out << std::setw(2) << std::left << "| " << std::setw(9) << std::left << order->getId()
			<< std::setw(2) << std::left << "| " << std::setw(16) << std::left << ((order->getState()) ? "�����" : "� ���������")
			<< std::setw(2) << std::left << "| " << std::setw(10) << std::left << order->getClientID()
			<< std::setw(2) << std::left << "| " << std::setw(11) << std::left << order->getProductID()
			<< std::setw(2) << std::left << "| " << std::setw(17) << std::left << order->getPurchaseCount()
			<< std::setw(2) << std::left << "| " << std::setw(21) << std::left << order->getPurchaseDate()
			<< std::setw(2) << std::left << "| " << std::setw(38) << std::left << order->getName() << std::endl
			<< std::setfill('�') << std::setw(125) << "" << std::setfill(' ') << std::endl;
	}

	out << "\n�����: " << ordersVector.size() << " ������� �� ����� " << totalCost << " ���.";

	system("cls");
	std::cout << "\t\t\t\t\t\t\t\t����� � ������� ��������!\n\n\t\t\t\t\t\t\t\t";
	system("pause");
	system("cls");
}



void employeeMenu(employee::Employee* employee) {
	std::string menu = "\t\t\t\t\t\t\t\t���� ����������:\n"
					   "\t\t\t\t\t\t\t\t1. �������� �����\n"
					   "\t\t\t\t\t\t\t\t2. ������� �����\n"
					   "\t\t\t\t\t\t\t\t3. ����� ������\n"
					   "\t\t\t\t\t\t\t\t4. ����������� ��� ������\n"
					   "\t\t\t\t\t\t\t\t5. ������ �������\n"
					   "\t\t\t\t\t\t\t\t6. ����������� ����� � �������\n"
					   "\t\t\t\t\t\t\t\t0. �����\n";
	for (;;) {
		std::cout << menu;
		int menuChoice = stringToNum(menu, 81, EMPLOYEE_MENU::EMPLOYEE_MENU_Y_COORD);

		switch (menuChoice) {
			case EMPLOYEE_MENU::ADD_PRODUCT: {
				addProduct();
				break;
			}
			case EMPLOYEE_MENU::DELETE_PRODUCT: {
				deleteProduct();
				break;
			}
			case EMPLOYEE_MENU::FIND_PRODUCTS: {
				findProducts();
				break;
			}
			case EMPLOYEE_MENU::SHOW_ALL_PRODUCTS: {
				showAllProducts();
				break;
			}
			case EMPLOYEE_MENU::PERSONAL_CAB: {
				personalCabinet(employee);
				break;
			}
			case EMPLOYEE_MENU::ORDERS_REPORT: {
				createOrdersReport();
				break;
			}
			case STATE::EXIT: {
				system("cls");
				return;
			}
			default: { errorInput(81, EMPLOYEE_MENU::EMPLOYEE_MENU_Y_COORD); }
		}
	}
}
