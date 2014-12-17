#pragma once

#include <string>
#include <vector>
using namespace std;

class DD
{
public:
  DD(void);
  ~DD(void);
  void Init(string DDFilename);
private:

  struct GraphicColorDepthColors {
      unsigned int Blue   :3;
      unsigned int Green  :3; 
      unsigned int Red    :5; 
      unsigned int Grey   :3; 
      unsigned int Dummy  :2; 
  }; 

  union UGraphicColorDepth{
    unsigned short value;
    struct GraphicColorDepthColors colors;
  };

  struct GeneralInformationList {
    unsigned short FileType;
    unsigned short DatabaseLength;
    unsigned char StructureVersion;
    unsigned char SoftwareStructureVersion;
    unsigned char ContentModifiedVersion;
    unsigned char ManufacturerID;
    unsigned short InstrumentID;
    unsigned short TagPtr;
    unsigned short SelectionListPtr;
    unsigned short StatePtr;
    unsigned short TextPtr;
    unsigned short UnitsPtr;
    unsigned short ParmPtr;
    unsigned short FontBitmapPtr;
    unsigned short WarningListPtr;
    unsigned short ErrorListPtr;
    unsigned short ProcessEvetsPtr;
    unsigned short LogEventPtr;
    unsigned short LogChannelPtr;
    unsigned char GraphicFile_IDFileType;
    unsigned char GraphicFile_FileNumber;
    unsigned short ResolutionXTagNumber;
    unsigned short ResolutionYTagNumber;
    unsigned short Graphic_MaxResolutionX;
    unsigned short Graphic_MaxResolutionY;
    unsigned short DepthTagNumber;
//    union UGraphicColorDepth GraphicColorDepth;
    unsigned short GraphicColorDepth;
    unsigned char Log_Ticks;
    unsigned char Log_PackageSize;
    unsigned short LocationStringTagNumber;
    unsigned short FunctionCodeTagNumber;
    unsigned short NextStateTagNumber;
    unsigned short NumberOfMeasurement;

    unsigned short* MeasurementTagNumbers;

    unsigned short DeviceNameTextNumber;
    unsigned short AutoRangingListPtr;
    unsigned short ChannelLocationListPtr;
    unsigned short ReminderListPtr;
  };

  struct TagList {
    unsigned short Register;
    unsigned short AdditionalSetupData;
    unsigned char AdditionalSetupData2;
    unsigned char DisplayFormat;
    unsigned short EditFormat;
    unsigned short Name;
    unsigned short RangeValues[4];
    unsigned short UnitsValue;
    unsigned short ParameterValue;
  };

  unsigned char* DDData;
  GeneralInformationList  generalInformationList;
  TagList* tags;
  vector<string> StringListEnglish;

  void ProceedGeneralInformation();
  void ProceedTagList();
  void ProceedSelectionList();
  void ProceedText();
};


