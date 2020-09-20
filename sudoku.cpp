#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <sys/time.h> 
#include <time.h>


using namespace std;

typedef vector<vector<int> > vvi;
typedef vector<int> vi;

void print(vvi* state, int id){
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
			cout<<(*state)[i][j]<<" ";
		if(id==1)
			cout<<endl;
	}
}

vvi* generate_sudoku(vi &nums)
{
	vvi *state=new vvi(0);
	for(int i=0;i<9;i++)
	{
		vi bla;
		for(int j=0;j<9;j++)
		{
			bla.push_back((nums)[9*i+j]);
		}
		state->push_back(bla);
		bla.clear();
	}
	return state;
}
vector<vvi*>* get_sole_element(vvi* state)
{	
	vector<vvi*> stack_temps;
	stack_temps.clear();
	vvi* temp=new vvi(0);
	for(int i=0;i<9;i++)
	{
		vi bla;
		bla.clear();
		for(int j=0;j<9;j++)
			if((*state)[i][j]>0)
				bla.push_back(0);
			else
				bla.push_back(1);
		temp->push_back(bla);
		bla.clear();
	}
	vvi* result=new vvi(0);
	for(int i=0;i<9;i++)
	{
		vi bla;
		for(int j=0;j<9;j++)
		{
			bla.push_back((*state)[i][j]);
		}
		result->push_back(bla);
		bla.clear();
	}
	int trip=0;
	vvi indices;
	for(int cur_num=1;cur_num<10;cur_num++)
	{
		indices.clear();
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if((*state)[i][j]==cur_num)
				{
					vi index{i,j};
					indices.push_back(index);
				}
			}
		}
		while(indices.size()>0)
		{
			vi index=indices.back();
			assert(index.size()==2);
			indices.pop_back();
			int row=index[0];
			int col=index[1];
			for(int x=0;x<9;x++)
			{
				(*temp)[row][x]=0;
			}
			for(int x=0;x<9;x++)
			{
				(*temp)[x][col]=0;
			}
			int start_r=row/3;
			int start_c=col/3;
			for(int x=0;x<3;x++)
				for(int y=0;y<3;y++){
					(*temp)[3*start_r+x][3*start_c+y]=0;
				}
		}
		int temp_sum;
		int last_possible;
		for(int i=0;i<9;i++)
		{
			temp_sum=0;
			for(int j=0;j<9;j++)
			{
				temp_sum=temp_sum+(*temp)[i][j];
				if((*temp)[i][j]==1)
					last_possible=j;
			}
			if(temp_sum==1)
			{
				(*result)[i][last_possible]=cur_num;
				(*state)[i][last_possible]=cur_num;
				(*temp)[i][last_possible]=0;
				for(int temp_row=0;temp_row<9;temp_row++)
					(*temp)[temp_row][last_possible]=0;
				int st_r=i/3;
				int st_c=last_possible/3;
				for(int temp_row=0;temp_row<3;temp_row++)
					for(int temp_col=0;temp_col<3;temp_col++)
						(*temp)[3*st_r+temp_row][3*st_c+temp_col]=0;
				trip=1;
			}

		}
		for(int j=0;j<9;j++)
		{
			temp_sum=0;
			for(int i=0;i<9;i++)
			{
				temp_sum+=(*temp)[i][j];
				if((*temp)[i][j]==1)
					last_possible=i;
			}
			if(temp_sum==1)
			{
				(*result)[last_possible][j]=cur_num;
				(*state)[last_possible][j]=cur_num;
				(*temp)[last_possible][j]=0;
				for(int temp_col=0;temp_col<9;temp_col++)
					(*temp)[last_possible][temp_col]=0;
				trip=1;
				int st_r=last_possible/3;
				int st_c=j/3;
				for(int temp_row=0;temp_row<3;temp_row++)
					for(int temp_col=0;temp_col<3;temp_col++)
						(*temp)[3*st_r+temp_row][3*st_c+temp_col]=0;
			}
		}
		for(int start_row=0;start_row<9;start_row+=3)
		{
			for(int start_col=0;start_col<9;start_col+=3)
			{
				temp_sum=0;
				int insert_row, insert_col;
				for(int i=0;i<3;i++)
				{
					for(int j=0;j<3;j++)
					{
						temp_sum+=(*temp)[start_row+i][start_col+j];
						if ((*temp)[start_row+i][start_col+j]==1)
						{
							insert_row=start_row+i;
							insert_col=start_col+j;
						}
					}
				}
				if(temp_sum==1)
				{
					(*state)[insert_row][insert_col]=cur_num;
					(*result)[insert_row][insert_col]=cur_num;
					(*temp)[insert_row][insert_col]=0;
					for(int temp_common_index=0;temp_common_index<9;temp_common_index++)
					{
						(*temp)[insert_row][temp_common_index]=0;
						(*temp)[temp_common_index][insert_col]=0;
					}
					trip=1;
				}
			}
		}
		stack_temps.push_back(temp);
		temp=new vvi(0);
		for(int i=0;i<9;i++)
		{
			vi bla;
			for(int j=0;j<9;j++)
			{
				if((*state)[i][j]>0)
					bla.push_back(0);
				else
					bla.push_back(1);
			}
			temp->push_back(bla);
			bla.clear();
		}

	}
	int temp_sum;
	int last_possible;
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if( (*result)[i][j]>0)
			{
				for(int digit=0;digit<9;digit++)
					(*(stack_temps[digit]))[i][j]=0;
			}
		}
	}
	for(int k=0;k<9;k++)
	{
		for(int g=0;g<9;g++)
		{
			temp_sum=0;
			for(int digit=0; digit<9;digit++)
			{
				if((*(stack_temps[digit]))[k][g]==1)
				{
					temp_sum++;
					last_possible=digit+1;
				}
			}
			if(temp_sum==1)
			{
				(*result)[k][g]=last_possible;
				for(int temp_var=0;temp_var<9;temp_var++)
				{
					(*(stack_temps[last_possible-1]))[k][temp_var]=0;
					(*(stack_temps[last_possible-1]))[temp_var][g]=0;
				}
				int st_r=k/3;
				int st_c=g/3;
				for(int temp_row=0;temp_row<3;temp_row++)
					for(int temp_col=0;temp_col<3;temp_col++)
						(*(stack_temps[last_possible-1]))[3*st_r+temp_row][3*st_c+temp_col]=0;
			}
		}
	}

	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			(*state)[i][j]=(*result)[i][j];

	
	
	stack_temps.clear();
	temp=new vvi();
	(*temp).clear();
	for(int i=0;i<9;i++)
	{
		vi bla;
		bla.clear();
		for(int j=0;j<9;j++)
			if((*state)[i][j]>0)
				bla.push_back(0);
			else
				bla.push_back(1);
		temp->push_back(bla);
		bla.clear();
	}
	for(int cur_num=1;cur_num<10;cur_num++)
	{
		indices.clear();
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if((*state)[i][j]==cur_num)
				{
					vi index{i,j};
					indices.push_back(index);
				}
			}
		}
		int m=indices.size();
		while(indices.size()>0)
		{
			vi index=indices.back();
			assert(index.size()==2);
			indices.pop_back();
			int row=index[0];
			int col=index[1];
			for(int x=0;x<9;x++)
			{
				(*temp)[row][x]=0;
			}
			for(int x=0;x<9;x++)
			{
				(*temp)[x][col]=0;
			}
			int start_r=row/3;
			int start_c=col/3;
			for(int x=0;x<3;x++)
				for(int y=0;y<3;y++){
					(*temp)[3*start_r+x][3*start_c+y]=0;
				}
		}
		stack_temps.push_back(temp);
		temp=new vvi(0);
		for(int i=0;i<9;i++)
		{
			vi bla;
			for(int j=0;j<9;j++)
			{
				if((*state)[i][j]>0)
					bla.push_back(0);
				else
					bla.push_back(1);
			}
			temp->push_back(bla);
			bla.clear();
		}

	}
	vector<vvi*> *list=new vector<vvi*>();
	for(int digit=0;digit<9;digit++)
	{
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				if((*(stack_temps[digit]))[i][j]==1)
				{
					vvi* neighbour=new vvi();
					vi bla;
					for(int row=0;row<9;row++)
					{
						bla.clear();
						for(int col=0;col<9;col++)
						{
							bla.push_back((*state)[row][col]);
						}
						neighbour->push_back(bla);
					}
					(*neighbour)[i][j]=digit+1;
					list->push_back(neighbour);
				}
			}
		}
	} 
	return list;
}
double get_num_components(vvi* state)
{
	double num_comp=0;
	vvi visited;
	vi bla;
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if((*state)[i][j]==0)
				bla.push_back(1);
			else
				bla.push_back(0);
		}
		visited.push_back(bla);
		bla.clear();
	}
	int trip=0;
	while(trip==0)
	{
		trip=1;
		int source_i, source_j;
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++)
			{
				if(visited[i][j]==0)
				{
					trip=0;
					source_i=i;
					source_j=j;
				}
			}
		if(trip==1)
			break;
		num_comp++;
		vvi stack;
		vi index{source_i, source_j};
		stack.push_back(index);
		while(stack.size()>0)
		{
			index=stack.back();
			stack.pop_back();
			int row=index[0];
			int col=index[1];
			visited[row][col]=1;
			if(row-1>=0 && visited[row-1][col]==0)
			{
				index.clear();
				index.push_back(row-1);
				index.push_back(col);
				stack.push_back(index);
			}
			if(row+1<9 && visited[row+1][col]==0)
			{
				index.clear();
				index.push_back(row+1);
				index.push_back(col);
				stack.push_back(index);
			}
			if(col-1>=0 && visited[row][col-1]==0)
			{
				index.clear();
				index.push_back(row);
				index.push_back(col-1);
				stack.push_back(index);
			}
			if(col+1<9 && visited[row][col+1]==0)
			{
				index.clear();
				index.push_back(row);
				index.push_back(col+1);
				stack.push_back(index);
			}
		}
	}
	return num_comp;

}
double get_score(vvi* state)
{
	double res=get_num_components(state);
	return res;
}
bool is_solved(vvi* state)
{
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			if((*state)[i][j]==0)
				return false;
	return true;
}

