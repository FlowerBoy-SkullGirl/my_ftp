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
#include <fstream>

struct gpu_node{
	std::string sku_name;
	double price;
};

struct tree_node{
	struct gpu_node data;
	struct tree_node *lchild;
	struct tree_node *rchild;
};


struct tree_node *create_node(struct tree_node *root, struct gpu_node src)
{
	struct tree_node *np = new struct tree_node;
	np->lchild = NULL;
	np->rchild = NULL;
	np->data = src;
	return np;
}


bool compare_price(struct gpu_node newval, struct gpu_node oldval)
{
	if (newval.price != oldval.price)
		return true;
	else
		return false;
}


struct tree_node *insert_node(struct tree_node *root, struct gpu_node src)
{
	if (root == NULL){
		root = create_node(root, src);
		return root;
	}
	if (root->data.sku_name == src.sku_name){
		if (compare_price(src, root->data)){
			std::cout << "Price change: " << src.sku_name << " " << src.price << std::endl;
			root->data = src;
		}
	}
	else if (root->data.sku_name < src.sku_name){
		root->lchild = insert_node(root->lchild, src);
	}
	else{
		root->rchild = insert_node(root->rchild, src);
	}


	return root;
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

void write_tree(std::fstream &fp, struct tree_node *root)
{
	using namespace std;
	if (!fp.is_open()){
		cerr << "File op!" << endl;
		exit(1);
	}
	
	if (root->lchild != NULL){
		write_tree(fp, root->lchild);
	}
	if (root->rchild != NULL)
		write_tree(fp, root->rchild);

	fp << root->data.sku_name << endl << root->data.price << endl;
	cout << "Successful database entry" << endl;

}

struct tree_node *retrieve_tree()
{
	using namespace std;
		string reception;
		getline(cin, reception); 
		struct tree_node *root = NULL;
		int i = 0;

		while (reception!= "end signal"){ 
			struct gpu_node nod = py_input();

			root = insert_node(root, nod);

			cout << nod.sku_name << nod.price << endl;

			getline(cin, reception);
		}

		return root;
}


struct tree_node *retrieve_db(std::string filen)
{
	using namespace std;
		struct tree_node *root = NULL;
		int i = 0;
		fstream fp;
		string temp;
		fp.open(filen.c_str(), ios::in);

		if (!fp.is_open()){
			cerr << "File op!" << endl;
			exit(1);
		}

		while (getline(fp, temp)){ 
			cout << "Extracting data" << endl;
			struct gpu_node nod;
			nod.sku_name = temp;
			
			getline(fp, temp);

			nod.price = currency_to_double(temp);

			root = insert_node(root, nod);

			cout << nod.sku_name << nod.price << endl;
		}
		
		fp.close();

		return root;
}

struct tree_node *merge_to(struct tree_node *dest, struct tree_node *src)
{
	if (src != NULL){
		merge_to(dest, src->lchild);
		dest = insert_node(dest, src->data);
		merge_to(dest, src->rchild);

		delete src;
	}
	return dest;
}

int main()
{
	using namespace std;
	string filen = "/home/damean/c-stuff/gpu-db/database/db";
	fstream fp;

	cout << "Gathering data" << endl;

	struct tree_node *root = retrieve_tree();

	if (root == NULL){
		cerr << "Null ROOT" << endl;
		exit(2);
	}

	cout << "Opening old data" << endl;

	struct tree_node *root_old = retrieve_db(filen);

	if (!root_old == NULL){
		cout << "Merging data" << endl;
		root_old = merge_to(root_old, root);
	}

	cout << "Writing database" << endl;

	fp.open(filen.c_str(), ios::out);

	write_tree(fp, root_old);

	fp.close();

	cout << "File closed" << endl;


	return 0;
}
