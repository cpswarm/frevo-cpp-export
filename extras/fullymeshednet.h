/*

File: fullymeshednet.h
 
Copyright (C) 2019 FREVO project contributors

Universitaet Klagenfurt licenses this file to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License. You may obtain a
copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License
is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
or implied. See the License for the specific language governing permissions and limitations under
the License.

*/

#ifndef FREVO_FULLY_MESHED_NET_H
#define FREVO_FULLY_MESHED_NET_H

#include <math.h>

#include "representation.h"

namespace frevo
{

enum class ActivationFunction
{
  kTanh,
  kSigmoid,
  kRelu
};

class FullyMeshedNet : Representation
{
public:
  FullyMeshedNet() : input_count_(0), output_count_(0), activation_function_(ActivationFunction::kRelu),
                     iteration_count_(0), weights_(nullptr), biases_(nullptr), random_biases_(nullptr),
                     states_(nullptr), sums_(nullptr), random_(nullptr)
  {
  }

  FullyMeshedNet(int input_count, int output_count, int hidden_count,
                 ActivationFunction activation_function,
                 int iteration_count, float *weights, float *biases,
                 float *random_biases, float *states, float *sums, float (*random)())
  {
    reset(input_count, output_count, hidden_count, activation_function,
          iteration_count, weights, biases, random_biases, states, sums, random);
  }

  void reset(int input_count, int output_count, int hidden_count,
             ActivationFunction activation_function, int iteration_count,
             float *weights, float *biases, float *random_biases, float *states, float *sums, float (*random)())
  {
    input_count_ = input_count;
    output_count_ = output_count;
    hidden_count_ = hidden_count;
    node_count_ = input_count + output_count + hidden_count;
    activation_function_ = activation_function;
    iteration_count_ = iteration_count;
    weights_ = weights;
    biases_ = biases;
    random_biases_ = random_biases;
    states_ = states;
    sums_ = sums;
    random_ = random;
    reset();
  }

  void reset()
  {
    for (int i = 0; i < node_count_; i++)
    {
      states_[i] = 0;
    }
  }

  void calculate(float *input, float *output)
  {
    // copy in input
    for (int i = 0; i < input_count_; i++)
    {
      states_[i] = input[i];
    }

    for (int k = 0; k < iteration_count_; k++)
    {
      // calculate sums
      for (int i = input_count_; i < node_count_; i++)
      {
        float sum = 0;
        for (int j = 0; j < node_count_; j++)
        {
          sum += weights_[i * node_count_ + j] * states_[j];
        }
        sums_[i] = biases_[i] + sum;

        float random_bias = random_biases_[i];
        if (random_bias > 0)
        {
          sums_[i] += uniformRandomReal(-random_bias, random_bias);
        }
      }

      // activate to calculate new state
      for (int i = input_count_; i < node_count_; i++)
      {
        switch (activation_function_)
        {
        case ActivationFunction::kTanh:
          states_[i] = tanhFunction(sums_[i]);
          break;

        case ActivationFunction::kSigmoid:
          states_[i] = sigmoidFunction(sums_[i]);
          break;

        default:
          states_[i] = reluFunction(sums_[i]);
          break;
        }
      }
    }

    // copy out output
    for (int i = 0; i < output_count_; i++)
    {
      output[i] = states_[input_count_ + hidden_count_ + i];
    }
  }

private:
  int input_count_;
  int output_count_;
  int hidden_count_;
  int node_count_;
  ActivationFunction activation_function_;
  int iteration_count_;

  float *weights_;
  float *biases_;
  float *random_biases_;
  float *states_;
  float *sums_;
  float (*random_)();

  float tanhFunction(float x) { return (float)tanh(x); }

  float sigmoidFunction(float x) { return 1.0f / (1.0f + (float)exp(-x)); }

  float reluFunction(float x)
  {
    if (x >= 1)
    {
      return 1;
    }
    else
    {
      if (x <= 0)
      {
        return 0;
      }
      else
      {
        return x;
      }
    }
  }

  float uniformRandomReal(float lower, float upper)
  {
    return lower + random_() * (upper - lower);
  }
};

} // namespace frevo

#endif