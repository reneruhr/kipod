#pragma once
#include "Matrix.h"
#include "MatrixManipulations.h"

namespace siegel2021{
struct   MatrixTest {
	static void CreatesIntegerMatrix()
	{
		Matrix<int, 2> mat;
		mat[0][0] = 1;
		mat[1][1] = 1;

		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				std::cout << mat(i, j) << " ";
		std::cout << std::endl;
	}

	static void AccessWrongIndex() {
		Matrix<int, 2> mat;
		mat(2, 1);
	}

	static void ProxyMatrix()
	{
		Matrix<int, 2> mat;
		mat[0][1] = 1;
		mat[1][0] = 1;

		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 2; ++j)
				std::cout << mat[i][j] << " ";
		std::cout << std::endl;
	}

	static void AccessRowAndColumnSize()
	{
		Matrix<int, 4, 5> mat;
		std::cout << "Columns: " << Columns(mat) << ", Rows: " << Rows(mat);
		std::cout << std::endl;
	}

	static void PrintMatrix()
	{
		Matrix<int, 2> mat;
		mat[0][1] = 1;
		mat[1][0] = 1;

		std::cout << mat;
		std::cout << std::endl;
	}

	static void CopyMoveMatrix()
	{
		Matrix<float, 2> mat;
		mat[0][1] = 1;
		mat[1][0] = 1;

		auto mat2 = Matrix<float, 2>(mat);
		auto mat3 = Matrix<float, 2>();
		mat3 = mat2;
		std::cout << mat;
		std::cout << mat2;
		std::cout << mat3;
		auto mat4(std::move(mat3));
		std::cout << mat4;
		std::cout << std::endl;
	}

	static void VectorMatrix()
	{
		auto vec = Matrix<double, 4, 1>();
		auto vec2 = Matrix<double, 1, 4>();
		auto vec3 = Matrix<double, 1, 4>();
		auto vec4 = Matrix<double, 2, 4>();
		auto vec5 = Matrix<double, 2, 4>();
		auto vec6 = Matrix<double, 5, 4>();
		for (int i = 0; i < 4; ++i)
			vec[i] = i;
		std::cout << "Column Vector: " << std::endl << vec << std::endl;
		for (int i = 0; i < 4; ++i)
			vec2[0][i] = i;
		std::cout << "Row Vector: " << std::endl << vec2 << std::endl;
		std::cout << vec3 << std::endl;
		vec3[0] = vec2[0];
		std::cout << vec3 << std::endl;
		for (int i = 0; i < 4; ++i)
			vec4[1][i] = i;
		vec5[1] = vec4[1];
		std::cout << "Column Vector of Matrix: " << std::endl << vec5[1] << std::endl;
		vec6[4] = vec5[1];
		std::cout << "Column Vector of Matrix: " << std::endl << vec6 << std::endl;
	}

	static void InitializerListMatrix()
	{
		auto column_vec = Matrix<double, 1, 4>{ 1.,2.,3.,4. };
		std::cout << column_vec << std::endl;
		auto row_vec = Matrix<double, 6, 1>{ 1.,2.,3.,4.,5., 6. };
		std::cout << row_vec << std::endl;
		auto row_vec2 = Matrix<double, 6, 1>{};
		std::cout << row_vec2 << std::endl;
	}


	static void PrintTransposeMatrix()
	{
		Matrix<int, 2, 3> mat = { 1,2,3,4,5,6 };
		std::cout << "A matrix" << std::endl << mat;
		std::cout << "Columns: " << Columns(mat) << ", Rows: " << Rows(mat);
		std::cout << std::endl;
		std::cout << "and its transposed view" << std::endl << ViewTranspose(mat);
		std::cout << "Columns: " << Columns(ViewTranspose(mat)) << ", Rows: " << Rows(ViewTranspose(mat));
		std::cout << std::endl;
	}

	static void ConstMatrix()
	{
		const Matrix<int, 2> mat = {1,2,3,4};
		std::cout << "Print TransposedView of Constant Matrix:" << std::endl 
			<< TransposedView(mat) << std::endl;
	}

	static void IsSquareMatrix(){
		Matrix<int, 2> mat = { 1,2,3,4 };
		std::cout << "Is " << std::endl <<  mat << " Square? " << (IsSquare(mat) ? "Yes" : "No") << std::endl;
		Matrix<int, 2,3> mat2 = { 1,2,3,4,1,2 };
		std::cout << "Is " << std::endl << mat2 << " Square? " << (IsSquare(mat2) ? "Yes" : "No") << std::endl;
	}

	static void SameSizeMatrix() {
		Matrix<int, 2> mat = { 1,2,3,4 };
		Matrix<float, 2, 3> mat2 = { 1.f,2.f,3.f,4.f,4.f,4.f };
		Matrix<double, 2, 2> mat3 = { 1.,1.,1.,1. };
		std::cout << "Are " << std::endl << mat << " and " << std::endl << mat2 << std::endl << "of same size?"
				  << (AreSameSize(mat, mat2) ? "Yes" : "No") << std::endl;
		std::cout << "Are " << std::endl << mat << " and " << std::endl << mat3 << std::endl << "of same size?"
			<< (AreSameSize(mat, mat3) ? "Yes" : "No") << std::endl;
	}

