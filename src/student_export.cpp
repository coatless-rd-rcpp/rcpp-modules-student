// Include Rcpp system header file (e.g. <>)
#include <Rcpp.h>

// Include our definition of the student file (e.g. "")
#include "student.h"

// Expose (some of) the Student class
RCPP_MODULE(RcppStudentEx){
  Rcpp::class_<Student>("Student")
  .constructor<std::string, int, bool>()
  .method("GetName", &Student::GetName)
  .method("GetAge", &Student::GetAge)
  .method("IsMale", &Student::IsMale)
  .method("GetFavoriteNumbers", &Student::GetFavoriteNumbers)
  .method("LikesBlue", &Student::LikesBlue);
}