int main(int argc, char** argv)
{
	// vi temp{0,0,4,5,0,0,0,2,3,0,0,8,0,0,7,0,0,9,0,7,6,0,9,2,0,0,0,0,0,9,0,1,8,0,6,0,0,0,0,0,0,0,0,0,0,0,6,0,9,2,0,8,0,0,0,0,0,2,5,0,3,4,0,2,0,0,3,0,0,9,0,0,1,4,0,0,0,9,2,0,0};
	// vi temp{4,0,0,0,1,0,0,5,0,9,0,3,8,4,5,1,0,6,0,0,1,9,0,3,2,8,0,0,3,0,0,0,0,5,0,0,1,0,6,0,0,0,9,0,2,0,0,9,0,0,0,0,4,0,0,1,4,2,0,7,6,0,0,7,0,5,6,9,1,4,0,3,0,9,0,0,5,0,0,0,8};
	// vi temp{0,0,0,0,0,0,8,0,0,0,0,0,0,0,5,0,9,2,0,4,3,7,0,0,0,0,0,0,0,0,6,0,4,0,0,5,0,5,0,0,0,0,0,0,9,0,0,4,0,2,8,0,3,0,0,0,0,0,0,0,6,0,0,0,0,0,0,6,0,0,0,0,2,0,7,1,0,9,0,0,0};
	// vi temp{0, 0, 0, 9, 0, 6, 8, 1, 0, 0, 0, 6, 4, 0, 5, 0, 9, 2, 9, 4, 3, 7, 8, 2, 0, 6, 0, 0, 0, 8, 6, 9, 4, 0, 0, 5, 6, 5, 2, 3, 7, 1, 4, 8, 9, 0, 9, 4, 5, 2, 8, 0, 3, 6, 4, 0, 9, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 6, 0, 9, 0, 0, 2, 6, 7, 1, 4, 9, 0, 0, 8};
	// vi temp{0,0,4,3,0,0,2,0,9,0,0,5,0,0,9,0,0,1,0,7,0,0,6,0,0,4,3,0,0,6,0,0,2,0,8,7,1,9,0,0,0,7,4,0,0,0,5,0,0,8,3,0,0,0,6,0,0,0,0,0,1,0,5,0,0,3,5,0,8,6,9,0,0,4,2,9,1,0,3,0,0};
	// vi temp{0,0,4,3,0,0,2,0,9,0,0,5,0,0,9,0,0,1,0,7,0,0,6,0,0,4,3,0,0,6,0,0,2,0,8,7,1,9,0,0,0,7,4,0,0,0,5,0,0,8,3,0,0,0,6,0,0,0,0,0,1,0,5,0,0,3,5,0,8,6,9,0,0,4,2,9,1,0,3,0,0,};
	vi temp{9, 1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 2, 0, 0, 3, 7, 0, 9, 0, 0, 0, 0, 0, 4, 0, 8, 6, 0, 0, 9, 3, 0, 0, 0, 0, 0, 0, 2, 0, 0, 5, 0, 0, 7, 0, 0, 0, 0, 0, 0, 8, 9, 0, 0 ,2 ,7 ,0 , 4, 0, 0, 0, 0, 0, 5, 0, 1, 2, 0, 0, 6, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2, 7};
	// cout<<argv[1]<<endl;
	if(stoi(argv[1])==1)
	{
		cout<<"Provide Row Wise, left to right, traversal of the sudoku. Separate with a single whitespace."<<endl;
		temp.clear();
		string num;
		cin>>num;
		for(int i=0;i<81;i++)
			temp.push_back(num[2*i]-'0');

	}
	// vi temp{0,0,2,6,0,0,0,0,0,0,6,0,4,0,5,0,8,0,0,0,0,9,0,0,0,0,2,0,8,0,0,0,0,7,5,6,0,0,0,0,3,0,0,0,0,7,2,5,0,0,0,0,3,0,6,0,0,0,0,8,0,0,0,0,1,0,3,0,6,0,4,0,0,0,0,0,0,9,3,0,0};
	vvi* state=generate_sudoku(temp);
	print(state, 1);
	cout<<endl<<endl<<endl<<endl;
	vvi* state_to_fall_back=new vvi();
	vi bla;
	for(int i=0;i<9;i++){
		bla.clear();
		for(int j=0;j<9;j++)
			bla.push_back((*state)[i][j]);
		state_to_fall_back->push_back(bla);
	}
	int num_steps=0;
	int max_steps=20;
	srand( (unsigned)time( NULL ) );
	float prob_of_best_move=0.3;
	float prob_num;
	double best_score, comparator_variable;
	vvi* best_neighbour;//=new vvi();
	vvi* random_neighbour;//=new vvi();
	vector<vvi*> *neighbour_list;//=new vector<vvi*>();
	int net=0;
	//*************************************************
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    ios_base::sync_with_stdio(false);
    //*************************************************
    
	while(!is_solved(state))
	{
		// print(state, 0);	
		// cout<<endl<<endl;
		net=net+1;
		// cout<<net;
		if(num_steps==max_steps)
		{
			for(int i=0;i<9;i++)
				for(int j=0;j<9;j++)
					(*state)[i][j]=(*state_to_fall_back)[i][j];
			num_steps=0;
			cout<<"Restarting  due to overcount"<<endl;
		}
		prob_num=(float) rand()/RAND_MAX;
		num_steps=num_steps+1;
		neighbour_list =get_sole_element(state);
		if(is_solved(state))
		{
			print(state, 1);
			//********************timing**********************
		    clock_gettime(CLOCK_MONOTONIC, &end); 
		    double time_taken; 
		    time_taken = (end.tv_sec - start.tv_sec) * 1e9; 
		    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9; 
		    cout << "Time taken by program is : " << fixed << time_taken; 
		    cout << " sec" << endl; 
		    //************************************************
		    return 0;
		}
		if(neighbour_list->size()==0)
		{
			for(int i=0;i<9;i++)
				for(int j=0;j<9;j++)
					(*state)[i][j]=(*state_to_fall_back)[i][j];
			num_steps=0;
			// cout<<"No neighbours. Restarting."<<endl;
			continue;
		}
		else if(prob_num<prob_of_best_move)
		{
			// Execute best move
			best_score=get_score((*neighbour_list)[0]);
			best_neighbour =(*neighbour_list)[0];
			for(int i=1;i<neighbour_list->size();i++)
			{
				comparator_variable=get_score((*neighbour_list)[i]);
				if(comparator_variable>=best_score)
				{
					best_score=comparator_variable;
					best_neighbour=(*neighbour_list)[i];
				}
			}
			state=best_neighbour;		
		} 
		else
		{
			random_neighbour=(*neighbour_list)[(int) rand()%(neighbour_list->size())];
			state=random_neighbour;
		}
	}
	//********************timing**********************
    clock_gettime(CLOCK_MONOTONIC, &end); 
    double time_taken; 
    time_taken = (end.tv_sec - start.tv_sec) * 1e9; 
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9; 
    cout << "Time taken by program is : " << fixed << time_taken; 
    cout << " sec" << endl; 
    //************************************************
    print(state, 1);
    return 0;
}


