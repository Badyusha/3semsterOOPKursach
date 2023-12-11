#ifndef ORDER_HPP
#define ORDER_HPP

#include "Client.hpp"
#include "Product.hpp"

namespace order {

	class Order {
		int id;
		bool state;
		int clientID;
		int productID;
		float purchaseCount;
		std::string name;
		std::string purchaseDate;
	public:
		Order() : id(0), state(false), purchaseCount(0.F), name("none"), purchaseDate("none"), clientID(0), productID(0) {};
		Order(const Order* object);
		Order(const int& id_, const bool& state_, const float& purchaseCount_, const std::string& name_,
			  const std::string& purchaseDate_, const int& clientID_, const int& productID_);
		~Order() {};

		int getId() const { return this->id; }
		bool getState() const { return this->state; }
		int getClientID() const { return this->clientID; }
		int getProductID() const { return this->productID; }
		float getPurchaseCount() const { return this->purchaseCount; }
		std::string getName() const { return this->name; }
		std::string getPurchaseDate() const { return this->purchaseDate; }

		void setId(const int& id_) { this->id = id_; }
		void setState(const bool& state_) { this->state = state_; }
		void setPurchaseCount(const float& purchaseCount_) { this->purchaseCount = purchaseCount_; }
		void setClientID(const int& clientID_) { this->clientID = clientID_; }
		void setProductID(const int& productID_) { this->productID = productID_; }
		void setName(const std::string& name_) { this->name = name_; }
		void setPurchaseDate(const std::string& purchaseDate_) { this->purchaseDate = purchaseDate_; }

		friend std::ostream& operator <<(std::ostream& out, const Order* order);
		friend std::istream& operator >>(std::istream& in, Order& order);

		int writeIntoFile();
	};

	inline Order::Order(const Order* object) {
		this->id = object->id;
		this->state = object->state;
		this->purchaseCount = object->purchaseCount;
		this->clientID = object->clientID;
		this->productID = object->productID;
		this->name = object->name;
		this->purchaseDate = object->purchaseDate;
	}

	inline Order::Order(const int& id_, const bool& state_, const float& purchaseCount_, const std::string& name_,
						const std::string& purchaseDate_, const int& clientID_, const int& productID_)
	{
		this->id = id_;
		this->state = state_;
		this->purchaseCount = purchaseCount_;
		this->name = name_;
		this->purchaseDate = purchaseDate_;
		this->clientID = clientID_;
		this->productID = productID_;
	}

	inline std::ostream& operator <<(std::ostream& out, const Order* order) {
		out << order->getId() << '\t' << order->getState() << '\t' << order->getClientID() << '\t' <<
			order->getProductID() << '\t' << order->getPurchaseCount() << '\t' << order->getPurchaseDate() << '\t' << order->getName() << '\n';
		return out;
	}

	inline std::istream& operator >>(std::istream& in, Order& order) {
		int id = 0, clientID = 0, productID = 0;
		bool state = false;
		float purchaseCount = 0.F;
		std::string purchaseDate, name;

		in >> id;
		if (!id) { return in; }

		in >> state >> clientID >> productID >> purchaseCount >> purchaseDate;
		getline(in, name);
		name.erase(name.begin());

		order.setId(id);
		order.setState(state);
		order.setPurchaseCount(purchaseCount);
		order.setName(name);
		order.setPurchaseDate(purchaseDate);
		order.setClientID(clientID);
		order.setProductID(productID);

		return in;
	}

	inline int Order::writeIntoFile() {
		std::ofstream output("Orders.txt", std::ios::out | std::ios::app);

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

		return STABLE;
	}

}

#endif