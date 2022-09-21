#include <stdio.h>
#include <math.h>


#define G_CONSTANT 6.674e-11
#define PI 3.1415
#define TICK 1
#define TEST_MASS_1 5e8
#define TEST_MASS_2 1.2e12
#define SCALE 1
#define RAD_ADJ PI / 180.0


/*
 * Formulas:
 * Newtonian gravity F = G (m1*m2)/(r^2)
 * Formulas for vector components
 * x = r (cos(al)*sin(be))/(sqrt(cos^2(be)*sin^2(al)*sin^2(be)))
 * y = r (cos(be)*sin(al)..etc
 * z = r (sin(al)*sin(be))/etc
 * where r is the magnitude of the vector and alpha is the angle measured from x along the z axis
 * and beta is measured from y along the z axis
 * z/x = tan alpha
 * z/y = tan beta
 * Sin theta = opp/hyp
 * Cos theta = adj/hyp
 * Tan theta = opp/adj
 * Useful math functions: sin, cos, tan, acos, asin, atan
 * fabs, abs, 
 */

struct vector_3{
	double magnitude;
	double x_comp;
	double y_comp;
	double z_comp;
	double alpha;
	double beta;
};

struct massive_object{
	double mass;
	struct vector_3 vect;
	double loc_x;
	double loc_y;
	double loc_z;
};


//Find the length of a vector given the components of each axis
double hypot_3d(struct massive_object mo1, struct massive_object mo2)
{
	double x_diff = fabs(mo2.loc_x - mo1.loc_x);
	double y_diff = fabs(mo2.loc_y - mo1.loc_y);
	double z_diff = fabs(mo2.loc_z - mo1.loc_z);
	double h_xy = sqrt((x_diff * x_diff) + (y_diff * y_diff));
	return sqrt((z_diff * z_diff) + (h_xy * h_xy));
}

void component_3d(struct vector_3 *vect)
{
	double sin_alpha = sin(vect->alpha * RAD_ADJ);
	double sin_beta = sin(vect->beta * RAD_ADJ);
	double cos_alpha = cos(vect->alpha * RAD_ADJ);
	double cos_beta = cos(vect->beta * RAD_ADJ);
	vect->x_comp = vect->magnitude * ((cos_alpha*sin_beta)/(sqrt((cos_beta*cos_beta)*(sin_alpha*sin_alpha)+(sin_beta*sin_beta))));
	vect->z_comp = tan(vect->alpha * RAD_ADJ) * vect->x_comp;
	vect->y_comp = vect->z_comp / tan(vect->beta * RAD_ADJ);
	
	printf("Sina, sinb, cosa, cosb: %f, %f, %f, %f\n", sin_alpha, sin_beta, cos_alpha, cos_beta);
	printf("Vector components: [%e, %e, %e]\n", vect->x_comp, vect->z_comp, vect->y_comp);
}

//Find alpha and beta angles of a vector such that they corrsepsond to x-z angle and y-z angle
void find_angle_vector(struct vector_3 *dest, struct massive_object mo1, struct massive_object mo2)
{
	double x_diff = fabs(mo2.loc_x - mo1.loc_x);
	double y_diff = fabs(mo2.loc_y - mo1.loc_y);
	double z_diff = fabs(mo2.loc_z - mo1.loc_z);
	dest->alpha = atan((z_diff/x_diff)*RAD_ADJ);
	dest->beta = atan((z_diff/y_diff)*RAD_ADJ);
}

//find the force of gravity between two masses
double force_gravity(struct massive_object mo1, struct massive_object mo2)
{
	double radius = hypot_3d(mo1, mo2);
	printf("Object radius: %f\n", radius);
	return G_CONSTANT * ((mo1.mass * mo2.mass)/(radius * radius));
}

//Find the acceleration due to gravity on mass 1 
double acceleration_due_to_gravity(struct massive_object mo1, struct massive_object mo2)
{
	printf("Force of gravity between MOs: %EN\n", force_gravity(mo1, mo2));
	return (force_gravity(mo1, mo2)/mo1.mass);
	
}

/*
 * Find the vector created by one tick time of acceleration due to gravity for mo1
 * break it into components x, y, z
 * determine the sign of each component based on coordinates of mo1 and mo2
 */
struct vector_3 vector_acc_per_tick(struct massive_object mo1, struct massive_object mo2)
{
	struct vector_3 v_acc;
	v_acc.magnitude = acceleration_due_to_gravity(mo1, mo2) * TICK;
	find_angle_vector(&v_acc, mo1, mo2);
	component_3d(&v_acc);
	
