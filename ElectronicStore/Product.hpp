#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include "Libraries.hpp"

namespace product {

	class Product {
		int id;
		int count;
		float price;
		std::string name;
	public:
		Product() : id(0), count(0), price(0.F), name("none") {};
		Product(const Product* object);
		Product(const int& id_, const int& count_,
				const float& price_, const std::string& name_);
		virtual ~Product() {};

		int getId() const { return this->id; }
		int getCount() const { return this->count; }
		float getPrice() const { return this->price; }
		std::string getName() const { return this->name; }

		void setId(const int& id_) { this->id = id_; }
		void setCount(const int& count_) { this->count = count_; }
		void setPrice(const float& price_) { this->price = price_; }
		void setName(const std::string& name_) { this->name = name_; }

		virtual void displayDescription(const std::string& str);
	};

	inline Product::Product(const Product* object) {
		this->id = object->id;
		this->count = object->count;
		this->price = object->price;
		this->name = object->name;
	}

	inline Product::Product(const int& id_, const int& count_,
							const float& price_, const std::string& name_)
	{
		this->id = id_;
		this->count = count_;
		this->price = price_;
		this->name = name_;
	}

	inline void Product::displayDescription(const std::string& str) {
		std::cout << std::setw(PRODUCTS_INFO::PRODUCT_TYPE) << std::left << "| " + str
				  << std::setw(PRODUCTS_INFO::PRODUCT_ID) << std::left << "| " +  std::to_string(this->id)
				  << std::setw(PRODUCTS_INFO::PRODUCT_PRICE_SEPARATOR) << "| " << std::setw(PRODUCTS_INFO::PRODUCT_PRICE - PRODUCTS_INFO::PRODUCT_PRICE_SEPARATOR) << std::left << std::fixed << std::setprecision(2) << this->price
				  << std::setw(PRODUCTS_INFO::PRODUCT_NAME) << std::left << "| " + this->name;
	}
}

#endif