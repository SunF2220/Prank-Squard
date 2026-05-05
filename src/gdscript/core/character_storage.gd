extends Node

# Autoload - stores generated characters persistently

var characters: Array[Dictionary] = []

func add_character(data: Dictionary):
	characters.append(data)

func get_characters() -> Array:
	return characters

func clear():
	characters.clear()
