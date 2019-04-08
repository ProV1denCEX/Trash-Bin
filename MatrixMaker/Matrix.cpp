#include "Matrix.h"

/*
 * Create default matrix with height, weight = 3 and default value = 0
 */
Matrix::Matrix(): pmatrix(new double*[3]), nrow_(3), ncol_(3)
{
	/*
	* Consider	the properties of matrix, 
	* it is better to use double[] to construct matrix rather than vector
	* 
	* It is also better to create the whole matrix on a continues mem blocks
	*	rather than create discrete blocks
	*/
	pmatrix[0] = new double[3 * 3];
	for (auto i = 1; i < 3; i++)
	{
		pmatrix[i] = pmatrix[i - 1] + 3;
	}

	// Initialize all values in matrix
	for (auto i = 0; i < 3; ++i)
	{
		for (auto j = 0; j < 3; ++j)
		{
			pmatrix[i][j] = 0;
		}
	}
}

Matrix::Matrix(const unsigned nscale) : pmatrix(new double*[nscale]), nrow_(nscale), ncol_(nscale)
{
	if (nscale > 0)
	{
		pmatrix[0] = new double[nscale * nscale];
		for (auto i = 1; i < nscale; i++)
		{
			pmatrix[i] = pmatrix[i - 1] + nscale;
		}

		// Initialize all values in matrix
		for (auto i = 0; i < nscale; ++i)
		{
			for (auto j = 0; j < nscale; ++j)
			{
				pmatrix[i][j] = 0;
			}
		}
	}
	else
	{
		pmatrix = nullptr;
	}

}

Matrix::Matrix(const unsigned ncol, const unsigned nrow): pmatrix(new double*[ncol]), nrow_(nrow), ncol_(ncol)
{
	if (ncol * nrow > 0)
	{
		pmatrix[0] = new double[ncol * nrow];
		for (auto i = 1; i < ncol; i++)
		{
			pmatrix[i] = pmatrix[i - 1] + nrow;
		}

		// Initialize all values in matrix
		for (auto i = 0; i < ncol; ++i)
		{
			for (auto j = 0; j < nrow; ++j)
			{
				pmatrix[i][j] = 0;
			}
		}
	}
	else
	{
		pmatrix = nullptr;
	}

}

Matrix::Matrix(const unsigned ncol, const unsigned nrow, const double nvalue) : pmatrix(new double*[ncol]), nrow_(nrow), ncol_(ncol)
{
	if (ncol * nrow > 0)
	{
		pmatrix[0] = new double[ncol * nrow];
		for (auto i = 1; i < ncol; i++)
		{
			pmatrix[i] = pmatrix[i - 1] + nrow;
		}

		// Initialize all values in matrix
		for (auto i = 0; i < ncol; ++i)
		{
			for (auto j = 0; j < nrow; ++j)
			{
				pmatrix[i][j] = nvalue;
			}
		}
	}
	else
	{
		pmatrix = nullptr;
	}

}

/*
 * Deep copy to avoid destructor error
 */
Matrix::Matrix(Matrix* matrix)
{
	this->ncol_ = matrix->get_col();
	this->nrow_ = matrix->get_row();

	this->pmatrix = new double*[ncol_];
	this->pmatrix[0] = new double[ncol_ * nrow_];

	// create independent pointer
	for (auto i = 1; i < ncol_; i++)
	{
		this->pmatrix[i] = this->pmatrix[i - 1] + nrow_;
	}

	// deep copy the value
	for (auto i = 0; i < ncol_; ++i)
	{
		for (auto j = 0; j < nrow_; ++j)
		{
			this->pmatrix[i][j] = matrix->pmatrix[i][j];
		}
	}
}

Matrix::~Matrix()
{
	/*
	 *There is no need to delete[] for pmatrix[i] i > 0
	 * as they don't have their own mem block, 
	 * they just point into the single large block
	 */
	if (pmatrix != nullptr)
	{
		delete[] pmatrix[0];
		delete[] pmatrix;
		pmatrix = nullptr;
	}
}

int Matrix::get_row() const
{
	return this->nrow_;
}

int Matrix::get_col() const
{
	return this->ncol_;
}

/*
 * Create identity matrix of input matrix
 * Compiler will call destructor to destruct identity matrix automatically when finished 
 * according to our destructor, we MUST use new to create identity_matrix 
 * rather than call constructor directly
 */
Matrix Matrix::create_identity()
{
	if (this->get_col() == this->get_row() && this->get_col() > 0)
	{
		const auto identity_matrix = new Matrix(this->get_col(), this->get_row());
		for (auto i = 0; i < this->get_col(); ++i)
		{
			for (auto j = 0; j < this->get_row(); ++j)
			{
				if (i != j)
				{
					identity_matrix->pmatrix[i][j] = 0;
				}
				else
				{
					identity_matrix->pmatrix[i][j] = 1;
				}
			}
		}
		return identity_matrix;
	}
	else
	{
		throw "Col num and Row num mismatch! Refuse to cal identity matrix";
	}
}

