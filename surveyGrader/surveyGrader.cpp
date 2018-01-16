#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

void zoneCheck(string ethnicity, string party, string sociology, int g, int f);
void average(float numbersPercent[]);
void deviationFunc(int g, int f);
void generalZone();
void specificZone();

string words[3] = { "above", "within", "below" };
string ethnicityCheck[4] = { "white", "black", "asian", "latino" };
string partyCheck[5] = { "democrat", "republican", "libertarian", "green", "other" };
string sociologyCheck[2] = { "yes", "no" };
int checkEthnicity[4][3][5] = { 0 };
int checkParty[5][3][5] = { 0 };
int checkSociology[2][3][5] = { 0 };
float deviation[2][3][5] = { 0 };
float numbers[5];
string ethnicity;
string party;
string sociology;
float sampleTot = 0;
float totalTracker[3][5] = { 0 };

int main()
{
	int i = 0;

	const float G_REAL[] = { 62, 13, 6, 18, 330 };
	float numbersPercent[5] = { 0 };
	float percent = 0;
	float percentTotal = 0;
	string ethnicityChoice;
	string partyChoice;
	string sociologyChoice;


	cout << "Enter \"white\", \"black\", \"asian\", \"latino\", or \"none\":" << endl;
	cin >> ethnicityChoice;
	cout << "Enter \"democrat\", \"republican\", \"libertarian\", \"other\", or \"none\":" << endl;
	cin >> partyChoice;
	cout << "Enter \"yes\", or \"no\", or \"none\":" << endl;
	cin >> sociologyChoice;

	ifstream inFile;

	inFile.open("C:\\Users\\Moses\\Desktop\\Professional\\saddle S17\\Survey.txt");
	if (inFile.fail())
	{
		cout << "Failure opening file\n\n";
	}
	while (!inFile.eof()) // reads information from one respondant, sorts and prints data
	{
		if (i == 0)
		{
			cout << left << setw(14) << "Ethnicity"
				<< left << setw(14) << "Party"
				<< left << setw(14) << "Sociology"
				<< left << setw(14) << "White%"
				<< left << setw(14) << "Black%"
				<< left << setw(14) << "Asian%"
				<< left << setw(14) << "Latino%"
				<< left << setw(14) << "population" << endl;
		}
		inFile >> ethnicity >> party >> sociology >> numbers[0] >> numbers[1] >> numbers[2] >> numbers[3] >> numbers[4];

		if (((ethnicity == ethnicityChoice) || (ethnicityChoice == "none")) // checks respondant to see if they are within ethnicity search requirements
			&& ((party == partyChoice) || (partyChoice == "none")) // checks respondant to see if they are within party search requirements
			&& ((sociology == sociologyChoice) || (sociologyChoice == "none"))) // checks respondant to see if they are within sociology search requirements
		{
			int f = 0;
			int g = 0;

			for (int a = 0; a < 5; a++) // grabs numbers from respondant to put into total average of all respondants at the end
			{
				numbersPercent[a] += numbers[a];
			}
			for (int a = 0; a < 5; a++) // sorts respondant into category of either within 3% of actual ethnicity percentage, or above, or below
			{
				if (f == 4)// sorts respondant into category of either within 30 million of actual population, or above, or below
				{
					if (numbers[f] >(G_REAL[f] + 30))
					{
						g = 0; // sets category to above 30 million
						deviationFunc(g, f);
						zoneCheck(ethnicity, party, sociology, g, f);
					}
					else if (numbers[f] < (G_REAL[f] - 30))
					{
						g = 2;// sets category to below 30 million
						deviationFunc(g, f);
						zoneCheck(ethnicity, party, sociology, g, f);
					}
					else
					{
						g = 1;// sets category to within 30 million
						deviationFunc(g, f);
						zoneCheck(ethnicity, party, sociology, g, f);
					}
				}
				else
				{
					if (numbers[f] > (G_REAL[f] + 3))
					{
						g = 0;
						deviationFunc(g, f);// sets category to above 3%
						zoneCheck(ethnicity, party, sociology, g, f);
					}
					else if (numbers[f] < (G_REAL[f] - 3))
					{
						g = 2;
						deviationFunc(g, f);// sets category to below 3%
						zoneCheck(ethnicity, party, sociology, g, f);
					}
					else
					{
						g = 1;
						deviationFunc(g, f);// sets category to within 3%
						zoneCheck(ethnicity, party, sociology, g, f);
					}
				}
				f++;
			}
			percent = numbers[0] + numbers[1] + numbers[2] + numbers[3]; // adds together individual respondant's total percentage entered on survey
			percentTotal += percent;

			cout << setw(14) << ethnicity //prints respondant's data
				<< setw(14) << party
				<< setw(14) << sociology
				<< setw(14) << numbers[0]
				<< setw(14) << numbers[1]
				<< setw(14) << numbers[2]
				<< setw(14) << numbers[3]
				<< setw(14) << numbers[4]
				<< setw(14) << percent << endl;
			sampleTot++;
		}
		i++;
	}
	cout << endl;
	average(numbersPercent); // prints average scores of all respondants
	cout << endl;
	generalZone(); // prints total respondants above, below, and within 3%
	specificZone(); // prints specific data for each ethnicity, party, and sociology that is above, below, or within 3%

	return 0;
}