	static void AdditionViaExpressionTemplateMatrix() {
		Matrix<int, 2> mat = { 1,2,3,4 };
		Matrix<float, 2, 2> mat2 = { 1.f,2.f,3.f,4.f};
		Matrix<double, 2, 2> mat3 = { 1.,1.,1.,1. };
		std::cout << mat << " + " << std::endl << mat2 << std::endl << "="
			<< mat+mat2 << std::endl;
		std::cout << mat << " + " << std::endl << mat2 << std::endl << " + " << std::endl << mat3 << std::endl << "="
			<< mat + mat2 + mat3 << std::endl;
	}

	static void WrongSizeAdditionViaExpressionTemplateMatrix() {
		Matrix<int, 2> mat = { 1,2,3,4 };
		Matrix<float, 2, 3> mat2 = { 1.f,2.f,3.f,4.f,4.f,4.f };
		mat + mat2;
	}

	static void AssignmentOperatorMatrix() {
		Matrix<int, 2> mat = { 1,2,3,4 };
		Matrix<float, 2> mat2 = { 1.5f,2.f,3.f,4.f };
		auto mat3 = mat + mat2;
		std::cout << mat << " + " << std::endl << mat2 << std::endl << "=\n "
			<< mat3 << std::endl;
		Matrix<int, 2, 3> mat4 = { 1,2,3,4,5,6 };
		auto mat5 = TransposedView(mat4);
		std::cout << "Transpose of\n" << mat4 << " is\n" << mat5 << std::endl;
	}


	static void AssignmentMatrixExpressionMatrix() {
		std::cout << "AssignmentMatrixExpressionMatrix\n";
		Matrix<int, 2, 2> mat = { 1,2,3,4};
		std::cout << mat;
		auto mat2 = mat + mat;
		std::cout << mat2;
		decltype(mat) mat3 = mat2;
		mat3(0) = 100 ;
		std::cout << mat3;
	}

	static void ProductMatrix() {
		std::cout << "ProductMatrix\n";
		Matrix<int, 1, 2> row_vec = { 1,2};
		Matrix<int, 2, 1> col_vec = { 3,4 };
		Matrix<int, 2, 2> mat = { 1,0,0,1 };
		Matrix<int, 2, 3> mat2 = { 1,0,0,1,5,5 };
		Matrix<int, 3, 2> mat3 = { 1,0,0,1,6,6 };
		std::cout << "Row Vector\n";
		std::cout << row_vec;
		std::cout << "Colum Vector\n";
		std::cout << col_vec;
		std::cout << "Inner product \n";
		std::cout << row_vec * col_vec;
		std::cout << "Outer product \n";
		std::cout << col_vec * row_vec;
		std::cout << "Matrix  Vector  \n";
		std::cout << mat * col_vec;
		std::cout << "Matrix  Matrix  \n";
		std::cout << mat3 << "*\n" << mat2 << "=\n";
		std::cout << mat3 * mat2;
	}


	static void ScalarProductMatrix() {
		std::cout << "ScalarProductMatrix\n";
		Matrix<int, 2, 2> mat = { 1,0,0,1 };
		std::cout << "3 *\n" << mat << "=\n" << 3 * mat;
		std::cout << "0.5 *\n" << mat << "*" << mat <<  "*2 =\n";
		std::cout << (0.5 * mat) * (mat*2);

	}

	static void DivisionAndSubtractionMatrix() {
		std::cout << "Division Test:\n";
		Matrix<float, 2, 2> mat = { 1,0,0,1 };
		std::cout << mat << "/3=\n" << mat/3;
		std::cout << "3/\n"<< mat << "=\n" << 3/mat;
		Matrix<float, 2, 2> mat2 = { 3,2,0,1 };
		std::cout << mat2 << "/det=\n" << mat2 / Determinant(mat2);

		std::cout << "Substraction Test:\n";
		std::cout << mat << "-0.5\n" << mat<<"=\n" << mat- (0.5*mat);

		std::cout << mat - mat;

	}


	static void TransposeMatrix() {
		std::cout << "TransposeMatrix\n";

		Matrix<float, 2, 4> mat = { 1,2,3,4,
									0,0,0,0};

		std::cout << mat << "Transpose * 2:\n" << 2*Transpose(mat);
	}
	

	static void MinorMatrix() {
		std::cout << std::endl;
		std::cout << "MinorMatrix\n";
		std::cout << std::endl;
		Matrix<float, 3, 4> mat = { 2,3,2,2,
									3,3,3,3,
								    2,3,2,2};

		std::cout << mat << "(1,1) SubMatrix:\n" << SubMatrix(mat, 1, 1)
			<< "(0,0) SubMatrix:\n" << SubMatrix(mat, 0, 0)
		<< "(0,2) SubMatrix:\n" << SubMatrix(mat, 0, 2);
	}