void Matrix::create_identity() const
{
	if (this->get_col() == this->get_row() && this->get_col() > 0)
	{
		for (auto i = 0; i < this->get_col(); ++i)
		{
			for (auto j = 0; j < this->get_row(); ++j)
			{
				if (i != j)
				{
					this->pmatrix[i][j] = 0;
				}
				else
				{
					this->pmatrix[i][j] = 1;
				}
			}
		}
	}
	else
	{
		throw "Col num and Row num mismatch! Refuse to cal identity matrix";
	}
}


// Overload the operator = as deep copy
// the original operator is shallow copy
void Matrix::operator=(Matrix &matrix)
{
	this->ncol_ = matrix.get_col();
	this->nrow_ = matrix.get_row();

	this->pmatrix = new double*[ncol_];
	this->pmatrix[0] = new double[ncol_ * nrow_];

	// create independent pointer
	for (auto i = 1; i < ncol_; i++)
	{
		this->pmatrix[i] = this->pmatrix[i - 1] + nrow_;
	}

	// deep copy the value
	for (auto i = 0; i < ncol_; ++i)
	{
		for (auto j = 0; j < nrow_; ++j)
		{
			this->pmatrix[i][j] = matrix.pmatrix[i][j];
		}
	}
}

// get det matrix
double Matrix::get_det() const
{
	if (ncol_ == nrow_)
		return cal_dets(nrow_, this->pmatrix[0]);
	else
	{
		std::cout << "Cannot cal the det matrix, col neq row" << std::endl;
		return 0;
	}
}

// cal dets matrix
double Matrix::cal_dets(const unsigned nscale, double *&matrix)
{
	if (nscale == 1)
	{
		return matrix[0];
	}

	// create n-1 cofactor matrix
	auto temp = new double[(nscale - 1)*(nscale - 1)];	
	// sum of vector
	auto sum = 0.0;

	// start cal det matrix
	for (auto irow = 0; irow < nscale; irow++)
	{
		for (auto jrow = 0; jrow < nscale - 1; jrow++)
		{
			// is row moved
			const auto is_mov = irow > jrow ? 0 : 1;
			for (auto j = 0; j < nscale - 1; j++)
			{
				temp[jrow*(nscale - 1) + j] = matrix[(jrow + is_mov)*nscale + j + 1];
			}
		}
		const auto flag = (irow % 2 == 0 ? 1 : -1);
		// \sigma ele *(det) = det matrix
		sum += flag * matrix[irow*nscale] * cal_dets(nscale - 1, temp);
	}
	delete[]temp;
	return sum;
}

/*
 * Cal joint matrix to get inverse matrix
 */
Matrix Matrix::cal_join() const
{ 
	if (nrow_ != ncol_)
	{
		return Matrix();
	}

	// data 2 return
	const auto result = new Matrix(nrow_);

	// create n-1 cofactor matrix
	auto temp = new double[(nrow_ - 1)*(nrow_ - 1)]; 

	// Start cal join matrix
	int ifac, jfac, cofactor;
	for (auto ii = 0; ii < nrow_; ii++) 
	{
		for (auto jj = 0; jj < nrow_; jj++)
		{
			for (auto ii_2 = 0; ii_2 < nrow_ - 1; ii_2++) 
			{
				for (auto jj_2 = 0; jj_2 < nrow_ - 1; jj_2++)
				{
					if (ii > ii_2)
					{
						ifac = 0;
					}
					else
					{
						ifac = 1; 
					}
						        
					if (jj > jj_2)
					{
						jfac = 0;
					}
					else
					{
						jfac = 1;
					}
					temp[ii_2*(nrow_ - 1) + jj_2] = pmatrix[(ii_2 + ifac)][jj_2 + jfac];
				}
			}
			if ((ii + jj) % 2 == 0)
			{
				cofactor = 1;
			}  
			else
			{
				cofactor = -1;
			}
			result->pmatrix[ii][jj] = cofactor * cal_dets(nrow_ - 1, temp); 
		}
	}
	delete[]temp;
	return result;
}

/*
 * Cal the inverse matrix via LU decomposition
 * https://zh.wikipedia.org/wiki/LU%E5%88%86%E8%A7%A3
 */
Matrix Matrix::creat_inverse() const
{
	// Get det matrix
	const auto det_temp = get_det();
	if (det_temp == 0)
	{
		throw "The det cannot be 0!";
	}

	// Get the joint matrix
	const auto join = cal_join();
	const auto result = new Matrix(nrow_);

	// fill in the value
	for (unsigned i = 0; i < nrow_; i++)    
	{
		for (unsigned j = 0; j < ncol_; j++)
		{
			result->pmatrix[i][ j] = join.pmatrix[i][j] / det_temp;
		}
	}
	return result;
}

/*
 * the dot multiply of 2 matrixs
 */
Matrix Matrix::multi(const Matrix& matrix) const
{
	if (this->get_col() == matrix.get_col() && this->get_row() == matrix.get_row())
	{
		const auto result = new Matrix(ncol_, nrow_);
		for (auto i = 0; i < this->get_col(); ++i)
		{
			for (auto j = 0; j < this->get_row(); ++j)
			{
				result->pmatrix[i][j] = this->pmatrix[i][j] * matrix.pmatrix[i][j];
			}
		}
		return result;
	}
	else
	{
		throw "Matrix mismatch !";
	}
}

