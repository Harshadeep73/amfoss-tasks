import random, discord
from datetime import date
from discord.ext import commands
from db import db
from db.data_rep import Pirate,LOOTED

class Player(commands.Cog):
    def __init__(self,bot):
        self.bot = bot

    @commands.command()
    async def embark(self,ctx):
            user_id = ctx.author.id
            name = ctx.author.display_name
            if db.get_pirate(user_id):
                await ctx.send("You're already a pirate, buddy!")
                return
            pirate = Pirate(user_id,name,None)
            db.create_pirate(pirate)
            await ctx.send(
                f"Welcome aboard, {name}!\n"
                f"You've started your journey with {pirate.berries} Berries as a gift!\n"
                f"Claim your daily berries with !setsail"
            )

    @commands.command()
    async def treasury(self,ctx):
        user_id = ctx.author.id
        name = ctx.author.display_name
        pirate = db.get_pirate(user_id)
        if not pirate:
            await ctx.send(f"Start your journey first {name}! Use !embark")
            return
        await ctx.send(f"{name}'s Treasury: {pirate.berries}")

    @commands.command()
    async def bounty(self,ctx):
        user_id = ctx.author.id
        name = ctx.author.display_name
        pirate = db.get_pirate(user_id)
        if not pirate:
            await ctx.send(f"Start your journey first {name}! Use !embark !")
            return
        await ctx.send( f"Bounty: {pirate.bounty}\n"
                        f"Rank: {pirate.rank}\n"
                        f"bounty needed for next Rank: {pirate.next_rank()}"
                      )
        return

    @commands.command()
    async def setsail(self,ctx):
        user_id = ctx.author.id
        name = ctx.author.display_name
        pirate = db.get_pirate(user_id)
        if not pirate:
            await ctx.send(f"Start your journey first {name}! Use !embark !")
            return
        today = str(date.today())
        if pirate.last_claim == today:
            await ctx.send("The marines are looking! Its too dangerous to raid innocents now!")
            return
        luck = random.randint(1,4)
        pirate.berries += luck * 500
        pirate.last_claim = today
        db.update_pirate(pirate)
        looted = list(LOOTED.keys())[luck-1]
        await ctx.send(f"You've Looted {looted} and gained {LOOTED[looted]} berries."
                       f"Use !treasury to check your current berry count!")
        return

    @commands.command()
    async def trade(self,ctx,member: discord.Member,amount: int):
        sender = db.get_pirate(ctx.author.id)
        receiver = db.get_pirate(member.id)
        today = str(date.today())
        if not sender:
            await ctx.send(f"Start your journey first {sender.name}! Use !embark !")
            return
        if not receiver:
            await ctx.send(f"{receiver.name} hasn't embarked yet!")
            return

        if amount <= 0:
            await ctx.send("You can't send that amount!")
            return
        if sender.berries < amount:
            await ctx.send("You don't have the required amount of berries!")
            if today != sender.last_claim:
                await ctx.send(f"{sender.name} can claim today's berries through !setsail")
            return

        sender.berries -= amount
        receiver.berries += amount

        db.update_pirate(sender)
        db.update_pirate(receiver)

        await ctx.send(
            f"{sender.name} traded {amount} Berries "
            f"to {receiver.name}!"
        )


async def setup(bot):
    await bot.add_cog(Player(bot))
    print("PLAYER LOADED")