	static void DeterminantMatrix() {
		std::cout << std::endl;
		std::cout << "DeterminantMatrix\n";
		std::cout << std::endl;
		Matrix<float, 3, 3> mat = { 1,0,0,
									0,1,0,
									0,0,2 };
		std::cout << mat << "Determinant:\n" << Determinant(mat) << '\n';

		Matrix<float, 2, 2> mat2 = { 3,1,2,1 };
		std::cout << std::endl;
		std::cout << mat2 << "Determinant:\n" << Determinant(mat2) << '\n';
		std::cout << std::endl;
		Matrix<int, 3, 3> mat3 = { 6,1,1,
							4,-2,5,
							2,8,7 };
		auto det = Determinant(mat3);
		assert(det == -306);
		std::cout << mat3 << "Determinant:\n" << det;
		std::cout << std::endl;
		Matrix<int, 4, 4> mat4 = {  6,1,1,0,
									4,-2,5,0,
									2,8,7,0,
									0,0,0,1};
		std::cout << Determinant(mat4) << '\n';

		std::cout << std::endl;
		Matrix<float, 3, 3> mat5 = { 1,1,0,
									0,1,0,
									0,0,2 };

		std::cout << mat5 << "/Determinant(mat5) = " << '\n';
		std::cout << mat5/ Determinant(mat5) << '\n';
	}


	static void InverseMatrix() {
		std::cout << std::endl;
		std::cout << "InverseMatrix\n";
		std::cout << std::endl;
		Matrix<float, 3, 3> mat = { 1,1,0,
									0,1,0,
									0,0,2 };
		std::cout << mat << "Inverse:\n" << Inverse(mat);
		std::cout << "Should be Identity: \n" << mat * Inverse(mat);
		std::cout << std::endl;
		Matrix<int, 2, 2> mat2 = { 3,1,2,1 };
		std::cout << mat2 << "Inverse:\n" << Inverse(mat2);
		std::cout << "Should be Identity: \n" << mat2 * Inverse(mat2);

		std::cout << std::endl;
		std::cout << "Float: \n";
		Matrix<float, 3, 3> mat3 = { 6,1,1,
									4,-2,5,
									2,8,7};
		std::cout << mat3 << "Inverse:\n" << Inverse(mat3);	
		std::cout << "Should be Identity: \n" << mat3 * Inverse(mat3);

		std::cout << std::endl;
		std::cout << "Double: \n";
		Matrix<double, 3, 3> mat3d = { 6,1,1,
									4,-2,5,
									2,8,7 };
		std::cout << mat3d << "Inverse:\n" << Inverse(mat3d);
		std::cout << "Should be Identity: \n" << mat3d * Inverse(mat3d);
		std::cout << std::endl;
		std::cout << "Long Double: \n";
		Matrix<long double, 3, 3> mat3ld = { 6,1,1,
									4,-2,5,
									2,8,7 };
		std::cout << mat3ld << "Inverse:\n" << Inverse(mat3ld);
		std::cout << "Should be Identity: \n" << mat3ld * Inverse(mat3ld);
		std::cout << std::endl;
		Matrix<float, 4, 4> mat4 = { 6,1,1,0,
									4,-2,5,0,
									2,8,7,0,
									0,0,0,1 };
		std::cout << mat4 << "Inverse:\n" << Inverse(mat4);
		std::cout << "Should be Identity: \n" << mat4 * Inverse(mat4);
	}

	static void DivideEqualMatrix() {
		std::cout << std::endl;
				std::cout << "DivideEqualMatrix\n";
		
		Matrix<float, 3, 3> mat = { 1,1,0,
									0,1,0,
									0,0,2 };
		std::cout << mat << "/=3 =\n" << (mat /=3 ) ;

	}
};

void RunMatrixTest()
{
	using namespace siegel2021;
	MatrixTest::CreatesIntegerMatrix();
	//MatrixTest::AccessWrongIndex();
	MatrixTest::ProxyMatrix();
	MatrixTest::AccessRowAndColumnSize();
	MatrixTest::PrintMatrix();
	MatrixTest::CopyMoveMatrix();
	MatrixTest::VectorMatrix();
	MatrixTest::InitializerListMatrix();
	MatrixTest::PrintTransposeMatrix();
	MatrixTest::ConstMatrix();
	MatrixTest::IsSquareMatrix();
	MatrixTest::SameSizeMatrix();
	MatrixTest::AdditionViaExpressionTemplateMatrix();
	//MatrixTest::WrongSizeAdditionViaExpressionTemplateMatrix();
	MatrixTest::AssignmentOperatorMatrix();
	MatrixTest::AssignmentMatrixExpressionMatrix();
	MatrixTest::ProductMatrix();
	MatrixTest::ScalarProductMatrix();
	MatrixTest::DivisionAndSubtractionMatrix();
	MatrixTest::TransposeMatrix();
	MatrixTest::MinorMatrix();
	MatrixTest::DeterminantMatrix();
	MatrixTest::InverseMatrix();
	MatrixTest::DivideEqualMatrix();
}

}