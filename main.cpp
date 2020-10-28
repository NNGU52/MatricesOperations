/*
* Найти определитель и обратную матрицу для заданной матрицы, вывести на экран и в файл
* полученные результаты, а так же произведение данной и обратной матрицы.
*/


#include "stdafx.h"	// инклуд файл, который служит для генерации файла предкомпилированных заголовков
#include <iostream>	// для cin и cout
#include <iomanip>	// параметризированные манипуляторы потока
#include <fstream>	// для работы с файлами
#include <vector>	// для работы с векторами
#include <cmath>	// для математических функций

using namespace std;

void print(vector<vector<double>> _matrix1, int count = 5, int number = 9) 
{
	int m = _matrix1.size();
	int n = _matrix1[0].size();

	cout.setf(ios::right);			// установка флага вывода ; выравнивание по правой границе(по умолчанию)
	cout.setf(ios::fixed);			// установка флага вывода ; вывод чисел с плавающей точкой в фиксированной форме(по умолчанию)
	cout << setprecision(count);	// вывод чисел с установленным кол-ом знаков после запятой, где count кол-во знаков после точки

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cout << setw(number) << _matrix1[i][j];		// вывод с установленной шириной поля, где number кол-во позиций, символов
		}
		cout << endl;
	}

	cout << endl;
}


void fprint(vector<vector<double>> matrix, int mode, int count = 20, int number = 30)
{
	int n = matrix.size();

	ofstream file("result.txt", mode);	// запись в файл, где mode режим работы с открываемым файлом

	file.setf(ios::right);
	file.setf(ios::fixed);
	file << setprecision(count);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			file << setw(number) << matrix[i][j];
		}
		file << endl;
	}

	file << endl;
}


// находим определитель матрицы методом Гаусса
double det(vector<vector<double>> matrix)
{
	int n = matrix.size();
	double result = 1.0;

	for (int k = 0; k < n; k++) {
		for (int i = k + 1; i < n; i++) {
			for (int j = n - 1; j >= 0; j--) {
				matrix[i][j] -= matrix[i][k] * matrix[k][j] / matrix[k][k];		// делаем верхнетреугольную матрицу
			}
		}
		result *= matrix[k][k];		// находим определитель матрицы, путем перемножения элементов главной диагонали
	}

	return result;
}


// находим обратную матрицу методом Жордана - Гаусса
vector<vector<double>> inverse(vector<vector<double>> matrix)
{
	int n = matrix.size();

	vector<vector<double>> augmented(n, vector<double>(2 * n, 0));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 2 * n; j++) {
			double value = 0;
			if (j < n)
			{
				value = matrix[i][j];	// Создаем матрицу [n][2 * n]
			}
			else
			{
				if (i == j - n)
				{
				   value = 1;
				}
				else
				{
				   value = 0;
				}
			}
			augmented[i][j] = value;
		}
	}

	for (int k = 0; k < n; k++) {                          
		for (int i = 2 * n - 1; i >= 0; i--) {     
			augmented[k][i] /= augmented[k][k];
		}
		for (int i = k + 1; i < n; i++) {               
			for (int j = 2 * n - 1; j >= 0; j--) {
				augmented[i][j] -= augmented[i][k] * augmented[k][j];
			}
		}
	}

	for (int k = n - 1; k >= 0; k--) {
		for (int i = k - 1; i >= 0; i--) {
			for (int j = 2 * n - 1; j >= 0; j--) {
				augmented[i][j] -= augmented[i][k] * augmented[k][j];
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrix[i][j] = augmented[i][j + n];
		}
	}

	return matrix;
}


// Считаем произведение матриц
vector<vector<double>> mul(vector<vector<double>> a, vector<vector<double>> b)
{
	int n = a.size();

	vector<vector<double>> result(n, vector<double>(n, 0));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			double value = 0;
			for (int k = 0; k < n; k++) {
				value += a[i][k] * b[k][j];
			}
			result[i][j] = value;
		}
	}

	return result;
}


// Считает норму квадратной матрицы
double norm(vector<vector<double>> matrix)
{
	int n = matrix.size();

	double result = 0;

	for (int j = 0; j < n; j++) {
		double sum = 0;
		for (int i = 0; i < n; i++) {
			sum += abs(matrix[i][j]);
		}
		if (result < sum) {
			result = sum;
		}
	}

	return result;
}


int main()
{
	int n = 6;		// Размерность матрицы

	// Инициализация пустой матрицы порядка {n}
	vector<vector<double>> matrix(n, vector<double>(n, 0));

	// Заполнение матрицы значениями функции
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			double value = sqrt(abs(1 / tan(i + j)));
			matrix[i - 1][j - 1] = value;
		}
	}

	print(matrix);

	// Получение обратной матрицы
	vector<vector<double>> inverseMatrix = inverse(matrix);

	print(inverseMatrix);

	print(mul(matrix, inverseMatrix));
	//Вывод полученных матриц в файл
	fprint(matrix, ios_base::trunc);
	fprint(inverseMatrix, ios_base::app);
	fprint(mul(matrix, inverseMatrix), ios_base::app);

	// Вывод cond (число обусловленности матрицы) и det (детерминант)
	double cond = norm(matrix) * norm(inverseMatrix);
	cout << "  " << "cond_A = " << cond << endl << endl;
	cout << "  " << "Determinant_A = " << det(matrix) << endl;

	getchar();
}
