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

#include <string>
#include <cctype>
#include <iostream>
#include <list>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <boost/algorithm/string.hpp>

template<typename T>
void kiir(const std::list<T> list, const char *s)
{
  std::copy(list.begin(), list.end(), std::ostream_iterator<T>(std::cout, s));
  std::cout << "\n";
}

void beolvas(std::list<std::string>& list)
{
  std::string line;

  while (std::getline(std::cin, line))
    list.push_back(line);
}

int is_punct(char c)
{
  return (c == '.' || c == '?' || c == '!');
}

int get_Nsent(const std::list<std::string> list)
{
  int Nsent = 0;
  bool end_of_sentence = false;

  for (std::string s : list)
  {
    for (char c : s)
    {
      if (isblank(c) || c == '\n')
      {
        if (end_of_sentence)
        {
          end_of_sentence = false;
          Nsent++;
        }
      }
      else if (is_punct(c))
      {
        if (!end_of_sentence)
          end_of_sentence = true;
      }
    }
  }

  if (end_of_sentence)
    Nsent++;

  return Nsent;
}

void get_params(const std::list<std::string> list, std::list<float>& params)
{
  int Nchars = 0, Nalpha = 0, Ndigit = 0, Nwhitespace = 0, Nother = 0, Npunct = 0;
  int Nwords = 0, Nshort_words = 0, Nsent = get_Nsent(list);

  for (std::string s : list)
  {
    Nchars += s.length() + 1;

    for (char c : s)
    {
      if (isalpha(c))
        Nalpha++;
      else if (isdigit(c))
        Ndigit++;
      else if (isblank(c))
        Nwhitespace++;
      else if (is_punct(c))
        Npunct++;
      else
        Nother++;
    }

    std::list<std::string> words;
    boost::algorithm::split(words, s, boost::algorithm::is_any_of(";,.!? "), boost::algorithm::token_compress_on);
    words.pop_back();
    Nwords += words.size();

    for (std::string w : words)
      if (w.length() <= 2)
        Nshort_words++;
  }

  params.push_back(Nchars);
  params.push_back((float) Nalpha/Nchars);
  params.push_back((float) Ndigit/Nchars);
  params.push_back((float) Nwhitespace/Nchars);
  params.push_back((float) Npunct/Nchars);
  params.push_back((float) Nother/Nchars);
  params.push_back(Nwords);
  params.push_back(Nshort_words);
  params.push_back((float) Nalpha/Nwords);  // avg. word length
  params.push_back((float) Nchars/Nsent);  // avg. sentence length in chars
  params.push_back((float) Nwords/Nsent);  // avg. sentence length in words
}

int main()
{
  std::list<std::string> mail;
  std::list<float> params;

  beolvas(mail);
  get_params(mail, params);

  kiir(mail, "\n");
  kiir(params, ", ");

  return 0;
}
