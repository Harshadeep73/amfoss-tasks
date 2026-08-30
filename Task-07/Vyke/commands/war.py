import discord
from discord.ext import commands
from db import db


class War(commands.Cog):
    def __init__(self,bot):
        self.bot = bot

    @commands.command()
    async def war(self,ctx,*,name):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if not pirate.fleet_id:
            await ctx.send("You need to be in a fleet to declare war!")
            return
        if db.get_captain(pirate.fleet_id) != ctx.author.id:
            await ctx.send("Only the captain can declare war!")
            return
        enemy = db.get_fleet(name,False)
        if not enemy:
            await ctx.send("That fleet doesn't exist!")
            return
        if enemy[0] == pirate.fleet_id:
            await ctx.send("You cannot declare war on your own fleet!")
            return
        if db.are_at_war(pirate.fleet_id,enemy[0]):
            await ctx.send("Your fleets are already at war!")
            return
        db.add_war_history(pirate.user_id,enemy[2],pirate.fleet_id,enemy[0],0,1,str(discord.utils.utcnow()),"WAR")
        await ctx.send(
            f"{pirate.name} has declared war on the {enemy[1]} fleet!"
        )

    @commands.command()
    async def history(self,ctx):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if not pirate.fleet_id:
            await ctx.send("You're not in a fleet!")
            return
        history = db.get_war_history(pirate.fleet_id)
        if not history:
            await ctx.send("The Ancient Scroll of Wars is empty.")
            return
        message = "The Ancient Scroll of Wars:\n"
        for event in history:
            attacker = db.get_pirate(event[1])
            victim = db.get_pirate(event[2])
            if not attacker or not victim:
                continue
            if event[8] == "WAR":
                message += f"{attacker.name} declared war on {victim.name}'s fleet.\n"
            elif event[8] == "ATTACK":
                if event[6] == 1:
                    message += f"{attacker.name} attacked {victim.name} and took {event[5]} Berries.\n"
                else:
                    message += f"{attacker.name} attacked {victim.name} but failed.\n"
            elif event[8] == "END_WAR":
                message += f"{attacker.name} surrendered to {victim.name}'s fleet.\n"
        await ctx.send(message)

    @commands.command()
    async def attack(self,ctx,member: discord.Member):
        attacker = db.get_pirate(ctx.author.id)
        victim = db.get_pirate(member.id)
        if not attacker:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if not victim:
            await ctx.send(f"{member.display_name} hasn't embarked yet!")
            return
        if not attacker.fleet_id:
            await ctx.send("You need to be in a fleet to attack!")
            return
        if not victim.fleet_id:
            await ctx.send(f"{victim.name} isn't in a fleet!")
            return
        if attacker.fleet_id == victim.fleet_id:
            await ctx.send("You can't attack a member of your own fleet!")
            return
        if not db.are_at_war(attacker.fleet_id,victim.fleet_id):
            await ctx.send("Your fleets are not at war!")
            return
        player = self.bot.get_cog("Player")
        result, reward = await player.raid.callback(player,ctx,member)
        db.add_war_history(attacker.user_id,victim.user_id,attacker.fleet_id,victim.fleet_id,reward,int(result),str(discord.utils.utcnow()),"ATTACK")

    @commands.command()
    async def surrender(self,ctx,*,name):
        pirate = db.get_pirate(ctx.author.id)
        if not pirate:
            await ctx.send(f"Start your journey first {ctx.author.display_name}! Use !embark")
            return
        if not pirate.fleet_id:
            await ctx.send("You're not in a fleet!")
            return
        if db.get_captain(pirate.fleet_id) != ctx.author.id:
            await ctx.send("Only the captain can surrender!")
            return
        enemy = db.get_fleet(name,False)
        if not enemy:
            await ctx.send("That fleet doesn't exist!")
            return
        if enemy[0] == pirate.fleet_id:
            await ctx.send("You cannot surrender to your own fleet!")
            return
        if not db.are_at_war(pirate.fleet_id,enemy[0]):
            await ctx.send("Your fleets are not at war!")
            return
        db.add_war_history(pirate.user_id,enemy[2],pirate.fleet_id,enemy[0],0,1,str(discord.utils.utcnow()),"END_WAR")
        await ctx.send(f"{pirate.name} has surrendered to the {enemy[1]} fleet!")

async def setup(bot):
    await bot.add_cog(War(bot))
    print("WAR LOADED")
