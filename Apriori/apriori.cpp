#include <bits/stdc++.h>
#define mp make_pair
#define pb push_back
#define tr(c, i) for(auto i = (c).begin(); i != c.end(); ++i) printf("%3d ", *i);
#define all(c) (c).begin(), (c).end()
using namespace std;
vector<vector<int> > database;
int minconf, minsupp = 3000;
vector<set<pair<set<int>, int> > > Cset, Lset;
int row, col;
int maxitemno = 80;

void scanDatabase() {
	FILE *fp = fopen("chess.dat", "r");
	fscanf(fp,"%d %d", &row, &col);
	printf("%d %d\n", row, col);
	database.clear();
	database.resize(row, vector<int>(col, 0));
	// scan the database;
	 for (int i = 0; i < row ; ++i) 	
	 	for (int j = 0; j < col; ++j)
	 		fscanf(fp, "%d", &database.at(i).at(j));		
		
}
void print() {
	for (int i = 0; i < row; ++i) 	{
		for (int j = 0; j < col; ++j)
			printf("%d ", database[i][j]);	
		cout << endl;
	}
}
void print(int i) {
	printf("\n\nC%d goes below\n", i+1);	
	for(auto it = Cset[i].begin(); it != Cset[i].end(); ++it) {
		tr(it->first, i);
		cout << " count " << it->second << endl;
	}	
	
	printf("\n\nL%d goes below\n", i+1);
	for(auto it = Lset[i].begin(); it != Lset[i].end(); ++it) {
		tr(it->first, i);
		cout << " count " << it->second << endl;
	}	
}
void calculateC1andL1() {
	vector<int> count(100, 0);
	
	//count occurence of each item
	for(int i = 0; i < row; ++i)
		for(int j = 0; j < col; ++j)
			count[database[i][j]]++;
	
	set<pair<set<int>, int> > C, L;

	//calculate c1 and L1 from count
	set<int> tset;	 
	for(int i = 1; i <= maxitemno; ++i) {
		if(count[i]) {
			tset.insert(i);
			C.insert(mp(tset, count[i]));
			if(count[i] >= minsupp)
				L.insert(mp(tset, count[i]));
		} 
		tset.clear();
	}
	//insert all L and C to their set
	Cset.pb(C);
	Lset.pb(L);
}
void computeC(int i) {
	map<set<int>, int > count;
	set<pair<set<int>, int> > L , C;
	L = Lset.at(i-1);
	set<set<int>> temp;
	set<int> stemp, data;
	set<int> first, second;

	//finding appropriate set for union  whose size is i+1
	for(auto it = L.begin(); it != L.end(); ++it) {
		first = it->first; 
		for(auto jt = next(it); jt != L.end(); ++jt) {
			second = jt->first;
			//merge both sets 
			stemp.insert(first.begin(), first.end());
			stemp.insert(second.begin(), second.end());
			//check size equal to i+1
			if(stemp.size() == i+1) {
				temp.insert(stemp);
			}
			stemp.clear();
		}
	}

	bool flag = true;
	L.clear(); //No further use of L;

	for (auto it = temp.begin(); it != temp.end(); ++it) {
		stemp = *it;
		for(int i = 0; i < row; ++i) {			
			flag = true;
			for(auto jt = stemp.begin(); jt != stemp.end(); ++jt) {
				int item = *jt;
				if(binary_search(all(database[i]), item) == false) {  //find item present in database or not;
					flag = false;
					break;
				}
			}
			if(flag)                              //increase count of itemset if it is there in row of database
				 count[stemp]++;			
		}	
		//insert in C if count of itemset exist	
		 if(count[stemp]) {
			C.insert(mp(stemp, count[stemp]));
			if(count[stemp] >= minsupp)           // if count is greater or equal to minsupp
				L.insert(mp(stemp, count[stemp]));
		} 
	}
	Lset.pb(L);
	Cset.pb(L);
}
void apriori() {
	calculateC1andL1();
	print(0);
	 for(int i = 1; Lset[i-1].size() != 0; ++i) {
		computeC(i);
		if(Lset[i].size())
			print(i);
	}
}
int main() {
	scanDatabase();
	apriori();
	//print(0);
//	apriori();
	return 0;
}