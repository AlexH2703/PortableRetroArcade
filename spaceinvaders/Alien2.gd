extends Area2D

@export var speed: float = 20.0
@export var points = 30
var bullet_scene: PackedScene = preload("res://AlienBullet.tscn")
var shoot_timer = Timer
func _on_Hit():
	get_node("/root/Main").add_score(points)  # Assuming the GameManager is the parent, or adjust as needed


func _on_area_entered(area):
	if area.name == "Bullet":
		queue_free()  # Remove alien if hit by bullet

func _ready():
	shoot_timer = Timer.new()
	add_child(shoot_timer)
	shoot_timer.one_shot = true
	shoot_timer.connect("timeout", Callable(self, "_on_shoot_timer_timeout"))
	reset_shoot_timer()

func _on_shoot_timer_timeout():
	if can_shoot():
		shoot()
	reset_shoot_timer()

func reset_shoot_timer():
	shoot_timer.wait_time = randf_range(3.0, 8.0)  # Random interval
	shoot_timer.start()

func can_shoot() -> bool:
	for alien in get_tree().get_nodes_in_group("enemy"):
		if alien == self:
			continue
		if abs(alien.position.x - self.position.x) < 10 and alien.position.y > self.position.y:
			return false  # Another alien is below
	return true  # No alien below, safe to shoot

func shoot():
	var bullet = bullet_scene.instantiate()
	bullet.position = self.position
	get_parent().add_child(bullet)
