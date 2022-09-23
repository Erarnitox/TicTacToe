#include <cstdint>															 //includes the int types of a specific size
#include <vector>															 //container: continuous in memory
#include <algorithm>														
#include <cstdio>
#include <iostream>

#include "NeuralNet.h"

enum Modus{ UNDEFINED=0, NORMAL=1, AI_GAME, AI_TRAINING };

int main(){
	auto matrix{ std::vector<std::vector<char>>(3) };                        //this is our playing field

	for(int i{0}; i < 3; ++i){											     //loop over each row to initialize it
		matrix[i] = std::vector<char>(3);
		for(int j{0}; j<3; ++j){											 //loop over each entry in the row
			matrix[i][j] = ' ';
		}
	}

	auto choice{ 0 };
	std::cout << "Wie willst du spielen:\n"
				 "1) Lokal gegen einen Freund\n"
				 "2) Gegen eine KI\n"
				 "3) KI trainiern\n"
				 "Auswahl: "
	<< std::endl;
	std::cin >> choice;

	Modus modus = (Modus)choice;

	//game loop
	auto end{ false };
	auto p1_turn{ true };
	auto row{ 0 };
	auto col{ 0 };
	auto won{ 0 };

	NeuronalNet ai;

	while(!end){
		
	if(modus != AI_TRAINING){
		//display field
		std::cout << "\n-------" << std::endl;
		for(auto row : matrix){
			std::cout << "|";
			for(auto el : row){
				std::cout << el << "|"; 
			}
			std::cout << "\n-------" << std::endl;
		}

		//player input:
		//---------------------------------------------------------
		do {
			if(modus == NORMAL || (modus==AI_GAME && p1_turn)){
				std::cout << "Player" << (p1_turn?"1":"2") << "\nZeile: ";
				std::cin >> row;
				std::cout << "\nSpalte: ";
				std::cin >> col;
			}else{
				auto turn{ ai.predict(matrix) };
				row = std::get<0>(turn);
				col = std::get<1>(turn);
			}
		} while(matrix[row][col] != ' ');

		matrix[row][col] = p1_turn?'O':'X';
	}


		//check if game is over:
		//---------------------------------------------------------
		//diagonal: "\"
		if(matrix[0][0] == matrix[1][1]&& 
		   matrix[1][1] == matrix[2][2]){
			if(matrix[0][0] != ' '){
				won = matrix[0][0];
				end = true;
				continue;
			}
		}

		//diagonal: "/"
		if(matrix[0][2] == matrix[1][1]&& 
		   matrix[1][1] == matrix[2][0]){
		    if(matrix[0][2] != ' '){
				won = matrix[0][2];
				end = true;
				continue;
			}
		}
	
		//cols:
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

		//rows:
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

		//check for tie
		end = true;
		for(unsigned y{ 0 }; y < matrix.size(); ++y){
			for(unsigned x{ 0 }; x < matrix[y].size(); ++x){
				if(matrix[y][x] == ' '){
					end = false;
					won = -1;
					break;
				}
			}
			if(!end) break;
		}
		//-------------------------------------------------------

		p1_turn = !p1_turn;
	}

	if(won == -1){
		std::cout << "It's a tie!" << std::endl;
		return 0;
	}

	//who has won
	std::cout << "Player" << (won==1?"1":"2") << " has won!\n" << std::endl;

	return 0;
}
