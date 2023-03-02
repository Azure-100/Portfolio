// LogisticRegressionScratch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include <algorithm>
//#include <math.h>
//#include <typeinfo>
#include <chrono>

using namespace std;

double probability(double z);

// Keep trck of useful constants
double e = 2.71828;


int main(int argc, char** argv)
{
    // Section for opening the csv file
    ifstream inFS;
    string line;
    string IDIn, pclassIn, survivedIn, sexIn, ageIn;
    const int MAX_LEN = 1500;

    vector<int> pclass(MAX_LEN);
    vector<int> survived(MAX_LEN);
    vector<int> sex(MAX_LEN);
    vector<double> age(MAX_LEN);

    // Try to open file
    cout << "Opening file titanic.csv." << endl;

    inFS.open("titanic_project.csv");
    if (!inFS.is_open())
    {
        cout << "Could not open file titanic.csv." << endl;
        return 1; // 1 indicated error
    }

    // Can now use inFS stream line cin stream
    // titanic.csv should contain 5 values

    cout << "Reading line 1" << endl;
    getline(inFS, line);

    // echo heading
    cout << "Heading(s): " << line << endl;

    int numObservations = 0;
    while (inFS.good())
    {
        getline(inFS, IDIn, ',');
        //cout << "IDIn: " <<  IDIn << endl;
        getline(inFS, pclassIn, ',');
        //cout << "pclassIn: " << pclassIn << endl;
        getline(inFS, survivedIn, ',');
        //cout << "survivedIn: " << survivedIn << endl;
        getline(inFS, sexIn, ',');
        //cout << "sexIn: " << sexIn << endl;
        getline(inFS, ageIn, '\n');
        //cout << "ageIn: " << ageIn << endl;

        pclass.at(numObservations) = stoi(pclassIn);
        survived.at(numObservations) = stoi(survivedIn);
        sex.at(numObservations) = stoi(sexIn);
        age.at(numObservations) = stod(ageIn);

        numObservations++;
    }

    pclass.resize(numObservations);
    survived.resize(numObservations);
    sex.resize(numObservations);
    age.resize(numObservations);

    cout << "New length: " << pclass.size() << endl;

    cout << "Closing file titanic.csv" << endl
        << "--------------------------------------------------------" << endl;
    inFS.close(); // Done with file, so close it

    cout << "Predicting survived based on gender" << endl;
    //-----------------------------------------------
    // First, make vectors keeping track of what factors are present
    vector<int> male(numObservations);
    for (int i = 0; i < numObservations; i++)
    {
        if (sex.at(i) == 1)
        {
            male.at(i) = 1;
        }
        else
        {
            male.at(i) = 0;
        }
    }

    // Make a prediction of survived based on sex

    double alpha = .0002;
    long long int epoch = 5000;
    vector<double> B = { 0, 0 };
    int numTrain = 799;

    // Create and start the clock
    std::chrono::time_point<std::chrono::system_clock> start1, end1;
    start1 = std::chrono::system_clock::now();

    for (int i = 0; i < numTrain * epoch; i++)
    {
        int id = i % numTrain;

        // Prep the variables
        // // B = b0, b1
        // z = (b0 * x1, b1 * x2)
        double probabilityValue = 0;
        double z = 0;

        // Calculate z as b0 + b1 * male
        z = B[0] + (B[1] * male.at(id));

        // Plug z into the probablility function
        probabilityValue = probability(z);

        // Calculate error
        double error = survived.at(id) - probabilityValue;

        // Update the weights
        B[0] = B[0] + alpha * error;
        B[1] = B[1] + alpha * (male.at(id) * error);
    }
    // End the clock and count seconds
    end1 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds1 = end1 - start1;
    std::time_t end_time1 = std::chrono::system_clock::to_time_t(end1);
    cout << "Training time: " << elapsed_seconds1.count() << " second(s)" << endl << endl;

    // Display B values
    cout << "Intercept: " << B[0] << endl;
    cout << "Age: " << B[1] << endl << endl;

    // Test the data
    int numTest = numObservations - numTrain;
    double TP = 0;
    double FP = 0;
    double TN = 0;
    double FN = 0;

    // Count the TP, TN, FP, FN values

    for (int i = (numTrain + 1); i < numObservations; i++)
    {
        // Get probability
        int z = B[0] + (B[1] * male.at(i));
        double probabilityValue = probability(z);

        // Determine if predicted correctly or not
        if (probabilityValue >= .5)
        {
            if (survived.at(i) == 1)
            {
                TP += 1;
            }
            else
            {
                FP += 1;
            }
        }
        else
        {
            if (survived.at(i) == 0)
            {
                TN += 1;
            }
            else
            {
                FN += 1;
            }
        }
    }
    // Display results
    cout << "Accuracy: " << ( (TP + TN) / numTest) << endl;
    cout << "Sensitivity: " << (TP / (TP + FN)) << endl;
    cout << "Specificity: " << (TN / (TN + FP)) << endl
        << "--------------------------------------------------------" << endl;

    cout << "Predicting survived based on gender, class, and age" << endl;
    // Predict again based on sex, class, and age

    // Make the dummy variables for class types
    vector<int> class2(numObservations);
    vector<int> class3(numObservations);
    for (int i = 0; i < numObservations; i++)
    {
        if (pclass.at(i) == 2)
        {
            class2.at(i) = 1;
            class3.at(i) = 0;
        }
        else if (pclass.at(i) == 3)
        {
            class2.at(i) = 0;
            class3.at(i) = 1;
        }
        else
        {
            class2.at(i) = 0;
            class3.at(i) = 0;
        }
    }

    B.clear();
    //b0 = intercet, b1=male, b2=class2, b3=class3, b4=age
    B = { 0, 0, 0, 0, 0 };

    // Create and start the clock
    std::chrono::time_point<std::chrono::system_clock> start2, end2;
    start2 = std::chrono::system_clock::now();

    for (int i = 0; i < numTrain * epoch; i++)
    {
        int id = i % numTrain;

        // Prep the variables
        // // B = b0, b1, b2, b3, b4
        // z = (b0 * x1, b1 * x2, b2 * x3, b3 * x3, b4 * x4)
        double probabilityValue = 0;
        double z = 0;

        // Calculate z as b0 + (b1 * male) + (b2 * class2) + (b3 * class3) + (b4 * age)
        z = B[0] + (B[1] * male.at(id)) + (B[2] * class2.at(id)) + (B[3] * class3.at(id))
            + (B[4] * age.at(id));

        // Plug z into the probablility function
        probabilityValue = probability(z);

        // Calculate error
        double error = survived.at(id) - probabilityValue;

        // Update the weights
        B[0] = B[0] + alpha * error;
        B[1] = B[1] + alpha * (male.at(id) * error);
        B[2] = B[2] + alpha * (class2.at(id) * error);
        B[3] = B[3] + alpha * (class3.at(id) * error);
        B[4] = B[4] + alpha * (age.at(id) * error);
    }
    // End the clock and count seconds
    end2 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds2 = end2 - start2;
    std::time_t end_time2 = std::chrono::system_clock::to_time_t(end2);
    cout << "Training time: " << elapsed_seconds2.count() << " second(s)" << endl << endl;

    // Display B values
    cout << "Intercept: " << B[0] << endl;
    cout << "Male: " << B[1] << endl;
    cout << "Class2: " << B[2] << endl;
    cout << "Class3: " << B[3] << endl;
    cout << "Age: " << B[4] << endl << endl;

    // Reset values and compare against test data
    TP = 0;
    FP = 0;
    TN = 0;
    FN = 0;

    for (int i = 800; i < numObservations; i++)
    {
        // Get probability
        int z = B[0] + (B[1] * male.at(i)) + (B[2] * class2.at(i)) + (B[3] * class3.at(i))
            + (B[4] * age.at(i));
        double probabilityValue = probability(z);

        // Compare against test values
        if (probabilityValue >= .5)
        {
            if (survived.at(i) == 1)
            {
                TP += 1;
            }
            else
            {
                FP += 1;
            }
        }
        else
        {
            if (survived.at(i) == 0)
            {
                TN += 1;
            }
            else
            {
                FN += 1;
            }
        }
    }
    // Display results
    cout << "Accuracy: " << ((TP + TN) / numTest) << endl;
    cout << "Sensitivity: " << (TP / (TP + FN)) << endl;
    cout << "Specificity: " << (TN / (TN + FP)) << endl;


    return 0;
}

double probability(double z)
{
    return 1 / (1 + pow(e, (-1 * z)));
}