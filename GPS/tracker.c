#include <stdio.h>
#include <math.h>

/* Global variable declarations */
#ifndef SECRET_TESTS
double M1=-1.0, B1=0;
double M2=1.0, B2=0;
#else
double M1=SECRET_M1, B1=SECRET_B1;
double M2=SECRET_M2, B2=SECRET_B2;
#endif

#define INTERSECT_X (B2 - B1) / (M1 - M2)
#define INTERSECT_Y (B2 * M1 - B1 * M2) / (M1 - M2)

int Road = 1, First_road = 1;
int Changed_direction = 0;
int Changed_road = 0;
double Total_distance = 0;
double Peak_speed = 0.0;

/* Computes the distance between two points */
double distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

/* Get the sign of a number*/
int sign(double x)
{
	return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

/**
 *	Set the m and b parameters based on the specified road
 *	Args:
 *		road (int): the enum representing the road
 *		m (double): The slope of the specified road
 *		b (double): the y intercept of the specified road
 */
void set_line_params(int road, double *m, double *b)
{
	if (road == 1)
	{
		*m = M1;
		*b = B1;
	} else {
		*m = M2;
		*b = B2;
	}
}

/**
 *	Finds the nearest point on a road (pependicular distance)
 *	Args:
 *		road (int): The enum integer that specifies the road
 *		x (double): The x coord of the input
 *		y (double): The y coord of the input
 *	Returns:
 *		(double) the x value of the point on the nearest road
 */
double nearest_point_on_road(int road, double x, double y)
{
	double m_road = 0., b_road = 0.;
	set_line_params(road, &m_road, &b_road);
	double rslt_numerator = (x/m_road) + y - b_road;
	double rslt_denominator = m_road + 1/m_road;

	return rslt_numerator / rslt_denominator;
}

/**
 *	Finds the distance to the nearest road
 *	Args:
 *		road (int): The enum integer that specifies the road
 *		x (double): The x coord of the input
 *		y (double): The y coord of the input
 *	Returns:
 *		(double) the distance to the road
 */
double distance_to_road(int road, double x, double y)
{
	double m_road = 0., b_road = 0.;
	if (road == 1)
	{
		m_road = M1;
		b_road = B1;
	} else {
		m_road = M2;
		b_road = B2;
	}
	double rslt_numerator = fabs(b_road + m_road * x - y);
	double rslt_denominator = pow(1 + pow(m_road, 2), 0.5);

	return rslt_numerator / rslt_denominator;
}

/**
 *	Finds the closest road
 *	Args:
 *		road (int): The enum integer that specifies the road
 *		x (double): The x coord of the input
 *		y (double): The y coord of the input
 *	Returns:
 *		(int) The enum  of the closest road
 */
int closest_road(int road, double x, double y)
{
	if (distance_to_road(road, x, y) < 0.001) return road;
	else {
		double road_one_dist = distance_to_road(1, x, y);
		double road_two_dist = distance_to_road(2, x, y);
		return (road_two_dist < road_one_dist) ? 2: 1;
	}
}

/**
 *	A function that smooths and tracks input gps fix locations
 *	Args:
 *		time (double): The time delta since the last gps track
 *		x (double): The x coord of the input
 *		y (double): The y coord of the input
 */
void track(double time, double x, double y)
{
	static int count = 0;
	static double prior_x = 0.0;
	static double prior_delta = 0.0;

	if(count == 0)
	{
		Road = First_road = closest_road(1, x, y);
		prior_x = nearest_point_on_road(First_road, x, y);

	} else {
		double prior_m_road = 0., prior_b_road = 0.;
		set_line_params(Road, &prior_m_road, &prior_b_road);
		int curr_road = closest_road(Road, x, y);
		double curr_m_road = 0., curr_b_road = 0.;
		set_line_params(curr_road, &curr_m_road, &curr_b_road);
		double curr_x = nearest_point_on_road(curr_road, x, y);
		double dist = 0.;

		if(curr_road == Road)
		{
			if(count == 1)
			{
				prior_delta = curr_x - prior_x;
			} else {
				double curr_delta = curr_x - prior_x;
				if (sign(prior_delta) != sign(curr_delta)) Changed_direction++;
				prior_delta = curr_delta;
			}
			dist = distance(prior_x, prior_m_road * prior_x + prior_b_road, 
							curr_x, curr_m_road * curr_x + curr_b_road);
		} else {
			double dist1 = distance(prior_x, prior_m_road * prior_x + prior_b_road, 
				INTERSECT_X, INTERSECT_Y);
			double dist2 = distance(INTERSECT_X, INTERSECT_Y, 
				curr_x, curr_m_road * curr_x + curr_b_road);
			dist = dist1 + dist2;
			prior_delta = curr_x - INTERSECT_X;
			Changed_direction++;
			Changed_road++;
		}

		Total_distance += dist;
		prior_x = curr_x;
		Road = curr_road;
		double curr_speed = dist / time;
		if (curr_speed > Peak_speed)
			Peak_speed = curr_speed;
	}

	count++;
}

int main()
{
	double time, x, y;
	int ret;

	do
	{
		ret = scanf("%lf%lf%lf", &time, &x, &y); // get inputs from stdin
		if (ret == 3)
			track(time, x, y); 
	} while(ret == 3); // quit if invalid input

	// print out final results
	printf("Total distance = %.2lf\n", Total_distance);
	printf("Peak speed = %.2lf\n", Peak_speed);
	printf("# of changes in direction = %d\n", Changed_direction);
	printf("# of changes in road = %d\n", Changed_road);
	printf("First Road = %d, Last Road = %d\n", First_road, Road);
	
	return 0;
}