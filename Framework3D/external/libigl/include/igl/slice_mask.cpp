// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2015 Alec Jacobson <alecjacobson@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "slice_mask.h"
#include "slice.h"
#include "slice_sorted.h"
#include "find.h"
#include <cassert>

template <typename DerivedX,typename DerivedY>
IGL_INLINE void igl::slice_mask(
  const Eigen::DenseBase<DerivedX> & X,
  const Eigen::Array<bool,Eigen::Dynamic,1> & R,
  const Eigen::Array<bool,Eigen::Dynamic,1> & C,
  Eigen::PlainObjectBase<DerivedY> & Y)
{
  int xm = X.rows();
  int xn = X.cols();
  int ym = R.count();
  int yn = C.count();
  assert(R.size() == X.rows() && "R.size() should match X.rows()");
  assert(C.size() == X.cols() && "C.size() should match X.cols()");
  Y.resize(ym,yn);
  {
    int yi = 0;
    for(int i = 0;i<xm;i++)
    {
      if(R(i))
      {
        int yj = 0;
        for(int j = 0;j<xn;j++)
        {
          if(C(j))
          {
            Y(yi,yj) = X(i,j);
            yj++;
          }
        }
        yi++;
      }
    }
  }
}

template <typename DerivedX, typename DerivedY>
IGL_INLINE void igl::slice_mask(
  const Eigen::DenseBase<DerivedX> & X,
  const Eigen::Array<bool,Eigen::Dynamic,1> & R,
  const int dim,
  Eigen::PlainObjectBase<DerivedY> & Y)
{
  switch(dim)
  {
    case 1:
    {
      const int ym = R.count();
      assert(X.rows() == R.size() && "X.rows() should match R.size()");
      Y.resize(ym,X.cols());
      {
        int yi = 0;
        for(int i = 0;i<X.rows();i++)
        {
          if(R(i))
          {
            for(int j = 0;j<X.cols();j++)
            {
              Y(yi,j) = X(i,j);
            }
            yi++;
          }
        }
      }
      return;
    }
    case 2:
    {
      const auto & C = R;
      const int yn = C.count();
      Y.resize(X.rows(),yn);
      assert(X.cols() == R.size() && "X.cols() should match R.size()");
      {
        int yj = 0;
        for(int j = 0;j<X.cols();j++)
        {
          if(C(j))
          {
            for(int i = 0;i<X.rows();i++)
            {
              Y(i,yj) = X(i,j);
            }
            yj++;
          }
        }
      }
      return;
    }
    default:
      assert(false && "Unsupported dimension");
      return;
  }
}

template <typename DerivedX>
IGL_INLINE DerivedX igl::slice_mask(
  const Eigen::DenseBase<DerivedX> & X,
  const Eigen::Array<bool,Eigen::Dynamic,1> & R,
  const Eigen::Array<bool,Eigen::Dynamic,1> & C)
{
  // This is not safe. See PlainMatrix
  DerivedX Y;
  igl::slice_mask(X,R,C,Y);
  return Y;
}

template <typename DerivedX>
IGL_INLINE DerivedX igl::slice_mask(
  const Eigen::DenseBase<DerivedX>& X,
  const Eigen::Array<bool,Eigen::Dynamic,1> & R,
  const int dim)
{
  // This is not safe. See PlainMatrix
  DerivedX Y;
  igl::slice_mask(X,R,dim,Y);
  return Y;
}


template <typename XType, typename YType>
IGL_INLINE void igl::slice_mask(
  const Eigen::SparseMatrix<XType> & X,
  const Eigen::Array<bool,Eigen::Dynamic,1> & R,
  const int dim,
  Eigen::SparseMatrix<YType> & Y)
{
  // Cheapskate solution
  Eigen::VectorXi Ri;
  find(R,Ri);
  return slice(X,Ri,dim,Y);
}

