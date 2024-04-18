#include "fileinfo.h"
#include <windows.h>
#include <string>
#include <fileapi.h>
#include <godot_cpp/core/class_db.hpp>

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
    FileTimeToSystemTime(&creationTime, &systemtime);
    return 1;
}


int FileInfo::day() const
{
    return systemtime.wDay;
}

int FileInfo::dayofweek() const
{
    return systemtime.wDayOfWeek;
}
int FileInfo::month() const
{
    return systemtime.wDay;
}

int FileInfo::year() const
{
    return systemtime.wYear;
}

String FileInfo::get_filedate() 
{
   
    std::string s = std::to_string(systemtime.wDay)+"/"+ std::to_string(systemtime.wMonth)+"/"+std::to_string(systemtime.wYear)+"   "+std::to_string(systemtime.wHour)+":"+std::to_string(systemtime.wMinute);
    return godot::String(s.c_str());
}

String FileInfo::get_filedatetime() 
{
    std::string s = std::to_string(systemtime.wDay)+"/"+ std::to_string(systemtime.wMonth)+"/"+std::to_string(systemtime.wYear);
    return godot::String(s.c_str());
}


void FileInfo::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_file"), &FileInfo::set_file);
    ClassDB::bind_method(D_METHOD("day"), &FileInfo::day);
    ClassDB::bind_method(D_METHOD("dayofweek"), &FileInfo::dayofweek);
    ClassDB::bind_method(D_METHOD("month"), &FileInfo::month);
    ClassDB::bind_method(D_METHOD("year"), &FileInfo::year);
    ClassDB::bind_method(D_METHOD("get_filedate"), &FileInfo::get_filedate);
    ClassDB::bind_method(D_METHOD("get_filedatetime"), &FileInfo::get_filedatetime);
}