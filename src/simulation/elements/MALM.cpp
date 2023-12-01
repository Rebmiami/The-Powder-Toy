#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_MALM()
{
	Identifier = "DEFAULT_PT_MALM";
	Name = "MALM";
	Colour = 0x9D95A4_rgb;
	MenuVisible = 1;
	MenuSection = SC_LIQUID;
	Enabled = 1;

	Advection = 0.2f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.95f;
	Loss = 0.90f;
	Collision = 0.0f;
	Gravity = 0.15f;
	Diffusion = 0.00f;
	HotAir = 0.0001f* CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 45;

	DefaultProperties.temp = 660.32f + 200.0f;
	HeatConduct = 251;
	Description = "Molten aluminium. Does not conduct electricity; can be used as solder.";

	Properties = TYPE_LIQUID|PROP_HOT_GLOW;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL; // State transition handled through update function
	LowTemperatureTransition = PT_ALUM;
	HighTemperature = 1773.0f;
	HighTemperatureTransition = PT_LAVA;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].temp < 660.32f)
	{
		sim->part_change_type(i,x,y,PT_ALUM);
		parts[i].vx = 
		parts[i].vy = 0;
		return 1;
	}
	return 0;
}