/*
 * spam.hpp
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

/*
 * http://stackoverflow.com/questions/770238/neural-networks-for-email-spam-detection
 * Frequency of each letter / C (36 letters of the keyboard – A-Z, 0-9)
 * Word length freq. distribution/M Ratio of words of length n, n between 1 and 15
 * Type Token Ratio No. Of unique Words/ M
 * Hapax Legomena Freq. of once-occurring words
 * Hapax Dislegomena Freq. of twice-occurring words
 * Yule’s K measure
 * Simpson’s D measure
 * Sichel’s S measure
 * Brunet’s W measure
 * Honore’s R measure
 */

#ifndef SPAM_HPP
#define SPAM_HPP

#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <list>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <boost/algorithm/string.hpp>

class Spam
{
public:
  Spam();
  ~Spam();
  void get_input(std::string input);
  int copy_train(double *input, double *output, int type);
  int copy_test(double *input);

private:
  std::list< std::pair< std::list<std::string>, std::list<float> > > normal, spam;
  std::list< std::pair< std::list<std::string>, std::list<float> > >::iterator it_normal, it_spam;

  template<typename T>
  void print(const std::list<T>& list, const char *s);
  int read(std::list<std::string>& list, std::ifstream& be);

  int is_punct(char c);
  int get_Nsent(const std::list<std::string>& list);
  void get_params(const std::list<std::string>& list, std::list<float>& params);
};

#endif  // SPAM_HPP