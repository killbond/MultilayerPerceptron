// Evo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <ctime>
#include <string>

using namespace std;

// Функция активации
double activation(double x)
{
	return 1 / (1 + exp(-x));
}

// Производная от функции активации
double derivative(double x)
{
	return x * (1 - x);
}

int _tmain(int argc, _TCHAR* argv[])
{

	// Параметры НС
	// Число входов,
	// Число нейронов в скрытом слое
	// Число нейронов в выходном слое
	int nInputs = 3,
		nHiddenLayer = 10,
		nOutputs = 1;

	// Скорость обучения
	double trainSpeed = .0001, randomValue;

	srand(time(NULL));
	vector<vector<double>> hiddenLayer, outputLayer, dHiddenLayer, dOutputLayer;

	// Инициализация связей между входом НС и скрытым слоем случайными значениями 
	hiddenLayer.resize(nInputs);
	dHiddenLayer.resize(nInputs);
	for (int input(0); input < nInputs; input++) {
		hiddenLayer[input].resize(nHiddenLayer);
		dHiddenLayer[input].resize(nHiddenLayer);
		for (int hidden(0); hidden < nHiddenLayer; hidden++) {
			randomValue = floor((double(rand()) / RAND_MAX * 1000) + .5) / 1000;
			hiddenLayer[input][hidden] = randomValue;
			dHiddenLayer[input][hidden] = randomValue;
		}
	}

	// Инициализация связей между скрытым слоем НС и выходным слоем случайными значениями
	outputLayer.resize(nHiddenLayer);
	dOutputLayer.resize(nHiddenLayer);
	for (int hidden(0); hidden < nHiddenLayer; hidden++) {
		outputLayer[hidden].resize(nOutputs);
		dOutputLayer[hidden].resize(nOutputs);
		for (int output(0); output < nOutputs; output++) {
			randomValue = floor((double(rand()) / RAND_MAX * 1000) + .5) / 1000;
			outputLayer[hidden][output] = randomValue;
			dOutputLayer[hidden][output] = randomValue;
		}
	}

	// Массив с данным для обучения
	vector<vector<double>> X = {
		{ 2, 3, 6 },
		{ 3, 6, 4 },
		{ 6, 4, 1 },
		{ 4, 1, 6 },
		{ 1, 6, 3 },
	};

	// Массив с желаемыми откликами
	vector<vector<double>> T = {
		{ .4 },
		{ .1 },
		{ .6 },
		{ .3 },
		{ .6 },
	};

	double sum, modifyCoefficient;
	vector<double> sumHidden, sumOutput, deltaHiddenLayer, deltaOutputLayer;
	sumOutput.resize(nOutputs);
	sumHidden.resize(nHiddenLayer);
	deltaOutputLayer.resize(nOutputs);
	deltaHiddenLayer.resize(nHiddenLayer);


	for (int t(0); t < 10; t++) {
		// Для кажодго паттерна данных обучения
		for (int i(0); i < X.size(); i++) {

			// Эпохи обучения
			for (int epoch(0); epoch < 10; epoch++) {

				// Зануляем массивы с дельтами и суммами выходного слоя
				for (int outputIndex(0); outputIndex < nOutputs; outputIndex++) {
					deltaOutputLayer[outputIndex] = .0;
					sumOutput[outputIndex] = .0;
				}

				// Зануляем массивы с дельтами и суммами скрытого слоя
				for (int hiddenIndex(0); hiddenIndex < nHiddenLayer; hiddenIndex++) {
					deltaHiddenLayer[hiddenIndex] = .0;
					sumHidden[hiddenIndex] = .0;
				}

				// Считаем суммы для скрытого слоя
				for (int hidden(0); hidden < nHiddenLayer; hidden++) {
					for (int x(0); x < nInputs; x++) {
						sumHidden[hidden] += hiddenLayer[x][hidden] * X[i][x];
					}
				}

				// Считаем суммы для выходного слоя
				for (int output(0); output < nOutputs; output++) {
					for (int hidden(0); hidden < nHiddenLayer; hidden++) {
						sumOutput[output] += outputLayer[hidden][output] * activation(sumHidden[hidden]);
					}
					for (int x(0); x < X[i].size(); x++) {
						cout << "x[" << x << "] = " << X[i][x] << "; ";
					}
					cout << "T = " << T[i][output] << "; " << "Y = " << activation(sumOutput[output]) << "; " << "ERROR = " << T[i][output] - activation(sumOutput[output]) << endl;
				}

				// Считаем ошибку для каждого выхода (delta)
				for (int output(0); output < nOutputs; output++) {
					deltaOutputLayer[output] = T[i][output] - activation(sumOutput[output]);
				}

				// Считаем ошибку для каждого нейрона скрытого слоя
				for (int hidden(0); hidden < nHiddenLayer; hidden++) {
					for (int output(0); output < nOutputs; output++) {
						deltaHiddenLayer[hidden] += outputLayer[hidden][output] * deltaOutputLayer[output];
					}
				}

				// Модификация весовых коэффициентов выходного слоя
				for (int output(0); output < nOutputs; output++) {
					modifyCoefficient = deltaOutputLayer[output] * trainSpeed;
					for (int hidden(0); hidden < nHiddenLayer; hidden++) {
						outputLayer[hidden][output] += modifyCoefficient * activation(sumHidden[hidden]);
					}
				}

				// Модификация весовых коэффициентов скрытого слоя
				for (int hidden(0); hidden < nHiddenLayer; hidden++) {
					modifyCoefficient = deltaHiddenLayer[hidden] * derivative(sumHidden[hidden]) * trainSpeed;
					for (int input(0); input < nInputs; input++) {
						hiddenLayer[input][hidden] += modifyCoefficient * X[i][input];
					}
				}
			}
		}
	}

	for (int input(0); input < nInputs; input++) {
		hiddenLayer[input].resize(nHiddenLayer);
		for (int hidden(0); hidden < nHiddenLayer; hidden++) {
			cout << "hiddenLayer[" << input << "][" << hidden << "] = " << hiddenLayer[input][hidden] << endl;
		}
	}
	cout << endl;
	for (int hidden(0); hidden < nHiddenLayer; hidden++) {
		outputLayer[hidden].resize(nOutputs);
		for (int output(0); output < nOutputs; output++) {
			cout << "outputLayer[" << hidden << "][" << output << "] = " << outputLayer[hidden][output] << endl;
		}
	}
	system("PAUSE");
	return 0;
}