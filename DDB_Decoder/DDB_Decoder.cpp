// DDB_Decoder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

#include "ControllerText.h"
#include "DD.h"

using namespace std;

unsigned short TagPtr=0;
unsigned short SelPtr=0;
unsigned short TextPtr=0;
unsigned short UnitPtr=0;

ControllerText StdTxt;
DD deviceDriver;

void SwapWord(unsigned short *val)
{
	*val = ((*val & 0xFF) << 8) | ((*val & 0xFF00) >> 8);
}

void ProceedGeneralInformation(ifstream &file)
{
	unsigned short usData=0;
	unsigned char ucData=0;
	unsigned short MeasNumber=0;

	file.seekg(0, ios::beg);
	file.read((char *)&usData, sizeof(usData));
	cout << "Filetype: " << usData << endl;

	file.seekg(2*2, ios::beg);
	file.read((char *)&ucData, sizeof(ucData));
	cout << "Structure version: " << (int)ucData << endl;
	file.read((char *)&ucData, sizeof(ucData));
	cout << "Firmware Structure version: " << (int)ucData << endl;
	file.read((char *)&ucData, sizeof(ucData));
	cout << "Content Modified version: " << (int)ucData << endl;
	file.read((char *)&ucData, sizeof(ucData));
	cout << "Manufacturer ID: " << (int)ucData << endl;
	file.read((char *)&usData, sizeof(usData));
	cout << "Instrument ID: " << usData << endl;
	file.read((char *)&TagPtr, sizeof(TagPtr));
  cout << "TagPtr: " << hex << TagPtr << endl;
	file.read((char *)&SelPtr, sizeof(SelPtr));
	cout << "SelectionListPtr: " << hex << SelPtr << endl;

	file.seekg(8*2, ios::beg);
	file.read((char *)&TextPtr, sizeof(TextPtr));
	cout << "TextPtr: " << hex << TextPtr << endl;
	file.read((char *)&UnitPtr, sizeof(UnitPtr));
	cout << "UnitPtr: " << hex << UnitPtr << endl;
	
	file.seekg(28*2, ios::beg);
	file.read((char *)&usData, sizeof(usData));
	cout << "Number of measurement: " << usData << endl;
  unsigned char count = usData;
	for (int i=0;i<count;i++)
	{
		file.read((char *)&usData, sizeof(usData));
		cout << "Measurement " << i << " : " << usData << endl;
	}
}


string GetText(ifstream &file, unsigned int TextNumber)
{
	unsigned short usData=0;
	unsigned short nbLang=0;
	char str[100];

	if (TextNumber == 0xFFFF)
	{
		return "<none>";
	}
	else if (TextNumber > 0x8000)
	{
		string result = StdTxt.GetStdText(TextNumber-0x8000);
		result = _T("Controller: ") + result;
		return result.c_str();
	}
	else
	{
		file.seekg(TextPtr, ios::beg);
		file.read((char *)&nbLang, sizeof(nbLang));
    // Get Txt pointer
		file.seekg(TextPtr+(nbLang+1)*2+(2*nbLang*(TextNumber)), ios::beg);
		file.read((char *)&usData, sizeof(usData));
    // Get Txt
		file.seekg(TextPtr+usData, ios::beg);
		for (int i=0;i<100;i++)
		{
			file.read(str+i,1);
			if (str[i]== 0)
				break;
		}
		
		string strRes(str);
		strRes = _T("DD: ") + strRes;

		return strRes.c_str();
	}

}

string IDDToString(unsigned short IDD)
{
  string IDDTable[] = {"0-Float", "1-Unsigned integer", "2-Integer", "3-Selection List", "4-Text String", "5-Time",
                       "6-Units String (Not available for tags)", "7-Parameter String (Not available for tags)", "8-Date", "9-Time and Date"};
  if (IDD >= 10)
  {
    return "Unknown type";
  }
  else
  {
    return IDDTable[IDD];
  }
}

