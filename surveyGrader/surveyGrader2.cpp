//*****************************************************************************
//	This is a program that reads data from a file that holds survey responses.
//  It then presents the relevant information about the demographics of the
//  respondants.
//*****************************************************************************


#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

void demographicRegister(string ethnicity, string party, string sociology, int g, int f);
void average(float numbersPercent[]);
void zone();

float deviation[2][3][5] = { 0 };
int num[3][5][3][5];
float sampleTot = 0;
float totalTracker[3][5] = { 0 };
string name[3][5] = { { "white", "black", "asian", "latino","null" },
					  { "democrat", "republican", "libertarian", "green", "other" },
				   	  { "yes", "no" ,"null" ,"null" ,"null" } };

class Survey
{
private:
	string ethnicity, party, sociology;
	float numbers[5];
public:
	Survey(string e, string p, string s, float n[])
	{
		ethnicity = e;
		party = p;
		sociology = s;
		numbers[0] = n[0];
		numbers[1] = n[1];
		numbers[2] = n[2];
		numbers[3] = n[3];
		numbers[4] = n[4];
	}
	bool match(string choice[]) // checks respondant to see if they are within all search requirements
	{
		if (
			((ethnicity == choice[0]) || (choice[0] == "na")) &&  // ethnicity search requirements
			((party == choice[1]) || (choice[1] == "na")) &&  // party search requirements
			((sociology == choice[2]) || (choice[2] == "na")) // sociology search requirements
			)
			return true;
		else return false;
	}
	void print() // prints all information for individual survey
	{
		cout << left << setw(14) << ethnicity << setw(14) << party << setw(14) << sociology;
		printf("%-14.0f%-14.0f%-14.0f%-14.0f%-14.0f", numbers[0], numbers[1], numbers[2], numbers[3], numbers[4]);
		cout << numbers[0] + numbers[1] + numbers[2] + numbers[3] << "%" << endl;  
	}
	void percent(float numbersPercent[5]) // gathers total percentages of all survey sections
	{ 
		for (int i = 0; i < 5; i++)
			numbersPercent[i] += numbers[i];
	}
	void categorize() // places all relevant info about survey into appropriate categories
	{
		const float G_REAL[] = { 62, 13, 6, 18, 330 }; // real percentages of ethnic categories
		int cat, num = 3;

		for (int i = 0; i < 5; i++) // sorts respondant into category of either within 3% of actual ethnicity percentage, or above, or below
		{
			if (i == 4)// sorts respondant into category of either within 30 million of actual population, or above, or below
				num = 30;
			if (numbers[i] > (G_REAL[i] + num))
				cat = 0;
			else if (numbers[i] < (G_REAL[i] - num))
				cat = 2;
			else
				cat = 1;
			deviation[0][cat][i] += numbers[i];
			deviation[1][cat][i] += 1;
			demographicRegister(ethnicity, party, sociology, cat, i);
		}
	}
};

int main()
{
	vector<Survey> survey;
	float numbersPercent[5] = { 0 };
	float percent = 0;
	float percentTotal = 0;

	string choice[3];
	cout << "Enter \"white\", \"black\", \"asian\", \"latino\", or \"na\":" << endl;
	cin >> choice[0];
	cout << "Enter \"democrat\", \"republican\", \"libertarian\", \"other\", or \"na\":" << endl;
	cin >> choice[1];
	cout << "Enter \"yes\", or \"no\", or \"na\":" << endl;
	cin >> choice[2];

	ifstream inFile;
	inFile.open("C:\\programming\\projectsC++\\surveyFinal\\Survey.txt");	//file containing all survey info
	if (inFile.fail()) cout << "Failure opening file\n\n";		//notifies if file does not open correctly
	float numbers[5];
	string ethnicity, party, sociology;
	while (!inFile.eof())		//reads information from each respondant, stores in object vector called "survey"
	{
		inFile >> ethnicity >> party >> sociology >> numbers[0] >> numbers[1] >> numbers[2] >> numbers[3] >> numbers[4];
		survey.push_back(Survey(ethnicity, party, sociology, numbers));
	}
	printf("%-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s\n", 
		"Ethnicity", "Party", "Sociology", "White%", "Black%", "Asian%", "Latino%", "Population");
	
	for (unsigned int i = 0; i < survey.size(); ++i)
	{
		if (survey[i].match(choice))
		{
			sampleTot++;
			survey[i].categorize();
			survey[i].print();
			survey[i].percent(numbersPercent);
		}
	}

	average(numbersPercent); // prints average scores of all respondants
	zone(); // prints specific data for each ethnicity, party, and sociology that is above, below, or within 3%
	cout << "Total sample size: " << sampleTot;
	int end; cin >> end;
	return 0;
}

//*****************************************************************************
//	adds individual survey to the running tally of demographics
//*****************************************************************************
void demographicRegister(string ethnicity, string party, string sociology, int k, int l)
{
	string word[3] = { ethnicity, party, sociology };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (name[i][j] == word[i])
			{
				num[i][j][k][l] += 1; //counts the number of participants
				totalTracker[i][j] += 1; // adds to demographic of survey
			}
		}
	}
}

//*****************************************************************************
// Lists the average score amongst all surveys for each ethnicity entry
//*****************************************************************************
void average(float numbersPercent[])
{
	printf("\n%-41s", "average percentage of scores");
	for (int i = 0; i < 5; i++)
		printf("%3.0f (%.0f%-7s", numbersPercent[i]/ sampleTot, sampleTot, ")");
}

//*****************************************************************************
// Put in place to ensure the computer didn't divide by zero and make a mess
//*****************************************************************************
float safeDivide(float x, float y)
{
	if (y == 0)
		return 0;
	else return x / y;
}

//*****************************************************************************
// Lists information for each specific demographic by 
// raw number and percentage of total demographic for each category
//*****************************************************************************
void zone()
{
	string words[2][3] = { { "above", "within", "below" } ,
						   { "ethnicity", "party", "sociology" } };
	cout << endl;
	for (int m = 0; m < 3; m++) // Lists average scores of each category and raw number of people per category
	{
		cout << endl << "scores " << setw(6) << words[0][m] << setw(29) << " 3% from real number";
		for (int z = 0; z < 5; z++)
			printf("%-2.0f (%2.0f%-8s", safeDivide(deviation[0][m][z], deviation[1][m][z]), deviation[1][m][z], ")");
	}
	cout << endl;
	for (int i = 0; i < 3; i++) // 0 = ethnicity data, 1 = party data, 2 = sociology data
	{
		cout << endl << words[1][i] << endl; //the corresponding words from above^^^
		for (int j = 0; j < 5; j++) //the major categories of each of the i categories
		{
			if (name[i][j] != "null") //checks for an additional category in the sub category before printing
			{
				for (int k = 0; k < 3; k++) //0 = above, 1 = within, 2 = below
				{
					cout << setw(11) << name[i][j] << " " << setw(6) << words[0][k] << setw(24) << " real number";
					for (int l = 0; l < 5; l++) //0 = white guess, 1 = black guess, 2 = asian guess, 3 = latino guess, 4 = population guess
					{
						printf("%-2.0u(%5.1f%-6s", num[i][j][k][l], (safeDivide(num[i][j][k][l], (totalTracker[i][j] / 5)) * 100), "%)");
					}
					cout << endl;
				}
				cout << endl;
			}
		}
	}
}
