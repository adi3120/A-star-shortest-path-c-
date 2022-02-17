#define s_width 80
#define s_height 80
#include <vector>
#include<list>
#include <iostream>
using namespace std;
int rows = 25;
int cols = 25;
int w = 16;
int h = 16;
class Spot;
vector<vector<Spot>> grid;
class Spot
{
public:
	int i;
	int j;
	float f;
	float g;
	vector<Spot*> neighbors;
	Spot * parent;


	Spot()
	{
		i=-1;
		j=-1;
		f=10000;
		g=10000;
		parent=nullptr;
	}
	Spot(float x, float y)
	{
		i = x;
		j = y;
		f=10000;
		g=10000;
		parent=nullptr;

	}

	void addNeighbors()
	{
		if (i < cols - 1)
			this->neighbors.push_back(&(grid[i + 1][j]));
		if (i > 0)
			this->neighbors.push_back(&(grid[i - 1][j]));
		if (j < rows - 1)
			this->neighbors.push_back(&(grid[i][j + 1]));
		if (j > 0)
			this->neighbors.push_back(&(grid[i][j - 1]));
		if(i>0 && j>0){
			this->neighbors.push_back(&(grid[i-1][j-1]));
		}
		if(i<cols-1 && j>0){
			this->neighbors.push_back(&(grid[i+1][j-1]));
		}
		if(i>0 && j<rows-1){
			this->neighbors.push_back(&(grid[i-1][j+1]));
		}
		if(i<cols-1 && j<rows-1){
			this->neighbors.push_back(&(grid[i+1][j+1]));
		}
	}

	 bool equalSpots(Spot b)
	{
		if ((i == b.i) && (j == b.j))
		{
			return true;
		}
		return false;
	}

	void printSpot()
	{
		cout << "(" << i << "," << j << ")" << endl;
	}

	void printParents(){

		cout<<"(x : "<<parent->i<<", y : "<<parent->j<<")  ";
	}
};
void printneighbors(vector<vector<Spot>> grid)
{
	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			cout << grid[i][j].neighbors.size() << " ";
		}
		cout << endl;
	}
}

void printSpotList(vector<Spot> a)
{
	for (Spot i : a)
	{
		cout << "(" << i.i << "," << i.j << ")"
			 << " ";
	}
	cout << endl;
}



void removeFromVector(vector<Spot *> &a, Spot elt)
{

	for (int i = 0; i < a.size(); i++)
	{
		if (a[i]->equalSpots(elt))
		{
			a.erase(a.begin() + i);
		}
	}

}

list<Spot> reconstructPath(Spot *current,Spot start)
{
	list<Spot> total_path;
	total_path.push_back(*current);


	while((current->i!=start.i) && (current->j!=start.j)){
		// current.printParents();
		// cout<<endl;
		current=current->parent;
		total_path.push_front(*current);

	}

	return total_path;
}


bool includes(vector<Spot *> a, Spot b)
{
	for (Spot * i : a)
	{
		if (i->equalSpots(b))
		{
			return true;
		}
	}
	return false;
}

float heuristic(Spot a, Spot b)
{
	float d = abs(a.i - b.i) + abs(a.j - b.j);
	return d;
}

Spot* leastFnode(vector<Spot *> openSet){
	Spot * winner=openSet[0];

	for (Spot *p : openSet)
	{
		if(p->f<winner->f){
			winner=p;
		}
	}
	return winner;
}

void printgridParent(){
	for(int i=0;i<cols;i++){
		for(int j=0;j<rows;j++){
			grid[i][j].printParents();
		}
		cout<<endl;
	}
}
list<Spot> a_starNew(vector<vector<Spot>> & grid,Spot start, Spot endp){

	vector<Spot * > openSet;


	start.g=0;
	start.f=heuristic(start,endp);

	openSet.push_back(&start);


	while(openSet.empty()==false){



		Spot * current=leastFnode(openSet);

		if((current->i == endp.i) && (current->j==endp.j)){
            cout<<"finished"<<endl;
			return reconstructPath(current,start);
		}

		removeFromVector(openSet,*current);

		for(int i=0;i<current->neighbors.size();i++){

			

			
			float tg=current->g+1;

			if(tg<current->neighbors[i]->g){

				current->neighbors[i]->parent=current;



				current->neighbors[i]->g=tg;

				current->neighbors[i]->f=tg+heuristic(*current->neighbors[i],endp);


				if(!includes(openSet,*current->neighbors[i])){
					openSet.push_back(current->neighbors[i]);
				}
			}


		}

	}
	return {};
}


int main()
{

	Spot start;
	Spot endp;
	cout<<"Enter number of columns : ";
	cin>>cols;
	cout<<"Enter number of rows : ";
	cin>>rows;

	grid.resize(cols);

	char canvas[rows][cols];

	for(int i=0;i<cols;i++){
		for(int j=0;j<rows;j++){
			canvas[i][j]='.';
		}
	}


	for (int i = 0; i < cols; i++)
	{
		grid[i].resize(rows);

		for (int j = 0; j < rows; j++)
		{
			grid[i][j].i = i;
			grid[i][j].j = j;
			grid[i][j].parent = new Spot(-1,-1);

		}
	}

	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			grid[i][j].addNeighbors();
		}
	}
	int sx,sy,ex,ey;
	cout<<"Enter starting point's x : ";
	cin>>sx;
	cout<<"Enter starting point's y : ";
	cin>>sy;
	cout<<"Enter Goal point's x : ";
	cin>>ex;
	cout<<"Enter Goal point's y : ";
	cin>>ey;

	start = grid[sx][sy];
	endp = grid[ex- 1][ey - 1];


	list<Spot> path=a_starNew(grid,start,endp);

	// for(Spot i : path){
	// 	i.printSpot();
	// 	canvas[i.i][i.j]='#';
	// }
	canvas[start.i][start.j]='O';
	canvas[endp.i][endp.j]='X';

	for(int i=0;i<cols;i++){
		for(int j=0;j<rows;j++){
			cout<<canvas[i][j]<<" ";
		}
		cout<<endl;
	}

}