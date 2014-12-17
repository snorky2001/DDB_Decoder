#pragma once

#include <string>
#include <vector>
using namespace std;

class ControllerText
{
public:
  ControllerText(void);
  ~ControllerText(void);
  void Init(string StdTextFileName);
  string GetStdText(unsigned int TextNumber);

private:
  vector<string> StdStringListEnglish;
  vector<string> StdStringListGerman;
  vector<string> StdStringListSpanish;
  vector<string> StdStringListFrench;
  vector<string> StdStringListItalian;

  void LoadStdText(string StdTextFileName);
};
