#include <stdio.h>
#include "MeanShift.h"
#include <math.h>

#define EPSILON 0.0000001

double euclidean_distance(vector<double> point_a, vector<double> point_b){
    double total = 0;
    for(int i=0; i<point_a.size(); i++){
        total += (point_a[i] - point_b[i]) * (point_a[i] - point_b[i]);
    }
    return sqrt(total);
}

double gaussian_kernel(double distance, double kernel_bandwidth){
    double temp =  exp(-(distance*distance) / (kernel_bandwidth));
    return temp;
}

MeanShift::MeanShift( double (*_kernel_func)(double,double) ) {
    if(!_kernel_func){
        kernel_func = gaussian_kernel;
    } else {
        kernel_func = _kernel_func;    
    }
}

vector<double> MeanShift::shift_point(vector<double> point, vector<vector<double> > points, double kernel_bandwidth) {
    vector<double> shifted_point = point;
    for(int dim = 0; dim<shifted_point.size(); dim++){
        shifted_point[dim] = 0;
    }
    double total_weight = 0;
    for(int i=0; i<points.size(); i++){
        vector<double> temp_point = points[i];
        double distance = euclidean_distance(point, temp_point);
        double weight = kernel_func(distance, kernel_bandwidth);
        for(int j=0; j<shifted_point.size(); j++){
            shifted_point[j] += temp_point[j] * weight;
        }
        total_weight += weight;
    }

    for(int i=0; i<shifted_point.size(); i++){
        shifted_point[i] /= total_weight;
    }
    return shifted_point;
}

vector<vector<double> > MeanShift::cluster(vector<vector<double> > points, double kernel_bandwidth){
    vector<vector<double> > shifted_points = points;
    double max_shift_distance;
    do {
        max_shift_distance = 0;
        for(int i=0; i<shifted_points.size(); i++){
            vector<double>point_new = shifted_points[i];
            point_new = shift_point(point_new, points, kernel_bandwidth);
            double shift_distance = euclidean_distance(point_new, shifted_points[i]);
            if(shift_distance > max_shift_distance){
                max_shift_distance = shift_distance;
            }
            shifted_points[i] = point_new;
        }
        printf("max_shift_distance: %f\n", max_shift_distance);
    } while (max_shift_distance > EPSILON);
    return shifted_points;
}