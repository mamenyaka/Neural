/*
 * neural.cpp
 *
 * Written by: Paras Chopra
 * Email: paras1987@gmail.com
 * Web: www.paraschopra.com
 * Comment: Use this code as you like, but please give me credit whenever I deserve it.
 *
 * Improved version by András Mamenyák, Roland Bamli
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Changelog
 * - fixed build errors
 * - fixed segfault errors
 * - cleaner code
 * - improved runtime: 25% faster
 * - test the Barker 11 code
 */

#include "neural.hpp"

Dendrite::Dendrite(double weight = 0.0, int points_to = 0) : weight(weight), points_to(points_to) {}

Dendrite::~Dendrite() {}

Neuron::Neuron(const int id = 0, const double value = 0.0, const double bias = 0.0) : id(id), value(value), bias(bias)
{
  delta = 0.0;
}

Neuron::~Neuron()
{
  delete[] dendrite;
}

/*
 * Set the dendrite from the neuron to given dendrite
 */
void Neuron::set_dendrite(const int n)
{
  dendrite = new Dendrite[n];

  for (int i = 0; i < n; i++)  // Initialize the dendrite to attach to next layer
    dendrite[i].points_to = i;
}

Layer::Layer() {}

Layer::~Layer()
{
  delete[] neuron;
}

void Layer::initialize(const int size)
{
  neuron = new Neuron[size];
}

Neuron Layer::get_neuron(const int index) const
{
  return neuron[index];
}

void Layer::set_neuron(Neuron neuron, const int index)
{
  this->neuron[index] = neuron;
}

Network::Network()
{
  srand(time(NULL));
}

Network::~Network() {}

void Network::set_data(const double learning_rate, const int *layer)
{
  this->learning_rate = learning_rate;

  for (int i = 0; i < 3; i++)
  {
    neuron_per_layer[i] = layer[i];
    this->layer[i].initialize(layer[i]);  // Initialize each layer with the specified size
  }

  randomize();
}

void Network::train(const double *input, const double *output)
{
  double Actual, Delta;

  for (int i = 0; i < neuron_per_layer[0]; i++)
    layer[0].neuron[i].value = input[i];

  update_output();

  for (int i = 2; i > 0; i--)  // Go from last layer to first layer
    for (int j = 0; j < neuron_per_layer[i]; j++)
    {
      Actual = layer[i].neuron[j].value;  // Actual value

      if (i == 2)  // Output layer
      {
        Delta = (output[j] - Actual) * Actual * (1.0 - Actual);  // Function to compute error
        layer[i].neuron[j].delta = Delta;
      }
      else  // Hidden layer
      {
        Delta = Actual * (1.0 - Actual) * sum_weight_delta(i);
      }

      if (i > 0)  // Input layer does not have a bias
        layer[i].neuron[j].bias += Delta*learning_rate;

      for (int k = 0; k < neuron_per_layer[i-1]; k++)  // Calculate the new weights
        layer[i-1].neuron[k].dendrite[j].weight += Delta*learning_rate*layer[i-1].neuron[k].value;
    }
}

void Network::test(const double *input, double *output)
{
  for (int i = 0; i < neuron_per_layer[0]; i++)
    layer[0].neuron[i].value = input[i];

  update_output();

  for (int i = 0; i < neuron_per_layer[2]; i++)
    output[i] = layer[2].neuron[i].value;
}

void Network::randomize()
{
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < neuron_per_layer[i]; j++)
    {
      if (i < 2)  // Last layer does not require weights
      {
        layer[i].neuron[j].set_dendrite(neuron_per_layer[i+1]); // Initialize the dendites

        for (int k = 0; k < neuron_per_layer[i+1]; k++)
          layer[i].neuron[j].dendrite[k].weight = get_rand(); // Let weight be the random value
      }

      if (i > 0)  // First layer does not need biases
        layer[i].neuron[j].bias = get_rand();
    }
}

void Network::update_output()
{
  for (int i = 1; i < 3; i++)
    for (int j = 0; j < neuron_per_layer[i]; j++)
    {
      for (int k = 0; k < neuron_per_layer[i-1]; k++)  // Multiply and add all the inputs
        layer[i].neuron[j].value += layer[i-1].neuron[k].value*layer[i-1].neuron[k].dendrite[j].weight;

      layer[i].neuron[j].value += layer[i].neuron[j].bias;  // Add bias
      layer[i].neuron[j].value = limiter(layer[i].neuron[j].value);  // Squash that value
    }
}

double Network::limiter(const double x) const
{
  return 1.0/(1.0 + exp(-x));
}

double Network::sum_weight_delta(const int Nlayer) const
{
  double result = 0.0;

  for (int i = 0; i < neuron_per_layer[Nlayer+1]; i++)  // Go through all the neuron in the next layer
    result += layer[Nlayer].neuron[Nlayer].dendrite[i].weight*layer[Nlayer+1].neuron[i].delta;

  return result;
}

double Network::get_rand() const
{
  return -1.0 + ((double) rand()/RAND_MAX)*2.0;
}