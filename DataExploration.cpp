// DataExploration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

void print(vector<double> &v);
double vSum(vector<double> &v);
double vMean(vector<double> &v);
double vMedian(vector<double> v);
double vRange(vector<double> v);
double vCovariance(vector<double> v1, vector<double> v2);
double vCorrelation(vector<double> v1, vector<double> v2);

int main(int argc, char** argv)
{
    ifstream inFS;
    string line;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);

    // Try to open file
    cout << "Opening file Boston.csv." << endl;

    inFS.open("Boston.csv");
    if (!inFS.is_open())
    {
        cout << "Could not open file Boston.csv." << endl;
        return 1; // 1 indicated error
    }

    // Can now use inFS stream line cin stream
    // Boston.csv should contain two doubles

    cout << "Reading line 1" << endl;
    getline(inFS, line);

    // echo heading
    cout << "Heading(s): " << line << endl;

    int numObservations = 0;
    while (inFS.good())
    {
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');

        rm.at(numObservations) = stof(rm_in);
        medv.at(numObservations) = stof(medv_in);

        numObservations++;
    }

    rm.resize(numObservations);
    medv.resize(numObservations);

    cout << "New length: " << rm.size() << endl;

    cout << "Closing file Boston.csv" << endl;
    inFS.close(); // Done with file, so close it

    // Sort vector

    cout << "Number of records: " << numObservations << endl << endl;

    // Print vector
    cout << "First 4 values for rm" << endl;
    cout << "Sum: " << vSum(rm) << endl;
    cout << "Mean: " << vMean(rm) << endl;
    cout << "Median: " << vMedian(rm) << endl;
    cout << "Range: " << vRange(rm) << endl << endl;

    cout << "First 4 values for medv" << endl;
    cout << "Sum: " << vSum(medv) << endl;
    cout << "Mean: " << vMean(medv) << endl;
    cout << "Median: " << vMedian(medv) << endl;
    cout << "Range: " << vRange(medv) << endl << endl;

    cout << "Covariane between rm and medv: " << vCovariance(rm, medv) << endl;
    cout << "Correlation between rm and medv: " << vCorrelation(rm, medv) << endl;

    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////


void print(vector<double> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << i << ": " << v.at(i) << endl;
    }
}
//--------------------

double vSum(vector<double> &v)
{
    double sum = 0;
    for (int i = 0; i < v.size(); i++)
    {
        sum += v.at(i);
    }
    return sum;
}
//--------------------

double vMean(vector<double> &v)
{
    double sum = vSum(v);
    return sum / v.size();
}
//--------------------

double vMedian(vector<double> v)
{
    sort(v.begin(), v.end());

    int size = v.size();

    if (size % 2 != 0)
    {
        return v.at(floor(size / 2));
    }
    else
    {
        int half = size / 2;

        return ( ( v.at(half) + v.at(half - 1) ) / 2);
    }
}
//--------------------

double vRange(vector<double> v)
{
    if (v.size() == 1)
    {
        return 0;
    }

    double highest;
    double lowest;

    if (v.at(0) > v.at(1))
    {
        highest = v.at(0);
        lowest = v.at(1);
    }
    else
    {
        highest = v.at(1);
        lowest = v.at(0);
    }

    for (int i = 2; i < v.size(); i++)
    {
        if (v.at(i) > highest)
        {
            highest = v.at(i);
        }
        else if (v.at(i) < lowest)
        {
            lowest = v.at(i);
        }
    }

    return highest - lowest;
}
//--------------------

double vCovariance(vector<double> v1, vector<double> v2)
{
    double v1Mean = vMean(v1);
    double v2Mean = vMean(v2);

    double summation = 0;
    for (int i = 0; i < v1.size(); i++)
    {
        summation += ( (v1Mean - v1.at(i)) * (v2Mean - v2.at(i)) );
    }
    return summation / v1.size();
}

double vCorrelation(vector<double> v1, vector<double> v2)
{
    double covariance = vCovariance(v1, v2);

    double v1Mean = vMean(v1);
    double v2Mean = vMean(v2);
    double v1Summation = 0;
    double v2Summation = 0;

    for (int i = 0; i < v1.size(); i++)
    {
        v1Summation += pow( (v1Mean - v1.at(i)), 2 );
        v2Summation += pow((v2Mean - v2.at(i)), 2);
    }

    double v1Variance = v1Summation / v1.size();
    double v2Variance = v2Summation / v2.size();

    return covariance / sqrt(v1Variance * v2Variance);
}