	if (mo1.loc_x > mo2.loc_x)
		v_acc.x_comp *= -1.0;
	if (mo1.loc_y > mo2.loc_y)
		v_acc.y_comp *= -1.0;
	if (mo1.loc_z > mo2.loc_z)
		v_acc.z_comp *= -1.0;
	
	printf("Magnitude total %e\n", v_acc.magnitude);
	printf("Vector x_comp: %f\n", v_acc.x_comp);
	printf("Vector y_comp: %f\n", v_acc.y_comp);
	printf("Vector z_comp: %f\n", v_acc.z_comp);

	return v_acc;
}

//Combine vectors by component values
struct vector_3 add_vectors(struct vector_3 *initial, struct vector_3 *v_acc)
{
	struct vector_3 v_final;
	v_final.x_comp = initial->x_comp + v_acc->x_comp;
	v_final.y_comp = initial->y_comp + v_acc->y_comp;
	v_final.z_comp = initial->z_comp + v_acc->z_comp;

	printf("V_INIT: [%f, %f, %f]\n", initial->x_comp, initial->y_comp, initial->z_comp);
	printf("V_FINAL: [%f, %f, %f]\n", v_final.x_comp, v_final.y_comp, v_final.z_comp);
	
	return v_final;
}

//Simulation setup
int main()
{
	struct massive_object mo1, mo2;
	mo1.mass = TEST_MASS_1;
	mo2.mass = TEST_MASS_2;
	mo1.loc_x = 20.0;
	mo1.loc_y = 30.0;
	mo1.loc_z = -25.0;
	
	mo2.loc_x = -20.0;
	mo2.loc_y = 27.0;
	mo2.loc_z = 4.0;
	
	struct vector_3 v_init_mo1, v_init_mo2;
	v_init_mo1.magnitude = 3.0;
	v_init_mo2.magnitude = 4.0;
	
	v_init_mo1.alpha = 60.0;
	v_init_mo1.beta = 30.0;
	v_init_mo2.alpha = 30.0;
	v_init_mo2.beta = 30.0;

	puts("Comp vi1");
	component_3d(&v_init_mo1);
	puts("Comp vi2");
	component_3d(&v_init_mo2);

	v_init_mo1.x_comp *= -1.0;
	v_init_mo1.y_comp *= -1.0;
	v_init_mo2.z_comp *= -1.0;
	
	mo1.vect = v_init_mo1;
	mo2.vect = v_init_mo2;
	
	long coord_x1, coord_y1, coord_z1, coord_x2, coord_y2, coord_z2 = 0;
	struct vector_3 v_change_1 = vector_acc_per_tick(mo1, mo2);
	struct vector_3 v_change_2 = vector_acc_per_tick(mo2, mo1);

	for (int i = 0; i < 5; i++){
		mo1.loc_x += mo1.vect.x_comp * TICK;
		mo1.loc_y += mo1.vect.y_comp * TICK;
		mo1.loc_z += mo1.vect.z_comp * TICK;	

		mo2.loc_x += mo2.vect.x_comp * TICK;
		mo2.loc_y += mo2.vect.y_comp * TICK;
		mo2.loc_z += mo2.vect.z_comp * TICK;	

		coord_x1 = mo1.loc_x / SCALE;
		coord_y1 = mo1.loc_y / SCALE;
		coord_z1 = mo1.loc_z / SCALE;
		coord_x2 = mo2.loc_x / SCALE;
		coord_y2 = mo2.loc_y / SCALE;
		coord_z2 = mo2.loc_z / SCALE;

		printf("MO1i: [%d, %d, %d]\n", coord_x1, coord_y1, coord_z1);
		printf("MO2i: [%d, %d, %d]\n", coord_x2, coord_y2, coord_z2);

	
		puts("Mo1 calc");
		v_change_1 = vector_acc_per_tick(mo1, mo2);
		puts("Mo2 calc");
		v_change_2 = vector_acc_per_tick(mo2, mo1);

		mo1.vect = add_vectors(&mo1.vect, &v_change_1);
		mo2.vect = add_vectors(&mo2.vect, &v_change_2);
		
		coord_x1 = mo1.loc_x / SCALE;
		coord_y1 = mo1.loc_y / SCALE;
		coord_z1 = mo1.loc_z / SCALE;
		coord_x2 = mo2.loc_x / SCALE;
		coord_y2 = mo2.loc_y / SCALE;
		coord_z2 = mo2.loc_z / SCALE;

		printf("MO1f: [%d, %d, %d]\n", coord_x1, coord_y1, coord_z1);
		printf("MO2f: [%d, %d, %d]\n", coord_x2, coord_y2, coord_z2);
	}

	double radius = hypot_3d(mo1, mo2);	
	printf("Radius: %E\n", radius);

	return 0;
}
