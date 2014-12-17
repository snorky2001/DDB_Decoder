#include "StdAfx.h"
#include "DD.h"

#include <iostream>
#include <fstream>

using namespace std;

DD::DD(void)
{
  DDData = NULL;
  generalInformationList.MeasurementTagNumbers = NULL;
  tags = NULL;
}

DD::~DD(void)
{
  if (tags != NULL)
  {
    delete[] tags;
  }

  if (generalInformationList.MeasurementTagNumbers != NULL)
  {
    delete[] generalInformationList.MeasurementTagNumbers;
  }

  if (DDData != NULL)
  {
    delete[] DDData;
  }
}

void DD::ProceedGeneralInformation()
{
  unsigned int measurementTagPos = (unsigned int)&generalInformationList.MeasurementTagNumbers - (unsigned int)&generalInformationList;
  unsigned int remainingSize = sizeof(generalInformationList) - sizeof(generalInformationList.MeasurementTagNumbers) - measurementTagPos;
  
  memcpy(&generalInformationList, DDData, measurementTagPos);
  generalInformationList.MeasurementTagNumbers = new unsigned short[generalInformationList.NumberOfMeasurement];
  memcpy(generalInformationList.MeasurementTagNumbers, &DDData[measurementTagPos], generalInformationList.NumberOfMeasurement * sizeof(unsigned short));
  memcpy(&generalInformationList.DeviceNameTextNumber, &DDData[measurementTagPos + generalInformationList.NumberOfMeasurement * sizeof(unsigned short)], remainingSize);
}

void DD::ProceedTagList()
{
	unsigned int tagCount;
	unsigned short usData=0;

	tagCount = (generalInformationList.SelectionListPtr - generalInformationList.TagPtr)/sizeof(TagList);

  tags = new TagList[tagCount];
  
  memcpy(tags, DDData + generalInformationList.TagPtr, sizeof(TagList)*tagCount);
}

void DD::ProceedText()
{
  unsigned short LangCount;
  unsigned short TextCount;
  unsigned short TxtPointer;
  unsigned int pos;
  char* strPos;


  pos = (unsigned int)DDData + generalInformationList.TextPtr;
  memcpy(&LangCount, (void *)pos, sizeof(LangCount));
  pos += (1+LangCount)*sizeof(unsigned short);
  TextCount = 135;
  for (int i=0; i<TextCount ;i++)
  {
    memcpy(&TxtPointer, (void *)pos, sizeof(TxtPointer));
    pos += (LangCount)*sizeof(TxtPointer);
    strPos = (char*)(&DDData[0] + generalInformationList.TextPtr + TxtPointer);
    StringListEnglish.push_back( string(strPos) );
  }
}

void DD::ProceedSelectionList()
{
}

void DD::Init(string DDFilename)
{
  ifstream file(DDFilename.c_str(),ios::in|ios::binary);
	if (file.is_open())
	{
    file.seekg(0, ios::end);
    int filesize = file.tellg();
    DDData = new unsigned char[filesize];
    file.seekg(0, ios::beg);

    file.read((char *)DDData, filesize);

		ProceedGeneralInformation();
    ProceedText();
    ProceedTagList();
    ProceedSelectionList();
	}
  else
  {
    throw invalid_argument("Unable to access file");
  }
}

