#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

template <typename T>
using v1d = vector<T>;
template <typename T>
using v2d = vector<vector<T>>;

// input parameter
string dataset_name; // the output filename
double region_length;
double region_width;
int num_candicate_positions; // the number of candicate positions for the base station
int num_users; // the number of users
int num_existing_macrocells; // the number of existing macrocells

int main(int argc, char** argv)
{
	if (argc != 7)
	{
		cout << "ERROR: Does not match the number of parameters." << endl;
		exit(0);
	}

	srand(time(0));

	dataset_name = argv[1];
	region_length = atof(argv[2]);
	region_width = atof(argv[3]);
	num_candicate_positions = atoi(argv[4]);
	num_users = atoi(argv[5]);
	num_existing_macrocells = atoi(argv[6]);

	cout << "dataset_name = " << dataset_name << endl;
	cout << "region_length = "<< region_length <<endl;
	cout << "region_width = " << region_width <<endl;
	cout << "num_candicate_positions = " << num_candicate_positions << endl;
	cout << "num_users = " << num_users << endl;
	cout << "num_existing_macrocells = " << num_existing_macrocells <<endl;
	cout << endl;

	v2d<double> candicate_positions; // [num_candicate_positions][2]
	v2d<double> users; // [num_users][2]
	v1d<int> existing_macrocells; // [num_original_macrocell]

	candicate_positions.assign(num_candicate_positions, v1d<double>(2,0.0));
	users.assign(num_users, v1d<double>(2,0.0));
	existing_macrocells.resize(num_existing_macrocells);
	
	for(int i = 0; i < num_candicate_positions; i++)
	{
		candicate_positions[i][0] = static_cast<double>(rand()) / RAND_MAX * region_length;
		candicate_positions[i][1] = static_cast<double>(rand()) / RAND_MAX * region_width;
	}
	for(int i = 0; i < num_users; i++)
	{
		users[i][0] = static_cast<double>(rand()) / RAND_MAX * region_length;
		users[i][1] = static_cast<double>(rand()) / RAND_MAX * region_width;
	}
	v1d<int> i1dtmp;
	for(int i = 0; i < num_candicate_positions; i++)
		i1dtmp.push_back(i);
	for(int i = 0; i < num_existing_macrocells; i++)
	{
		int f = rand() % i1dtmp.size();
		existing_macrocells[i] = i1dtmp[f];
		i1dtmp.erase(i1dtmp.begin() + f);
	}

	ofstream file;
	file.open(dataset_name.c_str());
	file << "region_length = "<< region_length << endl;
	file << "region_width = " << region_width << endl;
	file << "num_candicate_point = " << num_candicate_positions << endl;
	file << "num_user = " << num_users << endl;
	file << "num_original_macrocell = " << num_existing_macrocells << endl;
	file << endl;
	file << "Start the candicate point" << endl;
	for(int i = 0; i < num_candicate_positions; i++)
		file << fixed << setprecision(3) << i << "	" << candicate_positions[i][0] << "	" << candicate_positions[i][1] << endl;
	file << "End the candicate point" << endl;
	file << endl;
	file << "Start the user" << endl;
	for(int i = 0; i < num_users; i++)
		file << fixed << setprecision(3) << i << "	" << users[i][0] << "	" << users[i][1] << endl;
	file << "End the user"<<endl;
	file << endl;
	file << "Start the original macrocell" << endl;
	for(int i = 0; i < num_existing_macrocells; i++)
		file << i << "	" << existing_macrocells[i] << endl;
	file << "End the original macrocell" << endl;
	file.close();
}
