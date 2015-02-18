/*  TITLE
 * Soubor:  proj2.c
 * Datum:   16.11.2004 21:45
 * Autor:   Imrich Štoffa, xstoff02@stud.fit.vutbr.cz
 * Projekt:  projekt č.2  pre predmet IZP
 * Popis:	Jednoduchy program pre vyuzitie aproximacie tangensu
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TABLE_LEN 10
#define TAY_ITER_MAX 13
#define HEIGHT_MAX 100.0
#define MIN_RADS 0.0
#define MAX_RADS 1.40
#define EPS_PRECISION 1e-10

//USED FOR APROX MIN ITER. CALCULATION
// iters = KA*rads+QUE
#define KA 3.5984361197
#define QUE 4.1991

enum task_ids{
    UNKNOWN=0,
    HELP=1,
    TAN,
    HEIGHT,
    MEASURE,
    MEASURE2,
    PRECALC
};

enum errors{
    ARG_ERR=2,
    ARGC_ERR_MUCH,
    ARGC_ERR_LITTLE,
    ARG_VAL_ERR
};

const char *err_mesg[]=
{
	"Success\n",
	"General error\n",
	"Unknown argument, check --help\n",
	"Too much arguments\n",
	"Too little arguments\n",
	"Wrong argument, input condition violation\n"
};

const char *help_string=
{
	"Welcome to simple tanges caluculator help page\n"
                "Usage: --help | --tan A n m | [-c H] -m A [B]\n"
                "   --tan: prints comparsion table of tangens approximation functions\n"
                "       A - angle in radians to count tangent from\n"
                "       n,m - range of iterations for calculation, 0<n<=m<=13\n"
                "       \n"
                "   -c: explicitly sets height of observation point, note that default value is 1.5 meters\n"
                "       H - new height to calculation of distance, 0<H<=100\n"
                "       \n"
                "   -m: calculates distance and height of observed object from given angles\n"
                "       A - angle in radinas 0<A<=1.4, used for distance\n"
                "      [B] - optional, angle in radians 0<B<=1.4\n"
                "                                      \n"
                "                             _ -^     \n"
                "                         _ -    o     \n"
                "                     _ -        b     \n"
                "                 _ -            j     \n"
                "              _ -              height \n"
                "          _ -    B              c     \n"
                "        x-_- - - - -distance - -t---^ \n"
                "        |      - A  _           |   H \n"
                "       _|_                -    _v___v_\n"
                "______________________________________\n"
                "       \n"
                "Created by Imrich Stoffa - xstoff02@stud.fit.vutbr.cz\n"
};

//struct two_vals{
//			double rads;
//			int iter;
//        };


/** \brief Funtions that calculates absolute value
 *
 * \param x : Value to cacluate absolute value form
 * \return Absolue value of x
 *
 */
double my_fabs(double x)
{
        return x>=0.0?x:-x;
}

/** \brief Program argument decoding and verification function
 *
 * \param argc Number of arguments
 * \param argv Array of strings that hold arguments
 * \param buf Place to decode arguments into
 * \return Specific error code or success
 *
 */
