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
	int alum = 0; // Number of orthogonally adjacent aluminium particles, including self.
	for (int rx = -1; rx <= 1; rx++)
	{
		for (int ry = -1; ry <= 1; ry++)
		{
			// Excludes the diagonal neighbors from being detected.
			if (!rx != !ry)
			{
				if (TYP(pmap[y+ry][x+rx]) == PT_ALUM)
					alum++;
				if (alum == 4)
				{
					// Increases relative strength of surface aluminium, which improves the effect of mechanical failure.
					alum = 3;
					break;
				}
			}
		}
	}
    float pressureValues[9];
    float velocityDot[9];
    float velocityStrength[9];
    int pi = 0; // Not the numerical constant, short for "pressure index"
    float pressureAvg = 0; // The average pressure level around this particle.
    float velocityAvg = 0; // The average air speed around this particle.
    for (int rx = -1; rx <= 1; rx++)
	{
		for (int ry = -1; ry <= 1; ry++)
		{
            pressureValues[pi] = sim->pv[y/CELL + ry][x/CELL + rx];
            pressureAvg += pressureValues[pi];
            
		    float avx = sim->vx[y/CELL + ry][x/CELL + rx];
		    float avy = sim->vy[y/CELL + ry][x/CELL + rx];
            velocityStrength[pi] = sqrt(avx * avx + avy * avy);
			velocityAvg += velocityStrength[pi];
            velocityDot[pi] = 0;
            if (velocityStrength[pi] > 0.1f)
            {
				// Calculate the dot product of air velocity based on its relative position to this particle.
				// Positive = flowing towards the particle, negative = flowing away from the particle
            	float velDot = (rx * avx + ry * avy) / velocityStrength[pi];
                velocityDot[pi] = velDot;
            }
            pi++;
		}
	}
    pressureAvg /= 9;
    float pressureStdev = 0; // The intensity of the pressure gradient surrounding this particle.
    float velocitySum = 0; // The amount of direct force this particle is receiving from velocity.
    for (int j = 0; j < 9; j++)
    {
        pressureStdev += (pressureValues[j] - pressureAvg) * (pressureValues[j] - pressureAvg);
        velocitySum += velocityDot[j];
    }
    pressureStdev = sqrt(pressureStdev);
	// Aluminium can withstand high pressure and high velocity, but struggles with both at once.
	// Additionally, it is much stronger in bulk than in thin sheets.
	// This has the effect of failures in one area of a pressurized container causing the entire thing to violently burst open.
	float strain = (pressureStdev + velocitySum + pressureStdev * velocityAvg * 0.05f) / (alum + 1);
    
	if (strain > 20) // Deform under immense stress from pressure
    {
		parts[i].vx += 0.4f*sim->vx[y/CELL][x/CELL];
		parts[i].vy += 0.4f*sim->vy[y/CELL][x/CELL];
    }
	else if (alum >= 2) // Block pressure when not under too much stress
	{
		sim->air->bmap_blockair[y/CELL][x/CELL] = 1;
		sim->air->bmap_blockairh[y/CELL][x/CELL] = 0x8;
	}
	// When particles are broken off of a mass of aluminium and exposed to high air velocity, break into powder.
	if (nt == 8 && (pressureStdev + velocityAvg) / (alum + 1) > 40)
	{
		// Placeholder. Aluminium dust will be added as a new element later.
		sim->part_change_type(i, x, y, PT_BREC);
	}
	return 0;
}
