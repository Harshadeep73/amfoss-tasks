RANKS = {
    "Rookie": 0,
    "Pirate": 1000,
    "Notorious Pirate": 5000,
    "Supernova": 15000,
    "Worst Generation": 50000,
    "Yonko": 100000
}

LOOTED = {
    "Street Merchant": 500,
    "Travelling Merchant": 1000,
    "Grand Merchant": 1500,
    "Royal Merchant": 2000
}

class Pirate:
    def __init__(self,user_id,name,last_claim,berries=1000,bounty=0,rank="Rookie",fleet_id=None,devil_fruit_type=None):
        self.user_id = user_id
        self.name = name
        self.berries = berries
        self.bounty = bounty
        self.rank = rank
        self.fleet_id = fleet_id
        self.last_claim = last_claim
        self.devil_fruit_type = devil_fruit_type

    def update_rank(self):
        for rank,bounty in RANKS.items():
            if self.bounty >= bounty:
                self.rank = rank
        return None

    def next_rank(self):
        for bounty in RANKS.values():
            if bounty > self.bounty:
                return bounty-self.bounty
        return None

    def __eq__(self,other):
        if not isinstance(other,Pirate):
            return False
        return self.user_id == other.user_id

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
