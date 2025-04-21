extends Node2D

#@export var speed := 300.0
#var bullet_scene = preload("res://Bullet2.tscn")
#var fire_rate = 0.5
#var time_since_last_shot = 0
#var screen_size: Vector2
#var lives = 3
#var invincible = false
#var blink_count = 0
#var sprite: Sprite2D
#
#@onready var spaceship: Sprite2D = $SpaceShip  # Ensure this matches your node's name
#
#func _ready():
	#update_screen_size()  # Initialize screen size
	#sprite = get_node("SpaceShip")
	#
#func _process(delta):
	#var velocity = Vector2.ZERO
#
	## Left / Right Input (movement)
	#if Input.is_action_pressed("ui_left"):
		#velocity.x -= 1
	#elif Input.is_action_pressed("ui_right"):
		#velocity.x += 1
#
	## Apply speed to velocity
	#velocity = velocity.normalized() * speed
#
	## Move the spaceship
	#position += velocity * delta
	#position = position.clamp(Vector2.ZERO, screen_size)
#
	## Shooting logic
	#time_since_last_shot += delta
	#if Input.is_action_pressed("ui_shoot") and time_since_last_shot >= fire_rate:
		#print("Firing bullet")  # Debugging print statement
		#shoot_bullet()
		#time_since_last_shot = 0
#
## Function to shoot bullets
#func shoot_bullet():
	## Create a new instance of the Bullet scene
	#var bullet = bullet_scene.instantiate()
#
	## Position the bullet slightly above the player (adjust based on your ship size)
	#bullet.position = position + Vector2(5, 0)
#
	## Add the bullet to the scene
	#get_parent().add_child(bullet)
	#
#func update_screen_size():
	#screen_size = get_viewport_rect().size
#
#func _notification(what):
	#if what == NOTIFICATION_WM_SIZE_CHANGED:
		#update_screen_size()
#
#
#func take_damage():
	#if invincible:
		#return
	#invincible = true
	#lives -= 1
	#print("Ship hit! Lives left: ", lives)
	#start_blink()
	#if lives <= 0:
		#die()
#
#func start_blink():
	#var blink_timer = Timer.new()
	#blink_timer.wait_time = 0.1
	#blink_timer.one_shot = false
	#add_child(blink_timer)
#
	#blink_timer.timeout.connect(func():
		#if blink_count > 0:
			#sprite.visible = not sprite.visible
			#blink_count -= 1
		#else:
			#sprite.visible = true
			#invincible = false
			#blink_timer.queue_free()
	#)
#
	#blink_timer.start()
	#
#func _on_blink_timeout(sprite, blink_times, timer):
	#sprite.visible = not sprite.visible
	#blink_times -= 1
	#if blink_times <= 0:
		#sprite.visible = true
		#invincible = false
		#timer.queue_free()
#
#func die():
	#print("Game Over!")
	#queue_free()  # Or trigger your game over logic