int decode_args(int argc, char** argv, double* buf)
{
        //buffer index
        int bi=0;
        //-m arg displacement
        int argdp=0;
        char* check=NULL;

        if(argc>=2){
            if(0==strcmp(argv[1],"--help")){
                buf[bi++]=HELP;
                    return argc==2?EXIT_SUCCESS:ARGC_ERR_MUCH;
            }
            else if(0==strcmp(argv[1],"--tan")){
                buf[bi++]=TAN;
                if(argc>=5){

                    buf[bi]=strtod(argv[2],&check);
                    if(*check!='\0'){
                        return ARG_VAL_ERR;
                    }else if(!(MIN_RADS<buf[bi] && buf[bi]<=MAX_RADS)){
                        return ARG_VAL_ERR;
                    }
                    bi++;

                    buf[bi]=(double)strtol(argv[3],&check,10);
                    if(*check!='\0'){
                        return ARG_VAL_ERR;
                    }else if(!(0.0<buf[bi] && buf[bi]<=TAY_ITER_MAX)){
                        return ARG_VAL_ERR;
                    }
                    bi++;

                    buf[bi]=(double)strtol(argv[4],&check,10);
                    if(*check!='\0'){
                        return ARG_VAL_ERR;
                    }else if(!(buf[bi]>=buf[bi-1] && buf[bi]<=TAY_ITER_MAX)){
                        return ARG_VAL_ERR;
                    }
                    bi++;
                    return argc==5?EXIT_SUCCESS:ARGC_ERR_MUCH;
				}
			}else if(0==strcmp(argv[1],"-p")){
					buf[bi++]=PRECALC;
					return argc==2?EXIT_SUCCESS:ARGC_ERR_MUCH;
            }else{
				if(0==strcmp(argv[1],"-c")){
					buf[bi++]=HEIGHT;
					if(argc>=4){
						buf[bi]=strtod(argv[2],&check);
						if(*check!='\0'){
							return ARG_VAL_ERR;
						}else if(!(0.0<buf[bi] && buf[bi]<=HEIGHT_MAX)){
							return ARG_VAL_ERR;
						}
						bi++;
						argdp=2;
					}
				}
				if(0==strcmp(argv[1+argdp],"-m")){
					int bi2=bi;
					buf[bi++]=MEASURE;
					if(argc>=3+argdp){
						buf[bi]=strtod(argv[2+argdp],&check);
						if(*check!='\0'){
							return ARG_VAL_ERR;
						}else if(!(MIN_RADS<buf[bi] && buf[bi]<=MAX_RADS)){
							return ARG_VAL_ERR;
						}
						bi++;

							if(argc==4+argdp){
								buf[bi2]=MEASURE2;
								buf[bi]=strtod(argv[3+argdp],&check);
								if(*check!='\0'){
									return ARG_VAL_ERR;
								}else if(!(MIN_RADS<buf[bi] && buf[bi]<=MAX_RADS)){
									return ARG_VAL_ERR;
								}
								bi++;
							}
						return (argc==3+argdp||argc==4+argdp)?EXIT_SUCCESS:ARGC_ERR_MUCH;
					}
				}else{
					return ARG_ERR;
				}
            }
        }else{
			return ARG_ERR;
        }
        return ARGC_ERR_LITTLE;
}

/** \brief Procedure to print help string
 *
 *
 */
void print_help()
{
        printf(help_string);
        return;
}

/** \brief Function approximates tangens using taylor series
 *
 * \param x Angle in radians to count
 * \param n Number of fractions in taylor series (iterations) max is 13
 * \return Approximation of tanges x
 *
 */
double taylor_tan(double x, unsigned int n)
{
        static const double numer[13]={1,1,2,17,62,1382,21844,929569,6404582,
            443861162,18888466084,113927491862,58870668456604};
        static const double denom[13]={1,3,15,315,2835,155925,6081075,638512875,
            10854718875,1856156927625,194896477400625,49308808782358125,
            3698160658676859375};

		double sum, xe2;

		sum=0;  xe2=x*x;

		for(unsigned int i=0; i<n && i<TAY_ITER_MAX; i++){
			sum+=((x*numer[i])/denom[i]);
			x*=xe2;
		}
		return sum;
}

/** \brief Function approximates tangens using continued fractions
 *
 * \param x Angle in radians to count
 * \param n Number of fractions in continued fraction
 * \return Approximation of tanges x
 *
 */
double cfrac_tan(double x, unsigned int n)
{
        double res, xe2;

        res=0; xe2=x*x;
		unsigned int k=(2*n)-1;

        for(; n>1; k-=2, n--){
            res=(xe2/(k-res));
        }
		return (x/(k-res));
}



/** \brief  Function approximates tangens using continued fractions, number of
 * iterarions is dynamic, based on precalculated table
 *
 * \param x Angle in radians to count
 * \return Approximation of tanges x
 *
 */
