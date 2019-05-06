// [[Rcpp::depends(RcppArmadillo)]]
#include "alphasimr.h"

/*
 * Genotype data is stored in a field of cubes.
 * The field has length equal to nChr
 * Each cube has dimensions nLoci/8 by ploidy by nInd
 * Output return with dimensions nInd by nLoci
 */
// [[Rcpp::export]]
arma::Mat<unsigned char> getGeno(const arma::field<arma::Cube<unsigned char> >& geno, 
                                 const arma::Col<int>& lociPerChr,
                                 arma::uvec lociLoc, int nThreads){
  // R to C++ index correction
  lociLoc -= 1;
  
  arma::uword nInd = geno(0).n_slices;
  arma::uword nChr = geno.n_elem;
  arma::uword ploidy = geno(0).n_cols;
  arma::Mat<unsigned char> output(nInd,arma::sum(lociPerChr),arma::fill::zeros);
  int loc1;
  int loc2 = -1;
  for(arma::uword i=0; i<nChr; ++i){
    if(lociPerChr(i)>0){
      // Get loci locations
      loc1 = loc2+1;
      loc2 += lociPerChr(i);
      arma::uvec chrLociLoc = lociLoc(arma::span(loc1,loc2));
#ifdef _OPENMP
#pragma omp parallel for schedule(static) num_threads(nThreads)
#endif
      for(arma::uword ind=0; ind<nInd; ++ind){
        std::bitset<8> workBits;
        for(arma::uword p=0; p<ploidy; ++p){
          for(arma::uword j=0; j<chrLociLoc.n_elem; ++j){
            workBits = toBits(geno(i)(chrLociLoc(j)/8,p,ind));
            output(ind,j+loc1) += (unsigned char) workBits[chrLociLoc(j)%8];
          }
        }
      }
    }
  }
  return output;
}

// [[Rcpp::export]]
arma::Mat<unsigned char> getMaternalGeno(const arma::field<arma::Cube<unsigned char> >& geno, 
                                         const arma::Col<int>& lociPerChr,
                                         arma::uvec lociLoc, int nThreads){
  // R to C++ index correction
  lociLoc -= 1;
  
  arma::uword nInd = geno(0).n_slices;
  arma::uword nChr = geno.n_elem;
  arma::uword ploidy = geno(0).n_cols;
  arma::Mat<unsigned char> output(nInd,arma::sum(lociPerChr),arma::fill::zeros);
  int loc1;
  int loc2 = -1;
  for(arma::uword i=0; i<nChr; ++i){
    if(lociPerChr(i)>0){
      // Get loci locations
      loc1 = loc2+1;
      loc2 += lociPerChr(i);
      arma::uvec chrLociLoc = lociLoc(arma::span(loc1,loc2));
#ifdef _OPENMP
#pragma omp parallel for schedule(static) num_threads(nThreads)
#endif
      for(arma::uword ind=0; ind<nInd; ++ind){
        std::bitset<8> workBits;
        for(arma::uword p=0; p<ploidy/2; ++p){
          for(arma::uword j=0; j<chrLociLoc.n_elem; ++j){
            workBits = toBits(geno(i)(chrLociLoc(j)/8,p,ind));
            output(ind,j+loc1) += (unsigned char) workBits[chrLociLoc(j)%8];
          }
        }
      }
    }
  }
  return output;
}

// [[Rcpp::export]]
arma::Mat<unsigned char> getPaternalGeno(const arma::field<arma::Cube<unsigned char> >& geno, 
                                         const arma::Col<int>& lociPerChr,
                                         arma::uvec lociLoc, int nThreads){
  // R to C++ index correction
  lociLoc -= 1;
  
  arma::uword nInd = geno(0).n_slices;
  arma::uword nChr = geno.n_elem;
  arma::uword ploidy = geno(0).n_cols;
  arma::Mat<unsigned char> output(nInd,arma::sum(lociPerChr),arma::fill::zeros);
  int loc1;
  int loc2 = -1;
  for(arma::uword i=0; i<nChr; ++i){
    if(lociPerChr(i)>0){
      // Get loci locations
      loc1 = loc2+1;
      loc2 += lociPerChr(i);
      arma::uvec chrLociLoc = lociLoc(arma::span(loc1,loc2));
#ifdef _OPENMP
#pragma omp parallel for schedule(static) num_threads(nThreads)
#endif
      for(arma::uword ind=0; ind<nInd; ++ind){
        std::bitset<8> workBits;
        for(arma::uword p=ploidy/2; p<ploidy; ++p){
          for(arma::uword j=0; j<chrLociLoc.n_elem; ++j){
            workBits = toBits(geno(i)(chrLociLoc(j)/8,p,ind));
            output(ind,j+loc1) += (unsigned char) workBits[chrLociLoc(j)%8];
          }
        }
      }
    }
  }
  return output;
}

