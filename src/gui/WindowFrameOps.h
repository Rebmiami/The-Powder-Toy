#pragma once

struct WindowFrameOps
{
<<<<<<< HEAD
	bool resizable;
	bool fullscreen;
	bool changeResolution;
	bool forceIntegerScaling;
=======
	int scale = 1;
	bool resizable = false;
	bool fullscreen = false;
	bool changeResolution = false;
	bool forceIntegerScaling = false;
	bool blurryScaling = false;
>>>>>>> 75412736 (Add blurry scaling option)

	bool operator ==(const WindowFrameOps &other) const
	{
<<<<<<< HEAD
		if (resizable  != other.resizable ) return false;
		if (fullscreen != other.fullscreen) return false;
		if (fullscreen)
		{
			if (changeResolution    != other.changeResolution   ) return false;
			if (forceIntegerScaling != other.forceIntegerScaling) return false;
		}
		return true;
	}

	bool operator !=(const WindowFrameOps &other) const
	{
		return !(*this == other);
=======
		return {
			fullscreen ? 1     : scale              ,
			fullscreen ? false : resizable          ,
			fullscreen                              ,
			fullscreen ? changeResolution    : false,
			fullscreen ? forceIntegerScaling : false,
			blurryScaling                           ,
		};
>>>>>>> 75412736 (Add blurry scaling option)
	}
};
