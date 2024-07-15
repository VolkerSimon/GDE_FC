#ifndef FileInfo_CLASS_H
#define FileInfo_CLASS_H

#include <windows.h>

#include <godot_cpp/classes/ref.hpp>

using namespace godot;

class FileInfo : public RefCounted
{
    SYSTEMTIME creation_systemtime;
    SYSTEMTIME modified_systemtime;
    SYSTEMTIME lastaccess_systemtime;
    GDCLASS(FileInfo, RefCounted);
    godot::Dictionary creation_time;
    godot::Dictionary modified_time;
    godot::Dictionary lastaccess_time;
    godot::Dictionary dimensions;
    godot::Dictionary size;
    godot::Dictionary height;
    godot::String FileName;
    String FileInfo::CreateISODateTimeString(SYSTEMTIME inTime);
    String FileInfo::CreateISODateString(SYSTEMTIME inTime);
    uint32_t imageHeight;
    uint32_t imageWidth;
    String imageFileType;
    uint32_t fileSize;
    bool FileInfo::fileTypeDetails(const wchar_t *fn,uint32_t *x, uint32_t*y, String *fileType);

protected:
    static void _bind_methods();
    
public:
    FileInfo();
    ~FileInfo();
    int set_file(godot::String pathStr);
    godot::String get_fileCreationDate_string();
    godot::String get_fileCreationDateTime_string();
    godot::String get_fileModifiedDate_string();
    godot::String get_fileModifiedDateTime_string();
    godot::String get_fileLastAccessDate_string();
    godot::String get_fileLastAccessDateTime_string();
    String FileInfo::get_imageFileType();
    godot::Dictionary get_file_creation_time();
    godot::Dictionary get_file_modified_time();
    godot::Dictionary get_file_lastaccess_time();
    int FileInfo::get_file_size();
    int FileInfo::get_image_height();
    int FileInfo::get_image_width(); 
};


#endif // FileInfo_CLASS_H