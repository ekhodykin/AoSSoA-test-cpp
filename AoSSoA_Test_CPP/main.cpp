//
//  main.cpp
//  AoSSoA_Test_CPP
//
//  Created by ekhodykin on 25.02.2021.
//

#include <iostream>
#include <mach/mach_time.h>

static const int kSizeOfArray = 1<<24;

struct QuadraticAoS{
    float a, b, c;
};

struct QuadraticSoA {
    float a[kSizeOfArray];
    float b[kSizeOfArray];
    float c[kSizeOfArray];
};

void aos_test(float *results, struct QuadraticAoS *e)
{
    for(int i=0; i<kSizeOfArray; i++) {
        results[i] = (-e[i].b+sqrt(e[i].b*e[i].b-4*(e[i].a * e[i].c))) / (2.0f * e[i].a);
    }
}

void soa_test(float *results, struct QuadraticSoA *quads)
{
    for(int i=0; i<kSizeOfArray; i++) {
        results[i] = (-quads->b[i] + sqrt(quads->b[i]*quads->b[i] - 4*(quads->a[i] * quads->c[i])) / (2.0f * quads->a[i]));
    }
}


int main(int argc, const char * argv[])
{
    float *roots = new float[kSizeOfArray];
    QuadraticAoS *aosArray = new QuadraticAoS[kSizeOfArray];
    QuadraticSoA *soaArray = new QuadraticSoA;
    float tmpA, tmpB, tmpC;
    uint64_t startTime = mach_absolute_time();
    srand((unsigned int)time(NULL));
    for(int i=0; i<kSizeOfArray; i++) {
        tmpA = rand() % 100;
        tmpB = rand() % 100;
        tmpC = rand() % 100;
        aosArray[i].a = tmpA;
        aosArray[i].b = tmpB;
        aosArray[i].c = tmpC;
        soaArray->a[i] = tmpA;
        soaArray->b[i] = tmpB;
        soaArray->c[i] = tmpC;
    }
    uint64_t elapsedTime = mach_absolute_time() - startTime;
    std::cout<<"Initialization time: " << elapsedTime/NSEC_PER_MSEC<<" (ms)"<<std::endl;
    const int kNumberOfRuns = 10;
    startTime = mach_absolute_time();
    for(int runNum=0; runNum<kNumberOfRuns; runNum++) {
        aos_test(roots, aosArray);
    }
    elapsedTime = mach_absolute_time() - startTime;
    std::cout<<"AoS test done in " << elapsedTime/NSEC_PER_MSEC<<" (ms)"<<std::endl;
    startTime = mach_absolute_time();
    for(int runNum=0; runNum<kNumberOfRuns; runNum++) {
        soa_test(roots, soaArray);
    }
    elapsedTime = mach_absolute_time() - startTime;
    std::cout<<"SoA test done in " << elapsedTime/NSEC_PER_MSEC<<" (ms)"<<std::endl;

    delete[] roots;
    delete[] aosArray;
    delete soaArray;
    return 0;
}
