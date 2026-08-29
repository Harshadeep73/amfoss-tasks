import aiohttp
import random

BASE_URL = "https://onepieceapi.com/api"

async def get_data(endpoint):
    async with aiohttp.ClientSession() as session:
        async with session.get(f"{BASE_URL}/{endpoint}") as response:
            if response.status != 200:
                return None

            return await response.json()

async def get_random(endpoint):
    data = await get_data(endpoint)
    if not data:
        return None
    return random.choice(data)

async def get_characters():
    return await get_random("characters")

async def get_devil_fruit():
    return await get_random("devil-fruits")

async def get_islands():
    return await get_random("islands")
