#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Person.hpp"
#include "Exception.hpp"

namespace client {

	class Client : public Person {
		int productId;
		std::string email;
	public:
		Client() : Person(), productId(0), email("none") {};
		Client(const int& id_, const std::string& name_, const std::string& password_,
			const std::string& email_, const int& productId_ = 0);
		Client(const Client* client);
		~Client() {};

		int getProductId() const { return this->productId; }
		std::string getEmail() const { return this->email; }

		void setProductId(const int& productId_) { this->productId = productId_; }
		void setEmail(const std::string& email_) { this->email = email_; }

		int writeIntoFile();

		void showPersonalInfo() const override;
		void changePersonalInfo() override;

		static const std::string changeName(const std::string& name_);
		static const std::string changeEmail(const std::string& email_);

		friend std::ostream& operator <<(std::ostream& out, const Client* client);
		friend std::istream& operator >>(std::istream& in, Client& client);
	};



	inline Client::Client(const int& id_, const std::string& name_, const std::string& password_,
		const std::string& email_, const int& productId_) : Person(id_, name_, password_)
	{
		this->email = email_;
		this->productId = productId_;
	}

	inline Client::Client(const Client* client) {
		this->productId = client->productId;
		this->email = client->email;
	}



	inline void Client::showPersonalInfo() const {
		system("cls");
		std::cout << "\t\t\t\t\t\t\t\tИнформация о вас:\n" << std::endl << "\t\t\t\t\t\t\t\t"
			<< std::setfill('—') << std::setw(PERS_CAB::UNDERLINE_LEN) << "" << std::setfill(' ') << std::endl
			<< std::setw(PERS_CAB::LEFT_COLUMN_WIDTH) << std::left << "\t\t\t\t\t\t\t\t| Имя "
			<< std::setw(PERS_CAB::RIGHT_COLUMN_WIDTH) << std::left << "| " + this->getName() << std::endl << "\t\t\t\t\t\t\t\t"

			<< std::setfill('—') << std::setw(PERS_CAB::UNDERLINE_LEN) << "" << std::setfill(' ') << std::endl
			<< std::setw(PERS_CAB::LEFT_COLUMN_WIDTH) << std::left << "\t\t\t\t\t\t\t\t| Почта "
			<< std::setw(PERS_CAB::RIGHT_COLUMN_WIDTH) << std::left << "| " + this->email << std::endl << "\t\t\t\t\t\t\t\t"

			<< std::setfill('—') << std::setw(PERS_CAB::UNDERLINE_LEN) << "" << std::setfill(' ') << std::endl
			<< std::setw(PERS_CAB::LEFT_COLUMN_WIDTH) << std::left << "\t\t\t\t\t\t\t\t| ID "
			<< std::setw(PERS_CAB::RIGHT_COLUMN_WIDTH) << std::left << "| " + std::to_string(this->getId()) << std::endl << "\t\t\t\t\t\t\t\t"

			<< std::setfill('—') << std::setw(PERS_CAB::UNDERLINE_LEN) << "" << std::setfill(' ') << std::endl << std::endl;
	}

	inline void Client::changePersonalInfo() {
		std::string menu = "\t\t\t\t\t\t\t\tЧто вы хотите изменить:\n"
						   "\t\t\t\t\t\t\t\t1. Имя\n"
						   "\t\t\t\t\t\t\t\t2. Почту\n"
						   "\t\t\t\t\t\t\t\t0. Назад";

		for (;;) {
			this->showPersonalInfo();

			std::cout << menu;
			int menuChoice = stringToNum(menu, 82, PERS_CAB::CHANGE_INFO_Y_COORD);
			system("cls");

			switch (menuChoice) {
				case PERS_CAB::CHANGE_NAME : {
					this->setName(this->changeName(this->getName()));
					std::cout << "\n\t\t\t\t\t\t\t\tИмя успешно изменено!\n\n\t\t\t\t\t\t\t\t";
					system("pause");
					system("cls");
					break;
				}
				case PERS_CAB::CHANGE_EMAIL : {
					this->setEmail(this->changeEmail(this->getEmail()));
					std::cout << "\n\t\t\t\t\t\t\t\tПочта успешно изменена!\n\n\t\t\t\t\t\t\t\t";
					system("pause");
					system("cls");
					break;
				}
				case STATE::EXIT: {	break;	}
				default: { errorInput(82, PERS_CAB::CHANGE_INFO_Y_COORD); }
			}
			if (!menuChoice) { break; }
		}


		std::vector<client::Client*> clientsVector;

		std::ifstream getClient("Clients.txt");
		for (;!getClient.eof();) {

			client::Client client;

			getClient >> client;
			if (!client.getId()) { break; }

			clientsVector.push_back(new client::Client(client));
		}
		getClient.close();



		auto userIter = find_if(clientsVector.begin(), clientsVector.end(), [this](const Person* person) {
			return person->getId() == this->getId();
		});

		clientsVector.erase(userIter, userIter + 1);
		clientsVector.push_back(this);

		std::ofstream outClients("Clients.txt");
		outClients.close();


		for (const auto& client : clientsVector) {
			client->writeIntoFile();
		}
	}



