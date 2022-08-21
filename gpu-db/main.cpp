/*
 * Purpose: Find GPU Price changes based on data fed from python scraper
 * Author: Damean Murphy-Short
 * Version: 0.0
 * Date: August 14, 2022
 */

#include <iostream>
#include <fstream>
#include <iomanip> 
#include <string>
#include <cmath> //pow
#include <cstring>

struct gpu_node{
	std::string sku_name;
	double price;
};

struct tree_node{
	struct gpu_node data;
	struct tree_node *lchild;
	struct tree_node *rchild;
};


struct tree_node *insert_node(struct tree_node *root, struct gpu_node src)
{
	struct tree_node *np = new struct tree_node;
	np->lchild = NULL;
	np->rchild = NULL;
	np->data = src;
	return np;
}

struct tree_node *find_node(struct tree_node *root, std::string src)
{
	struct tree_node *np = root;
	if (np == NULL){
		std::cerr << "Search for non-existing data failed" << std::endl;
		exit(0);
	}
	if ((strcmp(np->data.sku_name.c_str(), src.c_str()) == 0)){
		return np;
	}
	else if ((strcmp(np->data.sku_name.c_str(), src.c_str()) < 0)){
		np = find_node(np->lchild, src);
		return np;
	}
	else{
		np = find_node(np->rchild, src);
		return np;
	}

	return np;
}

struct tree_node *find_node(struct tree_node *root, struct gpu_node src)
{
	struct tree_node *np = root;
	if (np == NULL){
		np = insert_node(np, src);
		return np;
	}
	if ((strcmp(np->data.sku_name.c_str(), src.sku_name.c_str()) == 0)){
		return np;
	}
	else if ((strcmp(np->data.sku_name.c_str(), src.sku_name.c_str()) < 0)){
		np = find_node(np->lchild, src);
		return np;
	}
	else{
		np = find_node(np->rchild, src);
		return np;
	}


	return np;
}

bool compare_price(struct gpu_node newval, struct gpu_node oldval)
{
	if (newval.price != oldval.price)
		return true;
	else
		return false;
}


double currency_to_double(std::string src)
{
	double real_val = 0.0;
	int powers_of_ten = 1;
	
	for (int i = 0; i < src.length(); i++){
		if (src[i] >= '0' && src[i] <= '9')
			real_val = (real_val * 10) + (src[i] - '0');
		if (src[i] == '.')
			powers_of_ten *= pow(10, ((src.length() - 1) - i));
	}

	real_val /= powers_of_ten;

	return real_val;
}

struct gpu_node py_input()
{
	using namespace std;

	struct gpu_node nv;
	
	string sku_title;
	string price_str;

	double real_price;

	getline(cin, sku_title);
	getline(cin, price_str);

	real_price = currency_to_double(price_str);

	nv.sku_name = sku_title;
	nv.price = real_price;

	return nv;
}


int main()
{
	using namespace std;
	string reception;
	getline(cin, reception); 
	struct tree_node *root = NULL;
	struct tree_node *np; 
	struct gpu_node test_gpu;

	while (reception!= "end signal"){ 
		struct gpu_node nod = test_gpu = py_input();

		np = find_node(root, nod);

		cout << nod.sku_name << nod.price << endl;

		getline(cin, reception);
	}

	np = find_node(root, test_gpu);
	
	cout << np->data.sku_name << endl;

	return 0;
}
