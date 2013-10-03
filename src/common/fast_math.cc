// File: fast_math.cc
// Author: Karl Moritz Hermann (mail@karlmoritz.com)
// Created: 23-01-2013
// Last Update: Tue 01 Oct 2013 07:05:18 PM BST

/*------------------------------------------------------------------------
 * Description: <DESC> 
 * 
 *------------------------------------------------------------------------
 * History:
 * TODO:    
 *========================================================================
 */

#include <cmath> 
#include <algorithm>
#include <iostream>
#include <cassert>

#include "fast_math.h"
#include "shared_defs.h"

using namespace std;

///// fast exp() implementation
static union{
  Real d;
  struct{
    int j,i;
  } n;
} d2i;

#define EXP_A (1048576/M_LN2)
#define EXP_C 60801
#define FAST_EXP(y) (d2i.n.i = EXP_A*(y)+(1072693248-EXP_C),d2i.d)

Real getSigmoid(Real val)
{
  return 1/(1+exp(-val));
  //return 1/(1+FAST_EXP(-val));
}

Real getExp(Real val)
{
  return exp(val);
}

Real getTanh(Real val)
{
  return tanh(val);
}

Real getArgtanh(Real val)
{
  return atanh(val);
}

Real rectifiedLinear(Real val) {
  return std::max(val,0.0);
}

Real rectifiedLinearGrad(Real val) {
  if (val > 0.0)
    return 1.0;
  return 0.0;
  // return (val > 0.0) ? 1.0 : 0.0;
}



MatrixReal tanh_p(VectorReal& x) // Input: tanh array unnormalized
{
  // derivative of normalized tanh
  Real nrm = x.norm();
  VectorReal x3 = x.array().pow(3);
  VectorReal y = x - x3; // x3; //(x.array()*x.array()*x.array()).matrix();
  MatrixReal lt = (1 - x.array().pow(2)).matrix().asDiagonal();
  MatrixReal left = lt / nrm;

  MatrixReal right = (y*x.transpose()) / (nrm*nrm*nrm);
  left = left - right; // ).matrix();
  return left;
}

//f(x) = 1.7159 * tanh( 2/3 * x)
//f'(x) = 1.14393 * (1- tanh( 2/3 * x))
//error = 2/3 * (1.7159 - output^2) * (teacher - output)

Real getSpearmansRho(vector<float> s1, vector<float> s2)
{
 /***************************************************************************
  *           Calculates Spearman's Rank Correlation Coefficient            *
  ***************************************************************************/
 
  auto r1 = getSplitRank(s1);
  auto r2 = getSplitRank(s2);

  assert(r1.size() == r2.size());

  /*
   * Spearman's Rank Correlation Coefficient:
   *   1 - ( (6 Sum(d^2)) / ( n (n^2 - 1)) )
   */

  float denominator = r2.size() * (r2.size() * r2.size() - 1);
  float numerator = 0.0;

  for(size_t i = 0; i<r2.size(); ++i)
    numerator += (r1[i]-r2[i])*(r1[i]-r2[i]);
  numerator *= 6;

  float rho = 1.0 - numerator/denominator;
  return rho;

}

vector<float> getSplitRank(vector<float>& v)
{
  auto v_s = v;
  sort(v_s.begin(),v_s.end());

  vector<float> v_r;

  for(size_t i = 0; i<v.size(); ++i)
  {
    auto l = lower_bound(v_s.begin(),v_s.end(),v[i]);
    auto u = upper_bound(v_s.begin(),v_s.end(),v[i]);
    int l_val = int(l - v_s.begin());
    int u_val = int(u - v_s.begin());

    if (l_val+1 == u_val)
    {
      v_r.push_back(l_val);
    }
    else
    {
      v_r.push_back((l_val + u_val) / 2);
    }
  }
  return v_r;
}
