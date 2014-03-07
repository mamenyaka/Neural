/*
 * test.cpp
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
 */

#include <string>
#include <iostream>
#include <list>
#include <math.h>
#include <iterator>
#include <algorithm>
#include <boost/algorithm/string.hpp>

void kiir(std::list<std::string> list, std::ostream& out)
{
  std::copy(list.begin(), list.end(), std::ostream_iterator<std::string>(out, " "));
  out << "\n";
}

void beolvas(std::list<std::string>& list)
{
  std::string line;

  std::getline(std::cin, line);
  boost::algorithm::to_lower(line);

  boost::algorithm::split(list, line, boost::algorithm::is_any_of(";,.!? "), boost::algorithm::token_compress_on);
  list.sort();
  list.pop_front();
}

int minimum(int a, int b, int c)
{
  return fmin(fmin(a, b),c);
}

int LevenshteinDistance(std::string s, int len_s, std::string t, int len_t)
{
  /* base case: empty strings */
  if (len_s == 0) return len_t;
  if (len_t == 0) return len_s;

  /* test if last characters of the strings match */
  int cost;
  if (s[len_s-1] == t[len_t-1]) cost = 0;
  else                          cost = 1;

  /*return minimum of delete char from s, delete char from t, and delete char from both*/
  return minimum(LevenshteinDistance(s, len_s - 1, t, len_t    ) + 1,
                 LevenshteinDistance(s, len_s    , t, len_t - 1) + 1,
                 LevenshteinDistance(s, len_s - 1, t, len_t - 1) + cost);
}
int main()
{
  std::list<std::string> a, b;
  std::string spam ("rikkant");

  beolvas(a);
  beolvas(b);

  kiir(a, std::cout);
  kiir(b, std::cout);
  std::cout << "\n";

for (auto item : a)
    std::cout << "\"" << item << "\" " << LevenshteinDistance(item, item.length(), spam, spam.length()) << "\n";
  std::cout << "\n";

for (auto item : b)
    std::cout << "\"" << item << "\" " << LevenshteinDistance(item, item.length(), spam, spam.length()) << "\n";

  return 0;
}