#include "simulation/ElementCommon.h"
static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_NUKE()
{
	Identifier = "DEFAULT_PT_NUKE";
	Name = "C-4";
	Colour = 0x800080_rgb;
	MenuVisible = 1;
	MenuSection = SC_EXPLOSIVE;
	Enabled = 1;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;
	Weight = 100;
	HeatConduct = 0;
	Description = "Super powerful atomic nuke. Activate with PSCN";
	
	Properties = TYPE_PART|PROP_LIFE_DEC;
	Update = &update;
	Create = &create;
	Graphics = &graphics;
}
static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].life > 0)
	{
		parts[i].vy -= 1.0f;
	}
for (auto rx = -1; rx < 1; rx++)
for (auto ry = -1; ry < 1; ry++)
		if (rx || ry)
	{
			auto r = pmap[y + ry][x + rx];
			if (!r)
			continue;
		if (parts[ID(r)].type == PT_SPRK && parts[ID(r)].ctype == PT_PSCN && parts[ID(r)].life < 3)
		{
			parts[i].life = parts[i].tmp;
		}
	}
if (parts[i].life > 0 and parts[i].life < 10)
{
for (auto rx = -(parts[i].life); rx < (parts[i].life); rx++)
for (auto ry = -(parts[i].life); ry < (parts[i].life); ry++)
		if (rx || ry)
	{
			auto r = pmap[y + ry][x + rx];
			if (!r)
			continue;
		sim->pv[(y / CELL)][(x / CELL)] = 250-parts[i].life;
		sim->gv[(y / CELL)][(x / CELL)] = 100-parts[i].life;
		parts[ID(i).temp] += 500;
		parts[ID(r).temp] += 500;
		if (parts[i].life == 1)
		{
		parts[i].tmp = 255;
		parts[i].type = PT_SING;
		}
	}
}
			
	return 0;
}
static int graphics(GRAPHICS_FUNC_ARGS)
{
	if (cpart->life < 150 and cpart->life > 0)
	{
	ren->BlendFilledEllipse(Vec2((int)(cpart->x), (int)(cpart->y)),Vec2(cpart->401-life, cpart->401-life), RGBA<uint8_t>(128,0, 128, 255));
	ren->BlendFilledEllipse(Vec2((int)(cpart->x), (int)(cpart->y)),Vec2(cpart->400-life, cpart->400-life), RGBA<uint8_t>(128,0, 128, 200));
	*pixel_mode |= PMODE_LFLARE;
	}
	return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS)
{
 sim->parts[i].tmp = sim->rbg.between(150,400);
}