double prec_tan(double x)
{
	//Vyber minminalnej  iteracie pomocou predpocitanej tabulky
//		static struct two_vals table[TABLE_LEN]={
//			{0.0, 3},
//			{2.000000e-04, 3},
//			{2.141000e-02, 4},
//			{1.065620e-01, 5},
//			{2.759400e-01, 6},
//			{5.205160e-01, 7},
//			{8.149120e-01, 8},
//			{1.122250e+00, 9},
//			{1.387534e+00, 10},
//			{1.5, 10}
//        };
//
//        int s, e, it;
//        s=1, e=TABLE_LEN-2;
//        for(it=(s+e)/2; s<=e; it=(s+e)/2){
//			if(x<table[it].rads){
//				if(table[it-1].rads<=x){
//					it--;
//					break;
//				}else{
//					e=it;
//				}
//			}else{
//				s=it+1;
//			}
//        }
		return cfrac_tan(x,(int)(KA*x+QUE));
}

/** \brief Function that prints comparation of tanges calculated with different
 * methods
 *
 * \param rads angle in radians
 * \param it_b number of iterations for comparsion
 * \param it_e number of iterations for last comparsion
 * \return N/A
 *
 */
void tan_task(double rads, int it_b, int it_e)
{
        double tan_r, tay_r, cf_r;

        tan_r       = tan(rads);

        for(; it_b<=it_e; it_b++){
                tay_r       = taylor_tan(rads, it_b);
                cf_r        = cfrac_tan(rads, it_b);
                //cf_r= prec_tan(rads);
                printf("%d %e %e %e %e %e\n", it_b, tan_r, tay_r,
					my_fabs(tan_r-tay_r), cf_r, my_fabs(tan_r-cf_r));
        }
        return;
}

/////////////////////////////////////////////////////////////////////////////////
//ADDODITTIONAL CODE
/** \brief Calculates tangent approx. and finds out number of iterations to match
 * desired precision
 *
 * \param rads angle in radians
 * \param iters number of iterations for precise calculation
 * \return approx. tangent
 *
 */
double precalc_tan(double x, unsigned int *iters, double* delta)
{
		double lr,r;
		r=1;lr=0;
		for(*iters=1; my_fabs(lr-r) > EPS_PRECISION  && *iters<40; (*iters)++){
			lr=r;
			r=cfrac_tan(x,*iters);
		}
		*delta=my_fabs(lr-r);
		(*iters)-=2;
		return r;
}


/** \brief Prints helper table of iteration depenndency on radians
 *
 */
void print_it_table()
{
        double rs, re;
        double delta;
        double tan;
        re=3.1415926536/2;

		printf("RADS, TAN, MIN_ITERS, DELTA_IT \n");
        unsigned int x=1;
        unsigned int ox=0;
        for(rs=0; rs<=re; rs+=2e-3){
            tan=precalc_tan(rs, &x, &delta);
            if(x>ox){
                printf("%e, %.10e, %d, %e \n",rs,tan,x,delta);
                ox=x;
            }
        }
        printf("%e, %.10e, %d, %e \n",rs,tan,x,delta);
        return;
}
/////////////////////////////////////////////////////////////////////////////////

int main(int argc, char**argv)
{
        double buf[6]={0.0};
        int err;
        if((err=decode_args(argc,argv,buf))!=0){
            fprintf(stderr,err_mesg[err]);
            return err;
        }

        //queuve pointer
        int qp=0;
        int qp2=0;

        double h, obj_dist, obj_height;
        obj_dist=obj_height=0;
        h=1.5;

        //qp ++ je posun ukazovatka na nasledujuci prvok na spracovanie
        switch((int)buf[qp]){
            case HEIGHT:
                qp++;
                h=buf[qp++];

            case MEASURE:
            case MEASURE2:

                qp2=qp++;
                obj_dist=(h/prec_tan(buf[qp]));
                qp++;
                printf("%.10e\n",obj_dist);

                if(buf[qp2]==MEASURE2){

					obj_height=obj_dist*prec_tan(buf[qp])+h;
                    qp++;
                    printf("%.10e\n",obj_height);
                }
                return EXIT_SUCCESS;
                break;

            case TAN:
                qp++;
                tan_task(buf[qp], buf[qp+1], buf[qp+2]);
                return EXIT_SUCCESS;
                break;

			case PRECALC:
				print_it_table();
				return EXIT_SUCCESS;
				break;

            default:
                //SOME ERROR //This is probably dead code
                fprintf(stderr,"No task match !\n");

            case HELP:
                print_help();
                return err;
                break;
        }
		return 0;
}
