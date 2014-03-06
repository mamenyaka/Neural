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

int main()
{
  std::list<std::string> a, b;

  beolvas(a);
  beolvas(b);

  kiir(a, std::cout);
  kiir(b, std::cout);
  std::cout << "\n";

  std::list<std::string>::iterator ita = a.begin(), itb = b.begin();

  while (ita != a.end() && itb != b.end())
  {
    std::cout << *ita << " - " << *itb << " " << (*ita).compare((*itb)) << "\n";
    ita++;
    itb++;
  }
  
  while (ita != a.end())
  {
    std::cout << *ita << " - null 0" << "\n";
    ita++;
  }
  
  while (itb != b.end())
  {
    std::cout << "null - " << *itb << " 0" << "\n";
    itb++;
  }    

  return 0;
}