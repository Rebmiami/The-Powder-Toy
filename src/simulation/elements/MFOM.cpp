#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);

void Element::Element_MFOM()
{
	Identifier = "DEFAULT_PT_MFOM";
	Name = "MFOM";
	Colour = 0x90EE90_rgb;
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 1;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 251;
	Description = "Memory foam material.Deforms under pressure. Returns to and sets its shape when heated/ cooled. Read WIKI.";

	Properties = TYPE_PART | PROP_HOT_GLOW;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1273.0f;
	HighTemperatureTransition = PT_LAVA;
	Update = &update;
	Create = &create;
	Graphics = &graphics;
}
	constexpr float ADVECTION = 0.1f;
static int update(UPDATE_FUNC_ARGS)
{
	
	if (!parts[i].tmp3 && sim->pv[y/CELL][x/CELL]>1.0f)
		parts[i].tmp3 = sim->rng.between(300, 379);
	if (parts[i].tmp3)
	{
		parts[i].vx += ADVECTION*sim->vx[y/CELL][x/CELL];
		parts[i].vy += ADVECTION*sim->vy[y/CELL][x/CELL];
	}
	
	if (parts[i].temp > 22+273.15f)
	{
		parts[i].temp -= 1.0f;
	}
	else if (parts[i].temp < 22+273.15f)
	{
		parts[i].temp += 1.0f;
	}
	if (parts[i].temp > 100+273.15f)
	{
		//Movement code
		if (parts[i].x > parts[i].tmp)
		{
			parts[i].vx -= 1.0f;
		}
		else if (parts[i].x < parts[i].tmp)
		{
			parts[i].vx += 1.0f;
		}
		
		if (parts[i].y > parts[i].tmp2)
		{
			parts[i].vy -= 1.0f;
		}
		else if (parts[i].y < parts[i].tmp2)
		{
			parts[i].vy += 1.0f;
		}
	}
		if (parts[i].temp < 273.15f)
	{
	//Sets shape when cold (below 0.0C)
    sim->parts[i].tmp = sim->parts[i].x;
	sim->parts[i].tmp2 = sim->parts[i].y;
	}
	return 0;
}
static int graphics(GRAPHICS_FUNC_ARGS)
{
	if (cpart->temp > 100+273.15f)
	{
	*pixel_mode |= PMODE_FLARE;
	}
	else if (cpart->temp < 273.15f)
	{
	*colr = 244;
	*colg = 84;
	*colb = 84;
	*pixel_mode |= PMODE_FLARE;
	}
	return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS)
{
	sim->parts[i].tmp = sim->parts[i].x;
	sim->parts[i].tmp2 = sim->parts[i].y;
}