extends Node

# Autoload - stores match selection data between scenes

var p1_character: Dictionary = {}
var p2_character: Dictionary = {}
var game_mode: int = 0

func set_selections(sel: Dictionary):
	p1_character = sel.get("p1", {})
	p2_character = sel.get("p2", {})

func get_p1() -> Dictionary:
	return p1_character

func get_p2() -> Dictionary:
	return p2_character
