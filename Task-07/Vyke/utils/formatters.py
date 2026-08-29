def character_str(character):
    name = character["name"]["en"]
    bounties = character.get("bounties", [])
    if bounties:
        return f"The Log Pose points to {name}, who has a bounty of {bounties[0]["amount"]} Berries."
    return f"The Log Pose points to {name}, but no bounty is known."

def devil_fruit_str(fruit):
    name = fruit["name"]["en"]
    fruit_type = fruit["type"]
    return f"The Log Pose reveals the {name}, a {fruit_type}-type Devil Fruit."

def island_str(island):
    name = island["name"]["en"]
    sea = island.get("sea", "unknown waters")
    return f"The Log Pose points toward {name}, an island located in the {sea}."
