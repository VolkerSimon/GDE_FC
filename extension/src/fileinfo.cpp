#include "fileinfo.h"
#include <windows.h>
#include <string>
#include <fileapi.h>
#include <godot_cpp/core/class_db.hpp>
#include <iostream>
#include <cstdio>
#include <godot_cpp/variant/utility_functions.hpp>
#include <filesystem>


using namespace godot;

FileInfo::FileInfo(){}
FileInfo::~FileInfo(){}

#include <windows.h>

int FileInfo::set_file(godot::String pathStr)
{
    HANDLE hFile;
    WIN32_FILE_ATTRIBUTE_DATA fInfo;   
    FileName=pathStr;

    std::string std_string_filename=pathStr.utf8().get_data();
    
    int wchars_num = MultiByteToWideChar(CP_UTF8, 0, std_string_filename.c_str(), -1, NULL, 0);
    wchar_t* wstr = new wchar_t[wchars_num];
    MultiByteToWideChar(CP_UTF8, 0, std_string_filename.c_str(), -1, wstr, wchars_num);
    
    LPCWSTR lpcwstr_filename = wstr; 

    GetFileAttributesExW(lpcwstr_filename, GetFileExInfoStandard, &fInfo);

    delete[] wstr;

    
    FILETIME creationTime = fInfo.ftCreationTime;
    FileTimeToSystemTime(&creationTime, &creation_systemtime);
    creation_time["year"]=creation_systemtime.wYear;
    creation_time["month"]=creation_systemtime.wMonth;
    creation_time["day"]=creation_systemtime.wDay;
    creation_time["hour"]=creation_systemtime.wHour;
    creation_time["minute"]=creation_systemtime.wMinute;
    creation_time["second"]=creation_systemtime.wDay;

    FILETIME modifiedTime = fInfo.ftLastWriteTime;
    FileTimeToSystemTime(&modifiedTime, &modified_systemtime);
    modified_time["year"]=modified_systemtime.wYear;
    modified_time["month"]=modified_systemtime.wMonth;
    modified_time["day"]=modified_systemtime.wDay;
    modified_time["hour"]=modified_systemtime.wHour;
    modified_time["minute"]=modified_systemtime.wMinute;
    modified_time["second"]=modified_systemtime.wDay;

    FILETIME lastAccessTime = fInfo.ftLastAccessTime;
    FileTimeToSystemTime(&lastAccessTime, &lastaccess_systemtime);
    lastaccess_time["year"]=lastaccess_systemtime.wYear;
    lastaccess_time["month"]=lastaccess_systemtime.wMonth;
    lastaccess_time["day"]=lastaccess_systemtime.wDay;
    lastaccess_time["hour"]=lastaccess_systemtime.wHour;
    lastaccess_time["minute"]=lastaccess_systemtime.wMinute;
    lastaccess_time["second"]=lastaccess_systemtime.wDay;



    uint32_t height =0;
    uint32_t width =0;
    String fileType="";
    
    bool result = false;

    result=FileInfo::fileTypeDetails(lpcwstr_filename, &width, &height,&fileType);
    imageHeight=height;
    imageWidth=width;
    imageFileType=fileType;

    return 1;
}

