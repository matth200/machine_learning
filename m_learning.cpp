#include "m_learning.h"

using namespace std;

double sigmoid(double a)
{
     double exp_value;
     double return_value;

     /*** Exponential calculation ***/
     exp_value = exp(-a);

     /*** Final sigmoid value ***/
     return_value = 1 / (1 + exp_value);

     return return_value;
}
