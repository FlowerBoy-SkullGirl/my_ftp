#include <stdio.h>
#include <math.h>
#include <ncurses.h>
#include <unistd.h>


#define G_CONSTANT 6.674e-11
#define PI 3.1415
#define TICK 1
#define TEST_MASS_1 10e11
#define TEST_MASS_2 10e11
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
	double gamma;
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
	double sin_alpha = sin(vect->alpha);
	double sin_beta = sin(vect->beta);
	double cos_alpha = cos(vect->alpha);
	double cos_beta = cos(vect->beta);
	vect->x_comp = vect->magnitude * ((cos_alpha*sin_beta)/(sqrt((cos_beta*cos_beta)*(sin_alpha*sin_alpha)+(sin_beta*sin_beta))));
	vect->z_comp = tan(vect->alpha) * vect->x_comp;
	vect->y_comp = vect->z_comp / tan(vect->beta);
	
	//printf("Sina, sinb, cosa, cosb: %f, %f, %f, %f\n", sin_alpha, sin_beta, cos_alpha, cos_beta);
	//printf("Vector components: [%e, %e, %e]\n", vect->x_comp, vect->z_comp, vect->y_comp);
}

void component_3d_alt(struct vector_3 *vect)
{
	if (vect->gamma){
		vect->y_comp = vect->magnitude * cos(vect->gamma);
		double hypot_xz = vect->magnitude * sin(vect->gamma);
		vect->x_comp = hypot_xz * cos(vect->alpha);
		vect->z_comp = hypot_xz * sin(vect->alpha);
	}else if (vect->alpha){
		vect->x_comp = vect->magnitude * cos(vect->alpha);
		vect->z_comp = vect->magnitude * sin(vect->alpha);
	}else{
		vect->x_comp = vect->magnitude;
		vect->y_comp = 0.0;
		vect->z_comp = 0.0;
	}
}

//Find alpha and beta angles of a vector such that they corrsepsond to x-z angle and y-z angle
void find_angle_vector(struct vector_3 *dest, struct massive_object mo1, struct massive_object mo2)
{
	double x_diff = fabs(mo2.loc_x - mo1.loc_x);
	double y_diff = fabs(mo2.loc_y - mo1.loc_y);
	double z_diff = fabs(mo2.loc_z - mo1.loc_z);
	if (x_diff == 0.0)
		dest->alpha = 0.0;
	else
		dest->alpha = atan(z_diff/x_diff);
	if (y_diff == 0.0){
		dest->beta = 0.0;
		dest->gamma = 0.0;
	}else{
		dest->beta = atan(z_diff/y_diff);
		dest->gamma = atan(x_diff/y_diff);
	}
	//printf("Angles a, b, g: %f, %f, %f\n", dest->alpha, dest->beta, dest->gamma);
}

//find the force of gravity between two masses
double force_gravity(struct massive_object mo1, struct massive_object mo2)
{
	double radius = hypot_3d(mo1, mo2);
	//printf("Object radius: %f\n", radius);
	return G_CONSTANT * ((mo1.mass * mo2.mass)/(radius * radius));
}

//Find the acceleration due to gravity on mass 1 
double acceleration_due_to_gravity(struct massive_object mo1, struct massive_object mo2)
{
	//printf("Force of gravity between MOs: %EN\n", force_gravity(mo1, mo2));
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
	component_3d_alt(&v_acc);
	
	if (mo1.loc_x > mo2.loc_x)
		v_acc.x_comp *= -1.0;
	if (mo1.loc_y > mo2.loc_y)
		v_acc.y_comp *= -1.0;
	if (mo1.loc_z > mo2.loc_z)
		v_acc.z_comp *= -1.0;
	
	//printf("Magnitude total %e\n", v_acc.magnitude);
	//printf("Vector x_comp: %f\n", v_acc.x_comp);
	//printf("Vector y_comp: %f\n", v_acc.y_comp);
	//printf("Vector z_comp: %f\n", v_acc.z_comp);

	return v_acc;
}

//Combine vectors by component values
struct vector_3 add_vectors(struct vector_3 *initial, struct vector_3 *v_acc)
{
	struct vector_3 v_final;
	v_final.x_comp = initial->x_comp + v_acc->x_comp;
	v_final.y_comp = initial->y_comp + v_acc->y_comp;
	v_final.z_comp = initial->z_comp + v_acc->z_comp;

	//printf("V_INIT: [%f, %f, %f]\n", initial->x_comp, initial->y_comp, initial->z_comp);
	//printf("V_FINAL: [%f, %f, %f]\n", v_final.x_comp, v_final.y_comp, v_final.z_comp);
	