/*
 * dot plus of two matrixs
 */
Matrix operator+(const Matrix& matrix_1, const Matrix& matrix_2)
{
	if (matrix_1.get_col() == matrix_2.get_col() && matrix_1.get_row() == matrix_2.get_row())
	{
		const auto result = new Matrix(matrix_1.get_col(), matrix_1.get_row());
		for (auto i = 0; i < matrix_1.get_col(); ++i)
		{
			for (auto j = 0; j < matrix_1.get_row(); ++j)
			{
				result->pmatrix[i][j] = matrix_1.pmatrix[i][j] + matrix_2.pmatrix[i][j];
			}
		}
		return result;
	}
	else
	{
		throw "Matrix mismatch !";
	}
}


/*
 * dot minus of two matrixs
 */
Matrix operator-(const Matrix& matrix_1, const Matrix& matrix_2)
{
	if (matrix_1.get_col() == matrix_2.get_col() && matrix_1.get_row() == matrix_2.get_row())
	{
		const auto result = new Matrix(matrix_1.get_col(), matrix_1.get_row());
		for (auto i = 0; i < matrix_1.get_col(); ++i)
		{
			for (auto j = 0; j < matrix_1.get_row(); ++j)
			{
				result->pmatrix[i][j] = matrix_1.pmatrix[i][j] - matrix_2.pmatrix[i][j];
			}
		}
		return result;
	}
	else
	{
		throw "Matrix mismatch !";
	}
}

/*
 * matrix mutiply
 * mxn nxp
 */
Matrix operator*(const Matrix& matrix_1, const Matrix& matrix_2)
{
	if (matrix_1.get_row() * matrix_1.get_col() == 0 || 
		matrix_2.get_row() * matrix_2.get_col() == 0 || 
		matrix_1.get_col() != matrix_2.get_row())
	{
		throw "matrix mismatch !";
	}
	else
	{
		const auto result = new Matrix(matrix_1.get_row(), matrix_2.get_col());
		for (auto i = 0; i < matrix_1.get_row(); i++)
		{
			for (auto j = 0; j < matrix_2.get_col(); j++)
			{
				for (auto k = 0; k < matrix_1.get_row(); k++)   
				{
					result->pmatrix[i][j] += matrix_1.pmatrix[i][k] * matrix_2.pmatrix[k][j];
				}
			}
		}
		return result;
	}
}

/*
 * the dot multiply of matrix and num
 */
Matrix operator*(const Matrix& matrix_1, const double num)
{
	if (matrix_1.get_col() != 0 && matrix_1.get_row() != 0)
	{
		const auto result = new Matrix(matrix_1.get_col(), matrix_1.get_row());
		for (auto i = 0; i < matrix_1.get_col(); ++i)
		{
			for (auto j = 0; j < matrix_1.get_row(); ++j)
			{
				result->pmatrix[i][j] = matrix_1.pmatrix[i][j] * num;
			}
		}
		return result;
	}
	else
	{
		throw "Matrix mismatch !";
	}
}

/*
 * the dot multiply of num and matrix
 */
Matrix operator*(const double num, const Matrix& matrix_1)
{
	if (matrix_1.get_col() != 0 && matrix_1.get_row() != 0)
	{
		const auto result = new Matrix(matrix_1.get_col(), matrix_1.get_row());
		for (auto i = 0; i < matrix_1.get_col(); ++i)
		{
			for (auto j = 0; j < matrix_1.get_row(); ++j)
			{
				result->pmatrix[i][j] = matrix_1.pmatrix[i][j] * num;
			}
		}
		return result;
	}
	else
	{
		throw "Matrix mismatch !";
	}
}

/*
 * overload non-member friend function >>
 */
std::istream& operator>>(std::istream& input, Matrix* matrix)
{
	for (unsigned i = 0; i < matrix->get_col(); i++)
	{
		for (unsigned j = 0; j < matrix->get_row(); j++)
		{
			input >> matrix->pmatrix[i][j];
		}
	}
	return input;
}

std::istream& operator>>(std::istream& input, Matrix & matrix)
{
	for (unsigned i = 0; i < matrix.get_col(); i++)
	{
		for (unsigned j = 0; j < matrix.get_row(); j++)
		{
			input >> matrix.pmatrix[i][j];
		}
	}
	return input;
}


/*
 * overload non-member friend function <<
 */
std::ostream& operator<<(std::ostream &output, Matrix* matrix)
{
	for (unsigned i = 0; i < matrix->get_col(); i++)
	{
		for (unsigned j = 0; j < matrix->get_row(); j++)
		{
			output << matrix->pmatrix[i][j] << ' ';
		}
		output << '\n';
	}
	return output;
}

std::ostream& operator<<(std::ostream &output, Matrix &matrix)
{
	for (unsigned i = 0; i < matrix.get_col(); i++)
	{
		for (unsigned j = 0; j < matrix.get_row(); j++)
		{
			output << matrix.pmatrix[i][j] << ' ';
		}
		output << '\n';
	}
	return output;
}