bool FileInfo::fileTypeDetails(const wchar_t *fn,uint32_t *x, uint32_t*y, String *fileType)
{
    FILE *file=_wfopen(fn,L"rb");
    if (file==0) return false;
    fseek(file,0,SEEK_END);
    fileSize=ftell(file);
    fseek(file,0,SEEK_SET);
    uint8_t buf[128];
    fread(buf,1,120,file);
    uint8_t oBuf[8];
        
    
    // JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
    if (buf[0]==0xFF && buf[1]==0xD8 && buf[2]==0xFF)
    { 

        uint8_t marker[4];
        fseek(file,0,SEEK_SET);
        fread(marker, 1, 2, file);//STart Marker
        while (fread(marker, 1, 2, file) == 2) {
            if (marker[0] == 0xFF && marker[1] != 0x00 && marker[1] != 0xFF) {
                if (marker[1] == 0xC0 || marker[1] == 0xC1 || marker[1] == 0xC2 || marker[1] == 0xC3)
                {
                    // Found the SOF0 marker
                    //std::cout<<"Marker SOF0 found! At:"<<std::hex<<ftell(file)-2<<std::endl;
                    fseek(file, 3, SEEK_CUR); // Skip segment length and precision
                    fread(marker, 1, 4, file); // Read height and width
                    *y = (marker[0] << 8) | marker[1];
                    *x = (marker[2] << 8) | marker[3];
                    fclose(file);
                    if (marker[1] == 0xC0)
                    {
                        *fileType="JIFF";
                    }else{
                        *fileType="JPG";
                    }
                        
                    return true;
                
                }else if (marker[1] == 0xd8){
                    std::cout<<"Start found!"<<std::endl;
                    fseek(file,1, SEEK_CUR);    
                }else{
                    // Other Marker found!
                    //not the Segment we are looking for.. read the length of the segment and skip it
                    //std::cout<<std::hex<<static_cast<int>(marker[0])<<std::hex<<static_cast<int>(marker[1])<<"  AT:"<<std::hex<<ftell(file)-2<<std::endl;
                    uint16_t segment_length;
                    fread(&segment_length, 1, 2, file);
                    segment_length = (segment_length << 8) | (segment_length >> 8); // Correct endianness
                    std::cout<<"Jump forward:"<<std::hex<<segment_length<<" TO:"<<std::hex<<ftell(file)-2<<std::endl;
                    fseek(file, segment_length - 2, SEEK_CUR);
                }
            } else {
                //std::cout<<".";
                fseek(file,1, SEEK_CUR);
            }
        }
        return true;
    }

  // GIF: first three bytes say "GIF", next three give version number. Then dimensions
    if (buf[0]=='G' && buf[1]=='I' && buf[2]=='F')
    { 
        *x = buf[6] + (buf[7]<<8);
        *y = buf[8] + (buf[9]<<8);
        *fileType="GIF";
        return true;
    }

    // PNG: the first frame is by definition an IHDR frame, which gives dimensions
    if ( buf[0]==0x89 && buf[1]=='P' && buf[2]=='N' && buf[3]=='G' && buf[4]==0x0D && buf[5]==0x0A && buf[6]==0x1A && buf[7]==0x0A && buf[12]=='I' && buf[13]=='H' && buf[14]=='D' && buf[15]=='R')
    {
        *x = (buf[16]<<24) + (buf[17]<<16) + (buf[18]<<8) + (buf[19]<<0);
        *y = (buf[20]<<24) + (buf[21]<<16) + (buf[22]<<8) + (buf[23]<<0);
        *fileType="PNG";
        return true;
    }

    //AVI
    if (buf[0]=='R' && buf[1]=='I' && buf[2]=='F'&& buf[3]=='F')
    { 
        *x = buf[64] + (buf[65]<<8) + (buf[66]<<16) + (buf[67]<<24);   
        *y = buf[68] + (buf[69]<<8) + (buf[70]<<16) + (buf[71]<<24);
        *fileType="AVI";
        return true;
    }

    //TIFF
    if (buf[0]==0x49 && buf[1]==0x49 && buf[2]==0x2A)
    {
        uint32_t ifdOffset=(buf[4]<<0) + (buf[5]<<8) + (buf[6]<<16) + (buf[7]<<24);
        
        // Zum IFD springen
        fseek(file, ifdOffset, SEEK_SET);

        // Anzahl der Einträge im IFD lesen
        uint16_t numEntries;
        fread(&numEntries, sizeof(numEntries), 1, file);

        // Schleife durch die IFD-Einträge
        for (int i = 0; i < numEntries; ++i) {
            uint16_t tag;
            fread(&tag, sizeof(tag), 1, file);

            if (tag == 0x100) { // Tag für Breite (Width)
                uint32_t width;
                fseek(file, 6, SEEK_CUR);//field Type(2Byte) and  Count of the indicated type(4Byte)
                fread(&width, sizeof(width), 1, file);
                *x = width;
            } else if (tag == 0x101) { // Tag für Höhe (Height)
                uint32_t height;
                fseek(file, 6, SEEK_CUR);//field Type(2Byte) and  Count of the indicated type(4Byte)
                fread(&height, sizeof(height), 1, file);
                *y = height;
            } else {

                fseek(file, 10, SEEK_CUR);
            }
        }
        *fileType="TIFF";
        return true;
    }
    fclose(file);
    return false;
}



godot::Dictionary  FileInfo::get_file_creation_time() 
{
    return creation_time;
}

