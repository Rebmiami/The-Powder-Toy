#include "simulation/ElementCommon.h"
static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_NUKE()
{
	Identifier = "DEFAULT_PT_NUKE";
	Name = "NUKE";
	Colour = 0x800080_rgb;
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Advection = 0.0f;
	AirDrag = 0.08f * CFDS;
	AirLoss = 0.00f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.040f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;
	Weight = 100;
	HeatConduct = 0;
	Description = "Atomic nuke. Activate with PSCN. Use one at a time. Needs gravity.";
	
	Properties = TYPE_PART|PROP_LIFE_DEC;
	Update = &update;
	Create = &create;
	Graphics = &graphics;
}
static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].life > 0)
	{
		if (parts[i].y > 100)
		{parts[i].vy = -0.5f;
		}
		else if (parts[i].y < 100)
		{parts[i].vy = +0.5f;
		}
	parts[i].vx = 0.0f;
	}
for (auto rx = -2; rx < 2; rx++)
for (auto ry = -2; ry < 2; ry++)
if (rx || ry)
	{
			auto r = pmap[y + ry][x + rx];
			if (!r)
			continue;
		{
		if (parts[ID(r)].type == PT_SPRK && parts[ID(r)].ctype == PT_PSCN && parts[ID(r)].life < 3)
		{
			parts[i].life = parts[i].tmp;
		}
		if (parts[i].life > 0)
		{
		sim->pv[(ry / CELL)][(rx / CELL)] = 250-parts[i].life;
		sim->hv[ry/CELL][rx/CELL] = 255;
		sim->gravmap[(ry/CELL)*XCELLS+(rx/CELL)] = 100;
		parts[ID(r)].temp += 500;
		}
		}
	}
		
if (parts[i].life > 0)
{
		parts[ID(i)].temp += 500;
		sim->create_part(-1,x,y+3,PT_GBMB);
		sim->create_part(-1,x+sim->rng.between(1,6),y+sim->rng.between(1,6),PT_PLSM);
		sim->create_part(-1,x-sim->rng.between(1,6),y+sim->rng.between(1,6),PT_PLSM);
		sim->create_part(-1,x+sim->rng.between(1,6),y+sim->rng.between(1,6),PT_FIRE);
		sim->create_part(-1,x-sim->rng.between(1,6),y+sim->rng.between(1,6),PT_FIRE);

if (parts[i].life < 60)
		{
	    sim->pv[(y / CELL)][(x / CELL)] += 250;	
		sim->gravmap[(y/CELL)*XCELLS+(x/CELL)] += 200;
		sim->create_part(-1,x+sim->rng.between(1,6),y+sim->rng.between(1,6),PT_THDR);
		sim->create_part(-1,x-sim->rng.between(1,6),y+sim->rng.between(1,6),PT_THDR);
		sim->create_part(-1,x+sim->rng.between(1,6),y+sim->rng.between(1,6),PT_LIGH);
		sim->create_part(-1,x-sim->rng.between(1,6),y+sim->rng.between(1,6),PT_LIGH);
		sim->create_part(-1,x+sim->rng.between(1,6),y+sim->rng.between(1,6),PT_NAPM);
		sim->create_part(-1,x-sim->rng.between(1,6),y+sim->rng.between(1,6),PT_NAPM);
		}
		if (parts[i].life == 1)
		{
		parts[i].tmp = 9700;
		parts[i].type = PT_SING;
		}
}
			
	return 0;
}
static int graphics(GRAPHICS_FUNC_ARGS)
{
	if (cpart->life > 0)
	{
	ren->BlendFilledEllipse(Vec2((int)(cpart->x), (int)(cpart->y+8)),Vec2(5,20), RGBA<uint8_t>(ren->rng.between(50,255),ren->rng.between(50,255), ren->rng.between(50,255), 255));
	ren->BlendFilledEllipse(Vec2((int)(cpart->x), (int)(cpart->y+8)),Vec2(20,5), RGBA<uint8_t>(ren->rng.between(50,255),ren->rng.between(50,255), ren->rng.between(50,255), 255));
	*pixel_mode = PMODE_LFLARE;
	}
	return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS)
{
 sim->parts[i].tmp = 400;
}