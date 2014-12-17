#include "StdAfx.h"
#include "ControllerText.h"
#include <iostream>
#include <fstream>

using namespace std;

ControllerText::ControllerText(void)
{
}

ControllerText::~ControllerText(void)
{
}

void ControllerText::Init(string StdTextFileName)
{
  LoadStdText(StdTextFileName);
}

void ControllerText::LoadStdText(string StdTextFileName)
{
	string s;
	char *tok;
	char *line;
	int index=0;

	ifstream file(StdTextFileName.c_str() ,ios::in|ios::binary);
	if (file.is_open())
	{
    // Skip header
		getline(file,s);

		while (getline(file, s))
		{
			//Ref
			line = (char *)s.c_str();
			tok = strtok(line,",");
//			cout << "Index: " << index << endl;
//			cout << tok;
	
			//Label
			tok = strtok(NULL,",");
			//Description
			tok = strtok(NULL,",");
			//Translate
			tok = strtok(NULL,",");
			//MaxChars
			tok = strtok(NULL,",");
			//English   ... ,German,Spanish,French,Italian,Netherland,Danish,Swedish,Chinese,Polish,Japanese,Korean,Portuguese,Slovak,Russian,Hungarian,Bulgarian,Romanian,Czech,Turkish,Finnish,Greek,T-Chinese,Serbian,Slovenian,Thai,Polish UTF8
			tok = strtok(NULL,",");
			StdStringListEnglish.push_back(string(tok));
			tok = strtok(NULL,",");
			StdStringListGerman.push_back(string(tok));
//			tok = strtok(NULL,",");
//			StdStringListSpanish.push_back(string(tok));
//			tok = strtok(NULL,",");
//			StdStringListFrench.push_back(string(tok));
//			tok = strtok(NULL,",");
//			StdStringListItalian.push_back(string(tok));
		}
		cout << "TextStd properly imported" << endl;

	}
	else
	{
		cout << "Fail to open the " << StdTextFileName << " file" << endl;
	}
}

string ControllerText::GetStdText(unsigned int TextNumber)
{
	if (StdStringListEnglish.size() > TextNumber)
	{
		return	StdStringListEnglish[TextNumber];
	}
	else
	{
		return "UNKNOWN STRING";	
	}
}