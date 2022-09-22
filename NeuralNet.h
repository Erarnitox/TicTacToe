#pragma once

#include <iostream>
#include <vector>

template <typename T> 
T sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

class Neuron{
	using vec = std::vector<float>;
	float value;
	vec weights;

	public:
	Neuron():
		value{ 0 },
		weights{ vec(9) }
	{
		//initialize weights
		for(unsigned i{ 0 }; i < 9; ++i){
			weights[i] = sgn(rand());
		}
	}

	void setValue(float value){
		this->value = sgn(value);
	}

	float getValue(){
		return this->value;
	}

	void addValue(float value){
		this->value = sgn(this->value + value);
	}

	float getWeightedValue(int weightId){
		return this->value * weights[weightId];
	}

};

class NeuronalNet{
	std::vector<std::vector<Neuron>> layer;

public:
	NeuronalNet():
		layer{ std::vector<std::vector<Neuron>>(4) }
	{
		for(unsigned i{ 0 }; i < layer.size(); ++i){
			layer[i] = std::vector<Neuron>(9);
		}
	}


	std::vector<float> feedForward(const std::vector<float> input){
		auto output{ std::vector<float>(9) };

		//reset neural network
		for(unsigned lay{ 0 }; lay < layer.size(); ++lay){
			for(unsigned neu{ 0 }; neu < layer[lay].size(); ++neu){
				layer[lay][neu].setValue(0);
			}
		}
		
		//get input into neuronal network
		{
			auto i{ 0 };
			for(auto val : input){
				layer[0][i].setValue(val);
				
				if(i > 8){
					std::cerr << "Too many elements!" << std::endl;
					throw "Too many elements!";
				}
				++i;
			}
		}
		
		//feed forward
		for(unsigned lay{ 0 }; lay < (layer.size()-1); ++lay){
			for(unsigned neu{ 0 }; neu < layer[lay].size(); ++neu){
				for(unsigned wei{ 0 }; wei < layer[lay+1].size(); ++wei){
					auto cNeuron{ layer[lay][neu] };
					layer[lay+1][wei].addValue(cNeuron.getWeightedValue(wei)); 
				}
			}
		}
			
		//get values from output layer
		for(unsigned i{ 0 }; i < layer[layer.size()-1].size(); ++i){
			output[i] = layer[layer.size()-1][i].getValue();
		}

		return output;
	};


	std::pair<int, int> predict(const std::vector<std::vector<char>>& mat){
		auto input{ std::vector<float>(9) };
	
		auto i{ 0 };
		for(auto row : mat){
			for(auto el : row){
				input[i] = (el*0.1f);
				++i;
			}
		}

		auto result = this->feedForward(input);
		std::pair<int, int> turn;
		auto max{ 0 };
		for(auto i{ 0 }; i < 3; ++i){
			for(auto j{ 0 }; j < 3; ++j){
				if(result[i*3+j] > max){
					max = result[i*3+j];
					turn = std::make_pair(i, j);
				}
			}
		}

		return turn;
	}
};
