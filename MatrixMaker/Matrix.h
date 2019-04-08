#pragma once
#include <iostream>

class Matrix
{
friend std::istream &operator>>(std::istream &, Matrix*);
friend std::istream & operator>>(std::ostream &, Matrix &);
friend std::ostream &operator<<(std::ostream &, Matrix*);
friend std::ostream& operator<<(std::ostream &, Matrix &);


public:
	// Constructors
	// Cannot explict here, will cause bugs
	Matrix();
	Matrix(unsigned);
	Matrix(unsigned, unsigned);
	Matrix(unsigned, unsigned, double);
	Matrix(Matrix*);
	~Matrix();

	// matrix pointer
	double** pmatrix;

	// getters of matrix properties
	int get_row() const;
	int get_col() const;

	// functions
	Matrix create_identity();
	void create_identity() const;
	Matrix creat_inverse() const;
	Matrix multi(const Matrix&) const;

	// Overload the basic operators
	void operator=(Matrix &);

	friend Matrix  operator+(const Matrix&, const Matrix&);
	friend Matrix  operator-(const Matrix&, const Matrix&);
	friend Matrix  operator*(const Matrix&, const Matrix&);
	friend Matrix  operator*(double, const Matrix&);
	friend Matrix  operator*(const Matrix&, double);

private:
	int nrow_;
	int ncol_;

	// inner cal process
	static double cal_dets(unsigned n, double *&aa);
	Matrix cal_join() const;
	double get_det() const;
};

