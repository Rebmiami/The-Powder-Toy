#include "simulation/ElementCommon.h"
#include "simulation/Air.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_ALUM()
{
	Identifier = "DEFAULT_PT_ALUM";
	Name = "ALUM";
	Colour = 0xB0B0C4_rgb;
	MenuVisible = 1;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.70f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 1;
	Hardness = 20;

	Weight = 100;

	HeatConduct = 251;
	Description = "Aluminium.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 660.32f;
	HighTemperatureTransition = PT_LAVA;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	int alum = 0;
	if (nt <= 2)
		alum = 2;
	else if (nt <= 6)
	{
		for (int rx = -1; rx <= 1; rx++)
		{
			for (int ry = -1; ry <= 1; ry++)
			{
				if (!rx != !ry)
				{
					if (TYP(pmap[y+ry][x+rx]) == PT_ALUM)
						alum++;
				}
			}
		}
	}
    float* pressureValues = new float[9];
    // float* velocityDot = new float[9];
    int pi = 0;
    float pressureAvg = 0;
    for (int rx = -1; rx <= 1; rx++)
	{
		for (int ry = -1; ry <= 1; ry++)
		{
            pressureValues[pi] = sim->pv[y/CELL + ry][x/CELL + rx];
            pressureAvg += pressureValues[pi];
            
		    // float avx = sim->vx[y/CELL][x/CELL];
		    // float avy = sim->vy[y/CELL][x/CELL];
            // float velDot = (rx * avx + ry * avy) / (sqrt(avx * avx + avy * avy));
            // if (velDot > 0)
            // {
            //     velocityDot[pi] = velDot;
            // }
            pi++;
		}
	}
    pressureAvg /= 9;
    float pressureStdev = 0;
    // float velocitySum = 0;
    for (int j = 0; j < 9; j++)
    {
        pressureStdev += (pressureValues[j] - pressureAvg) * (pressureValues[j] - pressureAvg);
        // velocitySum += velocityDot[i];
    }
    pressureStdev = sqrt(pressureStdev);
    if (pressureStdev / alum > 20)
    {
        // sim->part_change_type(i, x, y, PT_BREC);
		parts[i].vx += 0.4f*sim->vx[y/CELL][x/CELL];
		parts[i].vy += 0.4f*sim->vy[y/CELL][x/CELL];
    }
	else if (alum >= 2)
	{
		sim->air->bmap_blockair[y/CELL][x/CELL] = 1;
		sim->air->bmap_blockairh[y/CELL][x/CELL] = 0x8;
	}
	return 0;
}
