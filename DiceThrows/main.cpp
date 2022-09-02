#include <iostream>
#include <cmath>
#include <ctime>
#include <string>

using namespace std;

//CONSTANTS
int MIN_ROLL = 1;
string null_str = "null";
int MAXBUF = 256;

struct dice_args{
	int num_dice;
	int num_sides;
	int num_add;
};

int generate_rand_num(int min, int max)
{
	int rand_num;

	rand_num = rand() % ((max + 1) - min) + min;
	
	cout << '+' << rand_num << endl; 

	return rand_num;
}

struct dice_args parse_dice(string src)
{
	struct dice_args die;
	int arg_count = 0;
	die.num_dice = 0;
	die.num_sides = 0;
	die.num_add = 0;
	bool is_dice_arg = false;
	bool is_neg = false;

	for (int i = 0; i < src.length(); i++){
		if (src[i] == ' ')
			arg_count++;
		if (arg_count == 0){
			if (src[i] >= '0' && src[i] <= '9')
				die.num_dice = (die.num_dice * 10) + (src[i] - '0');
		}else if (arg_count == 1){
			if (src[i] == 'd' || src[i] == 'D')
				is_dice_arg = true;
			if (src[i] >= '0' && src[i] <= '9')
				die.num_sides = (die.num_sides * 10) + (src[i] - '0');
		}else if (arg_count == 2){
			if (src[i] == '-')
				is_neg = true;
			if (src[i] >= '0' && src[i] <= '9')
				die.num_add = (die.num_add * 10) + (src[i] - '0');
		}
		if (is_neg)
			die.num_add *= -1;

	}

	return die;
}

int gen_roll(struct dice_args src)
{
	int total = 0;
	for (int i = 0; i < src.num_dice; i++){
		total += generate_rand_num(MIN_ROLL, src.num_sides);
	}
	total += src.num_add;

	return total;
}

int main()
{
	srand(time(NULL));
	string input = null_str;
	struct dice_args temp_die;

	cout << "Example: 1 d5" << endl << "3" << endl;
	while (true){
		getline(cin, input);
		temp_die = parse_dice(input);

		cout << gen_roll(temp_die) << endl;
	}
}
