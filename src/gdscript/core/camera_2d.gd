extends Camera2D
class_name BattleCamera

@export var min_zoom: float = 0.8
@export var max_zoom: float = 1.5
@export var zoom_speed: float = 2.0
@export var follow_smoothing: float = 5.0

var target1: Node2D
var target2: Node2D

func _ready():
	# Find fighters
	var parent = get_parent()
	if parent:
		target1 = parent.get_node_or_null("Fighter1")
		target2 = parent.get_node_or_null("Fighter2")
	make_current()

func _process(delta):
	if not target1 or not target2:
		return

	# Center between both fighters
	var midpoint = (target1.global_position + target2.global_position) / 2.0
	var distance = target1.global_position.distance_to(target2.global_position)

	# Smooth follow
	global_position = global_position.lerp(midpoint, follow_smoothing * delta)

	# Dynamic zoom based on distance
	var target_zoom = clamp(distance / 500.0, min_zoom, max_zoom)
	zoom = zoom.lerp(Vector2(target_zoom, target_zoom), zoom_speed * delta)
