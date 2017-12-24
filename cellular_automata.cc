#include <iostream>
#include <cstdio>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

#define B 10
#define L 10
#define resolution 3

class car;

set<int> grid[B*resolution][L*resolution];
map<int, car> toCar;

string print_set(set<int> x){
	string output="";
	bool comma = false;
	for(set<int>::iterator it=x.begin();it!=x.end();it++){
		if(comma) output += ",";
		comma = true;
		output += to_string(*it);
	}
	return output;
}


class car{

public:
	car(){}
	car(int id, int width, int length, int velocity,
		double v_angle, int acceleration, int alert,
		int row, int column, double angle, int collision_ratio,
		int mode):id(id), width(width), length(length),
		velocity(velocity), v_angle(v_angle), acceleration(acceleration),
		alert(alert), row(row), column(column), angle(angle), collision_ratio(collision_ratio){}

	int id;
	int width, length;
	int velocity;
	double v_angle;
	int acceleration;
	int alert;
	int row, column;
	double angle;
	int collision_ratio;
	int mode;

	void assign_grid(){
		for(int r=0;r<B*resolution;++r) for(int c=0;c<L*resolution;++c){
        // cout << " r-row : " << (double) (r-row) << endl;
        // cout << " c - column * 0: " << (double) (c - column)* sin(angle) << endl;
        // cout << "(double)(r-(row+(double)width*0+(double)length*1)): " << (double)(r-(row+(double)width*sin(angle)+(double)length*cos(angle))) << endl;
        // cout << "(double)(c-(column+(double)width*1-(double)length*0))*sin(angle):" << ( double)(c-(column+(double)width*cos(angle)-(double)length*sin(angle)))*sin(angle) << endl;
        if( r -row >= 0 and c - column >= 0 and r - (row + length) <= 0 and c - (column + width) <= 0){
        // cout << "hellosdlkfjsdlkjf" <<endl;
        grid[r][c].insert(id);
      }

		}
	}
	void assign_grid_empty(){
		for(int r=0;r<B*resolution;++r) for(int c=0;c<L*resolution;++c){
        if(grid[r][c].count(id))  
				grid[r][c].clear();
		}
	}

	int cal_time(int exit_id){
		int num = 0;
		for(int r=0;r<B*resolution;++r){
			if( !grid[r][column-exit_id*r/row].empty() and
				column-exit_id*r/row+1 < L*resolution and !grid[r][column-exit_id*r/row+1].empty() and
				column-exit_id*r/row-1 >= 0 and !grid[r][column-exit_id*r/row-1].empty()){
				num++;
			}
		}

		return (int)sqrt((double)(row*row + (exit_id-column)*(exit_id-column))) + num*collision_ratio;
	}

	void optimal_path(){
		int right_exit = 1;
		int shortest = cal_time(1);
		for(int i=1;i<=L;++i){
			if(shortest > cal_time(i)){
				right_exit = i;
				shortest = cal_time(i);
			}
		}
		v_angle = atan2(right_exit - column, row);
		mode = 1;
	}

	void move(){
		assign_grid_empty();
		if(column + (int)((double)velocity*sin(v_angle)) <= B*resolution){
      column += (int)((double)velocity*sin(v_angle));
        }
    if(row - (int)((double)velocity*cos(v_angle)) <= L*resolution){
      row -= (int)((double)velocity*cos(v_angle));
    }
    assign_grid();
	}

	void change_velocity(){
		// TODO: something
	}
};

bool check_collision(){
	set <int> carToErased;
	bool collision = false;
	for(int r=0;r<B*resolution;++r) for(int c=0;c<L*resolution;++c){
		if((int)grid[r][c].size() >= 2){
			for(set<int>::iterator it=grid[r][c].begin();it!=
				grid[r][c].end();it++){
				carToErased.insert(*it);
				if(toCar.count(*it)) toCar.erase(*it);
			}
			grid[r][c].clear();

			collision = true;
		}
	}

	for(int r=0;r<B*resolution;++r) for(int c=0;c<L*resolution;++c){
		for(set<int>::iterator it=carToErased.begin();it!=
				carToErased.end();it++){
			if(grid[r][c].count(*it)){
				grid[r][c].erase(*it);
			}
		}
	}
	return collision;
}

int main(){
	srand(time(NULL)); // for random functionality


	int id = 1;
	int simulation_time = 5;
	int t = 0;
	double traffic_index = 0.5;

	while(t < simulation_time){
		++t;
		// generate_new_car();

		// TODO: for all tolls
		for(int i=0;i<B;i++){
			double random_val = ((double) rand() / (RAND_MAX));
			if(grid[B*resolution-1][i*resolution].empty() and
				random_val < traffic_index){


				car new_car = car(id,   // id
							1, // width
              1, // length
							5, 			// velocity
							0.0, 		// v_angle
							rand()%5+1, // acceleration
							rand()%5+1, // alert
							B*resolution-1, // row
							i*resolution, // column,
							0.0, 		// angle
							0.0,			// collision_ratio
							0);


				toCar[id] = new_car;
				toCar[id].assign_grid();
				++id;
			}
		}

		// For all cars
		for(map<int,car>::iterator it=toCar.begin();it!=toCar.end();it++){
			(it->second).move(); // car.move()
      //	(it->second).change_velocity();  // TODO: warning
		}

                printf("time t = %d\n",t);
		for(int r=0;r<B*resolution;++r){
			for(int c=0;c<L*resolution;++c){
        // grid[r][c].insert(1);
				printf("[{%5s}]",print_set(grid[r][c]).c_str());
			}
			printf("\n");
		}

		check_collision();

		for(map<int,car>::iterator it=toCar.begin();it!=toCar.end();it++){
			if((it->second).mode == 0){
				(it->second).optimal_path(); // car.optimal_path()
			}
		}

	}
}
