/*

SPARSEREALMATRIX.cc -- routines for SPARSEREALMATRIX class
 
Copyright (C) 2001-2004 Norbert Mueller
 
This file is part of the iRRAM Library.
 
The iRRAM Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
 
The iRRAM Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
License for more details.
 
You should have received a copy of the GNU Library General Public License
along with the iRRAM Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. 
*/

#include "iRRAM/core.h"

namespace iRRAM {

/**************************************************************************/

void sparse_set(SPARSEREALMATRIX& m,unsigned int i,unsigned int j,
                const REAL& x){
  SPM_ELEMENT* ptr_r= m.rowvector[i];
  if ( m.hotspot != NULL && m.hotspot->rowindex==i && m.hotspot->colindex<=j)
       ptr_r=m.hotspot;
  if ( ptr_r!=NULL ) {
    if (ptr_r->colindex==j) {ptr_r->value=x;return;}
    while ( (ptr_r->nextcol != NULL)&& (ptr_r->nextcol->colindex <= j) ) {
      ptr_r=ptr_r->nextcol;
      if (ptr_r->colindex==j) {ptr_r->value=x;return;}
    }
  }
  SPM_ELEMENT* ptr= new SPM_ELEMENT;
  m.filled+=1;
  ptr->nextcol=NULL;
  ptr->nextrow=NULL;
  ptr->colindex=j;
  ptr->rowindex=i;
  ptr->value=x;

  if ( m.rowvector[i]!=NULL &&  m.rowvector[i]->colindex < j) {
    ptr->nextcol=ptr_r->nextcol;
    ptr_r->nextcol=ptr;
  } else {
    ptr->nextcol=m.rowvector[i];
    m.rowvector[i] = ptr;
  }
  SPM_ELEMENT* ptr_c= m.colvector[j];
  if ( ptr_c!=NULL && m.colvector[j]->rowindex < i ) { 
    while (ptr_c->nextrow != NULL && ptr_c->nextrow->rowindex < i)
      ptr_c=ptr_c->nextrow;
    ptr->nextrow=ptr_c->nextrow;
    ptr_c->nextrow=ptr;
  } else {
    ptr->nextrow=m.colvector[j];
    m.colvector[j] = ptr;
  }
  m.hotspot=ptr;
}


void sparse_unset(SPARSEREALMATRIX& m,unsigned int i,unsigned int j){
  SPM_ELEMENT* ptr_r= m.rowvector[i];
  SPM_ELEMENT* ptr;
  if ( m.rowvector[i] == NULL || m.rowvector[i]->colindex > j ) return; 
  ptr=m.rowvector[i];
  if (m.rowvector[i]->colindex==j) m.rowvector[i]=ptr->nextcol; 
  else {
    while  ( (ptr_r->nextcol != NULL)&&  (ptr_r->nextcol->colindex < j))
           ptr_r=ptr_r->nextcol;
    if ( ptr_r->nextcol == NULL || ptr_r->nextcol->colindex > j ) return ;
    ptr=ptr_r->nextcol;
    ptr_r->nextcol=ptr->nextcol;
    }

  ptr_r= m.colvector[j];
  if (m.colvector[j]->rowindex==i) m.colvector[j]=ptr->nextrow; 
  else {
    while  ( ptr_r->nextrow->rowindex < i)
           ptr_r=ptr_r->nextrow;
    ptr_r->nextrow=ptr->nextrow;
    }




//  m.hotspot= ptr->nextcol;
  m.hotspot=NULL;
  m.filled-=1;
  delete ptr;

}

REAL  SPARSEREALMATRIX::operator () (unsigned int i, unsigned int j) {
int anz=0;
  if ((i>= maxrow) || (j>= maxcolumn) ) {
     fprintf(stderr,"Illegal indices [%d,%d] for real matrix [%d,%d]\n",
           i,j,maxrow,maxcolumn);
     exit(1);
     }
  SPM_ELEMENT* ptr_r= rowvector[i];
  if ( hotspot != NULL && hotspot->rowindex<=i && hotspot->colindex==j) {
    ptr_r=hotspot;
    while ( (ptr_r->nextrow != NULL) && (ptr_r->nextrow->rowindex <= i))
      {anz+=1;ptr_r=ptr_r->nextrow;}
//printf("*%d(%d,%d) ",anz,i,j);
    this->hotspot=ptr_r;
    if (ptr_r->rowindex==i) return ptr_r->value;
    return 0; 
  }

  if ( hotspot != NULL && hotspot->rowindex==i && hotspot->colindex<=j)
       ptr_r=hotspot;
  if (ptr_r==NULL) return 0;  
  while ( (ptr_r->nextcol != NULL) && (ptr_r->nextcol->colindex <= j))
      {anz+=1;ptr_r=ptr_r->nextcol;}
//printf("#%d(%d,%d) ",anz,i,j);

  this->hotspot=ptr_r;
  if (ptr_r->colindex==j) return ptr_r->value;
  return 0; 
};

SPM_ELEMENT* sparse_get_ptr( SPARSEREALMATRIX& m,unsigned int i,unsigned int j){
  SPM_ELEMENT* ptr_r= m.rowvector[i];
  if ( m.hotspot != NULL && m.hotspot->rowindex==i && m.hotspot->colindex<=j)
       ptr_r=m.hotspot;
  if (ptr_r==NULL) return NULL;  
  while ( (ptr_r->nextcol != NULL) && (ptr_r->nextcol->colindex <= j))
      ptr_r=ptr_r->nextcol;
  m.hotspot=ptr_r;
  if (ptr_r->colindex==j) return ptr_r;
  return NULL; 
}


void SPARSEREALMATRIX::free() {
  SPM_ELEMENT* ptr;
  SPM_ELEMENT* ptr_r;
  for (unsigned int i=0;i<maxrow;i++) {
    ptr=rowvector[i];
    while (ptr != NULL) {
      ptr_r=ptr;
      ptr=ptr->nextcol;
      delete ptr_r;
    }
  }
  delete rowvector;
  delete colvector;
  hotspot=NULL;
  filled=0;
};


SPARSEREALMATRIX::SPARSEREALMATRIX(unsigned int rows,unsigned int columns){
  if ( (rows == 0)||  (columns == 0 ) ) { 
     fprintf(stderr,"Error in dimensioning real matrix of size [%d,%d]\n",
         rows,columns);
     exit(1);
     }
  maxrow=rows;maxcolumn=columns;
  rowvector=new SPM_ELEMENT*[rows];
  colvector=new SPM_ELEMENT*[columns];
  for (unsigned int i=0;i<rows;i++) rowvector[i]=NULL; 
  for (unsigned int i=0;i<columns;i++) colvector[i]=NULL;
  hotspot=NULL;
  filled=0;
};

SPARSEREALMATRIX sparse_eye (unsigned int rows) {
  SPARSEREALMATRIX prod(rows,rows);
  for (unsigned int i=0;i<rows;i++) {
    prod.rowvector[i]=new SPM_ELEMENT;
    prod.filled+=1;
    prod.rowvector[i]->nextcol=NULL;
    prod.rowvector[i]->nextrow=NULL;
    prod.colvector[i]=prod.rowvector[i];
    (prod.rowvector[i])->rowindex=i;
    (prod.rowvector[i])->colindex=i;
    (prod.rowvector[i])->value=1;
  }
  return prod;
}

SPARSEREALMATRIX sparse_zeroes  (unsigned int rows,
                    unsigned int columns) {
  SPARSEREALMATRIX result(rows,columns);
  return result;
}

SPARSEREALMATRIX sparse_ones    (unsigned int rows,
                    unsigned int columns) {
  SPARSEREALMATRIX result(rows,columns);
  SPM_ELEMENT* ptr;
  SPM_ELEMENT* ptr_r;

    result.rowvector[0]=new SPM_ELEMENT;
    result.filled+=1;
    result.rowvector[0]->nextcol=NULL;
    result.rowvector[0]->nextrow=NULL;
    result.colvector[0]=result.rowvector[0];
    (result.rowvector[0])->rowindex=0;
    (result.rowvector[0])->colindex=0;
    (result.rowvector[0])->value=1;

  ptr = result.colvector[0];
  for (unsigned int i=1;i<rows;i++) { 
    result.rowvector[i]=new SPM_ELEMENT;
    result.filled+=1;
    result.rowvector[i]->nextcol=NULL;
    result.rowvector[i]->nextrow=NULL;
    ptr->nextrow=result.rowvector[i];
    ptr=result.rowvector[i];
    (result.rowvector[i])->rowindex=i;
    (result.rowvector[i])->colindex=0;
    (result.rowvector[i])->value=1;
  }

  for (unsigned int j=1;j<columns;j++) { 
    ptr= result.colvector[j-1];
    result.colvector[j]=new SPM_ELEMENT;
    result.filled+=1;
    result.colvector[j]->nextcol=NULL;
    result.colvector[j]->nextrow=NULL;
    ptr->nextcol=result.colvector[j];
    (result.rowvector[0])->rowindex=0;
    (result.rowvector[0])->colindex=j;
    (result.rowvector[0])->value=1;
    ptr=ptr->nextrow;

    ptr_r = result.colvector[j];
    for (unsigned int i=1;i<rows;i++) { 
      ptr_r->nextrow=new SPM_ELEMENT;
      result.filled+=1;
      ptr->nextcol=ptr->nextrow;
      ptr=ptr->nextrow;
      ptr_r=ptr_r->nextrow;
      ptr_r->nextcol=NULL;
      ptr_r->nextrow=NULL;
      ptr_r->rowindex=i;
      ptr_r->colindex=j;
      ptr_r->value=1;
    }
  }
  return result;
}

SPARSEREALMATRIX& SPARSEREALMATRIX::operator = (const SPARSEREALMATRIX& y) {
  SPM_ELEMENT* ptr;

  (*this).free();

  maxrow = y.maxrow;
  maxcolumn = y.maxcolumn;
  rowvector=new SPM_ELEMENT*[maxrow];
  colvector=new SPM_ELEMENT*[maxcolumn];
  for (unsigned int i=0;i<maxrow;i++) rowvector[i]=NULL; 
  for (unsigned int i=0;i<maxcolumn;i++) colvector[i]=NULL; 
  hotspot=NULL;
  filled=0;

  for (unsigned int i=1;i<=y.maxrow;i++) {
    ptr=y.rowvector[y.maxrow-i];
    while (ptr != NULL) {
      sparse_set((*this),ptr->rowindex,ptr->colindex,ptr->value);
      ptr=ptr->nextcol;
    }
  }

  return(*this);
}

SPARSEREALMATRIX operator + (const SPARSEREALMATRIX& x, const SPARSEREALMATRIX& y) {
  SPARSEREALMATRIX sum(x.maxrow,x.maxcolumn);

  SPM_ELEMENT* ptr_x;
  SPM_ELEMENT* ptr_y;

  if ( x.maxrow != y.maxrow || x.maxcolumn != y.maxcolumn) { 
     fprintf(stderr,"Error in adding real matrices of different sizes \n");
     exit(1);
     }
  for (unsigned int i=0;i<x.maxrow;i++) {
    ptr_y=y.rowvector[i];
    ptr_x=x.rowvector[i];
    while (ptr_x != NULL || ptr_y != NULL) {
      if ( ptr_y == NULL ) { 
        sparse_set(sum,i,ptr_x->colindex,ptr_x->value);
        ptr_x=ptr_x->nextcol;
      } else if (  ptr_x == NULL ) {
        sparse_set(sum,i,ptr_y->colindex,ptr_y->value);
        ptr_y=ptr_y->nextcol;
      } else if ( ptr_x->colindex < ptr_y->colindex) { 
        sparse_set(sum,i,ptr_x->colindex,ptr_x->value);
        ptr_x=ptr_x->nextcol;
      } else if ( ptr_x->colindex > ptr_y->colindex) {
        sparse_set(sum,i,ptr_y->colindex,ptr_y->value);
        ptr_y=ptr_y->nextcol;
      } else {
       sparse_set(sum,i,ptr_y->colindex,ptr_x->value+ptr_y->value);
        ptr_x=ptr_x->nextcol;
        ptr_y=ptr_y->nextcol;
      }
    }
  }
  return sum;
}

SPARSEREALMATRIX operator - (const SPARSEREALMATRIX& x, const SPARSEREALMATRIX& y) {
  SPARSEREALMATRIX sum(x.maxrow,x.maxcolumn);

  SPM_ELEMENT* ptr_x;
  SPM_ELEMENT* ptr_y;

  if ( x.maxrow != y.maxrow || x.maxcolumn != y.maxcolumn) { 
     fprintf(stderr,"Error in adding real matrices of different sizes \n");
     exit(1);
     }
  for (unsigned int i=0;i<x.maxrow;i++) {
    ptr_y=y.rowvector[i];
    ptr_x=x.rowvector[i];
    while (ptr_x != NULL || ptr_y != NULL) {
      if ( ptr_y == NULL ) { 
        sparse_set(sum,i,ptr_x->colindex,ptr_x->value);
        ptr_x=ptr_x->nextcol;
      } else if (  ptr_x == NULL ) {
        sparse_set(sum,i,ptr_y->colindex,ptr_y->value);
        ptr_y=ptr_y->nextcol;
      } else if ( ptr_x->colindex < ptr_y->colindex) { 
        sparse_set(sum,i,ptr_x->colindex,ptr_x->value);
        ptr_x=ptr_x->nextcol;
      } else if ( ptr_x->colindex > ptr_y->colindex) {
        sparse_set(sum,i,ptr_y->colindex,ptr_y->value);
        ptr_y=ptr_y->nextcol;
      } else {
       sparse_set(sum,i,ptr_y->colindex,ptr_x->value - ptr_y->value);
        ptr_x=ptr_x->nextcol;
        ptr_y=ptr_y->nextcol;
      }
    }
  }
  return sum;
}

SPARSEREALMATRIX::~SPARSEREALMATRIX() {
  free();
};

SPARSEREALMATRIX::SPARSEREALMATRIX(){
  maxrow=0;maxcolumn=0;colvector=NULL; rowvector=NULL; filled=0; hotspot=NULL;
};

SPARSEREALMATRIX::SPARSEREALMATRIX(const SPARSEREALMATRIX& y){
  SPM_ELEMENT* ptr;
  maxrow = y.maxrow;
  maxcolumn = y.maxcolumn;
  rowvector=new SPM_ELEMENT*[maxrow];
  colvector=new SPM_ELEMENT*[maxcolumn];
  for (unsigned int i=0;i<maxrow;i++) rowvector[i]=NULL; 
  for (unsigned int i=0;i<maxcolumn;i++) colvector[i]=NULL; 
  hotspot=NULL;
  filled=0;

  for (unsigned int i=1;i<=y.maxrow;i++) {
    ptr=y.rowvector[y.maxrow-i];
    while (ptr != NULL) {
      sparse_set(*this,ptr->rowindex,ptr->colindex,ptr->value);
      ptr=ptr->nextcol;
    }
  }
};


void SPARSEREALMATRIX::adderror (sizetype error)
{ 
  SPM_ELEMENT* ptr;
  for (unsigned int i=0;i<maxrow;i++) {
    ptr=rowvector[i];
    while (ptr != NULL) {
      ptr->value.adderror(error);
      ptr=ptr->nextcol;
    }
  }
}

void SPARSEREALMATRIX::seterror (sizetype error)
{ 
  SPM_ELEMENT* ptr;
  for (unsigned int i=0;i<maxrow;i++) {
    ptr=rowvector[i];
    while (ptr != NULL) {
      ptr->value.seterror(error);
      ptr=ptr->nextcol;
    }
  }
}

void SPARSEREALMATRIX::geterror (sizetype& error) const
{
  unsigned t=0;
  sizetype lerror; 
  SPM_ELEMENT* ptr;
  for (unsigned int i=0;i<maxrow;i++) {
    ptr=rowvector[i];
    while (ptr != NULL) {
      if (t==0) {
        ptr->value.geterror(lerror);
        t=1;
      } else {
        ptr->value.geterror(lerror);
        sizetype_max(error,error,lerror);
        ptr=ptr->nextcol;
      }
    } 
  }
  if (t==0) sizetype_exact(error);
}


SPARSEREALMATRIX operator * (const SPARSEREALMATRIX& x ,const SPARSEREALMATRIX& y) {
  SPARSEREALMATRIX prod(x.maxrow,y.maxcolumn);
  REAL sum;
  SPM_ELEMENT* a;
  SPM_ELEMENT* b;
  if ( x.maxcolumn != y.maxrow) { 
     fprintf(stderr,"Error in multiplying real matrices of different sizes \n");
     exit(1);
     }
  for (unsigned int i=0; i<x.maxrow;i++) {
    a=x.rowvector[i]; 
    while (a != NULL ) {
      b=y.rowvector[a->colindex];
      while (b != NULL ) {
        sparse_set(prod,a->rowindex,b->colindex,
          prod(a->rowindex,b->colindex)+a->value*b->value);  
        b=b->nextcol; 
      }
    a=a->nextcol;
    }
  }
  return prod;
}

SPARSEREALMATRIX operator / (const SPARSEREALMATRIX& x ,const SPARSEREALMATRIX& y) {
  SPARSEREALMATRIX rside=x,lside=y,result;
  return solve(lside,rside,1);
}

SPARSEREALMATRIX solve (SPARSEREALMATRIX& lside, SPARSEREALMATRIX& rside,int use_pivot) {
continous_begin ();
  SPARSEREALMATRIX result(rside.maxrow,rside.maxcolumn);
  SPM_ELEMENT* ptr;
  REAL faktor;
  SPM_ELEMENT* ptr2;
  REAL faktor2;
  SPM_ELEMENT* ptr3;
  int testsize,pivotsize,pivot,pivotsave;
  std::vector<int> perm(lside.maxrow), backperm(lside.maxrow);
  

  if ( lside.maxrow != rside.maxrow) { 
     fprintf(stderr,"Error, real matrices of different sizes \n");
     exit(1);
     }
  for (unsigned int i=0; i<lside.maxrow;i++) perm[i]=i;
  for (unsigned int i=0; i<lside.maxrow;i++) backperm[i]=i;

  for (unsigned int i=0; i < lside.maxrow; i++) {
    pivot=i;
    if (use_pivot) {
      pivotsize=upperbound(lside(perm[i],i));
      for (unsigned int j=i+1; j < lside.maxcolumn; j++) {
        testsize=upperbound(lside(perm[j],i));
        if (testsize>pivotsize) {
          pivot=j;
          pivotsize=testsize;
        }
        else if ( testsize < pivotsize && use_pivot == 1) break ;
      }
      pivotsave=perm[pivot];
      perm[pivot]=perm[i];
      perm[i]=pivotsave;  
    }
    backperm[perm[i]]=i;
    backperm[perm[pivot]]=pivot;
    ptr=lside.colvector[i];
    while (ptr != NULL ) {
       unsigned int j=backperm[ptr->rowindex];
       if ( j > i) {
        faktor=ptr->value/lside(perm[i],i);
        ptr2=rside.rowvector[perm[i]];
        while (ptr2 != NULL ) {
          unsigned int k=ptr2->colindex;
          sparse_set(rside,perm[j],k,
                     rside(perm[j],k)
                    -faktor*ptr2->value);
          ptr2=ptr2->nextcol;
        }
        ptr2=lside.rowvector[perm[i]];
        while (ptr2 != NULL ) {
          unsigned int l=ptr2->colindex;
          if ( l>=i )
            sparse_set(lside,perm[j],l,
                       lside(perm[j],l)
                       -faktor*ptr2->value);
          ptr2=ptr2->nextcol;
        }
      }
      ptr=ptr->nextrow;
    }
  }

  for (int i=rside.maxrow-1; i >= 0; i--) {
    faktor=1/lside(perm[i],i);

    for (int k=rside.maxcolumn-1; k>=0; k--) {
      ptr2=sparse_get_ptr(rside,perm[i],k);
      if (ptr2 != NULL ) { 
        sparse_set(result,i,k,ptr2->value*faktor);
        faktor2=result(i,k);

        ptr3=lside.colvector[i];
        while (ptr3 != NULL) {
          int j= backperm[ptr3->rowindex];
          if ( j < i ) sparse_set(rside,perm[j],k,
                         rside(perm[j],k)
                       -ptr3->value*faktor2);
          ptr3=ptr3->nextrow;
      } }
  } }
  continous_end ();
  return result;
}

REAL       maxnorm (const SPARSEREALMATRIX& x){
  REAL m=0;
  SPM_ELEMENT* ptr;
  for (unsigned int i=0;i<x.maxrow;i++) {
    ptr=x.rowvector[i];
    while (ptr != NULL) {
      m=maximum(m,abs(ptr->value));
      ptr=ptr->nextcol;
    }
  }
  return m;
}

REAL       rowsumnorm (const SPARSEREALMATRIX& x){
  REAL m=0;
  SPM_ELEMENT* ptr;
  REAL sum;
  for (unsigned int i=0;i<x.maxrow;i++) {
    ptr=x.rowvector[i];
    sum=0;
    while (ptr != NULL) {
      sum=sum+abs(ptr->value);
      m=maximum(m,abs(ptr->value));
      ptr=ptr->nextcol;
    }
    m=maximum(m,sum);
  }
  return m;
}

REAL       colsumnorm (const SPARSEREALMATRIX& x){
  REAL m=0;
  SPM_ELEMENT* ptr;
  REAL sum;
  for (unsigned int i=0;i<x.maxcolumn;i++) {
    ptr=x.colvector[i];
    sum=0;
    while (ptr != NULL) {
      sum=sum+abs(ptr->value);
      m=maximum(m,abs(ptr->value));
      ptr=ptr->nextrow;
    }
    m=maximum(m,sum);
  }
  return m;
}

} // namespace iRRAM
