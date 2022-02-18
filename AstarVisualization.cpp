#define s_width 100
#define s_height 100

#include "Fazen.hpp"
#include <vector>
#include<list>
#include<iostream>

using namespace std;
int rows = 80;
int cols = 80;
int w = 1;
int h = 1;
class Spot;

vector<vector<Spot>> grid;

class Spot{
public:
	int i;
	int j;
	float f;
	float g;
	vector<Spot*> neighbors;
	Spot * parent;
	bool wall=false;


	Spot(){
		i=-1;
		j=-1;
		f=10000;
		g=10000;
		parent=nullptr;
		if(rand()%100<25){
			if(i!=cols-1 && j!=rows-1)
				wall=true;
		}
		
	}
	Spot(float x, float y){
		i = x;
		j = y;
		f=10000;
		g=10000;
		parent=nullptr;
		if(rand()%100<25){
			if(i!=cols-1 && j!=rows-1)
				wall=true;
		}

	}

	void addNeighbors(){
		if (i < cols - 1)
			this->neighbors.push_back(&(grid[i + 1][j]));
		if (i > 0)
			this->neighbors.push_back(&(grid[i - 1][j]));
		if (j < rows - 1)
			this->neighbors.push_back(&(grid[i][j + 1]));
		if (j > 0)
			this->neighbors.push_back(&(grid[i][j - 1]));
		// if(i>0 && j>0){
		// 	this->neighbors.push_back(&(grid[i-1][j-1]));
		// }
		// if(i<cols-1 && j>0){
		// 	this->neighbors.push_back(&(grid[i+1][j-1]));
		// }
		// if(i>0 && j<rows-1){
		// 	this->neighbors.push_back(&(grid[i-1][j+1]));
		// }
		// if(i<cols-1 && j<rows-1){
		// 	this->neighbors.push_back(&(grid[i+1][j+1]));
		// }
	}

	 bool equalSpots(Spot b){
		if ((i == b.i) && (j == b.j)){
			return true;
		}
		return false;
	}

	void show(Fazen2d game,short c=0x2588,short col=blueF){
		float border=0.0;
		if(wall==true){
			col=greyF;
			
		}
		game.Fill(i*w+border,j*h+border,(i+1)*w-border,(j+1)*h-border,c,col);
	}
};


void removeFromVector(vector<Spot *> &a, Spot elt){
	for (int i = 0; i < a.size(); i++){
		if (a[i]->equalSpots(elt)){
			a.erase(a.begin() + i);
		}
	}
}

list<Spot> reconstructPath(Spot *current,Spot start,Fazen2d game){
	list<Spot> total_path;
	total_path.push_back(*current);
	while((current->i!=start.i) && (current->j!=start.j)){
		current=current->parent;
		total_path.push_front(*current);

	}
	return total_path;
}


bool includes(vector<Spot *> a, Spot b){
	for (Spot * i : a){
		if (i->equalSpots(b)){
			return true;
		}
	}
	return false;
}

float heuristic(Spot a, Spot b){
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

int main(){
	Fazen2d game;
	game.make2DConsole();
	game.background(0);
	srand(time(nullptr));


	Spot start;
	Spot endp;

	grid.resize(cols);

	for (int i = 0; i < cols; i++){
		grid[i].resize(rows);
		for (int j = 0; j < rows; j++){
			grid[i][j].i = i;
			grid[i][j].j = j;
			grid[i][j].parent = new Spot(-1,-1);
		}
	}
	for (int i = 0; i < cols; i++){
		for (int j = 0; j < rows; j++){
			grid[i][j].addNeighbors();
		}
	}

	int sx=0,sy=0,ex=cols,ey=rows;
	start = grid[sx][sy];
	endp = grid[ex-1][ey-1];

	start.wall=false;
	endp.wall=false;

	
	vector<Spot * > openSet;
	vector<Spot * > closedSet;

	start.g=0;
	start.f=heuristic(start,endp);
	openSet.push_back(&start);

	while(openSet.empty()==false){
		start.wall=false;
		endp.wall=false;

		// Sleep(50);
		Spot * current=leastFnode(openSet);
		if((current->i == endp.i) && (current->j==endp.j)){
			break;
		}
		removeFromVector(openSet,*current);
		closedSet.push_back(current);
		for(int i=0;i<current->neighbors.size();i++){
			if (!includes(closedSet,*current->neighbors[i])&& !current->neighbors[i]->wall ){
				float tg=current->g+1;
				bool newpath=false;
				if(includes(openSet,*current->neighbors[i]) ){
					if(tg<current->neighbors[i]->g){
						current->neighbors[i]->g=tg;
						newpath=true;
					}
				}else{
					current->neighbors[i]->g=tg;
					newpath=true;
					openSet.push_back(current->neighbors[i]);
				}
				if(newpath){
					current->neighbors[i]->parent=current;
					current->neighbors[i]->f=tg+heuristic(*current->neighbors[i],endp);
				}
			}
		}

		game.background(whiteB);

		for(int i=0;i<cols;i++){
			for(int j=0;j<rows;j++){
				grid[i][j].show(game,0x2588,whiteF);
				if(grid[i][j].equalSpots(start)){
					grid[i][j].show(game,0x2588,greenF);
				}
				else if(grid[i][j].equalSpots(endp)){
					grid[i][j].show(game,0x2588,redF);
				}
			}
		}
		for(Spot * s:closedSet){
			s->show(game,0x2588,dyellowF);
		}
		for(Spot * s:openSet){
			s->show(game,0x2588,dgreenF);
		}

		list<Spot> total_path;
		total_path.push_back(*current);

		while((current->i!=start.i) && (current->j!=start.j)){
			current=current->parent;
			total_path.push_front(*current);
		}
		for(Spot p:total_path){
			p.show(game,0x2588,magentaF);
		}
		grid[start.i][start.j].show(game,0x2588,0x0090);
		grid[endp.i][endp.j].show(game,0x2588,dredF);

		game.display();
		CHECK_USER_EXIT
	}
	
}