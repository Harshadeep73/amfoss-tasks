import os,discord,asyncio
from discord.ext import commands
from dotenv import load_dotenv
from db.db import setup

load_dotenv()
TOKEN = os.getenv("BOT_TOKEN")

intents = discord.Intents.default()
intents.message_content = True
bot = commands.Bot(command_prefix="!", intents=intents)

@bot.event
async def on_ready():
    print(f"Vyke is online as {bot.user}")

setup()

async def main():
    await bot.load_extension("commands.player.player")
    await bot.load_extension("commands.economy.shop")
    if not TOKEN:
        raise ValueError("TOKEN is not set")
    await bot.start(TOKEN)

asyncio.run(main())
