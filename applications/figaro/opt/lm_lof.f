      subroutine lm_lof(m,n,xc,rc,fjac,ldfjac,iflag)
*+
* Name:
*    LM_LOF

* Invocation:
*    CALL LM_LOF(M,N,XC,RC,FJAC,LDFJAC,IFLAG)
*
* Description:
*  To calculate residuals or Jacobian for a Lorentz fit model.
*
* Purpose:
*  To calculate residuals or Jacobian for a Lorentz fit model.
*
* Arguments:
*     M = INTEGER (Given)
*        Number of data points
*     N = INTEGER (Given)
*        Number of variables
*     XC(N) = DOUBLE PRECISION ARRAY (Given)
*        Function parameters
*     LDFJAC = INTEGER (Given)
*        Dimension of fjac
*     IFLAG = INTEGER (Given)
*        Flag to determine action of routine, 1 => fvec,
*                        2 => fjac
*     RC(M) = DOUBLE PRECISION ARRAY (Returned)
*        Residuals
*     FJAC(LDFJAC,N) = DOUBLE PRECISION ARRAY (Returned)
*        Jacobian
* Subroutines/functions referenced:
*
* Author:
*   T.N.Wilkins, Cambridge,  21-Aug-1992 based on lm_mgf
* History:
*-
      implicit none
      integer m
      integer n
      double precision xc(n)
      double precision rc(m)
      integer ldfjac
      double precision fjac(ldfjac,n)
      integer iflag
      include 'opt_cmn'
      include 'dynamic_memory_inc'

*


* Calculate residuals or derivative

      call lm_lof_s(iflag,m,n,xc,rc,fjac,ldfjac,dynamic_mem(densptr),
     :     dynamic_mem(dataptr),dynamic_mem(weightptr))
      end
