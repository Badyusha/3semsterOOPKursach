#ifndef ACCESSORY_HPP
#define ACCESSORY_HPP

#include "Product.hpp"

// example of accessory in file:
// id	cost	count	isWireless compatibleWith	name

namespace accessory {

	class Accessory : public product::Product {
		bool isWireless;
		int compatibleWith; // gadgetId		one to many		if non-compatible -> 0
	public:
		Accessory() : isWireless(false), compatibleWith(0) {};
		Accessory(const Accessory* object);
		Accessory(const int& id_, const int& count_, const float& cost_, const std::string& name_,
				  const bool& isWireless_, const int& compatibleWith_);
		~Accessory() {};

		bool IsWireless() const { return this->isWireless; } 
		int CompatibleWith() const { return this->compatibleWith; }

		void IsWireless(const bool& isWireless_) { this->isWireless = isWireless_; } // перегрузка
		void setCompatibleWith(const int& compatibleWith_) { this->compatibleWith = compatibleWith_; }

		void displayDescription(const std::string& str) override;

		friend std::ostream& operator <<(std::ostream& out, const Accessory* accessory);
		friend std::istream& operator >>(std::istream& in, Accessory& accessory);
	};

	inline Accessory::Accessory(const Accessory* object) {
		this->isWireless = object->isWireless;
		this->compatibleWith = object->compatibleWith;
	}

	inline Accessory::Accessory(const int& id_, const int& count_, const float& cost_, const std::string& name_,
							    const bool& isWireless_, const int& compatibleWith_)
					 : Product(id_, count_, cost_, name_)
	{
		this->isWireless = isWireless_;
		this->compatibleWith = compatibleWith_;
	}

	inline void Accessory::displayDescription(const std::string& str) {
		product::Product::displayDescription(str);

		std::cout << std::setw(PRODUCTS_INFO::YEAR_PRODUCT_RELEASE) << std::left << "|  -"
				  << std::setw(PRODUCTS_INFO::PRODUCT_BRAND) << std::left << "|  -"
				  << std::setw(PRODUCTS_INFO::IS_WIRELESS) << std::left << (this->isWireless ? "| Да " : "| Нет ")
				  << std::setw(PRODUCTS_INFO::COMPATIBLE_WITH) << std::left << "| " + std::to_string(this->compatibleWith) << std::endl;
	}

	inline std::ostream& operator <<(std::ostream& out, const Accessory* accessory) {
		out << accessory->getId() << '\t' << accessory->getPrice() << '\t' << accessory->getCount() << "\t" <<
				accessory->isWireless << '\t' << accessory->compatibleWith << '\t' << accessory->getName();
		return out;
	}

	inline std::istream& operator >>(std::istream& in, Accessory& accessory) {
		int id = 0, count = 0, compatibleWith = 0;
		float cost = 0.F;
		bool isWireless = false;
		std::string name;

		in >> id;
		if (!id) { return in; }

		in >> cost >> count >> isWireless >> compatibleWith;
		getline(in, name);

		if (!name.size()) { getline(in, name); }
		
		name.erase(name.begin(), name.begin() + STATE::STABLE);

		accessory.setId(id);
		accessory.setPrice(cost);
		accessory.setCount(count);
		accessory.IsWireless(isWireless);
		accessory.setCompatibleWith(compatibleWith);
		accessory.setName(name);

		return in;
	}

}
#endif