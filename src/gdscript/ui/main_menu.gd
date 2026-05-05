extends Control

func _ready():
	# Play background music if available
	pass

func _on_vs_mode_pressed():
	get_tree().change_scene_to_file("res://src/gdscript/core/game.tscn")

func _on_stage_mode_pressed():
	# Stage mode - load stage select or directly go to game
	get_tree().change_scene_to_file("res://src/gdscript/core/game.tscn")

func _on_training_pressed():
	# Training mode
	get_tree().change_scene_to_file("res://src/gdscript/core/game.tscn")

func _on_character_creator_pressed():
	get_tree().change_scene_to_file("res://src/gdscript/ui/character_creator.tscn")

func _on_quit_pressed():
	get_tree().quit()
