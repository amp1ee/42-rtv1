#include "rtv1.h"

/*
	float3	Va = (obj->dir - obj->pos) / fast_length(obj->dir - obj->pos);

	D_Va = D - dot(D, Va) * Va;
	OC_Va = (O - obj->dir) - dot((O - obj->dir), Va) * Va;
	k1 = dot(D_Va, D_Va);
	k2 = 2.0F * dot(D_Va, OC_Va);
	k3 = dot(OC_Va, OC_Va) - obj->rad * obj->rad;

	descr = k2 * k2 - 4.0F * k1 * k3;
	if (descr < 0)
		return ((float2){INFINITY, INFINITY});
	T = (float2){
*/

bool		cylinder_intersect(void *data, t_vec3f eye, t_vec3f rdir,
				t_vec3f *intersect)
{
	const t_cylind	*cyl = data;
	const t_vec3f	pos = *cyl->center;
	const double	r = cyl->radius;
	const t_vec3f	dir = cyl->dir;

	t_vec3f		Va = (dir - pos);
	vec3f_normalize(&Va);
	double		d_rdirVa = vec3f_dot(rdir, Va);
	t_vec3f		m_Va = vec3f_multsc(Va, d_rdirVa);
	t_vec3f		D_Va = rdir - m_Va;
	t_vec3f		DO = (eye - dir);
	double		d_DO_Va = vec3f_dot(DO, Va);
	t_vec3f		OC_Va = DO - vec3f_multsc(Va, d_DO_Va);

	double A = vec3f_squared(D_Va);
	double B = 2.0 * vec3f_dot(D_Va, OC_Va);
	double C = vec3f_squared(OC_Va) - SQ(r);
	double d = SQ(B) - 4 * A * C;
	if (d < 0)
		return (false);
	double sqrtd = sqrt(d);
	const double		t1 = (-B + sqrtd) / (2.0 * A);
	const double		t2 = (-B - sqrtd) / (2.0 * A);
	const double		t = (MIN(t1, t2) >= 0) ? MIN(t1, t2) : MAX(t1, t2);
	*intersect = (t_vec3f){	eye[0] + t * rdir[0],
							eye[1] + t * rdir[1],
							eye[2] + t * rdir[2]};
	return (t > 0);
}

SDL_Color	cylinder_color(void *data, t_vec3f intersect)
{
	const t_cylind	*cylinder = data;

	(void)intersect;
	return (cylinder->color);
}

t_vec3f		cylinder_normalvec(void *data, t_vec3f intersect)
{
	const t_cylind	*cyl = data;
	t_vec3f			n, t;
	t_vec3f			dir;

	dir = cyl->dir;
	n = intersect - *(cyl->center);
	t = dir - *(cyl->center);
	vec3f_normalize(&t);
	n = n - vec3f_multsc(t, vec3f_dot(n, t));
	vec3f_normalize(&n);
	return (n);
}

void		cylinder_cleanup(void *data)
{
	t_cylind *cylinder;

	cylinder = data;
	free(cylinder);
}

t_obj		*new_cylinder(t_vec3f *center, t_vec3f dir, double radius, SDL_Color color)
{
	t_cylind	*cyl;
	t_obj		*obj;

	if (!(cyl = malloc(sizeof(t_cylind))))
		return (NULL);
	cyl->center = center;
	cyl->dir = dir;
	cyl->radius = radius;
	cyl->color = color;
	if (!(obj = malloc(sizeof(t_obj))))
		return (NULL);
	obj->type = CYLINDER;
	obj->data = cyl;
	obj->intersects = &cylinder_intersect;
	obj->get_color = &cylinder_color;
	obj->normal_vec = &cylinder_normalvec;
	obj->cleanup = &cylinder_cleanup;
	return (obj);
}