	return v_final;
}

//Simulation setup
int main()
{

	//ncurses init
	initscr();
	int ncur_x = 0;
	int ncur_y = 0;
	getmaxyx(stdscr, ncur_y, ncur_x);
	ncur_x /= 2;
	ncur_y /= 2;


	struct massive_object mo1, mo2;
	mo1.mass = TEST_MASS_1;
	mo2.mass = TEST_MASS_2;
	mo1.loc_x = 10.0;
	mo1.loc_y = 0.0;
	mo1.loc_z = 0.0;
	
	mo2.loc_x = -10.0;
	mo2.loc_y = 0.0;
	mo2.loc_z = 0.0;
	
	struct vector_3 v_init_mo1, v_init_mo2;
	v_init_mo1.magnitude = 2.0;
	v_init_mo2.magnitude = 2.0;
	
	v_init_mo1.alpha = 0.0;
	v_init_mo1.beta = 0.0;
	v_init_mo2.alpha = 0.0;
	v_init_mo2.beta = 0.0;
/*
	puts("Comp vi1");
	component_3d(&v_init_mo1);
	puts("Comp vi2");
	component_3d(&v_init_mo2);
*/
	v_init_mo1.x_comp = 1.0;
	v_init_mo1.y_comp = 1.0;
	v_init_mo1.z_comp = 0.0;
	v_init_mo2.x_comp = 1.0;
	v_init_mo2.y_comp = 1.0;
	v_init_mo2.z_comp = 0.0;
	v_init_mo2.y_comp *= -1.0;
	v_init_mo2.x_comp *= -1.0;
	
	mo1.vect = v_init_mo1;
	mo2.vect = v_init_mo2;
	
	long coord_x1, coord_y1, coord_z1, coord_x2, coord_y2, coord_z2 = 0;
	struct vector_3 v_change_1 = vector_acc_per_tick(mo1, mo2);
	struct vector_3 v_change_2 = vector_acc_per_tick(mo2, mo1);

	char c;
	for (int i = 0; c != 'q'; i++){
		clear();
		mo1.loc_x += mo1.vect.x_comp * TICK;
		mo1.loc_y += mo1.vect.y_comp * TICK;
		mo1.loc_z += mo1.vect.z_comp * TICK;	

		mo2.loc_x += mo2.vect.x_comp * TICK;
		mo2.loc_y += mo2.vect.y_comp * TICK;
		mo2.loc_z += mo2.vect.z_comp * TICK;	
		if (i == 0){
			mo1.loc_x = 10.0;
			mo1.loc_y = 0.0;
			mo1.loc_z = 0.0;
	
			mo2.loc_x = -10.0;
			mo2.loc_y = 0.0;
			mo2.loc_z = 0.0;
		}


		coord_x1 = mo1.loc_x / SCALE;
		coord_y1 = mo1.loc_y / SCALE;
		coord_z1 = mo1.loc_z / SCALE;
		coord_x2 = mo2.loc_x / SCALE;
		coord_y2 = mo2.loc_y / SCALE;
		coord_z2 = mo2.loc_z / SCALE;

		//printf("MO1i: [%d, %d, %d]\n", coord_x1, coord_y1, coord_z1);
		//printf("MO2i: [%d, %d, %d]\n", coord_x2, coord_y2, coord_z2);

	
		//puts("Mo1 calc");
		v_change_1 = vector_acc_per_tick(mo1, mo2);
		//puts("Mo2 calc");
		v_change_2 = vector_acc_per_tick(mo2, mo1);

		mo1.vect = add_vectors(&mo1.vect, &v_change_1);
		mo2.vect = add_vectors(&mo2.vect, &v_change_2);
		
		coord_x1 = mo1.loc_x / SCALE;
		coord_y1 = mo1.loc_y / SCALE;
		coord_z1 = mo1.loc_z / SCALE;
		coord_x2 = mo2.loc_x / SCALE;
		coord_y2 = mo2.loc_y / SCALE;
		coord_z2 = mo2.loc_z / SCALE;

		//printf("MO1f: [%d, %d, %d]\n", coord_x1, coord_y1, coord_z1);
		//printf("MO2f: [%d, %d, %d]\n", coord_x2, coord_y2, coord_z2);

		mvprintw(coord_y1 + ncur_y, coord_x1 + ncur_x, "1");
		mvprintw(coord_y2 + ncur_y, coord_x2 + ncur_x, "2");

		refresh();
		usleep(50000);
	}

	endwin();

	double radius = hypot_3d(mo1, mo2);	
	//printf("Radius: %E\n", radius);

	return 0;
}
