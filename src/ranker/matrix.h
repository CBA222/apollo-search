
class Matrix
{
public:

	int a = 8;

	Matrix() {
		
	}

	Matrix(int rows, int cols) {

	}

	int& operator[](int idx) {
		return a;
	} 

	static int dot_product(Matrix m1, Matrix m2) {

	}
};