#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "Libraries.hpp"


float stringToNum(const std::string& str, const short& xCoord, const short& yCoord);

std::string hashEncrypt(const unsigned char* str);

std::pair<std::string, std::string> reg();

std::string correctUserName();

std::string enteringPassword();

bool correctPassword(const std::string& password);

bool correctEmail(const std::string& email);

void gotoxy(const short& xCoord, const short& yCoord);

void errorInput(const short& xCoord, const short& yCoord);

int dateToInt(std::string& date);


void authorizeUser();
void registerUser();


void showAllProducts();
void sortProducts();
int ordersCount();


bool confirmOrder();

void sortByPrice();
void sortByCount();
void sortByName();

void addProduct();
void addGadget();
void addAccessory();

void deleteProduct();

void createOrdersReport();

std::string encrypt(const std::string& plain_text, const std::string& key);
std::string decrypt(const std::string& plain_text, const std::string& key);

#endif