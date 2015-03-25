#include <armadillo>
#include <cstdlib>

using namespace arma;
using namespace std;

void eval_wavelet(mat &matrix_, double coeff, int nx, int ny) {
    int maxx = matrix_.n_rows;
    int maxy = matrix_.n_cols;

    for(int x = 0; x < maxx; x++) {
        for(int y = 0; y < maxy; y++) {
            matrix_(x, y) = sin((nx + 1) * M_PI * x / double(maxx)) *
                            sin((ny + 1) * M_PI * y / double(maxy));
        }
    }
}

class sin_series_2d {
        int max_order;
        int xsize;
        int ysize;
        double * coeff;
        vector<mat> wavelets;
    public:
	sin_series_2d(int max_order_, int xsize_, int ysize_, double (*f)(double, double));// construct        
	mat sum();
	mat sum_at(double, double);
	 //takes a 2d function e.g. exp(x^2 + y^2)
};

// sin_series_2d constructor:
sin_series_2d::sin_series_2d(int max_order_, int xsize_, int ysize_, double (*f)(double, double))
        : wavelets(max_order_ *max_order_, mat(xsize_, ysize_))
{
    max_order = max_order_;
    xsize = xsize_;
    ysize = ysize_;

    // allocate coefficient vector
    coeff = new double[max_order * max_order];

    // alloc wavelets vector

    // evaluate wavelets (grid)
    for (int nx = 0; nx < max_order; nx++) {
        for (int ny = 0; ny < max_order; ny++) {
            
            int seq_ind = (nx * max_order) + ny;
            eval_wavelet(wavelets[seq_ind], coeff[seq_ind], nx, ny);
        }
    }

	//eval coefficients
	// to be fixed!!! ****

	float phony_coeff[max_order][max_order];

	for (int nx = 0; nx < max_order; nx++) {
        for (int ny = 0; ny < max_order; ny++) {
			//phony_coeff[nx][ny] = 1.0/(max_order * max_order);
			phony_coeff[nx][ny] = 0.01 * (rand() % 100);
		}
	}

	for (int nx = 0; nx < max_order; nx++) {
        for (int ny = 0; ny < max_order; ny++) {
			
			int seq_ind = (nx * max_order) + ny;
			coeff[seq_ind] = phony_coeff[nx][ny];					
		}
	}
	//********************
}


mat sin_series_2d::sum()
{
    mat series_sum(xsize, ysize);
    
    series_sum.zeros();
    for (int nx = 0; nx < max_order; nx++) {
        for (int ny = 0; ny < max_order; ny++) {                   

        	int seq_ind = (nx * max_order) + ny;
			if (coeff[seq_ind] <= 0.001) continue;
        	series_sum = series_sum + 
						 wavelets[seq_ind] * 
						 coeff[seq_ind];
        }
    }

    return series_sum;
}


mat sin_series_2d::sum_at(double time, double c_speed)
{
    mat series_sum(xsize, ysize);
    
    series_sum.zeros();
    for (int nx = 0; nx < max_order; nx++) {
        for (int ny = 0; ny < max_order; ny++) {

        	double tot_k = sqrt( pow( (double(nx + 1)/xsize), 2) +
                             pow( (double(ny + 1)/ysize), 2));
        	double ang_freq = c_speed * tot_k;
                   

        	int seq_ind = (nx * max_order) + ny;
			if (coeff[seq_ind] <= 0.001) continue;
        	series_sum = series_sum + 
						 wavelets[seq_ind] * 
						 sin(ang_freq * time) * coeff[seq_ind];
        }
    }

    return series_sum;
}
























