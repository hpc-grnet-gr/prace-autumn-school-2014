      integer dim1, dim2, dim3
      parameter (dim1 = 1000, dim2 = 1000, dim3 = 1000)
      double precision A(dim1, dim2), B(dim2, dim3), C(dim1, dim3)
      double precision alpha, beta
      real start, finish, tarray(2)
      alpha = 1.0d0
      beta = 0.0d0
      call srand(86456)
      do i = 1, dim1
        do j = 1, dim2
          A(i, j) = rand()
        enddo
      enddo
      do i = 1, dim2
        do j = 1, dim3
          B(i, j) = rand()
        enddo
      enddo
c etime is not a part of the standard 
      call etime(tarray, start)
      call dgemm('N','N',dim1,dim3,dim2,alpha,A,dim1,B,dim2,beta,C,dim1)
      call etime(tarray, finish)
      print *,'time for C(',dim1,',',dim3,') = A(',dim1,',',dim2,') B(',
     1dim2,',',dim3,') is',finish - start,' s'
      end

