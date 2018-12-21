// Include Rcpp system header file (e.g. <>)
#include <Rcpp.h>

// Include our definition of the student file (e.g. "")
#include "student.h"

//' Create a `Student` object.
//'
//' Constructes a `Student` object with non-default values and accesses
//' their favorite numbers.
//'
//' @return
//' An `integer` vector containing the student's favorite numbers.
//'
//' @export
// [[Rcpp::export]]
std::vector<int> simulate_student() {
  Student s = Student("bob", 10, true);
  return s.GetFavoriteNumbers();
}
