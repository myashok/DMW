#include <bits/stdc++.h>
#define mp make_pair
#define pb push_back
#define tr(c, i) for(auto i = (c).begin(); i != c.end(); ++i) cout << *i << " ";
#define wtr(c, i) for(auto i = (c).begin(); i != c.end(); ++i) cout << *i ;
#define all(c) (c).begin(), (c).end()
using namespace std;
vector<vector<string> > database;
int minconf, minsupp = 3;
int row;
const int maxitemno = 80;
vector<pair<int, string> > frequentItems; //pair<count, item>
vector<vector<string> > frequentItemsInRow;
//node for tree
struct node {
	int count;
	string  val;
	map<string,struct node *> child;
	struct node *parent;
	node() {
		val = "";
		count = 0;
		parent = nullptr;		
	}
};
typedef struct node node;
node *root;
vector<vector<string> > tree;
void scanDatabase() {
	FILE *fp = fopen("input.txt", "r");
	fscanf(fp,"%d ", &row);
	printf("Transaction count = %d \n\n", row);
	database.clear();
	database.resize(row);
	int col;
	char x[50];
	string cplusx;
	// scan the database;
	 for (int i = 0; i < row ; ++i) {	
	 		fscanf(fp, "%s", x);
	 		fscanf(fp, "%d", &col);
	 		//cout << x << endl << col <<  " ";
	 		for(int j = 0; j < col; ++j) {
	 			fscanf(fp, "%s", x);
	 			cplusx = x;
	 			database[i].pb(cplusx);
	 		//	cout << cplusx <<  " ";
	 		}
	 	//	cout << endl;
	 }		
		
}
void printfrequentItems() {
	cout << "Frequent Items and their count\n";
	for(auto it = frequentItems.begin(); it != frequentItems.end(); ++it) {
		cout << it->second  << " " << it->first << endl;
	}
}
void print() {
	cout << "Frequent Items in each row \n";
	for (int i = 0; i < row; ++i) 	{
		cout << "row " << (i+1) <<  ": ";
		for (int j = 0; j < frequentItemsInRow[i].size(); ++j) 
			cout << frequentItemsInRow[i][j] << " ";
		cout << endl;
	}
}
void calculateFrequentItemSet() {
	map<string, int> count;
	//count occurence of each item
	for(int i = 0; i < row; ++i)
		for(int j = 0; j < database[i].size(); ++j)
			count[database[i][j]]++;

	//calculate frequentItems;
	for(auto it = count.begin(); it != count.end(); ++it) {
		if(it->second >= minsupp)	
			frequentItems.pb(mp(it->second, it->first));
	}
	sort(all(frequentItems), greater<pair<int, string> > ());
	string item;
	//calculate frequentItem set
	frequentItemsInRow.clear();
	frequentItemsInRow.resize(row);
	cout << "DataBase Items are\n";
	for(int i = 0; i < row; ++i)  {
		tr(database[i], jj);
		cout << endl;
		for(auto it = frequentItems.begin(); it != frequentItems.end(); ++it) {
			item = it->second;			
			if(binary_search(all(database[i]), item))
				frequentItemsInRow[i].pb(item);
		} 
	}
}
void addToTree(node *root, vector<string> &itemRow, int index) {
	if(index >= itemRow.size())
		return;
	struct node *temp;
	if(root->child.find(itemRow[index]) == root->child.end()) {
		temp = new node();
		temp->val = itemRow[index];
		temp->parent = root;
		root->child[itemRow[index]] = temp;
	}
	root->child[itemRow[index]]->count += 1;
	addToTree(root->child[itemRow[index]], itemRow, index + 1);
}
void constructTree(node *root) {
	vector<string> itemRow;
	for(int i  = 0; i < row; ++i) {
		itemRow = frequentItemsInRow[i];
		addToTree(root, itemRow, 0);
	}
}
void printTree(node *root) {
	if(root->parent == nullptr) 
		cout << "node " << root->val << endl;		
	else 
		cout << "parent " << (root->parent->val=="" ? "root": root->parent->val)<< " child " << root->val << " count of child " << root->count << endl;		
	for(auto it = root->child.begin(); it != root->child.end(); ++it) {		
			printTree(it->second);		
	}
}
//declares a map for conditional base and frequent ItemSet
map<string, vector<pair<vector<string>, int >  > > mymap;
map<string, map<string, int> > frequenItemSet;

void dfs(node *root, vector<string> arr) {
//	cout << "entered " << root->child.size() << endl;
	if(root->child.empty())
		return;
	if(root->val != "")
		arr.pb(root->val);

	for(auto it = root->child.begin(); it != root->child.end(); ++it) {
		if(!arr.empty())
			mymap[it->second->val].pb(mp(arr, it->second->count));
		dfs(it->second, arr);
	}
}
void fp() {
	calculateFrequentItemSet();
	root = new node();
	constructTree(root);
	vector<string> arr;
	dfs(root, arr);		
}
void calculateConditionalPatternBase() {
	vector<string> map;
	for (auto it = mymap.begin(); it != mymap.end(); ++it) {
		cout << it->first << " => { ";
		for(int i = 0; i < it->second.size(); ++i) {
			for(auto jt = it->second[i].first.begin(); jt != it->second[i].first.end(); ++jt){
				cout << *jt;
				frequenItemSet[it->first][*jt] += it->second[i].second;
			}
			cout << ": " << it->second[i].second << ", " ;
		}
		cout << "}  { ";
		for(auto i = frequenItemSet[it->first].begin(); i != frequenItemSet[it->first].end(); ++i) {
			if(i->second >= minsupp) {
				cout << i->first << ": " << i->second << ", ";
			}
		}
		cout << "}";
		cout << endl;
	}
}

int main() {
	scanDatabase();
	fp();
	cout << endl;
	printfrequentItems();
	cout << endl;
	print();
	cout << endl;
	cout << "Fp Tree is\n";
	printTree(root);
	cout << endl << "printing conditional pattern bases and FrequentItemSet\n";
	//cout << mymap.size() << endl;
	calculateConditionalPatternBase();
	//cout << endl << "printing frequentItem Set \n";
	//printFrequentItemSet();
	return 0;
}