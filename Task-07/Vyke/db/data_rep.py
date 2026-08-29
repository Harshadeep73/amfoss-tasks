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
    def __init__(self,user_id,name,last_claim,berries=1000,bounty=0,rank="Rookie",fleet_id=None):
        self.user_id = user_id
        self.name = name
        self.berries = berries
        self.bounty = bounty
        self.rank = rank
        self.fleet_id = fleet_id
        self.last_claim = last_claim

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
