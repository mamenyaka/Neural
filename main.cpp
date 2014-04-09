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
  const int Niter = 100000;
  const int Nneuron = 11;
  const int Ntrain = 30;
  const int layer[3] = {Nneuron, Nneuron, 1};  // input, hidden, output

  Network network;
  network.set_data(0.1, layer);

  Spam spam;
  try
  {
    spam.get_input("sample");
  }
  catch (std::string ex)
  {
    std::cerr << ex << "\n";
    return 1;
  }
  
  std::cerr << "Start training.\n\n";

  double train_input[Ntrain][layer[0]];
  double train_output[Ntrain][layer[2]];
  int Nspam = 20, Nnormal = 10;
  
  int i = 0;
  while (spam.copy_train(train_input[i], train_output[i], 0) <= Nnormal)
    i++;
  while (spam.copy_train(train_input[i], train_output[i], 1) <= Nspam)
    i++;
  
  /*
  for (int i = 0; i < Ntrain; i++)
  {
    std::cout << train_output[i][0] << " : ";
    for (int j = 0; j < layer[0]; j++)
      std::cout << train_input[i][j] << " ";
    std::cout << "\n";
  }
  */

  std::cerr << "Number of training iterations: " << Niter;

  for (int i = 0; i < Niter; i++)
    for (int j = 0; j < Ntrain; j++)
      network.train(train_input[j], train_output[j]);

  std::cerr << "\nEnd training.\n";


  std::cerr << "\nStart testing.\n";

  double test_input[layer[0]];
  double test_output[layer[2]];
  
  while (1)
  {
    std::cout << "\n\n";
    int type = spam.copy_test(test_input);
    if (type == -1)
      break;

    for (int j = 0; j < layer[0]; j++)
      std::cout << test_input[j] << " ";
    std::cout << "\n";
    
    network.test(test_input, test_output);

    std::cout << "Output: ";
    for (int j = 0; j < layer[2]; j++)
      std::cout << test_output[j];
    std::cout << "\n";
  }

  std::cerr << "\nEnd testing.\n";

  return 0;
}
