#include <iostream>
#include <ctime>

int main() {
	srand(time(NULL));
	for (int i = 0; i < 100; i++) {
		int random = rand() % 5;
		if (random == 5) {
			std::cout << random << std::endl;
		}
	}
	std::cin.get();
}