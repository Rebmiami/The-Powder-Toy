#include "simulation/ElementCommon.h"
#include "simulation/Air.h"

static int update(UPDATE_FUNC_ARGS);
constexpr int AlmpBurnHealth = 40;

void Element::Element_ALMP()
{
	Identifier = "DEFAULT_PT_ALMP";
	Name = "ALMP";
	Colour = 0x87A4AF_rgb;
	MenuVisible = 1;
	MenuSection = SC_EXPLOSIVE;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.92f;
	Collision = -0.1f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 1;
	Hardness = 10;
	PhotonReflectWavelengths = 0x3FFFFFC0;

	Weight = 90;

	HeatConduct = 70;
	Description = "Aluminium powder. Flammable, burns with brilliant sparks.";

	Properties = TYPE_PART|PROP_SPARKSETTLE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	DefaultProperties.tmp = 40;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].tmp >= AlmpBurnHealth)
	{
		for (int rx = -1; rx <= 1; rx++)
		{
			for (int ry = -1; ry <= 1; ry++)
			{
				if (rx || ry)
				{
					int r = pmap[y+ry][x+rx];
					if (!r)
						continue;
					if (TYP(r) == PT_FIRE || TYP(r) == PT_PLSM || TYP(r) == PT_SPRK || TYP(r) == PT_LIGH)
					{
						parts[i].tmp = AlmpBurnHealth - sim->rng.between(1, 10);
					}
				}
			}
		}
	} else if (parts[i].tmp <= 0) {
		sim->create_part(i, x, y, PT_FIRE);
		sim->pv[y / CELL][x / CELL] += 2;
		return 1;
	} else if (parts[i].tmp < AlmpBurnHealth && sim->rng.chance(1, 2)) { // Erratic burning pattern
		parts[i].tmp--;
		if (sim->rng.chance(2, 3))
		{
			sim->pv[y / CELL][x / CELL] += 0.2f;
			int p = sim->create_part(-1, x + sim->rng.between(-1, 1), y + sim->rng.between(-1, 1), PT_EMBR);
			parts[p].tmp = 0;
			parts[p].life = 50;
			parts[p].vx = float(sim->rng.between(-4, 4));
			parts[p].vy = float(sim->rng.between(-4, 4));
		}
		else
		{
			sim->create_part(-1, x + sim->rng.between(-1, 1), y + sim->rng.between(-1, 1), PT_FIRE);
		}
	}
	return 0;
}