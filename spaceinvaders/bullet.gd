#extends Node2D
#
## Speed of the bullet
#var speed = 600
## Set up the collision detection
#@onready var collision_shape = $CollisionShape2D
#@onready var edge_collision_effect = $EdgeCollisionEffect  # The node where the edge collision image is
#
#func _ready():
	## Initially hide the edge collision effect
	#edge_collision_effect.visible = false
	## Destroy the bullet when it leaves the screen
	#set_process(true)
	#
#func _process(delta):
	## Move the bullet upwards
	#position.y -= speed * delta
	#
	## If the bullet goes off-screen, queue for deletion
	#if position.y < -10:
		#queue_free()
#
#func _on_Bullet_body_entered(body):
	## Check if the bullet hits a specific object or area
	#if body.is_in_group("back_edge"):  # Assuming back_edge is the group for the edge collision
		#show_edge_collision()
		#queue_free()  # Remove the bullet after collision
	#elif body.is_in_group("enemy"):  # If the bullet hits an enemy
		## Handle enemy collision here (optional)
		#queue_free()
#
## Function to show the edge collision effect
#func show_edge_collision():
	#edge_collision_effect.visible = true
	#edge_collision_effect.position = position  # Position it where the bullet is
#
	## Use 'await' for timing instead of 'yield' in Godot 4
	#await get_tree().create_timer(0.5).timeout  # Wait for 0.5 seconds
#
	#edge_collision_effect.visible = false
