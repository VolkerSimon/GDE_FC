#ifndef FileInfo_CLASS_H
#define FileInfo_CLASS_H

#include <windows.h>

#include <godot_cpp/classes/ref.hpp>

using namespace godot;

class FileInfo : public RefCounted
{
    SYSTEMTIME systemtime;
    GDCLASS(FileInfo, RefCounted);
    

protected:
    static void _bind_methods();
    
public:
    FileInfo();
    ~FileInfo();
    int set_file(godot::String pathStr);
    int day() const;
    int month() const;
    int year() const;
    int dayofweek() const;
    godot::String get_filedate();
    godot::String get_filedatetime();
};

#endif // FileInfo_CLASS_H