extends Area2D

@export var speed := 300.0
var bullet_scene = preload("res://Bullet2.tscn")
var fire_rate = 0.5
var time_since_last_shot = 0
var screen_size: Vector2

var lives = 3
var blink_count = 0
var invincible = false

signal ship_hit(lives_left)

@onready var sprite: Sprite2D = $SpaceShip

func _ready():
	screen_size = get_viewport_rect().size
	screen_size.x = screen_size.x - 70
	position = Vector2(screen_size.x / 2, screen_size.y - 100)

func _process(delta):
	var velocity = Vector2.ZERO

	# Left / Right Input (movement)
	if Input.is_action_pressed("ui_left"):
		velocity.x -= 1
	elif Input.is_action_pressed("ui_right"):
		velocity.x += 1
	
	# Apply speed to velocity
	velocity = velocity.normalized() * speed

	# Move the spaceship
	position += velocity * delta
	
	position = position.clamp(Vector2.ZERO, screen_size)

	# Shooting logic
	time_since_last_shot += delta
	if Input.is_action_pressed("ui_shoot") and time_since_last_shot >= fire_rate:
		print("Firing bullet")  # Debugging print statement
		shoot_bullet()
		time_since_last_shot = 0

# Function to shoot bullets
func shoot_bullet():
	var bullet = bullet_scene.instantiate()
	bullet.position = to_global(Vector2.ZERO)

	get_tree().current_scene.add_child(bullet)
	
func _notification(what):
	if what == NOTIFICATION_WM_SIZE_CHANGED:
		screen_size = get_viewport_rect().size
		screen_size.x = screen_size.x - 70

func damage_ship():
	if invincible:
		return  # Prevent taking damage if blinking
	print("Ship hit!")
	lives -= 1
	emit_signal("ship_hit", lives)  # Notify the main scene
	if lives <= 0:
		print("Game Over!")
		queue_free()  # Or trigger a game over scene
	else:
		blink_ship()

func blink_ship():
	blink_count = 0
	invincible = true
	blink()

func blink():
	if blink_count < 6:
		sprite.visible = !sprite.visible
		blink_count += 1
		await get_tree().create_timer(0.1).timeout
		blink()
	else:
		sprite.visible = true
		invincible = false