	inline const std::string Client::changeName(const std::string& name_) {
		std::string name;
		for (;;) {
			name = correctUserName();
			if (name != name_) { return name; }

			gotoxy(82, REG_MENU_COORD::USER_NAME_Y_COORD);

			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED); // change text color
			std::cout << "\t\t\t\t\t\t\t\tИмена должны отличаться!\0";
			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY);

			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear

			system("cls");
		}
	}

	inline const std::string Client::changeEmail(const std::string& email_) {
		std::string email;
		for (;;) {
			do {
				std::cout << "\t\t\t\t\t\t\t\tВведите свою почту (text@text): ";
				getline(std::cin, email);
			} while (!correctEmail(email));

			if (email != email_) { return email; }

			gotoxy(96, PERS_CAB::CHANGE_EMAIL_Y_COORD);

			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED); // change text color
			std::cout << "\t\t\t\t\t\t\t\tПочты должны отличаться!\0";
			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY);

			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear

			system("cls");
		}
	}



	inline int Client::writeIntoFile() {
		std::ofstream output("Clients.txt", std::ios::out | std::ios::app);

		if (!output.is_open()) {
			std::cout << "Невозможно открыть файл для записи данных!\n"
				"Все данные будут утеряны!\n"
				"Хотите остаться? (д): ";
			std::string choice;
			getline(std::cin, choice);

			for (auto letter : choice) { tolower(letter); }

			if (choice == "д" || choice == "да") {
				system("cls");
				return EXIT;
			}
		}

		output << this;
		output.close();

		return STABLE;
	}



	inline std::ostream& operator <<(std::ostream& out, const Client* client) {
		out << client->getId() << "\t" << client->getEmail() << "\t\t"
			<< client->getPassword() << "\t\t" << client->getProductId() << "\t" << client->getName() << "\n";
		return out;
	}

	inline std::istream& operator >>(std::istream& in, Client& client) {
		int id = 0, productId = 0;
		std::string email, password, name;

		in >> id;
		if (!id) { return in; }

		in >> email >> password >> productId;
		getline(in, name);
		name.erase(name.begin(), name.begin() + STATE::STABLE);

		client.setEmail(email);
		client.setId(id);
		client.setName(name);
		client.setPassword(password);
		client.setProductId(productId);

		return in;
	}



	inline int registerClient(std::vector<Person*>& usersVector) {
		auto clientInfoPair = reg();

		std::string tabulation = "\t\t\t\t\t\t\t\t";

		std::string email;
		do {
			std::cout << "\n\n" + tabulation + "Введите свою почту (text@text): ";
			getline(std::cin, email);
		} while (!correctEmail(email));

		int id = usersVector.size() + ID_NUM::MIN_ID_NUM + 1;
		for (auto user : usersVector) {
			if (user->getId() != id) {
				break;
			}
			id++;
		}

		auto newClient = new Client(id, clientInfoPair.first, encrypt(clientInfoPair.second, "0x30"), email);
		
		std::fstream out("Clients.txt", std::ios::app);
		out << newClient;
		out.close();

		try {
			usersVector.push_back(newClient);
		}
		catch (std::bad_alloc errro) {
			throw(new BadAllocException<std::string>("не удалось выделить память под вектор класса client::Client!\n\n"));
		}
		catch (BadAllocException<std::string>* error) {
			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED); // change text color
			std::cout << "\n\t\t\t\t\t\t\t\tОшибка: " << error->getException();
			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY); // change text color
			return STATE::EXIT;
		}


		std::cout << "\n\t\t\t\t\t\t\t\tВаш ID для авторизации: " << id << "\n\t\t\t\t\t\t\t\tНайти его можно также в вашем личном кабинете\n" << std::endl;
		std::cout << tabulation;
		system("pause");
		system("cls");

		return id;
	}
}

#endif