// Returns haplotype data in a matrix of nInd*ploidy by nLoci
// [[Rcpp::export]]
arma::Mat<unsigned char> getHaplo(const arma::field<arma::Cube<unsigned char> >& geno, 
                                  const arma::Col<int>& lociPerChr,
                                  arma::uvec lociLoc, int nThreads){
  // R to C++ index correction
  lociLoc -= 1;
  
  arma::uword nInd = geno(0).n_slices;
  arma::uword nChr = geno.n_elem;
  arma::uword ploidy = geno(0).n_cols;
  arma::Mat<unsigned char> output(nInd*ploidy,arma::sum(lociPerChr));
  int loc1;
  int loc2 = -1;
  for(arma::uword i=0; i<nChr; ++i){
    if(lociPerChr(i)>0){
      // Get loci locations
      loc1 = loc2+1;
      loc2 += lociPerChr(i);
      arma::uvec chrLociLoc = lociLoc(arma::span(loc1,loc2));
#ifdef _OPENMP
#pragma omp parallel for schedule(static) num_threads(nThreads)
#endif
      for(arma::uword ind=0; ind<nInd; ++ind){
        std::bitset<8> workBits;
        for(arma::uword p=0; p<ploidy; ++p){
          for(arma::uword j=0; j<chrLociLoc.n_elem; ++j){
            workBits = toBits(geno(i)(chrLociLoc(j)/8,p,ind));
            output(ind*ploidy+p,j+loc1) = (unsigned char) workBits[chrLociLoc(j)%8];
          }
        }
      }
    }
  }
  return output;
}

// Returns haplotype data in a matrix of nInd by nLoci for a single
// chromosome group. i.e. just female or male chromosomes for diploids
// [[Rcpp::export]]
arma::Mat<unsigned char> getOneHaplo(const arma::field<arma::Cube<unsigned char> >& geno, 
                                     const arma::Col<int>& lociPerChr,
                                     arma::uvec lociLoc, int haplo, int nThreads){
  // R to C++ index correction
  lociLoc -= 1;
  haplo -= 1;
  
  arma::uword nInd = geno(0).n_slices;
  arma::uword nChr = geno.n_elem;
  arma::Mat<unsigned char> output(nInd,arma::sum(lociPerChr));
  int loc1;
  int loc2 = -1;
  for(arma::uword i=0; i<nChr; ++i){
    if(lociPerChr(i)>0){
      // Get loci locations
      loc1 = loc2+1;
      loc2 += lociPerChr(i);
      arma::uvec chrLociLoc = lociLoc(arma::span(loc1,loc2));
#ifdef _OPENMP
#pragma omp parallel for schedule(static) num_threads(nThreads)
#endif
      for(arma::uword ind=0; ind<nInd; ++ind){
        std::bitset<8> workBits;
        for(arma::uword j=0; j<chrLociLoc.n_elem; ++j){
          workBits = toBits(geno(i)(chrLociLoc(j)/8,haplo,ind));
          output(ind,j+loc1) = (unsigned char) workBits[chrLociLoc(j)%8];
        }
      }
    }
  }
  return output;
}

