#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i, amb, dif, spe;
  amb = calculate_ambient(alight, areflect);
  dif = calculate_diffuse(light, dreflect, normal);
  spe = calculate_specular(light, sreflect, view, normal);
  i.red = amb.red + dif.red + spe.red;
  i.green = amb.green + amb.green + spe.green;
  i.blue = amb.blue + amb.blue + spe.green;
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double dot = dot_product(light[LOCATION], normal);
  d.red = light[COLOR][RED] * dreflect[RED] * dot;
  d.green = light[COLOR][GREEN] * dreflect[GREEN] * dot;
  d.blue = light[COLOR][BLUE] * dreflect[BLUE] * dot;
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  double r[3];
  double coef, cos_alpha, ca_exp;
  coef = 2 * dot_product(normal, light[LOCATION]);
  r[0] = normal[0] * coef - light[LOCATION][0];
  r[1] = normal[1] * coef - light[LOCATION][1];
  r[2] = normal[2] * coef - light[LOCATION][2];
  cos_alpha = dot_product(r, view);
  ca_exp = 0;
  if (cos_alpha >= 0) {
    ca_exp = pow(cos_alpha, SPECULAR_EXP);
  }
  s.red = light[COLOR][RED] * sreflect[RED] * ca_exp;
  s.green = light[COLOR][GREEN] * sreflect[GREEN] * ca_exp;
  s.blue = light[COLOR][BLUE] * sreflect[BLUE] * ca_exp;
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  if (c->red > 255) c->red = 255;
  if (c->green > 255) c->green = 255;
  if (c->blue > 255) c->blue = 255;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double mag;
  mag = pow(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2], .5);
  vector[0] = vector[0] / mag;
  vector[1] = vector[1] / mag;
  vector[2] = vector[2] / mag;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
