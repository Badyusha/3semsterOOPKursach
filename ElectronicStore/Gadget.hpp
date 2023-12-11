#ifndef GADGET_HPP
#define GADGET_HPP

#include "Product.hpp"

// example of gadget in file:
// id	cost	count	yearOfRelease	brand	name

namespace gadget {

	class Gadget : public product::Product {
		int yearOfRelease;
		std::string brand;
	public:
		Gadget() : yearOfRelease(0), brand("none") {};
		Gadget(const Gadget* object);
		Gadget(const int& id_, const int& count_, const float& cost_, const std::string& name_,
				const int& yearOfRelease_, const std::string& brand_);
		~Gadget() {};

		int getYearOfRelease() const { return this->yearOfRelease; }
		std::string getBrand() const { return this->brand; }

		void setYearOfRelease(const int& yearOfRelease_) { this->yearOfRelease = yearOfRelease_; }
		void setBrand(const std::string& brand_) { this->brand = brand_; }

		void displayDescription(const std::string& str) override;

		friend std::ostream& operator <<(std::ostream& out, const Gadget* gadget);
		friend std::istream& operator >>(std::istream& in, Gadget& gadget);
	};

	inline Gadget::Gadget(const Gadget* object) {
		this->yearOfRelease = object->yearOfRelease;
		this->brand = object->brand;
	}

	inline Gadget::Gadget(const int& id_, const int& count_, const float& cost_, const std::string& name_,
						  const int& yearOfRelease_, const std::string& brand_)
				  : Product(id_, count_, cost_, name_)
	{
		this->yearOfRelease = yearOfRelease_;
		this->brand = brand_;
	}

	inline void Gadget::displayDescription(const std::string& str) {
		product::Product::displayDescription(str);

		std::cout << std::setw(PRODUCTS_INFO::YEAR_PRODUCT_RELEASE) << std::left << "| " + std::to_string(this->yearOfRelease)
				  << std::setw(PRODUCTS_INFO::PRODUCT_BRAND) << std::left << "| " + this->brand
				  << std::setw(PRODUCTS_INFO::IS_WIRELESS) << std::left << "|  - "
				  << std::setw(PRODUCTS_INFO::COMPATIBLE_WITH) << std::left << "|  - " << std::endl;
	}

	inline std::ostream& operator <<(std::ostream& out, const Gadget* gadget) {
		out << gadget->getId() << '\t' << gadget->getPrice() << '\t' << gadget->getCount() << "\t\t" <<
				gadget->yearOfRelease << '\t' << gadget->brand << '\t' << gadget->getName();
		return out;
	}

	inline std::istream& operator >>(std::istream& in, Gadget& gadget) {
		int id = 0, count = 0, yearOfRelease = 0;
		float cost = 0.F;
		std::string brand, name;

		in >> id;
		if (!id) { return in; }

		in >> cost >> count >> yearOfRelease >> brand;
		getline(in, name);
		name.erase(name.begin(), name.begin() + STATE::STABLE);

		gadget.setId(id);
		gadget.setPrice(cost);
		gadget.setCount(count);
		gadget.setYearOfRelease(yearOfRelease);
		gadget.setBrand(brand);
		gadget.setName(name);

		return in;
	}

}
#endif