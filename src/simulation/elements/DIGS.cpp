#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_DIGS()
{
	Identifier = "DEFAULT_PT_DIGS";
	Name = "DIGS";
	Colour = 0x8A8AFF_rgb;
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.00f;
	Loss = 0.00f;
	Collision = -0.1f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 91;

	HeatConduct = 0;
	Description = "Digital signs for crack mod saves. Activates with PSCN. Tmp modes = different signs. (Read wiki for more info.)";
	DefaultProperties.temp = 274.15f;
	Properties = TYPE_SOLID;
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Create = &create;
	Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS)
{
	if (parts[i].tmp4 > 59)
	{
		parts[i].tmp4 -= 58;
	}
	if (parts[i].tmp3 < 0 || parts[i].tmp3 > 30)
	{
		parts[i].tmp3 = 0;
	}
	if (parts[i].life > 0) //Active
	{
		if (parts[i].tmp2 < 125)
		{
			parts[i].tmp2 += 1;
		}
		if (parts[i].tmp3 < 30)
		{
			parts[i].tmp3 += 1;
		}
		if (parts[i].tmp4 < 60)
		{
			parts[i].tmp4 += 1;
		}
	}
		if (parts[i].life == 0) //Inactive
	{
		if (parts[i].tmp2 > 0)
		{
			parts[i].tmp2 -= 1;
		}
		if (parts[i].tmp3 > 0)
		{
			parts[i].tmp3 -= 1;
		}
	}
	for (auto rx = -2; rx < 3; rx++)
		for (auto ry = -2; ry < 3; ry++)
			if (rx || ry)
			{
				auto r = pmap[y + ry][x + rx];
				if (!r)
					continue;
			if (sim->parts_avg(i, ID(r), PT_INSL) != PT_INSL)
			{
					if (parts[ID(r)].type == PT_SPRK && parts[ID(r)].ctype == PT_PSCN && parts[ID(r)].life < 3)
				{
					parts[i].life = 10;
				}
				else if (parts[ID(r)].type == PT_SPRK && parts[ID(r)].ctype == PT_NSCN && parts[ID(r)].life < 3)
				{
					parts[i].life = 0;
				}
			}
			}
	return 0;
}

static void create(ELEMENT_CREATE_FUNC_ARGS)
{
	sim->parts[i].dcolour = 0xFF478ED6;
	sim->parts[i].tmp = 1;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{ 
	*colr = ((cpart->dcolour >> 16) & 0xFF);
	*colg = ((cpart->dcolour >> 8) & 0xFF);
	*colb = ((cpart->dcolour) & 0xFF);
	if (cpart->tmp2 > 0) //Active state
	{
	if (cpart->tmp == 1) // Here Mode [1]
	{
	ren->BlendText(Vec2((int)(cpart->x+1-cpart->tmp3/2), (int)(cpart->y - 10.0f)),"[", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	if (cpart->life > 0)
	{
	ren->BlendText(Vec2((int)(cpart->x-8), (int)((cpart->y+5) - (cpart->tmp3/2))),"Here", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	}
	ren->BlendText(Vec2((int)(cpart->x-1+cpart->tmp3/2), (int)(cpart->y - 10.0f)),"]", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	}
	else if (cpart->tmp == 2 && cpart->life > 0) // Scrolling mode Left [2]
	{
	ren->BlendText(Vec2((int)(cpart->x + 15 - cpart->tmp4/2), (int)((cpart->y-10.0f))),"<<", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	}
	else if (cpart->tmp == 3 && cpart->life > 0) // Scrolling mode Right [3]
	{
	ren->BlendText(Vec2((int)(cpart->x - 15 + cpart->tmp4/2), (int)((cpart->y-10.0f))),">>", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	}
	else if (cpart->tmp == 4 && cpart->life > 0) // SPRK Sign [4]
	{
	if (cpart->dcolour == 0xFF478ED6)
	{
	cpart->dcolour = 0xFFFFFF00;
	}
	ren->BlendText(Vec2((int)(cpart->x-10), (int)((cpart->y-10.0f))),"SPRK", RGBA<uint8_t>(*colr, *colg, *colb, 255-cpart->tmp4*4));
	}
	else if (cpart->tmp == 5) // Hello [5]
	{
	ren->BlendText(Vec2((int)(cpart->x+1-cpart->tmp3/2), (int)(cpart->y - 10.0f)),"[", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	if (cpart->life > 0)
	{
	ren->BlendText(Vec2((int)(cpart->x-10), (int)((cpart->y+5) - (cpart->tmp3/2))),"Hello", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	}
	ren->BlendText(Vec2((int)(cpart->x-1+cpart->tmp3/2), (int)(cpart->y - 10.0f)),"]", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	}
	else if (cpart->tmp == 6) // Start [6]
	{
	if (cpart->dcolour == 0xFF478ED6)
	{
	cpart->dcolour = 0xFF00FF00;
	}	
	ren->BlendText(Vec2((int)(cpart->x+1-cpart->tmp3/2), (int)(cpart->y - 10.0f)),"[", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	if (cpart->life > 0)
	{
	ren->BlendText(Vec2((int)(cpart->x-10), (int)((cpart->y+5) - (cpart->tmp3/2))),"Start", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	}
	ren->BlendText(Vec2((int)(cpart->x-1+cpart->tmp3/2), (int)(cpart->y - 10.0f)),"]", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	}
	else if (cpart->tmp == 7) // STOP [7]
	{
	if (cpart->dcolour == 0xFF478ED6)
	{
	cpart->dcolour = 0xFFFF0000;
	}	
	ren->BlendText(Vec2((int)(cpart->x+1-cpart->tmp3/2), (int)(cpart->y - 10.0f)),"[", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	if (cpart->life > 0)
	{
	ren->BlendText(Vec2((int)(cpart->x-10), (int)((cpart->y+5) - (cpart->tmp3/2))),"STOP", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	}
	ren->BlendText(Vec2((int)(cpart->x-1+cpart->tmp3/2), (int)(cpart->y - 10.0f)),"]", RGBA<uint8_t>(*colr, *colg, *colb, cpart->tmp2*2));
	}
	else if (cpart->tmp == 8) // Crackermod [8]
	{
	if (cpart->dcolour == 0xFF478ED6)
	{
	cpart->dcolour = 0xFF8300FF;
	}	
	if (cpart->life > 0)
	{
	ren->DrawRect(RectSized(Vec2((int)(cpart->x - 18), (int)(cpart->y-13)),Vec2( 51, 13)),RGB<uint8_t>(*colr, *colg, *colb));
	ren->BlendFilledRect(RectSized(Vec2((int)(cpart->x - 18), (int)(cpart->y-13)),Vec2( 51, 13)),RGBA<uint8_t>(*colr, *colg, *colb,50));
	ren->BlendText(Vec2((int)(cpart->x-15), (int)((cpart->y+5) - (cpart->tmp3/2))),"Crack mod", RGBA<uint8_t>(*colr, *colg, *colb, 255-cpart->tmp4*4));
	}
	}
	}
	return 0;
}
