#include "fileinfo.h"
#include <windows.h>
#include <string>
#include <fileapi.h>
#include <godot_cpp/core/class_db.hpp>
#include <iostream>
#include <cstdio>

using namespace godot;

FileInfo::FileInfo(){}
FileInfo::~FileInfo(){}

int FileInfo::set_file(godot::String pathStr)
{
    HANDLE hFile;
    WIN32_FILE_ATTRIBUTE_DATA fInfo;   
    

    std::string std_string=pathStr.utf8().get_data();
    LPCSTR lpcstr = std_string.c_str();  
    GetFileAttributesExA(lpcstr, GetFileExInfoStandard, &fInfo);

    
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

    FILETIME lastAccessTime = fInfo.ftLastWriteTime;
    FileTimeToSystemTime(&lastAccessTime, &lastaccess_systemtime);
    lastaccess_time["year"]=lastaccess_systemtime.wYear;
    lastaccess_time["month"]=lastaccess_systemtime.wMonth;
    lastaccess_time["day"]=lastaccess_systemtime.wDay;
    lastaccess_time["hour"]=lastaccess_systemtime.wHour;
    lastaccess_time["minute"]=lastaccess_systemtime.wMinute;
    lastaccess_time["second"]=lastaccess_systemtime.wDay;


    return 1;
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
    std::sprintf(buf,"%04d-%02d-%02dT%02d:%02d:%02d+00:00",creation_systemtime.wYear,creation_systemtime.wMonth,creation_systemtime.wDay,creation_systemtime.wHour,creation_systemtime.wMinute,creation_systemtime.wSecond);
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

void FileInfo::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_file"), &FileInfo::set_file);
    ClassDB::bind_method(D_METHOD("get_fileCreationDate_string"), &FileInfo::get_fileCreationDate_string);
    ClassDB::bind_method(D_METHOD("get_fileCreationDateTime_string"), &FileInfo::get_fileCreationDateTime_string);
    ClassDB::bind_method(D_METHOD("get_fileModifiedDate_string"), &FileInfo::get_fileModifiedDate_string);
    ClassDB::bind_method(D_METHOD("get_fileModifiedDateTime_string"), &FileInfo::get_fileModifiedDateTime_string);
    ClassDB::bind_method(D_METHOD("get_fileLastAccessDate_string"), &FileInfo::get_fileLastAccessDate_string);
    ClassDB::bind_method(D_METHOD("get_fileLastAccessDateTime_string"), &FileInfo::get_fileLastAccessDateTime_string);
    ClassDB::bind_method(D_METHOD("get_file_creation_time"), &FileInfo::get_file_creation_time);
    ClassDB::bind_method(D_METHOD("get_file_modified_time"), &FileInfo::get_file_modified_time);
    ClassDB::bind_method(D_METHOD("get_file_lastaccess_time"), &FileInfo::get_file_lastaccess_time);
    
}