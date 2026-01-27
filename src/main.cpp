#include <iostream>
#include "config/ConfigValue.hpp"



template<typename T>
void func(T&& value) {

	//далее если здесь заполнять условно поле класса, то через forward 
	//чтоб сохранить тип, в который преобразовались при поступлении сюда те переменные
	//чтоб либо украсть, если rvalue либо сделать копию, чтоб не испортить изначалное, если lvalue


}

int main() {

	const int& ciref = 10;
	int i = 5;
	const int ci = 7;
	int&& irref = 5;
	int& iref = i;
	int& lref = irref; // ведёт к значению 5

	func(ciref);//lvalue, не можем грабить без move //преобразуется в int&
	func(i); //то же самое// int&
	func(ci); //то же самое //int&
	func(irref); // тоже lvalue  //int&
	func(iref); //тоже  //int&
	func(15); // можем rvalue, никому не принадлежит, протсо число //int&&
	func(lref);// не можем //int&


	config::ConfigValue cv1(123);
	config::ConfigValue cv2(13.2);
	config::ConfigValue cv3(true);
	config::ConfigValue cv4("on");
	std::vector<config::ConfigValue> values = { cv1, cv2, cv3, cv4 };
	config::ConfigValue cv5(values);

	std::cout << cv1.toString() << std::endl;
	std::cout << cv4.toString() << std::endl;
	std::cout << cv5.toString() << std::endl;

	

	

	

	return 0;



}

