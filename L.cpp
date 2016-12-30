// Created by Mohammed Sharieff

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <xstring>


using namespace std;



void PrintTable(double ** data, int n, double * countL)
{

	ofstream output;

	output.open("out.csv");

	double totalIPMT = 0, totalPMT = 0, begBal = 0;

	begBal = data[0][0];

	for(int i = 0; i < n; ++i){

		cout << i << "\tBAL: " << setprecision(15) << data[i][0] << "\tIPMT: " << data[i][1] << "\tPMT: " << data[i][2] << "\tPaid Off: " << countL[i] << endl;

		output << data[i][0] << "," << data[i][1] << "," << data[i][2] << "," << countL[i] << endl;

		totalIPMT += data[i][1];
		totalPMT += data[i][2];

	}

	cout << endl << "---------------------------------------------------" << endl;
	cout << "Beginning Balance: " << setprecision(15) << begBal << endl;
	cout << "Total Interest Paid: " << setprecision(15) << totalIPMT << endl;
	cout << "Total Payment: " << setprecision(15) << totalPMT << endl << endl;
	cout << "Total Loans: " << countL[n - 1] << endl << endl;

}

double * CountLoans(double ** data, int n, int M)
{

	double * result = new double[M];

	double count = 0;

	for(int i = 0; i < M; ++i){

		for(int j = 0; j < n; ++j){
			if((int)data[j][2] <= i){
				count += 1;
			}
		}
		result[i] = count;
		count = 0;
		//cout << result[i] << endl;

	}
	
	return result;
}


double maxLoan(double ** data, int n)
{
	double result = 0;
	double k = -100000;


	for(int i = 0; i < n; ++i){

		if(k < data[i][2]){
			k = data[i][2];
		}

	}

	result = k;

	return result;
}

double ** Aggregate(double *** data, double ** check, int n)
{
	int x = maxLoan(check, n);
	x += 1;
	double ** result = new double*[x];
	for(int i = 0; i < x; ++i){
		result[i] = new double[3];
	}

	for(int i = 0; i < x; ++i){

		for(int j = 0; j < n; ++j){

			result[i][0] += data[j][i][0];
			result[i][1] += data[j][i][1];
			result[i][2] += data[j][i][2];

		}

	}

	return result;

}

double pmt(double Amt, double APR, int T, int t)
{
	double payment = 0;
	double A = 0, B = 0, C = 0, D = 0;
	double E = 0, F = 0;

	A = (double) T - (double) t;
	B = APR / A;
	C = Amt * B;
	D = 1 + B;
	E = pow(D, -A);
	F = 1 - E;

	payment = C / F;

	return payment;
}

double ipmt(double Amt, double Apr, int T, int t)
{
	double interest = 0;

	interest = Amt * (Apr / ((double)T - (double)t));

	return interest;
}


double *** SolveLoan(double ** data, int n)
{
	
	double maxZ = maxLoan(data, n);
	int N = (int) maxZ;

	double APR = 0;

	int min = (int) data[5][3];
	int max = (int) data[5][4];
	
	double *** LOAN = new double**[n];
	for(int i = 0; i < n; ++i){
		LOAN[i] = new double*[N + 1];
	}
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < N + 1; ++j){
			LOAN[i][j] = new double[3];
		}
	}

	for(int i = 0; i < n; ++i){

		int M = (int) data[i][2];
		int a = (int) data[i][3];
		int b = (int) data[i][4];


		for(int j = 0; j <= M; ++j){
	
			int c = (rand()%(b-a))+a;
			double delta = (double) c / 5000;

			int z = j - 1;

			if(j == 0){

				LOAN[i][j][0] = data[i][0];

			} else {

				APR = data[i][1] + delta;
				LOAN[i][j][1] = ipmt(LOAN[i][z][0],APR,M,z);
				LOAN[i][j][2] = pmt(LOAN[i][z][0],APR,M,z);
				LOAN[i][j][0] = LOAN[i][z][0] + LOAN[i][j][1] - LOAN[i][j][2];

			}

		}


	}

	

	return LOAN;

}


double ** FixTime(double ** loan, int n)
{
	double ** result = new double*[n];
	for(int i = 0; i < n; ++i){
		result[i] = new double[5];
	}

	for(int i = 0; i < n; ++i){

		result[i][0] = loan[i][0];
		loan[i][1] /= 12;
		loan[i][2] *= 12;
		result[i][3] = loan[i][3];
		result[i][4] = loan[i][4];
		
	}



	return loan;

}

double ** ParseLoan(string * data, int n)
{
	double ** result = new double*[n];
	for(int i = 0; i < n; ++i){

		result[i] = new double[5];

	}

	for(int j = 0; j < n; ++j){
      
        string play = data[j];
        istringstream ss(play);
        string token;
         
        for(int k = 0; k < 5; ++k){
           
        getline(ss, token, ',');
        result[j][k] = atof(token.c_str());
        }
      
    }
	
	return result;

}



int main()
{
	ifstream data;

	data.open("loan.csv");
	char zinc[100];
	int k = 0;
	cout << "Enter the amount of loans: ";
	cin >> k;
	
	string * loan = new string[k];


	for(int i = 0; i < k; ++i){
		data >> zinc;
		loan[i] = zinc;
	}

	double ** loanData = ParseLoan(loan, k);
	double ** fixedData = FixTime(loanData, k);
	
	double *** LOAN = SolveLoan(fixedData, k);
	
	double ** TotalTable = Aggregate(LOAN, fixedData, k);

	int Z = maxLoan(fixedData, k);
	Z += 1;

	//int c = (rand()%(max-min))+min;

	double * loans = CountLoans(fixedData, k, Z);
	PrintTable(TotalTable, Z, loans);
	
	return 0;
}