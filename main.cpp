/*
 * main.cpp
 * András Mamenyák, Roland Bamli
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
 */

#include "neural.hpp"
#include "spam.hpp"

int main()
{
  int Nmessage;
  const int Niter = 100000;
  const int Nneuron = 11;
  const int layer[3] = {Nneuron, Nneuron, 1};  // input, hidden, output

  Network network;
  network.set_data(0.1, layer);

  std::cerr << "Start training.\n";
  Spam train;

  try
  {
    Nmessage = train.get_input("train");
  }
  catch (std::string ex)
  {
    std::cerr << ex << "\n";
    return 1;
  }

  double train_input[Nmessage][layer[0]];
  double train_output[Nmessage][layer[2]];

  try
  {
    for (int i = 0; i < Nmessage; i++)
      train_output[i][0] = (double) train.copy(train_input[i], i);
  }
  catch (std::string ex)
  {
    std::cerr << ex << "\n";
    return 1;
  }

  std::cerr << "Number of training iterations: " << Niter << "\n";

  for (int i = 0; i < Niter; i++)
    for (int j = 0; j < Nmessage; j++)
      network.train(train_input[j], train_output[j]);

  std::cerr << "End training.\n\n";

  // ==============================================================================

  std::cerr << "Start testing.\n";
  Spam test;

  try
  {
    Nmessage = test.get_input("test");
  }
  catch (std::string ex)
  {
    std::cerr << ex << "\n";
    return 1;
  }

  double test_input[Nmessage][layer[0]];
  double test_output[Nmessage][layer[2]];
  int real_output[Nmessage];

  try
  {
    for (int i = 0; i < Nmessage; i++)
      real_output[i] = test.copy(test_input[i], i);
  }
  catch (std::string ex)
  {
    std::cerr << ex << "\n";
    return 1;
  }

  for (int i = 0; i < Nmessage; i++)
    network.test(test_input[i], test_output[i]);

  std::cout << "\n";
  for (int i = 0; i < Nmessage; i++)
  {
    std::cout << "Input: " << (real_output[i] == 0 ? "NORMAL" : "SPAM") << "\n";
    std::cout << "Output: " << test_output[i][0] << "\n\n";
  }

  std::cerr << "End testing.\n";

  return 0;
}
