#ifndef LIBRARIES_HPP
#define LIBRARIES_HPP

#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "crypt32")
#pragma comment(lib, "ws2_32.lib")

#include <unordered_map>
#include <ctime>
#include <vector>
#include <memory>
#include <string>
#include <format>
#include <thread>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <filesystem>

// to encrypt data
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/aes.h>

#include <openssl/sha.h> // lib to encrypt data


inline HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // console descriptor


enum STATE {
	EXIT,
	STABLE,
	MIN_RELEASE_YEAR = 2000,
	DEFAULT_DIGITS_AFTER_POINT = 6,
	FIXED_DIGITS_AFTER_POINT = 2
};


enum ERROR_INPUT {
	ERR_IN_X_COORD,
	ERR_IN_Y_COORD = 0,
	WHITESPACES_LEN = 129
};

enum CONSOLE_COLOR {
	RED = 4,
	GRAY = 7
};

enum SLEEP_FOR {
	MILISEC = 1700
};



enum ID_NUM {
	MIN_PERSONAL_NUM,
	MIN_ID_NUM = 1000
};



enum MAIN_MENU {
	MAIN_MENU_Y_COORD,
	MAIN_MENU_X_COORD = 21
};



enum PERS_CAB {
	PERSONAL_CABINET_Y_COORD,
	SHOW_INFO,
	CHANGE_INFO,
	PERSONAL_CABINET_X_COORD = 16,
	UNDERLINE_LEN = 60,
	LEFT_COLUMN_WIDTH = 10,
	RIGHT_COLUMN_WIDTH = 50,
	CHANGE_INFO_X_COORD = 18,
	CHANGE_INFO_Y_COORD = 0,
	CHANGE_NAME,
	CHANGE_EMAIL,
	CHANGE_EMAIL_X_COORD = 32,
	CHANGE_EMAIL_Y_COORD = 0,
	DELETE_ACCOUNT = 3
};



enum CLIENT_MENU {
	CLIENT_MENU_Y_COORD,
	FIND_PRODUCT,
	SHOW_PRODUCT_CHARACT,
	ORDER_PRODUCT,
	TRACK_ORDER,
	PERSONAL_CABINET,
	SORT_PRODUCTS,
	CLIENT_MENU_X_COORD = 19,
};

enum FIND_PRODUCTS {
	ERROR_Y_COORD = 0,
	ERROR_X_COORD = 27,
	FIND_BY_PRICE = 1,
	FIND_BY_NAME = 2,
	FIND_BY_RELEASE_YEAR = 3,
	FIND_BY_ID = 4,
	LEFT_RANGE_X_COORD = 4,
	LEFT_RANGE_Y_COORD = 1,
	RIGHT_RANGE_X_COORD = 4,
	RIGHT_RANGE_Y_COORD = 2,
	FIND_PRODUCTS_Y_COORD = 0,
	FIND_PRODUCTS_X_COORD = 17,
	FIND_BY_RELEASE_YEAR_X_COORD = 22,
	FIND_BY_RELEASE_YEAR_Y_COORD = 0,
	FIND_BY_ID_X_COORD = 20,
	FIND_BY_ID_Y_COORD = 0
};

enum PRODUCTS_INFO {
	PRODUCT_TYPE = 15,
	PRODUCT_ID = 10,
	PRODUCT_PRICE = 15,
	PRODUCT_PRICE_SEPARATOR = 2,
	PRODUCT_NAME = 35,
	YEAR_PRODUCT_RELEASE = 15,
	PRODUCT_BRAND = 20,
	IS_WIRELESS = 15,
	COMPATIBLE_WITH = 10,
	UNDERSCORE_LEN = 155,
	NUM_COLUMN = 6
};

enum ORDER_PRODUCT {
	ORDER_PRO_Y_COORD,
	ORDER_PRO_X_COORD = 18,
	ORDER_IN_CONFIRMATION = 0,
	ORDER_CONFIRMED
};

enum SORT_PRODUCTS {
	SORT_CHOICE_Y_COORD,
	SORT_CHOICE_X_COORD = 18
};

enum SHOW_ORDER {
	ORDER_COUNT_Y_COORD,
	ORDER_COUNT_X_COORD = 47
};



enum EMPLOYEE_MENU {
	EMPLOYEE_MENU_Y_COORD,
	EMPLOYEE_MENU_X_COORD = 17,
	ADD_PRODUCT = 1,
	DELETE_PRODUCT,
	FIND_PRODUCTS,
	SHOW_ALL_PRODUCTS,
	PERSONAL_CAB,
	ORDERS_REPORT
};

enum ADD_PRODUCT {
	ADD_PROD_Y_COORD,
	ADD_PROD_X_COORD = 27,
	ADD_GADGET = 1,
	ADD_ACCESSORY = 2
};





enum MENU {
	AUTHORIZE = 1,
	REGISTER = 2
};

enum REG_MENU_COORD {
	REGISTER_MENU_Y_COORD,
	REGISTER_MENU_X_COORD = 15,
	USER_NAME_X_COORD = 18,
	USER_NAME_Y_COORD = 0
};

enum AUTH_MENU_COORD {
	AUTHORIZE_MENU_Y_COORD,
	AUTHORIZE_MENU_X_COORD = 17
};



enum REGISTER {
	CLIENT = 1,
	EMPLOYEE = 2,
};



enum CORRECT_PASSWORD_LEN {
	MIN_PASSWORD_LEN = 7,
	MAX_PASSWORD_LEN = 20
};

enum CORRECT_USERNAME_LEN {
	MIN_USERNAME_LEN = 10,
	MAX_USERNAME_LEN = 45
};

enum INCORRECT_PASSWORD {
	INCORRECT_PASSWORD_X_COORD,
	INCORRECT_PASSWORD_Y_COORD,
	ATTEMPTS_COUNT = 3
};

#endif