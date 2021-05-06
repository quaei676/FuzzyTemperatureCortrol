#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#define total_rules_num 16
#define output_num 5
struct RuleStruct
{
    int rule_ce;
    int rule_e;
    int rule_z;
};
struct RuleContentStruct
{
    double sigma;
    double m;
};
struct RuleOutputStruct
{
    RuleContentStruct x;
    RuleContentStruct y;
    RuleContentStruct z;
};

RuleStruct rules[] = { {1,4,1} ,{ 2,2,2 } ,{ 2,3,2 } ,{ 2,4,2 } ,{ 3,5,5 } ,{ 3,4,4 } ,{ 3,3,3 } ,
{ 3,2,2 } ,{ 3,1,1 },{ 4,5,5 } ,{ 4,4,4 } ,{ 4,3,4 } ,{ 4,2,4 }, { 5,5,5 } ,{ 5,4,5 } ,{ 5,3,4 } };
double gaussianmf(double x, RuleContentStruct output)
{
    double ua;
    ua = exp(-1 * pow(((x - output.m) / output.sigma), 2));
    return ua;
}

class fuzzy {
    RuleOutputStruct ms[output_num];
public:
    void addmf(int opt, int rules_num, RuleContentStruct data);
    double eval(double e, double ce);
};
void fuzzy::addmf(int opt, int rules_num, RuleContentStruct data)
{
    switch (opt)
    {
    case 1:
        ms[rules_num - 1].x = data;
    case 2:
        ms[rules_num - 1].y = data;
    case 3:
        ms[rules_num - 1].z = data;
    }
}
double fuzzy::eval(double e, double ce)
{
    int  i;
    int rule_e, rule_ce, rule_z;

    double out, phi[total_rules_num], sumphi = 0., sumz = 0.;
    for (i = 0; i < total_rules_num; i++)
    {
        rule_e = rules[i].rule_e - 1;
        rule_ce = rules[i].rule_ce - 1;
        phi[i] = gaussianmf(e, ms[rule_e].x);
        if (phi[i] > gaussianmf(ce, ms[rule_ce].y))
            phi[i] = gaussianmf(ce, ms[rule_ce].y);
        sumphi += phi[i];
    }

    for (i = 0; i < total_rules_num; i++)
    {
        rule_z = rules[i].rule_z - 1;
        sumz += ms[rule_z].z.m * phi[i];
    }

    out = sumz / sumphi;
    //printf("%f \n", out);
    return out;
}

int main() 
{
    FILE *fr;
    double sigma = 0.25, sigma_1 = 0.1;
    fuzzy fuzzy_1;
    fuzzy_1.addmf(1, 1, { sigma, -1.0 });
    fuzzy_1.addmf(1, 2, { sigma, -0.5 });
    fuzzy_1.addmf(1, 3, { sigma, 0 });
    fuzzy_1.addmf(1, 4, { sigma, 0.5 });
    fuzzy_1.addmf(1, 5, { sigma, 1.0 });
    fuzzy_1.addmf(2, 1, { sigma, -1.0 });
    fuzzy_1.addmf(2, 2, { sigma, -0.5 });
    fuzzy_1.addmf(2, 3, { sigma, 0 });
    fuzzy_1.addmf(2, 4, { sigma, 0.5 });
    fuzzy_1.addmf(2, 5, { sigma, 1.0 });
    fuzzy_1.addmf(3, 1, { sigma, -1.0 });
    fuzzy_1.addmf(3, 2, { sigma, -0.5 });
    fuzzy_1.addmf(3, 3, { sigma, 0 });
    fuzzy_1.addmf(3, 4, { sigma, 0.5 });
    fuzzy_1.addmf(3, 5, { sigma, 1.0 });
    int Ts = 25, i;
    double p = 1.00151*pow(10, -4), q = 8.67973*pow(10, -3), y0 = 25, r = 40., y[180], a = exp(-p * Ts), b = (q / p)*(1 - exp(-p * Ts));
    double GE = (1. / 15.), GC = 1. / 15., GU = 250.;
    double ref[180], e[180 - 1], ee[180 - 1], ce[180 - 1], cee[180 - 1], mu[180 - 1], u[180 - 1];
    y[0] = y0;
    for (i = 0; i < 180; i++)
    {
        if (i <= 40) ref[i] = 35;
        else if (i > 40 && i <= 80) ref[i] = 50;
        else if (i > 80 && i <= 120) ref[i] = 65;
        else ref[i] = 80;
    }
    for (i = 0; i < 179; i++)
    {
        e[i] = ref[i] - y[i]; ee[i] = e[i] * GE;
        if (i == 0) ce[i] = 0;
        else ce[i] = e[i] - e[i - 1];
        cee[i] = ce[i] * GC;
        mu[i] = fuzzy_1.eval(ee[i], cee[i]); u[i] = mu[i] * GU;
        if (u[i] > 5) u[i] = 5;
        else if (u[i] < 0)u[i] = 0;
        y[i + 1] = a * y[i] + b / (1 + exp(0.5*y[i] - r))*u[i] + (1 - a)*y0;

    }
    if (fopen_s(&fr, "fuzzyout.dat", "w") != 0) exit(1);
    for (i = 0; i < 179; i++)
    {
        fprintf(fr, "%f\t%f\t%f\n", u[i], ref[i], y[i]);
    }
    fclose(fr);
}