void zoneCheck(string ethnicity, string party, string sociology, int g, int f)
{
	for (int a = 0; a < 4; a++)
	{
		if (ethnicityCheck[a] == ethnicity)
		{
			checkEthnicity[a][g][f] += 1;
			totalTracker[0][a] += 1;
		}
	}
	for (int a = 0; a < 5; a++)
	{
		if (partyCheck[a] == party)
		{
			checkParty[a][g][f] += 1;
			totalTracker[1][a] += 1;
		}
	}
	for (int a = 0; a < 2; a++)
	{
		if (sociologyCheck[a] == sociology)
		{
			checkSociology[a][g][f] += 1;
			totalTracker[2][a] += 1;
		}
	}
}

void average(float numbersPercent[])
{
	int z = 0;
	while (z < 5)
	{
		if (z == 0)
			cout << setw(42) << "average percentage of scores" << setw(5) << setprecision(4) << numbersPercent[z] / sampleTot << " (" << sampleTot << left << setw(5) << ")";
		else
		{
			cout << setw(5) << setprecision(4) << numbersPercent[z] / sampleTot << " (" << sampleTot << setw(5) << ")";
		}
		z++;
	}
}

void deviationFunc(int g, int f)
{
	deviation[0][g][f] += numbers[f];
	deviation[1][g][f] += 1;
}
void generalZone()
{
	cout << endl;

	for (int m = 0; m < 3; m++)
	{
		for (int z = 0; z < 5; z++)
		{
			if (z == 0)
				cout << "scores " << setw(6) << words[m] << setw(29) << " 3% from real number" << setw(5) << setprecision(4) << deviation[0][m][z] / deviation[1][m][z] << " (" << deviation[1][m][z] << left << setw(5) << ")";
			else
				cout << setw(5) << setprecision(4) << (deviation[0][m][z] / deviation[1][m][z]) << " (" << deviation[1][m][z] << left << setw(5) << ")";
		}
		cout << endl;
	}
	cout << endl;
}
void specificZone()
{
	int ethnicityTracker = 0;
	cout << endl << "ethnicity data" << endl;
	for (int l = 0; l < 4; l++)
	{
		for (int m = 0; m < 3; m++)
		{
			cout << setw(11) << ethnicityCheck[l] << " " << setw(6) << words[m] << setw(24) << " real number";
			for (int n = 0; n < 5; n++)
			{
				cout << setw(2) << checkEthnicity[l][m][n] << " (" << right << setprecision(3) << setw(4) << ((checkEthnicity[l][m][n] / (totalTracker[0][l] / 5)) * 100) << left << setw(6) << "%)";
			}
			cout << endl;
		}
		cout << endl;
	}
	cout << endl << "party data" << endl;
	for (int l = 0; l < 5; l++)
	{
		for (int m = 0; m < 3; m++)
		{
			cout << setw(11) << partyCheck[l] << " " << setw(6) << words[m] << setw(24) << " real number";
			for (int n = 0; n < 5; n++)
			{
				cout << setw(2) << checkParty[l][m][n] << " (" << right << setprecision(3) << setw(4) << ((checkParty[l][m][n] / (totalTracker[1][l] / 5)) * 100) << left << setw(6) << "%)";
			}
			cout << endl;
		}
		cout << endl;
	}
	cout << endl << "sociology data" << endl;
	for (int l = 0; l < 2; l++)
	{
		for (int m = 0; m < 3; m++)
		{
			cout << setw(11) << sociologyCheck[l] << " " << setw(6) << words[m] << setw(24) << " real number";
			for (int n = 0; n < 5; n++)
			{
				cout << setw(2) << checkSociology[l][m][n] << " (" << right << setprecision(3) << setw(4) << ((checkSociology[l][m][n] / (totalTracker[2][l] / 5)) * 100) << left << setw(6) << "%)";
			}
			cout << endl;
		}
		cout << endl;
	}
}