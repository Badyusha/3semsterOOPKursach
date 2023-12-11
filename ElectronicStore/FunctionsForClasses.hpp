#ifndef FUNCTIONSFORCLASSES_HPP
#define FUNCTIONSFORCLASSES_HPP

#include "Client.hpp"
#include "Employee.hpp"
#include "Product.hpp"
#include "Order.hpp"



std::vector<product::Product*> readProducts();

int readUsersDataFromFiles(std::vector<Person*>& usersVector);
int writeUsersDataIntoFiles(const std::vector<Person*>& usersVector);


std::vector<product::Product*>& findProducts();

void findProductsByPrice(const std::vector<product::Product*>& productsVector);
void findProductsByName(const std::vector<product::Product*>& productsVector);
void findProductsByReleaseYear(const std::vector<product::Product*>& productsVector);
void findProductsByID(const std::vector<product::Product*>& productsVector);



void showProductsInfo(const std::vector<product::Product*>& selectedProductsVector);
void orderProduct(const client::Client* client);
void trackOrder(const client::Client* client);



void readOrders(std::vector<order::Order*>& ordersVector);
void showOrder(const int& count, product::Product* product);



void clientMenu(client::Client* client);
void employeeMenu(employee::Employee* employee);



#endif