// NaiveBayesScratch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>

using namespace std;

double const PI = 3.14159265;

double probabliityContinous(double x, double mean, double var);

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

    // Make the NB model
    cout << "Predicting survived based on gender, class, and age" << endl;

    // Prep the vectors
    vector<double> apriori = { 0, 0 };
    vector<double> sexS = { 0, 0 };
    vector<double> sexP = { 0, 0 };
    vector<double> ageS = { 0, 0 };
    vector<double> ageP = { 0, 0 };
    vector<double> classS = { 0, 0, 0};
    vector<double> classP = { 0, 0, 0};

    // Train the data on the first 800 entries
    int numTrain = 800;

    // Create and start the clock
    std::chrono::time_point<std::chrono::system_clock> start1, end1;
    start1 = std::chrono::system_clock::now();

    for (int i = 0; i < numTrain; i++)
    {
        // If the entry perished
        if (survived.at(i) == 0)
        {
            // Increment count of perished
            apriori.at(0) += 1;

            // Count thier gender
            if (sex.at(i) == 0)
            {
                // Increment female survived
                sexP.at(0) += 1;
            }
            else
            {
                // Increment male survived
                sexP.at(1) += 1;
            }

            // Count their class
            if (pclass.at(i) == 1)
            {
                // Increment first class perished
                classP.at(0) += 1;
            }
            else if (pclass.at(i) == 2)
            {
                // Increment second class perished
                classP.at(1) += 1;
            }
            else
            {
                // Increment third class perished
                classP.at(2) += 1;
            }

            // Count the age of perished
            ageP.at(0) += age.at(i);
        }
        // Else if they survived
        else
        {
            //Increment count of survived
            apriori.at(1) += 1;

            // Count thier gender
            if (sex.at(i) == 0)
            {
                // Increment female survived
                sexS.at(0) += 1;
            }
            else
            {
                // Increment male survived
                sexS.at(1) += 1;
            }

            // Count their class
            if (pclass.at(i) == 1)
            {
                // Increment first class survived
                classS.at(0) += 1;
            }
            else if (pclass.at(i) == 2)
            {
                // Increment second class survived
                classS.at(1) += 1;
            }
            else
            {
                // Increment third class survived
                classS.at(2) += 1;
            }

            // Count the age of survived
            ageS.at(0) += age.at(i);
        }
    }

    // Divide all values to get the probability
    int numP = apriori.at(0);
    int numS = apriori.at(1);

    apriori.at(0) /= numTrain;
    apriori.at(1) /= numTrain;

    for (int i = 0; i < 2; i++)
    {
        sexP.at(i) /= numP;
        sexS.at(i) /= numS;
    }

    for (int i = 0; i < 3; i++)
    {
        classP.at(i) /= numP;
        classS.at(i) /= numS;
    }

    ageP.at(0) /= numP;
    ageS.at(0) /= numS;

    // Calculate variance
    for (int i = 0; i < numTrain; i++)
    {
        double temp = 0;
        if (survived.at(i) == 0)
        {
            temp = age.at(i) - ageP.at(0);
            ageP.at(1) += pow( temp, 2 );
            //cout << "P Mean is " << ageP.at(0) << " and value is " << age.at(i) << endl
                //<< "The differnce is " << temp << " and that squared is " << pow(temp, 2) << endl;
        }
        else
        {
            temp = age.at(i) - ageS.at(0);
            ageS.at(1) += pow(temp, 2);
        }
    }

    ageP.at(1) /= numP;
    ageS.at(1) /= numS;

    // End the clock and count seconds
    end1 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds1 = end1 - start1;
    std::time_t end_time1 = std::chrono::system_clock::to_time_t(end1);
    cout << "Training time: " << elapsed_seconds1.count() << " second(s)" << endl << endl;

    // Display Probabilities
    cout << "apriori(Perished): " << apriori.at(0) << endl;
    cout << "apriori(Survived): " << apriori.at(1) << endl << endl;
    //-----------
    cout << "female(Perished): " << sexP.at(0) << endl;
    cout << "male(Perished): " << sexP.at(1) << endl;

    cout << "female(Survived): " << sexS.at(0) << endl;
    cout << "male(Survived): " << sexS.at(1) << endl << endl;
    //-----------
    cout << "first class(Perished): " << classP.at(0) << endl;
    cout << "second class(Perished): " << classP.at(1) << endl;
    cout << "third class(Perished): " << classP.at(2) << endl;

    cout << "first class(Survived): " << classS.at(0) << endl;
    cout << "second class(Survived): " << classS.at(1) << endl;
    cout << "third class(Survived): " << classS.at(2) << endl << endl;
    //-----------
    cout << "mean age (Perished): " << ageP.at(0) << endl;
    cout << "varinace age (Perished): " << sqrt(ageP.at(1)) << endl;
    cout << "mean age (Survived): " << ageS.at(0) << endl;
    cout << "varinace age (Survived): " << sqrt(ageS.at(1)) << endl << endl;

    //-----------------------------------------------------------------
    //Predict the next five values
    /*
    for (int i = numTrain; i < numTrain + 5; i++)
    {
        int tempSex = sex.at(i);
        double tempAge = age.at(i);
        int tempClass = pclass.at(i);

        double probablityP = apriori.at(0) * sexP.at(tempSex) * classP.at(tempClass - 1) * probabliityContinous(tempAge, ageP.at(0), ageP.at(1));
        double probablityS = apriori.at(1) * sexS.at(tempSex) * classS.at(tempClass - 1) * probabliityContinous(tempAge, ageS.at(0), ageS.at(1));

        double denominator = probablityP + probablityS;

        cout << "Probability perished: " << probablityP / denominator << endl;
        cout << "Probability survived: " << probablityS / denominator << endl;
    }
    */

    // Predict on all of the traning data
    int numTest = numObservations - numTrain;
    double TP = 0;
    double FP = 0;
    double TN = 0;
    double FN = 0;

    for (int i = numTrain; i < numObservations; i++)
    {
        // Get probability of survived
        int tempSex = sex.at(i);
        double tempAge = age.at(i);
        int tempClass = pclass.at(i);

        double probablityP = apriori.at(0) * sexP.at(tempSex) * classP.at(tempClass - 1) * probabliityContinous(tempAge, ageP.at(0), ageP.at(1));
        double probablityS = apriori.at(1) * sexS.at(tempSex) * classS.at(tempClass - 1) * probabliityContinous(tempAge, ageS.at(0), ageS.at(1));

        double denominator = probablityP + probablityS;

        probablityS /= denominator;

        // Determine if predicted correctly or not
        if (probablityS >= .5)
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
    // Display metrics
    cout << "Accuracy: " << ((TP + TN) / numTest) << endl;
    cout << "Sensitivity: " << (TP / (TP + FN)) << endl;
    cout << "Specificity: " << (TN / (TN + FP)) << endl;

    return 0;
}

double probabliityContinous(double x, double mean, double var)
{
    double a = sqrt(2 * PI * var);
    double b = pow((x - mean), 2);
    double c = 2 * var;

    return 1 / a * exp(-b / c);
}