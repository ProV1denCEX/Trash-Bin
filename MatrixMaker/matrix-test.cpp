#include "Matrix.h"

int main(int argc, char* argv[])
{
	// generate 4 test Matrix
	const auto test_1 = new Matrix();
	const auto test_2 = new Matrix(5);
	auto test_3 = Matrix(5, 4);
	auto test_4 = Matrix(5, 4, 1);
	const auto test_5 = Matrix(4, 5);

	// Print it
	std::cout << "Testing matrix creation" << std::endl;
	std::cout << test_1 << std::endl;
	std::cout << test_2 << std::endl;
	std::cout << test_3 << std::endl;
	std::cout << test_4 << std::endl;

	// Test +-*
	
	auto test_minus = test_3 - test_4;
	auto test_plus = test_3 + test_4;
	auto test_multi = test_3 * test_5;
	auto test_dotmulti = test_3.multi(test_3);

	std::cout << "Testing - of matrix" << std::endl;
	std::cout << test_minus << std::endl;
	std::cout << "Testing + of matrix" << std::endl;
	std::cout << test_plus << std::endl;
	std::cout << "Testing * of matrix" << std::endl;
	std::cout << test_multi << std::endl;
	std::cout << "Testing .* of matrix" << std::endl;
	std::cout << test_dotmulti << std::endl;

	// Test identity matrix
	test_1->create_identity();
	auto test_identity = test_2->create_identity();
	std::cout << "Testing identity matrix" << std::endl;
	std::cout << test_identity << std::endl;

	// Test input matrix
	/*
	 * we input matrix:
	 *		1 0 5
	 *		2 1 6
	 *		3 4 0
	 */
	std::cout << "Testing input matrix" << std::endl;
	const auto test_6 = new Matrix(3);
	std::cin >> test_6;
	std::cout << test_6 <<std::endl;

	// Test inverse matrix
	std::cout << "Testing inverse matrix" << std::endl;
	auto test_inverse = test_6->creat_inverse();
	std::cout << test_inverse << std::endl;

	// delete test matrix created by new
	delete test_1;
	delete test_2;
	delete test_6;

	return 0;
}
