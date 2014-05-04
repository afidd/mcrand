#include <random>
#include <iostream>

int main(int argc, char* argv[]) {

  std::mt19937 mt32{1};
  std::cout << std::hex << mt32.min() << "\t" << mt32.max() << std::endl;

  std::mt19937_64 mt64{1};
  std::cout << std::hex << mt64.min() << "\t" << mt64.max() << std::endl;
	return 0;
}
