extends Node2D

const ShieldBlock = preload("res://ShieldBlock.tscn")

func _ready():
	var block_size = 8
	var width = 21  # Reduced width by 2 for symmetry
	var height = 5

	for y in range(height):
		for x in range(width):
			var place_block = false

			# Row logic to mimic the classic "U" shape with reduced width
			if y == 0:
				# Top row: centered 9 blocks (was 11 before)
				if x >= 6 and x <= 14:
					place_block = true
			elif y == 1:
				# Second row: fully filled except edges (same as before)
				if x >= 3 and x <= 17:
					place_block = true
			elif y == 2:
				# Third row: remove one block from either end (previously fully filled)
				if x >= 1 and x <= 19:  # Range now excludes 0 and 20
					place_block = true
			elif y == 3:
				# Fourth row: fully filled
				place_block = true
			elif y == 4:
				# Bottom row: center gap like the original
				if x <= 5 or x >= 15:
					place_block = true

			if place_block:
				var block = ShieldBlock.instantiate()
				block.position = Vector2(x * block_size, y * block_size)
				add_child(block)

	# Optional: center the shield horizontally
	var total_size = Vector2(width * block_size, height * block_size)
	position -= total_size / 2
