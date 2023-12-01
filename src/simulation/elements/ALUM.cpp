#include "simulation/ElementCommon.h"
#include "simulation/Air.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

// Element overview:
// ALUM is designed to be a versatile building material with fun-to-use properties.
// Physically, it is a weaker version of TTAN that shares the pressure blocking ability
// but with the caveat that it breaks if the force from the pressure difference becomes too large.

// Its properties are used as follows:
// ctype: Normally 0/NONE. Becomes MERC if inundated with enough MERC, causing it to grow strange formations.
// life: Used for SPRK conduction.
// tmp: Oxidation level. When oxidized, becomes brighter and resists ACID/MERC corrosion.
// tmp2: MERC inundation level. When it reaches a high enough point, ctype becomes MERC.
// tmp3: Alloy level. When mixed with molten METL, they combine to create a tougher alloy that resists more pressure.
// tmp4: Not used.

static constexpr int BaseStrength = 10;
static constexpr int MaxOxidation = 10;
static constexpr int MaxAlloyLevel = 10;

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
	Description = "Aluminium. Breakable metal that contains pressure.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH; // Melting point is custom and handled through code
	HighTemperatureTransition = PT_MALM;

	Update = &update;
	Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS)
{
	int alum = 0; // Number of orthogonally adjacent aluminium particles, including self.
	for (int rx = -1; rx <= 1; rx++) {
		for (int ry = -1; ry <= 1; ry++) {
			// Excludes the diagonal neighbors from being detected.
			if (!rx != !ry) {
				if (TYP(pmap[y+ry][x+rx]) == PT_ALUM)
					alum++;
				if (alum == 4) {
					// Increases relative strength of surface aluminium, which improves the effect of mechanical failure.
					alum = 3;
					break;
				}
			}
		}
	}

    float pressureValues[9];
    int pi = 0; // Not the numerical constant, short for "pressure index"
    float pressureAvg = 0; // The average pressure level around this particle.
    float velocityAvg = 0; // The average air speed around this particle.
    float velocitySum = 0; // The amount of direct force this particle is receiving from velocity.

    for (int rx = -1; rx <= 1; rx++) {
		for (int ry = -1; ry <= 1; ry++) {
            pressureValues[pi] = sim->pv[y/CELL + ry][x/CELL + rx];
            pressureAvg += pressureValues[pi];
            
		    float avx = sim->vx[y/CELL + ry][x/CELL + rx];
		    float avy = sim->vy[y/CELL + ry][x/CELL + rx];
			float velocityStrength = sqrt(avx * avx + avy * avy);
			velocityAvg += velocityStrength;

            if (velocityStrength > 0.1f) {
				// Calculate the dot product of air velocity based on its relative position to this particle.
				// Positive = flowing towards the particle, negative = flowing away from the particle
            	float velDot = (rx * avx + ry * avy) / velocityStrength;
				velocitySum += velDot;
            }
            pi++;
		}
	}

    pressureAvg /= 9;
    float pressureStdev = 0; // The intensity of the pressure gradient surrounding this particle.
    for (int j = 0; j < 9; j++) {
        pressureStdev += (pressureValues[j] - pressureAvg) * (pressureValues[j] - pressureAvg);
        //velocitySum += velocityDot[j];
    }

    pressureStdev = sqrt(pressureStdev);
	// Aluminium can withstand high pressure and high velocity, but struggles with both at once.
	// Additionally, it is much stronger in bulk than in thin sheets.
	// This has the effect of failures in one area of a pressurized container causing the entire thing to violently burst open.
	float strain = (pressureStdev + velocitySum + pressureStdev * velocityAvg * 0.05f) / (alum + 1);
    
	float strength = BaseStrength + parts[i].tmp3;

	if (strain > strength) // Deform under immense stress from pressure
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
	if (nt == 8 && (pressureStdev + velocityAvg) / (alum + 1) > strength * 2) {
		sim->part_change_type(i, x, y, PT_ALMP);
		sim->parts[i].tmp = 40;
		sim->parts[i].tmp2 = sim->rng.between(0, 6);
		return 1;
	}

	// Reactions with neighbors
	for (int rx = -2; rx <= 2; rx++) {
		for (int ry = -2; ry <= 2; ry++) {
			int r = pmap[y+ry][x+rx];
			if (r >= 0) {
				if (parts[i].ctype != PT_MERC && TYP(r) == PT_MERC && sim->rng.chance(10 - sim->parts[i].tmp, 10))
				{
					parts[i].tmp2 += parts[ID(r)].tmp;
					sim->kill_part(ID(r));
				}
				if (TYP(r) == PT_O2 && sim->parts[i].tmp < 10)
				{
					parts[i].tmp += 1;
				}
			}
		}
	}

	if (parts[i].ctype == PT_MERC)
	{
		if (parts[i].tmp2 > 0)
		{

		}
		int px = x + sim->rng.between(-1, 1);
		int py = y + sim->rng.between(-1, 1);
	}

	if (parts[i].temp > 660.32f)
	{
		sim->part_change_type(i,x,y,PT_MALM);
	}

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	// Appear brighter when oxidized
	int z = (cpart->tmp) * 4;
	*colr += z;
	*colg += z;
	*colb += z;
	return 0;
}