void ProceedTagList(ifstream &file)
{
	int tagCount;
	unsigned short usData=0;

	tagCount = (SelPtr - TagPtr)/22;

	for (int i=0; i<tagCount; i++)
	{
		file.seekg(TagPtr+i*11*2, ios::beg);
		file.read((char *)&usData, sizeof(usData));
		cout << "Reg: " << dec << 40001+usData << endl;

    file.read((char *)&usData, sizeof(usData));
    cout << "Type: " << IDDToString( usData >> 11) << endl;

		file.seekg(4, ios::cur);
		file.read((char *)&usData, sizeof(usData));
		cout << "Name: " << hex << usData << " => " << GetText(file,usData) << endl << endl;
	}

  cout << endl;
}

void ProceedSelectionList(ifstream &file)
{
	unsigned short ListCount;
	unsigned short NbElements;
	unsigned short usData=0;
	unsigned short usTxt=0;
	unsigned short usElementSize;
  streampos currentPos;

	file.seekg(SelPtr, ios::beg);
	file.read((char *)&ListCount, sizeof(ListCount));
  // Get Txt pointer
  file.seekg(ListCount*2, ios::cur);

  for (int i=0; i<ListCount; i++)
	{
    file.seekg(SelPtr+2+(i*2), ios::beg);
		file.read((char *)&usData, sizeof(usData));
    file.seekg(SelPtr+usData, ios::beg);
		file.read((char *)&usData, sizeof(usData));
    cout << "Type: " << IDDToString( usData >> 8) << endl;
    usElementSize = usData & 0xFF;
    cout << "Data size (word): " << usElementSize << endl;
		file.read((char *)&NbElements, sizeof(NbElements));
    cout << "Number of element: " << dec << NbElements << endl;
		
    for (int j=0; j<NbElements; j++)
    {
      // Get enum val
		  file.read((char *)&usData, sizeof(usData));
		  file.read((char *)&usTxt, sizeof(usTxt));
      currentPos = file.tellg();
		  cout << "Value: " << usData << " / " << GetText(file,usTxt) << endl;
      file.seekg(currentPos, ios::beg);
    }
    cout << endl;
	}
  cout << endl;
}

void ProceedUnitList(ifstream &file)
{
	unsigned short unitCount;
	unsigned short usData=0;
  unsigned char txt[7];
  file.seekg(UnitPtr, ios::beg);
  file.read((char *)&unitCount, sizeof(unitCount));

	for (int i=0; i<unitCount; i++)
	{
		file.seekg(UnitPtr+2+i*8, ios::beg);
		file.read((char *)&usData, sizeof(usData));
		file.read((char *)&txt, 6);
    txt[6] = 0;
		cout << "Unit: " << dec << usData << " / " << txt << endl;
	}
  cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	string	DDBFilename = (char *)(argv[1]);
  string  StdTextFilename;

  if (argc < 2)
  {
    cout << "Missing filename" << endl;
    cout << "Syntax: " << argv[0] << " <DDB binary file> [<csv controller text file>]" << endl;
    return 1;
  }

	DDBFilename = (char *)(argv[1]);

  if (argc > 2)
  {
    StdTextFilename = (char *)(argv[2]);
  }
  else
  {
    StdTextFilename = "TextStd.csv";
  }

  StdTxt.Init(StdTextFilename);

  deviceDriver.Init(DDBFilename);

	ifstream file(DDBFilename.c_str(),ios::in|ios::binary);
	if (file.is_open())
	{
    cout << "General informations" << endl;
		ProceedGeneralInformation(file);
    cout << "Tag List" << endl;
		ProceedTagList(file);
    cout << "Selection List" << endl;
    ProceedSelectionList(file);
    cout << "Unit List" << endl;
    ProceedUnitList(file);
	}
	else
	{
		cout << "Fail to open the file" << endl;
	}
	return 0;
}

