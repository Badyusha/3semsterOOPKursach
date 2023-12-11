#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "Person.hpp"

namespace employee {

	class Employee : public Person {
	public:
		Employee() : Person() {};
		Employee(const int& id_, const std::string& name_, const std::string& password_);
		Employee(const Employee* employee);
		~Employee() {};

		void showPersonalInfo() const override;
		void changePersonalInfo() override;

		const std::string changeName(const std::string& name_);

		int writeIntoFile();

		friend std::ostream& operator <<(std::ostream& out, const Employee* employee);
		friend std::istream& operator >>(std::istream& in, Employee& employee);
	};

	inline Employee::Employee(const int& id_, const std::string& name_, const std::string& password_) 
					: Person(id_, name_, password_) {}

	inline Employee::Employee(const Employee* employee) { 
		this->setId(employee->getId());
		this->setName(employee->getName());
		this->setPassword(employee->getPassword());
	}



	inline int Employee::writeIntoFile() {
		std::ofstream output("Employees.txt", std::ios::out | std::ios::app);

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

	inline void Employee::showPersonalInfo() const {
		system("cls");
		std::cout << "\t\t\t\t\t\t\t\tИнформация о вас:\n" << std::endl << "\t\t\t\t\t\t\t\t"
			<< std::setfill('—') << std::setw(PERS_CAB::UNDERLINE_LEN) << "" << std::setfill(' ') << std::endl
			<< std::setw(PERS_CAB::LEFT_COLUMN_WIDTH) << std::left << "\t\t\t\t\t\t\t\t| Имя "
			<< std::setw(PERS_CAB::RIGHT_COLUMN_WIDTH) << std::left << "| " + this->getName() << std::endl << "\t\t\t\t\t\t\t\t"

			<< std::setfill('—') << std::setw(PERS_CAB::UNDERLINE_LEN) << "" << std::setfill(' ') << std::endl
			<< std::setw(PERS_CAB::LEFT_COLUMN_WIDTH) << std::left << "\t\t\t\t\t\t\t\t| ID "
			<< std::setw(PERS_CAB::RIGHT_COLUMN_WIDTH) << std::left << "| " + std::to_string(this->getId()) << std::endl << "\t\t\t\t\t\t\t\t"

			<< std::setfill('—') << std::setw(PERS_CAB::UNDERLINE_LEN) << "" << std::setfill(' ') << std::endl << std::endl;
	}

	inline void Employee::changePersonalInfo() {
		std::string menu = "\t\t\t\t\t\t\t\tЧто вы хотите изменить:\n"
							"\t\t\t\t\t\t\t\t1. Имя\n"
							"\t\t\t\t\t\t\t\t0. Назад";

		for (;;) {
			this->showPersonalInfo();

			std::cout << menu;
			int menuChoice = stringToNum(menu, 82, PERS_CAB::CHANGE_INFO_Y_COORD);
			system("cls");

			switch (menuChoice) {
			case PERS_CAB::CHANGE_NAME: {
				this->setName(this->changeName(this->getName()));
				std::cout << "\n\t\t\t\t\t\t\t\tИмя успешно изменено!\n\n\t\t\t\t\t\t\t\t";
				system("pause");
				system("cls");
				break;
			}
			case STATE::EXIT: {	break;	}
			default: { errorInput(FIND_PRODUCTS::FIND_PRODUCTS_X_COORD, FIND_PRODUCTS::FIND_PRODUCTS_Y_COORD); }
			}
			if (!menuChoice) { break; }
		}
		std::vector<employee::Employee*> employeesVector;

		std::ifstream getEmployee("Employees.txt");
		for (;!getEmployee.eof();) {

			employee::Employee employee;

			getEmployee >> employee;
			if (!employee.getId()) { break; }

			employeesVector.push_back(new employee::Employee(employee));
		}
		getEmployee.close();



		auto userIter = find_if(employeesVector.begin(), employeesVector.end(), [this](const Person* person) {
			return person->getId() == this->getId();
			});

		employeesVector.erase(userIter, userIter + 1);
		employeesVector.push_back(this);

		std::ofstream outClients("Employees.txt");
		outClients.close();


		for (const auto& employee : employeesVector) {
			employee->writeIntoFile();
		}
	}



	inline const std::string Employee::changeName(const std::string& name_) {
		std::string name;
		for (;;) {
			name = correctUserName();
			if (name != name_) { return name; }

			gotoxy(82, REG_MENU_COORD::USER_NAME_Y_COORD);

			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED); // change text color
			std::cout << "Имена должны отличаться!\0";
			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY);

			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR::MILISEC)); // wait until error input text disappear

			system("cls");
		}
	}




	inline std::ostream& operator <<(std::ostream& out, const Employee* employee) {
		out << employee->getId() << '\t' << employee->getPassword()
			<< "\t" << employee->getName() << "\n";
		return out;
	}

	inline std::istream& operator >>(std::istream& in, Employee& employee) {
		int id = 0;
		std::string password, name;

		in >> id;
		if (!id) { return in; }

		in >> password;
		getline(in, name);
		name.erase(name.begin(), name.begin() + STATE::STABLE);

		employee.setId(id);
		employee.setName(name);
		employee.setPassword(password);

		return in;
	}



	inline int registerEmployee(std::vector<Person*>& usersVector) {
		auto employeeInfoPair = reg();

		int id = usersVector.size() + ID_NUM::MIN_ID_NUM + 1;

		// this loop is to disable conflicts with id when we deleted user
		for (auto user : usersVector) {
			if (user->getId() != id) {
				break;
			}
			id++;
		}


		auto newEmployee = new Employee(id, employeeInfoPair.first, encrypt(employeeInfoPair.second, "0x30"));

		std::fstream out("Employees.txt", std::ios::app);
		out << newEmployee;
		out.close();

		try {
			usersVector.push_back(newEmployee);
		}
		catch (std::bad_alloc errro) {
			throw(new BadAllocException<std::string>("\n\t\t\t\t\t\t\t\tНе удалось выделить память под вектор класса employee::Employee!\n\n"));
		}
		catch (BadAllocException<std::string>* error) {
			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::RED); // change text color
			std::cout << "\n\t\t\t\t\t\t\t\tОшибка: " << error->getException();
			SetConsoleTextAttribute(hStdOut, CONSOLE_COLOR::GRAY); // change text color
			return STATE::EXIT;
		}

		std::cout << "\n\n\t\t\t\t\t\t\t\tВаш ID для авторизации: " << id << "\n\t\t\t\t\t\t\t\tНайти его можно также в вашем личном кабинете\n" << std::endl;
		std::cout << "\t\t\t\t\t\t\t\t";
		system("pause");
		system("cls");

		return id;
	}
}

#endif