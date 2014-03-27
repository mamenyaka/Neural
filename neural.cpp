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

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

class Dendrite
{
public:
  double weight;  // Weight of the neuron
  int points_to;  // The index of the neuron of the next layer to which it points

  Dendrite(double weight = 0.0, int points_to = 0) : weight(weight), points_to(points_to)
  {}
};

class Neuron
{
public:
  Neuron(const int id = 0, const double value = 0.0, const double bias = 0.0) : id(id), value(value), bias(bias)
  {
    delta = 0.0;
  }

  ~Neuron()
  {
    delete[] dendrite;
  }

  /*
   * Set the dendrite from the neuron to given dendrite
   */
  void set_dendrite(const int n)
  {
    dendrite = new Dendrite[n];

    for (int i = 0; i < n; i++)  // Initialize the dendrite to attach to next layer
      dendrite[i].points_to = i;
  }

  int id;
  double value, bias, delta;
  Dendrite *dendrite;
};

class Layer
{
public:
  Layer()
  {}

  ~Layer()
  {
    delete[] neuron;
  }

  void initialize(const int size)
  {
    neuron = new Neuron[size];
  }

  Neuron get_neuron(const int index) const
  {
    return neuron[index];
  }

  void set_neuron(Neuron neuron, const int index)
  {
    this->neuron[index] = neuron;
  }

  Neuron *neuron;
};

class Network
{
public:
  Network()
  {
    srand(time(NULL));
  }

  ~Network()
  {}

  /*
   * Set various parameters of the net
   */
  void set_data(const double learning_rate, const int layer[])
  {
    this->learning_rate = learning_rate;

    for (int i = 0; i < 3; i++)
    {
      neuron_per_layer[i] = layer[i];
      this->layer[i].initialize(layer[i]);  // Initialize each layer with the specified size
    }

    randomize();
  }

  /*
   * The real test
   */
  void test(const double input[], double output[])
  {
    for (int i = 0; i < neuron_per_layer[0]; i++)
      layer[0].neuron[i].value = input[i];

    update_output();

    for (int i = 0; i < neuron_per_layer[2]; i++)
      output[i] = layer[2].neuron[i].value;
  }

  /*
   * The standard backprop learning algorithm
   *
   * For output layer:
   * Delta = (Target - Actual) * Actual * (1 - Actual)
   *
   * For hidden layer:
   * Delta  = Actual * (1 - Actual) * Sum(Weight_from_current_to_next AND Delta_of_next)
   *
   * Weight += LearningRate * Delta * Input
   */
  void train(const double input[], const double output[])
  {
    double Actual, Delta;

    for (int i = 0; i < neuron_per_layer[0]; i++)
      layer[0].neuron[i].value = input[i];

    update_output();

    for (int i = 2; i > 0; i--)  // Go from last layer to first layer
      for (int j = 0; j < neuron_per_layer[i]; j++)
      {
        Actual = layer[i].neuron[j].value;  // Actual value

        if(i == 2)  // Output layer
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

private:
  /*
   * Randomize weights and biases
   */
  void randomize()
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

  /*
   * Gives the output of the net
   */
  void update_output()
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

  /*
   * Sigmoid activation function
   */
  double limiter(const double x) const
  {
    return 1.0/(1.0 + exp(-x));
  }

  double get_rand() const
  {
    return -1.0 + ((double) rand()/RAND_MAX)*2.0;
  }

  /*
   * Calculate sum of weights * delta. Used in back prop.
   */
  double sum_weight_delta(const int Nlayer) const
  {
    double result = 0.0;

    for (int i = 0; i < neuron_per_layer[Nlayer+1]; i++)  // Go through all the neuron in the next layer
      result += layer[Nlayer].neuron[Nlayer].dendrite[i].weight*layer[Nlayer+1].neuron[i].delta;

    return result;
  }

  double learning_rate;
  Layer layer[3];
  int neuron_per_layer[3];
};

int main()
{
  const int Niter = 10000;
  const int Nneuron = 11;  // TODO
  const int Ntrain = 111;  // TODO
  const int Ntest = 1111;  // TODO
  const int layer[3] = {Nneuron, Nneuron, 1};  // input, hidden, output

  Network network;
  network.set_data(0.1, layer);


  std::cout << "Start training.\n\n";

  double train_input[Ntrain][layer[0]];  // TODO
  double train_output[Ntrain][layer[2]];  // TODO

  std::cout << "Number of training iterations: " << Niter;

  for (int i = 0; i < Niter; i++)
    for (int j = 0; j < Ntrain; j++)
      network.train(train_input[j], train_output[j]);

  std::cout << "\nEnd training.\n";


  std::cout << "\nStart testing.\n";

  double test_input[Ntest][layer[0]];  // TODO
  double test_output[layer[2]];  // TODO
  int db = 0;

  for (int i = 0; i < Ntest; i++)
  {
    network.test(test_input[i], test_output);

    if (test_output[0] > 0.5)
    {
      db++;
      std::cout << "\nCase number: " << db << "\n";

      std::cout << "Input: ";
      for (int j = 0; j < layer[0]; j++)
        std::cout << test_input[i][j];
      std::cout << " (" << i+1 << ")\n";

      std::cout << "Output: ";
      for (int j = 0; j < layer[2]; j++)
        std::cout << test_output[j];
      std::cout << "\n";
    }
  }

  std::cout << "\nEnd testing.\n";

  return 0;
}
