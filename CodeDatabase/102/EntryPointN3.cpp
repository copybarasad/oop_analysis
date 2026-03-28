#include <iostream>
#include "../Headers/Controllers/SystemLoadController.h"

int main()
{
	system("chcp 65001");
	std::cout << "Бердичевский Максим, 4381, лабораторная работа №3." << std::endl;
	SystemLoadController loadController;
	loadController.ToLoadingLoop();
	return 0;
}