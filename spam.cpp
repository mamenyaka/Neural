/*
 * spam.cpp
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

#include "spam.hpp"

Spam::Spam() {}

Spam::~Spam() {}

int Spam::get_input(const std::string input)
{
  std::ifstream be(input);

  if (!be)
    throw (std::string) ("\"" + input + "\" file missing!");

  int db[2] = {0, 0};
  while (1)
  {
    int t;
    Message m;

    try
    {
      t = read(m.first, be);
    }
    catch (std::string ex)
    {
      be.close();

      if (!ex.compare("eof"))
        break;
      else
        throw (std::string) ("Input error! " + ex);
    }

    db[t]++;
    get_params(m);
    mails.push_back(std::make_pair(t, m));
  }
  
  normalise();

  std::cerr << db[0] << " normal and " << db[1] << " spam messages\n";

  return mails.size();
}

int Spam::copy(double *input, const int n)
{
  if (n > mails.size())
    throw (std::string) "Out of range";

  int i = 0;
  for (auto p : mails.at(n).second.second)
    input[i++] = p;

  return mails.at(n).first;
}

void Spam::print_message(const int n)
{
  print(mails.at(n).second.first, "\n");
}

void Spam::print_params(const int n)
{
  print(mails.at(n).second.second, ", ");
}

template<typename T>
void Spam::print(const std::vector<T>& list, const char *s)
{
  std::copy(list.begin(), list.end(), std::ostream_iterator<T>(std::cout, s));
  std::cout << "\n";
}

int Spam::read(std::vector<std::string>& m, std::ifstream& be)
{
  std::string line;
  bool mail = false;
  int spam;

  while (!mail)
  {
    if (be.eof())
      throw (std::string) "eof";

    std::getline(be, line);

    if (!line.compare("BEGIN NORMAL"))
    {
      mail = true;
      spam = 0;
    }
    else if (!line.compare("BEGIN SPAM"))
    {
      mail = true;
      spam = 1;
    }
  }

  while (mail)
  {
    if (be.eof())
      throw (std::string) "Unexpected end of file";

    std::getline(be, line);

    if (!line.compare("END"))
      mail = false;
    else
      m.push_back(line);
  }

  return spam;
}

void Spam::get_params(Message& m)
{
  int Nchars = 0, Nalpha = 0, Ndigit = 0, Nwhitespace = 0, Nother = 0, Npunct = 0;
  int Nwords = 0, Nshort_words = 0, Nsent = get_Nsent(m.first);

  for (auto s : m.first)
  {
    Nchars += s.length() + 1;

    for (auto c : s)
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

    std::vector<std::string> words;
    boost::algorithm::split(words, s, boost::algorithm::is_any_of(";,.!? "), boost::algorithm::token_compress_on);
    words.pop_back();
    Nwords += words.size();

    for (auto w : words)
      if (w.length() <= 2)
        Nshort_words++;
  }

  m.second.push_back((double) Nalpha/Nchars);
  m.second.push_back((double) Ndigit/Nchars);
  m.second.push_back((double) Nwhitespace/Nchars);
  m.second.push_back((double) Npunct/Nchars);
  m.second.push_back((double) Nother/Nchars);
  // need to be normalised
  m.second.push_back((double) Nchars);
  m.second.push_back((double) Nwords);
  m.second.push_back((double) Nshort_words);
  m.second.push_back((double) Nalpha/Nwords);  // avg. word length
  m.second.push_back((double) Nchars/Nsent);  // avg. sentence length in chars
  m.second.push_back((double) Nwords/Nsent);  // avg. sentence length in words
}

void Spam::normalise()
{
  for (int i = 0; i < Nparams; i++)
  {
    double max = 0.0, min = 1000000.0;

    for (auto m : mails)
    {
      double p = m.second.second.at(i);
    
      if (p > max)
        max = p;
      
      if (p < min)
        min = p;
    }
    
    if (max < 1.0)
      continue;
    
    for (auto& m : mails)
    {
      m.second.second.at(i) = (m.second.second.at(i) - min)/(max - min);
    }
  }
}

int Spam::is_punct(const char c) const
{
  return (c == '.' || c == '?' || c == '!');
}

int Spam::get_Nsent(const std::vector<std::string>& list) const
{
  int Nsent = 0;
  bool end_of_sentence = false;

  for (auto s : list)
  {
    for (auto c : s)
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
