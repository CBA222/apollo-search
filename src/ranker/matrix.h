#include <Eigen/Dense>

using Eigen::MatrixXd;

class Matrix
{
private:
	MatrixXd internal;
public:
	int a = 9;

	Matrix() {}

	Matrix(int rows, int cols) {
		internal = MatrixXd(rows, cols);
	}

	int& operator() (int a, int b) {
		return internal(a, b);
	}

	static int dot_product(Matrix m1, Matrix m2) {
		//return m1.internal.dot(m2.internal);
		return 0;
	}
};