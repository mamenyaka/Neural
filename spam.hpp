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
#include <vector>
#include <boost/algorithm/string.hpp>

class Spam
{
public:
  Spam();
  ~Spam();

  /*
   * Reads input message and sets its parameters
   * Return value: Number of messages
   */
  int get_input(const std::string input);

  /*
   * copy params from vector item "n" to array "input"
   * type = 0 -> NORMAL
   * type = 1 -> SPAM
   *
   * Return value: type of message procesed
   */
  int copy(double *input, const int n);

private:
  typedef std::pair< std::list<std::string>, std::list<double> > Message;
  std::vector< std::pair<int, Message> > mails;

  template<typename T>
  void print(const std::list<T>& list, const char *s);

  /*
   * Return value: type of message read
   */
  int read(std::list<std::string>& m, std::ifstream& be);

  void get_params(Message& m);

  int is_punct(const char c) const;
  int get_Nsent(const std::list<std::string>& list) const;
};

#endif  // SPAM_HPP