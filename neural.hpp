/*
 * neural.hpp
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

#ifndef NEURAL_HPP
#define NEURAL_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

class Dendrite
{
public:
  Dendrite(double weight, int points_to);
  ~Dendrite();

  double weight;  // Weight of the neuron
  int points_to;  // The index of the neuron of the next layer to which it points
};

class Neuron
{
public:
  Neuron(const int id, const double value, const double bias);
  ~Neuron();

  void set_dendrite(const int n);

  int id;
  double value, bias, delta;
  Dendrite *dendrite;
};

class Layer
{
public:
  Layer();
  ~Layer();

  void initialize(const int size);
  Neuron get_neuron(const int index) const;
  void set_neuron(Neuron neuron, const int index);

  Neuron *neuron;
};

class Network
{
public:
  Network();
  ~Network();

  /*
   * Set various parameters of the net
   */
  void set_data(const double learning_rate, const int *layer);
  
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
  void train(const double *input, const double *output);
  
  /*
   * The real test
   */
  void test(const double *input, double *output);

private:
  /*
   * Randomize weights and biases
   */
  void randomize();
  
  /*
   * Gives the output of the net
   */
  void update_output();
  
  /*
   * Sigmoid activation function
   */
  double limiter(const double x) const;

  /*
   * Calculate sum of weights * delta. Used in back prop.
   */
  double sum_weight_delta(const int Nlayer) const;
  
  double get_rand() const;

  double learning_rate;
  Layer layer[3];
  int neuron_per_layer[3];
};

#endif  // NEURAL_HPP