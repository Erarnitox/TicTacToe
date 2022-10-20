#pragma once

#include <iostream>
#include <vector>
#include <cmath>

constexpr bool DEBUG{ true };
auto sgn(float input) -> float {
	 //return (float)1 / ((float)1 + exp((float)-input));
	 return (float)input/1+std::abs((float)input);
}

template<typename T>
inline
void debugLog(const std::string msg, T value){
	if(DEBUG){
		std::cout << msg << " : " << value << "\n";
	}
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
			auto r_value{rand()};
			debugLog<int>("Random Value", r_value);
			weights[i] = sgn(rand());
			debugLog<float>("init weight", weights[i]);
		}
	}

	void setValue(float value){
		debugLog<float>("- New Value for Node:", sgn(value));
		this->value = sgn(value);
	}

	float getValue() const {
		return this->value;
	}

	void addValue(float value){
		this->value = sgn(this->value + value);
	}

	float getWeightedValue(int weightId) const {
		return this->value * weights[weightId];
	}

	float getWeightById(int id) const {
		return this->weights[id];
	}

	unsigned getWeightCount() const {
		return this->weights.size();
	}

	void setWeight(int id, float weight){
		this->weights[id] = weight;
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

	void kill(){
	}

	void krueppel_copy(const NeuronalNet& other){
		for(unsigned i{ 0 }; i < this->layer.size(); ++i){
			for(unsigned j { 0 }; this->layer[i].size(); ++j){
				this->layer[i][j].setValue(other.layer[i][j].getValue());
				auto rnd{ rand()%6 };
				if(rnd > 4){
					this->layer[i][j].addValue((rnd-5)*0.001f);
				}

				for(unsigned k { 0 }; this->layer[i][j].getWeightCount(); ++k){
					this->layer[i][j].setWeight(k, other.layer[i][j].getWeightById(k));
					auto rnd{ rand()%10 };
					if(rnd > 8){
						auto curr_weight{ this->layer[i][j].getWeightById(k) };
						curr_weight += (rnd-9)*0.001f;
						this->layer[i][j].setWeight(k, curr_weight);
					}
				}
			}
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
		auto max{ 0.0f };
		for(auto i{ 0 }; i < 3; ++i){
			for(auto j{ 0 }; j < 3; ++j){
				debugLog<float>("res value", result[i*3+j]);
				if(result[i*3+j] > max){
					debugLog<float>("New Max in prediction", max);
					max = result[i*3+j];
					turn = std::make_pair(i, j);
				}
			}
		}

		return turn;
	}
};
