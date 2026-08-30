from discord.ext import commands
from db import db
import discord

class Fleet(commands.Cog):
    def __init__(self,bot):
        self.bot = bot

    @commands.command()
    async def createfleet(self,ctx,*,name):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if pirate.fleet_id:
            await ctx.send("You're already in a fleet!")
            return
        fleet_id = db.create_fleet(name,pirate.user_id)
        pirate.fleet_id = fleet_id
        db.update_pirate(pirate)
        await ctx.send(f"You created the {name} fleet!")

    @commands.command()
    async def joinfleet(self,ctx,*,name):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if pirate.fleet_id:
            await ctx.send("You're already in a fleet!")
            return
        fleet = db.get_fleet(name,False)
        if not fleet:
            await ctx.send("That fleet doesn't exist!")
            return
        pirate.fleet_id = fleet[0]
        db.update_pirate(pirate)
        await ctx.send(f"You joined the {fleet[1]} fleet!")

    @commands.command()
    async def turnback(self,ctx):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if not pirate.fleet_id:
            await ctx.send("You're not in a fleet!")
            return
        fleet = db.get_fleet(pirate.fleet_id,True)
        members = db.get_fleet_members(pirate.fleet_id)
        message = f"Fleet: {fleet[1]}\n"
        message += "Members:\n"
        for i,member in enumerate(members):
            message += f"{i+1}. {member[0]}\n"
        await ctx.send(message)

    @commands.command()
    async def leavefleet(self,ctx):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if not pirate.fleet_id:
            await ctx.send("You're not in a fleet!")
            return
        if db.get_captain(pirate.fleet_id) == ctx.author.id:
            await ctx.send("You're the captain! You cannot leave! Either disband the fleet (!disbandfleet) or appoint a new captain (!handover).")
            return
        pirate.fleet_id = None
        db.update_pirate(pirate)
        await ctx.send("You left your fleet!")

    @commands.command()
    async def disbandfleet(self,ctx):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if not pirate.fleet_id:
            await ctx.send("You're not in a fleet!")
            return
        if db.get_captain(pirate.fleet_id) != ctx.author.id:
            await ctx.send("You're not the captain! You cannot disband the fleet!")
            return
        fleet = db.get_fleet(pirate.fleet_id,1)
        db.update_fleet(pirate.fleet_id,None)
        await ctx.send(f"The {fleet[1]} fleet has been disbanded!")

    @commands.command()
    async def handover(self,ctx,member: discord.Member):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if not pirate.fleet_id:
            await ctx.send("You're not in a fleet!")
            return
        if db.get_captain(pirate.fleet_id) != ctx.author.id:
            await ctx.send("You're not the captain! You cannot hand over the fleet!")
            return
        new_captain = db.get_pirate(member.id)
        if not new_captain:
            await ctx.send(f"{member.display_name} hasn't embarked yet!")
            return
        if new_captain.fleet_id != pirate.fleet_id:
            await ctx.send("That pirate isn't in your fleet!")
            return
        db.update_fleet(pirate.fleet_id,member.id)
        await ctx.send(f"You handed over the fleet to {member.display_name}!")

    @commands.command()
    async def fleetboard(self,ctx,stat):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if not pirate.fleet_id:
            await ctx.send("You're not in a fleet!")
            return
        if stat not in ["bounty","berries"]:
            await ctx.send("Choose either bounty or berries!")
            return
        leaderboard = db.get_fleet_leaderboard(pirate.fleet_id,stat)
        message = f"Fleet {stat.title()} Leaderboard:\n"
        for i,member in enumerate(leaderboard):
            message += f"{i+1}. {member[0]} - {member[1]} {stat.title()}\n"
        await ctx.send(message)



async def setup(bot):
    await bot.add_cog(Fleet(bot))
    print("FLEET LOADED")
