extends Node

func _ready() -> void:
	print("Hallo")
	var s = FileInfo.new()
	s.set_file(("c:\\vfcompat.dll"))
	print(s.day())
	print(s.get_filedate())
	print(s.get_filedatetime())
	