godot::Dictionary  FileInfo::get_file_modified_time() 
{
    return modified_time;
}
godot::Dictionary  FileInfo::get_file_lastaccess_time() 
{
    return lastaccess_time;
}

String FileInfo::get_fileCreationDate_string() 
{
    return CreateISODateString(creation_systemtime);
}

String FileInfo::get_fileCreationDateTime_string() 
{
    return CreateISODateTimeString(creation_systemtime);
}


String FileInfo::get_fileModifiedDateTime_string() 
{
   
    return CreateISODateTimeString(modified_systemtime);
}

String FileInfo::get_fileModifiedDate_string() 
{
    return CreateISODateString(modified_systemtime);
}

String FileInfo::get_fileLastAccessDateTime_string() 
{
   
    return CreateISODateTimeString(lastaccess_systemtime);
}

String FileInfo::get_fileLastAccessDate_string() 
{
    return CreateISODateString(lastaccess_systemtime);
}

String FileInfo::CreateISODateTimeString(SYSTEMTIME inTime)
{
    char buf[26];
    std::sprintf(buf,"%04d-%02d-%02dT%02d:%02d:%02d+00:00",inTime.wYear,inTime.wMonth,inTime.wDay,inTime.wHour,inTime.wMinute,inTime.wSecond);

    //std::string s = std::to_string(creation_systemtime.wYear)+"-"+ std::to_string(creation_systemtime.wMonth)+"-"+std::to_string(creation_systemtime.wDay)+"T"+std::to_string(creation_systemtime.wHour)+":"+std::to_string(creation_systemtime.wMinute)+":"+std::to_string(creation_systemtime.wSecond)+"+00:00";
    return buf;
}

String FileInfo::CreateISODateString(SYSTEMTIME inTime)
{
   
    char buf[11];
    std::sprintf(buf,"%04d-%02d-%02d",inTime.wYear,inTime.wMonth,inTime.wDay);
    //std::string s = std::to_string(creation_systemtime.wYear)+"-"+ std::to_string(creation_systemtime.wMonth)+"-"+std::to_string(creation_systemtime.wDay)+"T"+std::to_string(creation_systemtime.wHour)+":"+std::to_string(creation_systemtime.wMinute)+":"+std::to_string(creation_systemtime.wSecond)+"+00:00";
    return buf;
}


int FileInfo::get_image_height() 
{
    return FileInfo::imageHeight;
}

int FileInfo::get_image_width() 
{
    return FileInfo::imageWidth;
}

int FileInfo::get_file_size() 
{
    return FileInfo::fileSize;
}


String FileInfo::get_imageFileType() 
{
   
    return FileInfo::imageFileType;
}


void FileInfo::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_file"), &FileInfo::set_file);
    ClassDB::bind_method(D_METHOD("get_imageFileType"), &FileInfo::get_imageFileType);

    ClassDB::bind_method(D_METHOD("get_fileCreationDate_string"), &FileInfo::get_fileCreationDate_string);
    ClassDB::bind_method(D_METHOD("get_fileCreationDateTime_string"), &FileInfo::get_fileCreationDateTime_string);
    ClassDB::bind_method(D_METHOD("get_fileModifiedDate_string"), &FileInfo::get_fileModifiedDate_string);
    ClassDB::bind_method(D_METHOD("get_fileModifiedDateTime_string"), &FileInfo::get_fileModifiedDateTime_string);
    ClassDB::bind_method(D_METHOD("get_fileLastAccessDate_string"), &FileInfo::get_fileLastAccessDate_string);
    ClassDB::bind_method(D_METHOD("get_fileLastAccessDateTime_string"), &FileInfo::get_fileLastAccessDateTime_string);
    ClassDB::bind_method(D_METHOD("get_file_creation_time"), &FileInfo::get_file_creation_time);
    ClassDB::bind_method(D_METHOD("get_file_modified_time"), &FileInfo::get_file_modified_time);
    ClassDB::bind_method(D_METHOD("get_file_lastaccess_time"), &FileInfo::get_file_lastaccess_time);
    ClassDB::bind_method(D_METHOD("get_file_size"), &FileInfo::get_file_size);
    ClassDB::bind_method(D_METHOD("get_image_height"), &FileInfo::get_image_height);
    ClassDB::bind_method(D_METHOD("get_image_width"), &FileInfo::get_image_width);
    
}