template <typename XType, typename YType>
IGL_INLINE void igl::slice_mask(
  const Eigen::SparseMatrix<XType> & X,
  const Eigen::Array<bool,Eigen::Dynamic,1> & R,
  const Eigen::Array<bool,Eigen::Dynamic,1> & C,
  Eigen::SparseMatrix<YType> & Y)
{
  // Cheapskate solution
  Eigen::VectorXi Ri;
  find(R,Ri);
  Eigen::VectorXi Ci;
  find(C,Ci);
  return slice_sorted(X,Ri,Ci,Y);
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
template void igl::slice_mask<Eigen::Matrix<double, 2, 1, 0, 2, 1>, Eigen::Matrix<double, -1, 1, 0, -1, 1> >(Eigen::DenseBase<Eigen::Matrix<double, 2, 1, 0, 2, 1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&);
template void igl::slice_mask<Eigen::Matrix<double, 2, 2, 0, 2, 2>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::DenseBase<Eigen::Matrix<double, 2, 2, 0, 2, 2> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
template void igl::slice_mask<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 3, 0, -1, 3> >(Eigen::DenseBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
template void igl::slice_mask<Eigen::Matrix<double, 3, 3, 0, 3, 3>, Eigen::Matrix<double, -1, 1, 0, -1, 1> >(Eigen::DenseBase<Eigen::Matrix<double, 3, 3, 0, 3, 3> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&);
template void igl::slice_mask<Eigen::Matrix<double, 3, 1, 0, 3, 1>, Eigen::Matrix<double, -1, 1, 0, -1, 1> >(Eigen::DenseBase<Eigen::Matrix<double, 3, 1, 0, 3, 1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&);
template void igl::slice_mask<Eigen::Matrix<double, 3, 3, 0, 3, 3>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::DenseBase<Eigen::Matrix<double, 3, 3, 0, 3, 3> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
template Eigen::Array<int, -1, -1, 0, -1, -1> igl::slice_mask<Eigen::Array<int, -1, -1, 0, -1, -1> >(Eigen::DenseBase<Eigen::Array<int, -1, -1, 0, -1, -1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int);
template Eigen::Matrix<int, -1, 3, 1, -1, 3> igl::slice_mask<Eigen::Matrix<int, -1, 3, 1, -1, 3> >(Eigen::DenseBase<Eigen::Matrix<int, -1, 3, 1, -1, 3> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int);
template Eigen::Matrix<int, -1, 1, 0, -1, 1> igl::slice_mask<Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::DenseBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int);
template Eigen::Array<int, -1, 3, 1, -1, 3> igl::slice_mask<Eigen::Array<int, -1, 3, 1, -1, 3> >(Eigen::DenseBase<Eigen::Array<int, -1, 3, 1, -1, 3> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int);
template void igl::slice_mask<bool, bool>(Eigen::SparseMatrix<bool, 0, int> const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::SparseMatrix<bool, 0, int>&);
template void igl::slice_mask<Eigen::Matrix<double, -1, 2, 0, -1, 2>, Eigen::Matrix<double, -1, 2, 0, -1, 2> >(Eigen::DenseBase<Eigen::Matrix<double, -1, 2, 0, -1, 2> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 2, 0, -1, 2> >&);
template void igl::slice_mask<Eigen::Matrix<int, -1, 2, 0, -1, 2>, Eigen::Matrix<int, -1, 2, 0, -1, 2> >(Eigen::DenseBase<Eigen::Matrix<int, -1, 2, 0, -1, 2> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 2, 0, -1, 2> >&);
template void igl::slice_mask<Eigen::Matrix<int, -1, 2, 0, -1, 2>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::DenseBase<Eigen::Matrix<int, -1, 2, 0, -1, 2> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&);
template void igl::slice_mask<Eigen::Array<bool, -1, 1, 0, -1, 1>, Eigen::Array<bool, -1, 1, 0, -1, 1> >(Eigen::DenseBase<Eigen::Array<bool, -1, 1, 0, -1, 1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Array<bool, -1, 1, 0, -1, 1> >&);
template void igl::slice_mask<Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::DenseBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&);
template void igl::slice_mask<Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::DenseBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
template void igl::slice_mask<Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::DenseBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
template void igl::slice_mask<Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::DenseBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&);
template void igl::slice_mask<Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::DenseBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> >&);
template void igl::slice_mask<Eigen::Matrix<int, -1, 1, 0, -1, 1>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::DenseBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&);
template Eigen::Matrix<int, -1, -1, 0, -1, -1> igl::slice_mask<Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::DenseBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::Array<bool, -1, 1, 0, -1, 1> const&, int);
#endif
