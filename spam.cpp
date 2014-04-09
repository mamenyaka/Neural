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

void Spam::get_input(std::string input)
{
  std::ifstream be(input);
  if (!be)
    throw "\"" + input + "\" file missing!";

  while (1)
  {
    std::pair< std::list<std::string>, std::list<float> > m;
    int s = read(m.first, be);

    get_params(m.first, m.second);

    if (s == 0)
      normal.push_back(m);
    else if (s == 1)
      spam.push_back(m);
    else if (s == -1)
      break;
    else
      throw "Input error!";
  }
  
  std::cerr << normal.size() << " normal and " << spam.size() << " spam messages\n";
  
  it_normal = normal.begin();
  it_spam = spam.begin();

  be.close();
}


/*
 * copy params from list to array and set output
 * type = 0 -> NORMAL
 * type = 1 -> SPAM
 * 
 * Return value: Number of items processed in "type" list
 */
int Spam::copy_train(double *input, double *output, int type)
{
  if (std::distance(normal.end(), it_normal) == 0 || 
    std::distance(spam.end(), it_spam) == 0)
    return -1;

  int i = 0;
  std::list<float> &list = (type == 0) ? it_normal->second : it_spam->second;
  for (auto p : list)
    input[i++] = p;

  output[0] = (double) type;

  if (type == 0)
  {
    it_normal++;
    return std::distance(normal.begin(), it_normal);
  }
  else
  {
    it_spam++;
    return std::distance(spam.begin(), it_spam);
  }
}

int Spam::copy_test(double *input)
{
  int type;

  if (std::distance(normal.end(), it_normal) > 0)
    type = 0;
  else if (std::distance(spam.end(), it_spam) > 0)
    type = 1;
  else
    return -1;
  
  if (type == 0)
  {
    std::cout << "==============NORMAL==============\n";
    print(it_normal->first, "\n");
  }
  else
  {
    std::cout << "==============SPAM==============\n";
    print(it_spam->first, "\n");
  }
  
  std::list<float> &list = (type == 0) ? it_normal->second : it_spam->second;

  int i = 0;  
  for (auto p : list)
    input[i++] = p;
  
  if (type == 0)
  {
    it_normal++;
    return std::distance(normal.begin(), it_normal);
  }
  else
  {
    it_spam++;
    return std::distance(spam.begin(), it_spam);
  }
  
  return type;
}

template<typename T>
void Spam::print(const std::list<T>& list, const char *s)
{
  std::copy(list.begin(), list.end(), std::ostream_iterator<T>(std::cout, s));
  std::cout << "\n";
}

int Spam::read(std::list<std::string>& list, std::ifstream& be)
{
  std::string line;
  bool mail = false;
  bool spam;

  while (!mail)
  {
    if (be.eof())
      return -1;

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
      return 42;

    std::getline(be, line);

    if (!line.compare("END"))
      mail = false;
    else
      list.push_back(line);
  }

  return spam;
}

int Spam::is_punct(char c)
{
  return (c == '.' || c == '?' || c == '!');
}

int Spam::get_Nsent(const std::list<std::string>& list)
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

void Spam::get_params(const std::list<std::string>& list, std::list<float>& params)
{
  int Nchars = 0, Nalpha = 0, Ndigit = 0, Nwhitespace = 0, Nother = 0, Npunct = 0;
  int Nwords = 0, Nshort_words = 0, Nsent = get_Nsent(list);

  for (auto s : list)
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

    std::list<std::string> words;
    boost::algorithm::split(words, s, boost::algorithm::is_any_of(";,.!? "), boost::algorithm::token_compress_on);
    words.pop_back();
    Nwords += words.size();

    for (auto w : words)
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
