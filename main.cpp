
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <iostream>

int main(){
	auto matrix{ std::vector<std::vector<char>>(3) };

	for(int i{0}; i < 3; ++i){
		matrix[i] = std::vector<char>(3);
		for(int j{0}; j<3; ++j){
			matrix[i][j] = ' ';
		}
	}

	//game loop
	auto end{ false };
	auto p1_turn{ true };
	auto row{ 0 };
	auto col{ 0 };
	auto won{ 0 };

	while(!end){
		//display field
		std::cout << "\n-------" << std::endl;
		for(auto row : matrix){
			std::cout << "|";
			for(auto el : row){
				std::cout << el << "|"; 
			}
			std::cout << "\n-------" << std::endl;
		}

		//player input
		std::cout << "Player" << (p1_turn?"1":"2") << "\nZeile: ";
		std::cin >> row;
		std::cout << "\nSpalte: ";
		std::cin >> col;

		matrix[row][col] = p1_turn?'O':'X';

		//check if game is over
		//diagonal
		if(matrix[0][0] == matrix[1][1]&& 
		   matrix[1][1] == matrix[2][2]){
			if(matrix[0][0] != ' '){
				won = matrix[0][0];
				end = true;
				continue;
			}
		}


		if(matrix[0][2] == matrix[1][1]&& 
		   matrix[1][1] == matrix[2][0]){
		    if(matrix[0][2] != ' '){
				won = matrix[0][2];
				end = true;
				continue;
			}
		}
	
		//cols
		for(unsigned i{ 0 }; i < matrix.size(); ++i){
			if(matrix[i][0] == matrix[i][1]&& 
		   	   matrix[i][1] == matrix[i][2]){
				if(matrix[i][0] != ' '){
					won = matrix[i][0];
					end = true;
					continue;
				}
			}
		}

		//rows
		for(unsigned i{ 0 }; i < matrix.size(); ++i){
			if(matrix[0][i] == matrix[1][i]&& 
		   	   matrix[1][i] == matrix[2][i]){
				if(matrix[0][i] != ' '){
					won = matrix[0][i];
					end = true;
					continue;
				}
			}
		}

		p1_turn = !p1_turn;
	}

	//who has won
	std::cout << "Player" << (won==1?"1":"2") << " has won!\n" << std::endl;

	return 0;
}
