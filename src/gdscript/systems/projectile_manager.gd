extends Node
class_name ProjectileManager

# Simple projectile pool for special moves

class Projectile:
	var position: Vector2
	var velocity: Vector2
	var damage: int
	var lifetime: float
	var effect_type: int
	var owner_id: int
	var active: bool = false

	func _init(pos: Vector2, vel: Vector2, dmg: int, eff: int, owner: int):
		position = pos
		velocity = vel
		damage = dmg
		effect_type = eff
		owner_id = owner
		lifetime = 3.0
		active = true

var projectiles: Array = []

func spawn_projectile(pos: Vector2, vel: Vector2, damage: int, effect: int, owner_id: int):
	projectiles.append(Projectile.new(pos, vel, damage, effect, owner_id))

func update_projectiles(delta: float, fighters: Array):
	for p in projectiles:
		if not p.active:
			continue

		p.position += p.velocity * delta
		p.lifetime -= delta

		if p.lifetime <= 0:
			p.active = false
			continue

		# Check collision with fighters
		for f in fighters:
			if f.player_id == p.owner_id:
				continue
			if not f.alive:
				continue

			var f_rect = Rect2(f.global_position.x - 25, f.global_position.y - 80, 50, 85)
			var p_rect = Rect2(p.position.x - 8, p.position.y - 8, 16, 16)

			if p_rect.intersects(f_rect):
				f.take_hit(null, p.damage, p.velocity.normalized() * 8)
				p.active = false
				break

	# Clean up inactive
	projectiles = projectiles.filter(func(p): return p.active)

func _draw():
	for p in projectiles:
		if p.active:
			var c = Color.WHITE
			match p.effect_type:
				1: c = Color(1.0, 0.3, 0.0)
				2: c = Color(0.4, 0.7, 1.0)
				3: c = Color(1.0, 0.8, 0.0)
			draw_circle(p.position - global_position, 6, c)
