#ifndef PERSON_HPP
#define PERSON_HPP

#include "Libraries.hpp"
#include "Functions.hpp"

class Person {
	int id;
	std::string name;
	std::string password;
public:
	Person() : id(0), name("none"), password("none") {};
	Person(const Person& person);
	Person(const int& id_, const std::string& name_, const std::string& password_);
	virtual ~Person() {};

	int getId() const { return this->id; }
	std::string getName() const { return this->name; }
	std::string getPassword() const { return this->password; }

	void setId(const int& id_) { this->id = id_; }
	void setName(const std::string& name_) { this->name = name_; }
	void setPassword(std::string password_) { this->password = password_; }

	virtual void showPersonalInfo() const = 0;
	virtual void changePersonalInfo() = 0;
};

inline Person::Person(const Person& person) {
	this->id = person.id;
	this->name = person.name;
	this->password = person.password;
}

inline Person::Person(const int& id_, const std::string& name_, const std::string& password_) {
	this->id = id_;
	this->name = name_;
	this->password = password_;
}

#endif