// Returns IBD haplotype data in a matrix of nInd*2 by nLoci
// [[Rcpp::export]]
Rcpp::IntegerMatrix getIbdHaplo(const Rcpp::List          & ibdRecHist,
                                const Rcpp::IntegerVector & individuals,
                                const Rcpp::IntegerVector & nLociPerChr) {
  int nIndSet = individuals.size();
  int nChr = nLociPerChr.size();
  int nLoc = sum(nLociPerChr);
  Rcpp::IntegerMatrix output(nIndSet * 2, nLoc);
  for (int indSet = 0; indSet < nIndSet; ++indSet) {
    Rcpp::List ibdRecHistInd = ibdRecHist(individuals(indSet) - 1);
    for (int par = 0; par < 2; ++par) {
      int chrOrigin = 0;
      for (int chr = 0; chr < nChr; ++chr) {
        if (nLociPerChr(chr) > 0) {
          Rcpp::List ibdRecHistIndChr = ibdRecHistInd(chr);
          Rcpp::IntegerMatrix ibdRecHistIndChrPar = ibdRecHistIndChr(par);
          int nSeg = ibdRecHistIndChrPar.nrow();
          for (int seg = 0; seg < nSeg; ++seg) {
            int source = ibdRecHistIndChrPar(seg, 0);
            int start = chrOrigin + ibdRecHistIndChrPar(seg, 1);
            int stop;
            if (seg < (nSeg - 1)) {
              stop = chrOrigin + ibdRecHistIndChrPar(seg + 1, 1) - 1;
            } else {
              stop = chrOrigin + nLociPerChr[chr];
            }
            for (int loc = start - 1; loc < stop; ++loc) {
              output(2 * indSet + par, loc) = source;
            }
          }
        }
        chrOrigin = chrOrigin + nLociPerChr(chr);
      }
    }
  }
  return output;
}

// [[Rcpp::export]]
void writeGeno(const arma::field<arma::Cube<unsigned char> >& geno, 
               const arma::Col<int>& lociPerChr,
               arma::uvec lociLoc,
               Rcpp::String filePath, int nThreads){
  arma::Mat<unsigned char> output;
  output = getGeno(geno,lociPerChr,lociLoc,nThreads);
  std::ofstream outFile;
  outFile.open(filePath, std::ios_base::app);
  output.save(outFile,arma::raw_ascii);
  outFile.close();
}

// [[Rcpp::export]]
void writeOneHaplo(const arma::field<arma::Cube<unsigned char> >& geno, 
                   const arma::Col<int>& lociPerChr, 
                   arma::uvec lociLoc, int haplo,
                   Rcpp::String filePath, int nThreads){
  arma::Mat<unsigned char> output;
  output = getOneHaplo(geno,lociPerChr,lociLoc,haplo,nThreads);
  std::ofstream outFile;
  outFile.open(filePath, std::ios_base::app);
  output.save(outFile,arma::raw_ascii);
  outFile.close();
}

arma::mat genoToGenoA(const arma::Mat<unsigned char>& geno, 
                      arma::uword ploidy, int nThreads){
  arma::mat output(geno.n_rows,geno.n_cols);
  double dP = double(ploidy);
  arma::vec x(ploidy+1);
  for(arma::uword i=0; i<x.n_elem; ++i)
    x(i) = (double(i)-dP/2.0)*(2.0/dP);
#ifdef _OPENMP
#pragma omp parallel for schedule(static) num_threads(nThreads)
#endif
  for(arma::uword j=0; j<geno.n_cols; ++j){
    for(arma::uword i=0; i<geno.n_rows; ++i){
      output(i,j) = x(geno(i,j));
    }
  }
  return output;
}

arma::mat genoToGenoD(const arma::Mat<unsigned char>& geno, 
                      arma::uword ploidy, int nThreads){
  arma::mat output(geno.n_rows,geno.n_cols);
  double dP = double(ploidy);
  arma::vec x(ploidy+1);
  for(arma::uword i=0; i<x.n_elem; ++i)
    x(i) = double(i)*(dP-double(i))*(2.0/dP)*(2.0/dP);
#ifdef _OPENMP
#pragma omp parallel for schedule(static) num_threads(nThreads)
#endif
  for(arma::uword j=0; j<geno.n_cols; ++j){
    for(arma::uword i=0; i<geno.n_rows; ++i){
      output(i,j) = x(geno(i,j));
    }
  }
  return output;
}
