Small test extension to see if I can get file creation date. Windows only!

You get creation, modified and lastaccess dates as an dictonary. Dates may in UTC depending on Windows version.
<code>
func _ready() -> void:
	var s = FileInfo.new()
	s.set_file(("c:\\vfcompat.dll"))
	var t=s.get_file_creation_time()
	print(t["day"])
	print(s.get_file_creation_time()["month"])
	print(s.get_file_creation_time())
	print(s.get_file_modified_time())
	print(s.get_file_lastaccess_time())
</code>
