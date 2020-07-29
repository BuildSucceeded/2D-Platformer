#pragma once
#include <algorithm>

struct CollisionDistances
{
	double top;
	double bottom;
	double left;
	double right;

	void keepSmallest()
	{
		if (top != 0 && top < bottom && top < left && top < right)
		{
			bottom = 0;
			left = 0;
			right = 0;
		}
		if (bottom != 0 && bottom < top && bottom < left && bottom < right)
		{
			top = 0;
			left = 0;
			right = 0;
		}
		if (left != 0 && left < top && left < bottom && left < right)
		{
			top = 0;
			bottom = 0;
			right = 0;
		}
		if (right != 0 && right < top && right < bottom && right < left)
		{
			top = 0;
			bottom = 0;
			left = 0;
		}
	}

	void keepLargest()
	{
		if (top != 0 && top > bottom && top > left && top > right)
		{
			bottom = 0;
			left = 0;
			right = 0;
		}
		if (bottom != 0 && bottom > top && bottom > left && bottom > right)
		{
			top = 0;
			left = 0;
			right = 0;
		}
		if (left != 0 && left > top && left > bottom && left > right)
		{
			top = 0;
			bottom = 0;
			right = 0;
		}
		if (right != 0 && right > top && right > bottom && right > left)
		{
			top = 0;
			bottom = 0;
			left = 0;
		}
	}
};
